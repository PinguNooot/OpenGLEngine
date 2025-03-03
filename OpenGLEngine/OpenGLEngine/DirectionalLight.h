#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	// Initializes the directional light with custom shadow dimensions, color, intensity, and direction.
	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
					GLfloat red, GLfloat green, GLfloat blue,
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);
	~DirectionalLight();

	// Applies the light's properties such as ambient intensity, color, diffuse intensity, and direction to the shader program.
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);
	// Calculates the light's transformation matrix, useful for shadow mapping and light-space transformations.
	glm::mat4 CalculateLightTransform();



private:
	glm::vec3 direction; // Light Direction
};

