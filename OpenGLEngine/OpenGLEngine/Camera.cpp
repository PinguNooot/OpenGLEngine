#include "Camera.h"
#include "KeyMappings.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

// Handles camera movement based on key inputs and time elapsed
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;  // Base movement velocity based on frame time (deltaTime).

    // Handle speed up camera
    if (keys[KeyMappings::SPRINT])
    {
        velocity *= 2.0f;
    }

    // Handle slow down Camera speed
    if (keys[KeyMappings::CROUCH])
    {
        velocity *= 0.5f;
    }

    // Handle movement
    if (keys[KeyMappings::FORWARD] || keys[KeyMappings::FORWARD_ALT])
    {
        position += front * velocity;
    }

    if (keys[KeyMappings::BACKWARD] || keys[KeyMappings::BACKWARD_ALT])
    {
        position -= front * velocity;
    }

    if (keys[KeyMappings::LEFT] || keys[KeyMappings::LEFT_ALT])
    {
        position -= right * velocity;
    }

    if (keys[KeyMappings::RIGHT] || keys[KeyMappings::RIGHT_ALT])
    {
        position += right * velocity;
    }

    if (keys[KeyMappings::UP])
    {
        position += up * velocity;
    }

    if (keys[KeyMappings::DOWN])
    {
        position -= up * velocity;
    }
}

// Handles camera rotation based on mouse input
void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
