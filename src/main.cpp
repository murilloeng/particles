//std
#include <cmath>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

//ext
#include "external/cpp/inc/GL/glew.h"
#include "external/cpp/inc/GL/freeglut.h"

//particles
#include "particles/inc/Barrier.hpp"
#include "particles/inc/Particle.hpp"

//data
static unsigned width;
static unsigned height;
static clock_t timer_cpu;
static const unsigned nb_max = 100;
static const unsigned np_max = 200000;
static GLuint program[2], vao[3], vbo[3], ibo[3];
static std::vector<particles::Barrier> list_barriers;
static std::vector<particles::Particle> list_particles;
static std::chrono::high_resolution_clock::time_point timer_real;

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
		fprintf(stderr, "Shader %x compilation error: %s\n", type, info);
		exit(EXIT_FAILURE);
	}
	//return
	return shader;
}
static GLuint create_program(const char* path_vertex, const char* path_geometry, const char* path_fragment)
{
	//data
	GLuint shader_vertex, shader_geometry, shader_fragment;
	std::string source_vertex, source_fragment, source_geometry;
	//sources
	load_file(source_vertex, path_vertex);
	load_file(source_fragment, path_fragment);
	if(path_geometry) load_file(source_geometry, path_geometry);
	//shaders
	shader_vertex = compile_shader(GL_VERTEX_SHADER, source_vertex.c_str());
	shader_fragment = compile_shader(GL_FRAGMENT_SHADER, source_fragment.c_str());
	if(path_geometry) shader_geometry = compile_shader(GL_GEOMETRY_SHADER, source_geometry.c_str());
	//program
	GLuint program = glCreateProgram();
	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	if(path_geometry) glAttachShader(program, shader_geometry);
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
		exit(EXIT_FAILURE);
	}
	//delete
	glUseProgram(program);
	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);
	if(path_geometry) glDeleteShader(shader_geometry);
	//return
	return program;
}

//setup
static void setupGL(void)
{
	//create
	glGenBuffers(3, vbo);
	glGenBuffers(3, ibo);
	glGenVertexArrays(3, vao);
	//bind
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ARRAY_BUFFER, 10 * nb_max * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * nb_max * sizeof(unsigned), nullptr, GL_DYNAMIC_DRAW);
	//layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (unsigned*) (0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (unsigned*) (2 * sizeof(float)));
	//bind particles
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * np_max * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, np_max * sizeof(unsigned), nullptr, GL_DYNAMIC_DRAW);
	//layout particles
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (unsigned*) (0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (unsigned*) (2 * sizeof(float)));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (unsigned*) (5 * sizeof(float)));
	//programs
	program[0] = create_program("shd/barriers.vert", nullptr, "shd/barriers.frag");
	program[1] = create_program("shd/particles.vert", "shd/particles.geom", "shd/particles.frag");
	//timer
	timer_cpu = clock();
	timer_real = std::chrono::high_resolution_clock::now();
}
static void cleanupGL(void)
{
	glDeleteBuffers(3, vbo);
	glDeleteBuffers(3, ibo);
	glDeleteProgram(program[0]);
	glDeleteProgram(program[1]);
	glDeleteVertexArrays(3, vao);
}

//lists
static void add_barrier(math::vec3 x1, math::vec3 x2, math::vec3 color)
{
	//data
	particles::Barrier barrier;
	const unsigned nb = list_barriers.size();
	//setup
	barrier.m_x1 = x1;
	barrier.m_x2 = x2;
	barrier.m_index = nb;
	barrier.m_color = color;
	//list
	list_barriers.push_back(barrier);
	//buffers
	barrier.setup_buffers(ibo[0], vbo[0]);
}
static void add_particle(math::vec3 position, math::vec3 velocity, math::vec3 color, double radius)
{
	//data
	particles::Particle particle;
	const unsigned np = list_particles.size();
	//setup
	particle.m_index = np;
	particle.m_color = color;
	particle.m_radius = radius;
	particle.m_position = position;
	particle.m_velocity = velocity;
	particle.m_list_barriers = &list_barriers;
	particle.m_list_particles = &list_particles;
	//list
	list_particles.push_back(particle);
	//buffers
	particle.setup_buffers(ibo[1], vbo[1]);
}

//callbacks
static void callback_idle(void)
{
	//timer cpu
	const clock_t timer_cpu_now = clock();
	const double duration_cpu = double(timer_cpu_now - timer_cpu) / CLOCKS_PER_SEC;
	//timer real
	const std::chrono::high_resolution_clock::time_point timer_real_now = std::chrono::high_resolution_clock::now();
	int64_t duration_real = std::chrono::duration_cast<std::chrono::microseconds>(timer_real_now - timer_real).count();
	//timer
	timer_cpu = timer_cpu_now;
	timer_real = timer_real_now;
	printf("FPS: %.2e particles: %5ld\n", 1.0 / duration_cpu, list_particles.size());
	//create
	static unsigned index = 0;
	const math::vec3 colors[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	add_particle({-0.73 + 0.04 * index, 0.75, 0}, {0, 0, 0}, colors[index % 3], 0.02);
	index = (index + 1) % 36;
	//update
	const unsigned np = list_particles.size();
	float* vbo_data = (float*) alloca(6 * np * sizeof(float));
	for(particles::Particle& particle : list_particles)
	{
		particle.update(duration_real / 1e6, vbo_data);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * np * sizeof(float), vbo_data);
	//check
	if(list_particles.size() >= 1000)
	{
		glutLeaveMainLoop();
	}
	//draw
	glutPostRedisplay();
}
static void callback_display(void)
{
	//data
	const unsigned nb = list_barriers.size();
	const unsigned np = list_particles.size();
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//barriers
	glUseProgram(program[0]);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glDrawElements(GL_LINES, 2 * nb, GL_UNSIGNED_INT, nullptr);
	//particles
	glUseProgram(program[1]);
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	glDrawElements(GL_POINTS, np, GL_UNSIGNED_INT, nullptr);
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
	//program barriers
	glUseProgram(program[0]);
	glUniform1ui(glGetUniformLocation(program[0], "width"), width);
	glUniform1ui(glGetUniformLocation(program[0], "height"), height);
	//program particles
	glUseProgram(program[1]);
	glUniform1ui(glGetUniformLocation(program[1], "width"), width);
	glUniform1ui(glGetUniformLocation(program[1], "height"), height);
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
		add_particle({p1, p2, 0}, {0, 0, 0}, colors[color_index], 0.02);
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
	add_barrier({-0.75, -0.50, 0}, {+0.75, -0.50, 0}, {1, 1, 1});
	add_barrier({-0.75, -0.50, 0}, {-0.75, +0.50, 0}, {1, 1, 1});
	add_barrier({+0.75, -0.50, 0}, {+0.75, +0.50, 0}, {1, 1, 1});
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
	//cleanup
	cleanupGL();
	//return
	return EXIT_SUCCESS;
}