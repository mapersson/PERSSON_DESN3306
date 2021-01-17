// Lecture_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> //controls input/output streams
#include <GL\glew.h>  //controls basic opengl functionality
#include <GLFW\glfw3.h> //controls GUI window systems, input (keyboard, mouse, trackpad etc), and output


using namespace std;

// Determines OpenGL Window dimensions
const GLint WIDTH = 2400, HEIGHT = 1800; //this will vary depending on your computer

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


	while (!glfwWindowShouldClose(mainWindow)) 	// Loop until main window is closed
	{

		glfwPollEvents(); 		// Get/set/control user input events
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear window, here clear color is Blue
		glClear(GL_COLOR_BUFFER_BIT); // Clear the memory buffer

		glfwSwapBuffers(mainWindow); //Swap buffers, OpenGL main tains two Buffers, One is displayed, one is getting prepared
	}

	return 0;
}

