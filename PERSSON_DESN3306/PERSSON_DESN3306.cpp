// Lecture_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> //controls input/output streams
#include <GL\glew.h>  //controls basic opengl functionality
#include <GLFW\glfw3.h> //controls GUI window systems, input (keyboard, mouse, trackpad etc), and output


using namespace std;

// Determines OpenGL Window dimensions
const GLint WIDTH = 2400, HEIGHT = 1800; //this will vary depending on your computer

GLuint VBO, VAO;
GLuint shader;

// Vertex Shader code based on GLSL version 3.30
//A vertex is composed by of or several attributes
//(positions, normals, texture coordinates etc.).
//In the vertex shader, the line with layoutand position is just saying 
//"get the attribute 0 and put it in a variable called pos" 
//(the location represents the number of the attribute)

// Make the Shader code globally available

static const char* vShader = "                             \n\
#version 330                                               \n\
														   \n\
layout (location = 0) in vec3 pos;			               \n\
														   \n\
void main()                                                \n\
{                                                          \n\
	gl_Position = vec4(0.9*pos.x, 0.9*pos.y, pos.z, 1.0);  \n\
}";

// The Fragment Shader for version 3.30
static const char* fShader = "                             \n\
#version 330                                               \n\
														   \n\
out vec4 colour;                                           \n\
														   \n\
void main()                                                \n\
{                                                          \n\
	colour = vec4(1.0, 1.0, 0.0, 1.0);                     \n\
} ";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);
	
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);

}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,1.0f,
		
		1.0f,1.0f,
		1.0f,-1.0f,
		-1.0f,-1.0f,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


int main()
{
	// Initialise GLFW
	if (!glfwInit())  //initializes OpenGL Window System
	{
		printf("GLFW initialisation failed!");
		glfwTerminate(); //terminate due to error
		return 1;
	}

	// Setup GLFW window properties and version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Major and minor version is 3.X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile means No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 	// Allow Forward Compatbility

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Welcome to DESN3306!", NULL, NULL); // Create Main Window
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate(); //terminate due to error
		return 1;
	}
	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); //bufferwidth and bufferHeight is system dependent so, load them.
	glfwMakeContextCurrent(mainWindow); // Set mainWindow as context for GLEW
	glewExperimental = GL_TRUE; 	// Allow modern features

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);  //clear resources already allocated
		glfwTerminate();  //terminate due to error
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight); // Setup the Viewport dimension

	CreateTriangle();

	CompileShaders();


	while (!glfwWindowShouldClose(mainWindow)) 	// Loop until main window is closed
	{

		glfwPollEvents(); 		// Get/set/control user input events
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear window, here clear color is Blue
		glClear(GL_COLOR_BUFFER_BIT); // Clear the memory buffer

		glUseProgram(shader);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0); 
		glUseProgram(0);


		glfwSwapBuffers(mainWindow); //Swap buffers, OpenGL main tains two Buffers, One is displayed, one is getting prepared
	}

	return 0;
}

