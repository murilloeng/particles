//std
#include <string>
#include <cstdio>
#include <cstdlib>

//ext
#include "external/cpp/inc/GL/glew.h"
#include "external/cpp/inc/GL/freeglut.h"

//data
GLuint program;

//shaders
bool load_file(std::string& source, const char* path)
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
GLuint compile_shader(GLenum type, const char* source)
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
GLuint create_program(const char* path_vertex, const char* path_fragment)
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
void setupGL(void)
{
	//data
	GLuint VAO, VBO, IBO;
	const float vbo_data[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};
	const unsigned ibo_data[] = {
		0, 1, 2
	};
	//create
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenVertexArrays(1, &VAO);
	//bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);
	//layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (unsigned*) (0 * sizeof(float)));
	//program
	program = create_program("shd/base.vert", "shd/base.frag");
}


//callbacks
void callback_display(void)
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	//buffers
	glutSwapBuffers();
}
void callback_reshape(int width, int height)
{
	//viewport
	glViewport(0, 0, width, height);
	//uniforms
	glUniform1ui(glGetUniformLocation(program, "width"), width);
	glUniform1ui(glGetUniformLocation(program, "height"), height);
	//redraw
	glutPostRedisplay();
}
void callback_keyboard(unsigned char key, int x1, int x2)
{
	if(key == 27)
	{
		glutDestroyWindow(glutGetWindow());
	}
}

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
	glutCreateWindow("Canvas");
	//glew
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error: can't setup glew!\n");
		exit(EXIT_FAILURE);
	}
	setupGL();
	//callbacks
	glutDisplayFunc(callback_display);
	glutReshapeFunc(callback_reshape);
	glutKeyboardFunc(callback_keyboard);
	// glutIdleFunc(callback_idle);
	// glutMouseFunc(callback_mouse);
	// glutMotionFunc(callback_motion);
	// glutSpecialFunc(callback_special);
	// glutMouseWheelFunc(callback_wheel);
	//loop
	glutMainLoop();
	//return
	return EXIT_SUCCESS;
}