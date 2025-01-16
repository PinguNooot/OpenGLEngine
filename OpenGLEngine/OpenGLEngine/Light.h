#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	//initializes the light with custom shadow dimensions, color, and intensity.
	Light(GLuint shadowWidth, GLuint shadowHeight, 
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat aIntensity, GLfloat dIntensity);
	~Light();
		
	//Retrieves the shadow map associated with this light.
	ShadowMap* getShadowMap() { return shadowMap; }

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap; // Pointer to the shadow map associated with this light
};

