// Lecture_06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> //controls input/output streams
#include <GL\glew.h>  //controls basic opengl functionality
#include <GLFW\glfw3.h> //controls GUI window systems, input (keyboard, mouse, trackpad etc), and output
#include<conio.h>


using namespace std;

// Determines OpenGL Window dimensions
const GLint WIDTH = 2400, HEIGHT = 1800; //this will vary depending on your computer
GLuint VBO, VAO; //declare globally
GLuint shader; //use global declaration
GLuint uniformXMoveLoc, uniformYMoveLoc, uniformVColorLoc;



// Vertex Shader code based on GLSL version 3.30
// Make the Shader code globally available
static const char* vShader = "                                                \n\
#version 330                                                                  \n\                                                                            \n\
layout (location = 0) in vec3 pos;			                                  \n\
out vec4 myColor;                                                             \n\
uniform float xMove;                                                          \n\
uniform float yMove;                                                          \n\
uniform vec4 vColor;                                                          \n\
																			  \n\
void main()                                                                   \n\
{                                                                             \n\
	gl_Position = vec4(0.5*pos.x + xMove, 0.5*pos.y + yMove, pos.z, 1.0);	  \n\
	myColor=vColor;                                               \n\
	//myColor=vec4(1.0,1.0,0.0,1.0);                                            \n\
}";


// The Fragment Shader for version 3.30
static const char* fShader = "                                                 \n\
#version 330                                                                   \n\
																			   \n\
out vec4 color;                                                               \n\
in vec4 myColor;                                                               \n\
																			   \n\
void main()                                                                    \n\
{                                                                              \n\
	  color=myColor;                                                             \n\
}";  

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
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
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	//get the memory reference for uniform variables
	uniformXMoveLoc = glGetUniformLocation(shader, "xMove");
	uniformYMoveLoc = glGetUniformLocation(shader, "yMove");
	uniformVColorLoc= glGetUniformLocation(shader, "vColor");
}

void CreateTriangle(){

	GLfloat vertices[] = {     
			-1.0f, -1.0f,
			1.0f, -1.0f,
			0.0f, 1.0f,
			-1.0f, 1.0f,
			-0.25f, 1.0f,
			-1.0f, 0.0f
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


float getRandomColor()
{
	int x=rand();
	return x / 32767.0f;
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

	
	int sign = -1;
	int xUpdate = 3;
	int i = 1;
	int j = 1;
	int yUpdate = 4;

	int signNew = -1;
	int xUpdateNew = 2;
	int iNew = 1;
	int jNew = 1;
	int yUpdateNew = 3;


	float o1R=0.0f, o1G=0.0f, o1B=1.0f;
	float o2R=0.0f, o2G=1.0f, o2B=0.0f;


	while (!glfwWindowShouldClose(mainWindow)) 	// Loop until main window is closed
	{

		glfwPollEvents(); 		// Get/set/control user input events
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear window, here clear color is Blue
		glClear(GL_COLOR_BUFFER_BIT); // Clear the memory buffer

		glUseProgram(shader);

		
		
		if (i <= 0 || i >= 5000)
		{
			xUpdate *= sign;
			o1R = getRandomColor();
			o1G = getRandomColor();
			o1B = getRandomColor();
		}
		i += xUpdate;
		
		if (j <= 0 || j >= 3000)
		{
			yUpdate *= sign;
			o1R = getRandomColor();
			o1G = getRandomColor();
			o1B = getRandomColor();
		}
		j += yUpdate;




		if (iNew <= 0 || iNew >= 5000)
		{
			xUpdateNew *= signNew;
			o2R = getRandomColor();
			o2G = getRandomColor();
			o2B = getRandomColor();
		}
		iNew += xUpdateNew;


		if (jNew <= 0 || jNew >= 3000)
		{
			yUpdateNew *= signNew;
			o2R = getRandomColor();
			o2G = getRandomColor();
			o2B = getRandomColor();
		}
		jNew += yUpdateNew;
		

		glUniform1f(uniformXMoveLoc, (i-2500)/2500.0f);
		glUniform1f(uniformYMoveLoc, (j - 1500) / 1500.0f);		
		glUniform4f(uniformVColorLoc, o1R, o1G, o1B, 1.0);

		/*
		//uncomment to read value from uniform variables.
		GLfloat ValueFromUniformShaderX;
		GLfloat ValueFromUniformShaderY;

		if (_kbhit())
		{
			glGetUniformfv(shader, uniformXMoveLoc, &ValueFromUniformShaderX);
			glGetUniformfv(shader, uniformYMoveLoc, &ValueFromUniformShaderY);
			cout << "Current Uniform Value: X=" << ValueFromUniformShaderX<<" ";
			cout << ", Y=" << ValueFromUniformShaderY <<endl;
			_getch();

		}
		*/
		

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);





		glUniform1f(uniformXMoveLoc, (iNew - 2500) / 2500.0f);
		glUniform1f(uniformYMoveLoc, (jNew - 1500) / 1500.0f);
		glUniform4f(uniformVColorLoc, o2R, o2G, o2B, 1.0);
		glDrawArrays(GL_TRIANGLES, 3, 3);



		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow); //Swap buffers, OpenGL main tains two Buffers, One is displayed, one is getting prepared

	}

	return 0;
}
