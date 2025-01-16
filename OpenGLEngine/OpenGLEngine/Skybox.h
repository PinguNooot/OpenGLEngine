#pragma once

#include <vector>
#include <string>
#include <map>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"

enum class SkyboxType
{
	StylizedCloud1,
	StylizedCloudNight,
	Cupertin,
	Minecraft
};

class Skybox
{
public:
	Skybox();
	// Takes a vector of strings representing the file paths of the 6 cube map faces
	Skybox(std::vector<std::string> faceLocations);
	~Skybox();
	// Render the skybox
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void UpdateSkyboxTextures(SkyboxType type);
private:
	Mesh* skyMesh;
	Shader* skyShader;

	GLuint textureId;
	GLuint uniformProjection, uniformView;

	static std::vector<const char*> GetSkyboxPaths(SkyboxType type);
};

