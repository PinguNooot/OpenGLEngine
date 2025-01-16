#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();
	// Initialize Material with parameter
	Material(GLfloat sIntensity, GLfloat shine);
	~Material();

	// Applies the material properties to the shader program by updating the uniform variables
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private: 
	GLfloat specularIntensity;
	GLfloat shininess;
};

