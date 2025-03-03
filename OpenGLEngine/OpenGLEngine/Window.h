#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	~Window();

	Window(GLint windowWidth, GLint windowHeight);

	// Initialize the window and OpenGL context
	int Initialise();

	// Getter functions to retrieve the window buffer dimensions
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	// Check if the window should close (e.g., user closed the window)
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	// Getter for the key state array
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	// Function to swap the buffers and display the updated content
	void swapBuffers() { glfwSwapBuffers(mainWindow); }


private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};