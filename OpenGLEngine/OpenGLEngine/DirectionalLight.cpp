#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

// Applies the light's properties such as ambient intensity, color, diffuse intensity, and direction to the shader program.
DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
									GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	// Sets the direction of the light to the specified (x, y, z) vector.
	direction = glm::vec3(xDir, yDir, zDir);

	// Sets the orthographic projection matrix for the directional light, used for rendering shadows.
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	// Set the ambiant color and intensity of the light in the shader.
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	// Set the direction and the diffuse Intensity of the light in the shader.
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity); 
}

// Calculates the light's transformation matrix, useful for shadow mapping and light-space transformations.
glm::mat4 DirectionalLight::CalculateLightTransform()
{
	// Returns the light's transformation matrix, which is used for shadow mapping.
	// This combines the orthographic projection and the camera view (lookAt) to create a light-space matrix.
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


