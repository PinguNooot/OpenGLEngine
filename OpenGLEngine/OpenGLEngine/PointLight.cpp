#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}



// Initialize a PointLight with shadow width/height,light properties (color, intensity), position, and attenuation factors
PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
						GLfloat near, GLfloat far, 
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
	farPlane = far;

	// Calculating the aspect ratio based on the shadow texture dimensions
	float aspect = (float)shadowWidth / (float)shadowHeight;
	// Defining the light projection matrix (perspective) for the point light
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	
	// Creating and initializing the OmniShadowMap for the point light
	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	// Setting the ambient light color and intensity
	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	// Setting the diffuse intensity of the light
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	// Setting the position and attenuation factors (constant, linear, exponential)
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}

// CalculateLightTransform calculates the light's transformation matrix for each of the six cube map faces
std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;

	// Creating view matrices for each of the six faces of the cube map
	// For each axis (+X, -X, +Y, -Y, +Z, -Z) we generate a lookAt matrix with a different view direction.

	//+x, -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//+z, -z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

// Returns the far clipping plane value for the point light's shadow map
GLfloat PointLight::GetFarPlane()
{
	return farPlane;
}

// Returns the position of the point light in world space
glm::vec3 PointLight::GetPosition()
{
	return position;
}

