#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	// Initializes the camera with parameter ( Start Position, orientation (up vector, yaw, pitch) and movement/turning speed).
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	// Handles camera movement based on key inputs and time elapsed
	void keyControl(bool* keys, GLfloat deltaTime);
	// Handles camera rotation based on mouse input
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition(); 		// Retrieves the current camera position in WS (World Space)
	glm::vec3 getCameraDirection();		// Retrieves the current camera direction

	glm::mat4 calculateViewMatrix();	// Calculates the camera's view matrix, which is used for rendering from the camera's perspective.


private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update(); // Update camera values at runtime.
};

