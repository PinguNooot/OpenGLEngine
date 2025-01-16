#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	// Parameterized constructor to initialize a SpotLight with specific values, including shadow properties
	SpotLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);
	~SpotLight();

	// Sets the light properties in the shader (spotlight-specific attributes like direction and edge angle)
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	// Allows dynamic change of the spotlight's position and direction
	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	// Toggles the spotlight's state between on and off
	void Toggle() { isOn = !isOn; }
private:
	glm::vec3 direction;
	GLfloat edge, procEdge;
	bool isOn;
};

