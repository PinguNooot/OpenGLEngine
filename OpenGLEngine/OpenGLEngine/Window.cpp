#include "Window.h"

Window::Window()
{
	width  = 800;	// Default window width
	height = 600;   // default window height

	// Initialize the keys array (used for tracking key states)
	for(size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	
	xChange = 0.0f;
	yChange = 0.0f;
}

// Parameterized constructor that allows specifying the window size
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

// Initializes GLFW, creates a window, and sets up OpenGL context
int Window::Initialise()
{
	// Initialize GLFW library
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Set GLFW window properties (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);						// Set OpenGL major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					    // Set OpenGL minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// Set core profile (no deprecated OpenGL features)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);				// Allow forward compatibility for macOS

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "NOOT - OpenGLEngine", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Set up key and mouse input handling
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     // Hide mouse cursor and make it relative to the window

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	// Initialize GLEW to load OpenGL extensions
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth testing for 3D rendering
	glEnable(GL_DEPTH_TEST);

	// Set up the Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Set user pointer for the window 
	glfwSetWindowUserPointer(mainWindow, this);
}

// Set up GLFW callbacks for key and mouse input
void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);				 // Set the key callback to handle key events
	glfwSetCursorPosCallback(mainWindow, handleMouse);		 // Set the mouse callback to handle mouse movement events
}

// Return the change in mouse x position since the last frame and reset it
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

// Return the change in mouse y position since the last frame and reset it
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	// Get the Window object associated with the GLFW window
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// If the ESC key is pressed, close the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// If a key is pressed or released, update the keys array
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

// GLFW callback function to handle mouse movement events
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	// Get the Window object associated with the GLFW window
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// If this is the first mouse movement, store the current position
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	// Calculate the change in mouse position since the last frame
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	// Update the last known mouse position
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


