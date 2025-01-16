#pragma once
#include "Light.h"

#include <vector>

#include "OmniShadowMap.h"

class PointLight :
	public Light
{
public:
	PointLight();
	// Initialize a PointLight with shadow width/height,light properties (color, intensity), position, and attenuation factors
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);
	~PointLight();

	// Send light properties to shaders (ambient and diffuse intensity, position, attenuation factors)
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();	// Calculate and return the light's transformation matrix for multiple faces
	GLfloat GetFarPlane();								// Get the far clipping plane for the shadow map 
	glm::vec3 GetPosition();							// Get the position of the point light in the 3D space.
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
	GLfloat farPlane;
};

