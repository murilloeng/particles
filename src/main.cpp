//std
#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>
#include <cstdlib>

//ext
#include "external/cpp/inc/GL/glew.h"
#include "external/cpp/inc/GL/freeglut.h"

//particles
#include "particles/inc/Particle.hpp"

//data
static GLuint program;
static unsigned width;
static unsigned height;
static const unsigned np_max = 1000;
static std::vector<particles::Particle> list;
static std::chrono::high_resolution_clock::time_point timer;

//shaders
static bool load_file(std::string& source, const char* path)
{
	//open
	FILE* file = fopen(path, "r");
	//check
	if(!file) return false;
	//read
	source.clear();
	while(!feof(file)) source += fgetc(file);
	source.back() = '\0';
	//close
	fclose(file);
	//return
	return true;
}
static GLuint compile_shader(GLenum type, const char* source)
{
	//data
	GLuint shader = glCreateShader(type);
	//source
	glShaderSource(shader, 1, &source, nullptr);
	//compile
	glCompileShader(shader);
	//check
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	//info log
	if(!success)
	{
		char info[512];
		glGetShaderInfoLog(shader, 512, nullptr, info);
		fprintf(stderr, "Shader compilation error: %s\n", info);
	}
	//return
	return shader;
}
static GLuint create_program(const char* path_vertex, const char* path_fragment)
{
	//data
	std::string source_vertex, source_fragment;
	//sources
	load_file(source_vertex, path_vertex);
	load_file(source_fragment, path_fragment);
	//shaders
	GLuint shader_vertex = compile_shader(GL_VERTEX_SHADER, source_vertex.c_str());
	GLuint shader_fragment = compile_shader(GL_FRAGMENT_SHADER, source_fragment.c_str());
	//program
	GLuint program = glCreateProgram();
	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	//check
	GLint success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	//info log
	if(!success)
	{
		char info[512];
		glGetProgramInfoLog(program, 512, nullptr, info);
		fprintf(stderr, "Program linking error: %s\n", info);
	}
	//delete
	glUseProgram(program);
	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);
	//return
	return program;
}

//setup
static void setupGL(void)
{
	//data
	GLuint VAO, VBO, IBO;
	//create
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenVertexArrays(1, &VAO);
	//bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * np_max * particles::Particle::m_nv * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * np_max * (particles::Particle::m_nv - 2) * sizeof(unsigned), nullptr, GL_STATIC_DRAW);
	//layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (unsigned*) (0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (unsigned*) (2 * sizeof(float)));
	//program
	program = create_program("shd/base.vert", "shd/base.frag");
	//timer
	timer = std::chrono::high_resolution_clock::now();
}
static void update_buffers(void)
{
	//data
	const unsigned np = list.size();
	const unsigned nv = particles::Particle::m_nv;
	float* vbo_data = (float*) alloca(5 * np * nv * sizeof(float));
	unsigned* ibo_data = (unsigned*) alloca(3 * np * nv * sizeof(float));
	//buffers
	for(unsigned i = 0; i < list.size(); i++)
	{
		list[i].draw(ibo_data, vbo_data);
	}
	//transfer
	glBufferSubData(GL_ARRAY_BUFFER, 0, 5 * np * nv * sizeof(float), vbo_data);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3 * np * nv * sizeof(unsigned), ibo_data);
}

//particles
static void add_particle(double radius, math::vec3 color, math::vec3 position, math::vec3 velocity)
{
	//data
	particles::Particle particle;
	const unsigned np = list.size();
	//setup
	particle.m_index = np;
	particle.m_color = color;
	particle.m_radius = radius;
	particle.m_position = position;
	particle.m_velocity = velocity;
	//list
	list.push_back(particle);
	//buffers
	update_buffers();
}

//callbacks
static void callback_idle(void)
{
	//clock
	std::chrono::high_resolution_clock::time_point timer_now = std::chrono::high_resolution_clock::now();
	int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_now - timer).count();
	//timer
	timer = timer_now;
	printf("FPS: %.2lf\n", 1e6 / duration);
	//update
	for(particles::Particle& particle : list)
	{
		particle.update(duration / 1e6);
	}
	update_buffers();
	//draw
	glutPostRedisplay();
}
static void callback_display(void)
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw
	glDrawElements(GL_TRIANGLES, 3 * list.size() * (particles::Particle::m_nv - 2), GL_UNSIGNED_INT, nullptr);
	//buffers
	glutSwapBuffers();
}
static void callback_reshape(int width, int height)
{
	//viewport
	glViewport(0, 0, width, height);
	//uniforms
	::width = width;
	::height = height;
	glUniform1ui(glGetUniformLocation(program, "width"), width);
	glUniform1ui(glGetUniformLocation(program, "height"), height);
	//redraw
	glutPostRedisplay();
}
static void callback_mouse(int button, int state, int x1, int x2)
{
	//data
	static unsigned color_index = 0;
	const math::vec3 colors[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	//add particle
	if(state == GLUT_DOWN)
	{
		const float w = width;
		const float h = height;
		const float m = fmin(w, h);
		const float p1 = w / m * (2 * x1 / w - 1);
		const float p2 = h / m * (1 - 2 * x2 / h);
		add_particle(0.02, colors[color_index], {p1, p2, 0}, {0, 0, 0});
		glutPostRedisplay();
		color_index = (color_index + 1) % 3;
	}
}
static void callback_keyboard(unsigned char key, int x1, int x2)
{
	if(key == 27)
	{
		glutDestroyWindow(glutGetWindow());
	}
}

//main
int main(int argc, char** argv)
{
	//setup
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	//window
	glutCreateWindow("Particles");
	//glew
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error: can't setup glew!\n");
		exit(EXIT_FAILURE);
	}
	//setup
	setupGL();
	//callbacks
	glutIdleFunc(callback_idle);
	glutMouseFunc(callback_mouse);
	glutDisplayFunc(callback_display);
	glutReshapeFunc(callback_reshape);
	glutKeyboardFunc(callback_keyboard);
	// glutMotionFunc(callback_motion);
	// glutSpecialFunc(callback_special);
	// glutMouseWheelFunc(callback_wheel);
	//loop
	glutMainLoop();
	//return
	return EXIT_SUCCESS;
}