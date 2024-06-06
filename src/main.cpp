//std
#include <cstdio>
#include <cstdlib>

//ext
#include "external/cpp/inc/GL/glew.h"
#include "external/cpp/inc/GL/freeglut.h"

//callbacks
void callback_display(void)
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//buffers
	glutSwapBuffers();
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
	//callbacks
	glutDisplayFunc(callback_display);
	glutKeyboardFunc(callback_keyboard);
	// glutIdleFunc(callback_idle);
	// glutMouseFunc(callback_mouse);
	// glutMotionFunc(callback_motion);
	// glutReshapeFunc(callback_reshape);
	// glutSpecialFunc(callback_special);
	// glutMouseWheelFunc(callback_wheel);
	//loop
	glutMainLoop();
	//return
	return EXIT_SUCCESS;
}

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>

// // Function to read shader source code from file
// std::string readFile(const char* filePath) {
//     std::ifstream file(filePath);
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     return buffer.str();
// }

// // Function to compile a shader
// GLuint compileShader(GLenum type, const char* source) {
//     GLuint shader = glCreateShader(type);
//     glShaderSource(shader, 1, &source, nullptr);
//     glCompileShader(shader);

//     GLint success;
//     glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//         std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
//     }
//     return shader;
// }

// // Function to create a shader program
// GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
//     std::string vertexCode = readFile(vertexPath);
//     std::string fragmentCode = readFile(fragmentPath);

//     GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
//     GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

//     GLuint shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     GLint success;
//     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//         std::cerr << "Program linking error:\n" << infoLog << std::endl;
//     }

//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }

// int main(void)
// {
// 	//setup GLFW
// 	if(!glfwInit())
// 	{
// 		std::cerr << "Failed to initialize GLFW" << std::endl;
// 		return EXIT_FAILURE;
// 	}
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// 	GLFWwindow* window = glfwCreateWindow(800, 800, "Triangle", nullptr, nullptr);
// 	//window
// 	if(!window)
// 	{
// 		std::cerr << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
// 		return EXIT_FAILURE;
// 	}
// 	// Make the window's context current
// 	glfwMakeContextCurrent(window);

// 	//glew
// 	if(glewInit() != GLEW_OK)
// 	{
// 		std::cerr << "Failed to initialize GLEW" << std::endl;
// 		return EXIT_FAILURE;
// 	}
// 	// Define the vertex data for a triangle
// 	float vertices[] = {
// 		-0.5f, -0.5f, 0.0f,
// 		 0.5f, -0.5f, 0.0f,
// 		 0.0f,  0.5f, 0.0f
// 	};

// 	// Create and bind a Vertex Array Object (VAO)
// 	GLuint VAO;
// 	glGenVertexArrays(1, &VAO);
// 	glBindVertexArray(VAO);

// 	// Create and bind a Vertex Buffer Object (VBO)
// 	GLuint VBO;
// 	glGenBuffers(1, &VBO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	// Specify the layout of the vertex data
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	// Unbind the VAO
// 	glBindVertexArray(0);

// 	// Create the shader program
// 	GLuint shaderProgram = createShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

// 	// Main loop
// 	while (!glfwWindowShouldClose(window)) {
// 		// Clear the screen
// 		glClear(GL_COLOR_BUFFER_BIT);

// 		// Use the shader program
// 		glUseProgram(shaderProgram);

// 		// Bind the VAO
// 		glBindVertexArray(VAO);

// 		// Draw the triangle
// 		glDrawArrays(GL_TRIANGLES, 0, 3);

// 		// Unbind the VAO
// 		glBindVertexArray(0);

// 		// Swap buffers and poll events
// 		glfwSwapBuffers(window);
// 		glfwPollEvents();
// 	}

// 	// Clean up
// 	glDeleteVertexArrays(1, &VAO);
// 	glDeleteBuffers(1, &VBO);
// 	glDeleteProgram(shaderProgram);

// 	glfwTerminate();
// 	return 0;
// }