#include "Skybox.h"
#include "ResourcesPaths.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

// Takes a list of file paths for the cube map textures(6 faces)
Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// Shader Setup: Create and compile shaders for the skybox
	skyShader = new Shader();
	skyShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

	// Get the uniform locations for the projection and view matrices
	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();


	// Texture Setup
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, bitDepth;

	// Loop through each of the 6 faces of the cube map (positive X, negative X, positive Y, negative Y, positive Z, negative Z)
	for (size_t i = 0; i < 6; i++)
	{
		// Load the texture data for each face
		unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}

		// Set the cube map face with the loaded texture data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	// Set texture parameters for wrapping and filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Mesh Setup : Define the skybox geometry (vertices and indices for a cube)
	unsigned int skyboxIndices[] = {
		// front face
		0, 1, 2,
		2, 1, 3,
		// right face
		2, 3, 5,
		5, 3, 7,
		// back face
		5, 7, 4,
		4, 7, 6,
		// left face
		4, 6, 0,
		0, 6, 1,
		// top face
		4, 0, 5,
		5, 0, 2,
		// bottom face
		1, 6, 3,
		3, 6, 7
	};

	// Define the vertex positions for the cube
	float skyboxVertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	// Create the mesh for the skybox using the defined vertices and indices
	skyMesh = new Mesh();
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

// Draw the skybox, takes the view and projection matrices as arguments
void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	// Remove the translation components from the view matrix to prevent moving the skybox with the camera
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	// Disable depth writing so the skybox won't overwrite other objects
	glDepthMask(GL_FALSE);

	// Use the skybox shader
	skyShader->UseShader();

	// Pass the projection and view matrices to the shader
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Activate the texture unit for the cube map and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	// Validate the shader program 
	skyShader->Validate();

	// Render the skybox mesh
	skyMesh->RenderMesh();

	// Re-enable depth writing after rendering the skybox
	glDepthMask(GL_TRUE);
}

void Skybox::UpdateSkyboxTextures(SkyboxType type)
{
	// Get the new texture paths based on the skybox type
	std::vector<const char*> faceLocations = GetSkyboxPaths(type);

	// Bind the existing cube map texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, bitDepth;

	// Loop through each of the 6 faces of the cube map (right, left, top, down, back, front)
	for (size_t i = 0; i < 6; i++)
	{
		// Load the texture data for each face
		unsigned char* texData = stbi_load(faceLocations[i], &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to load texture: %s\n", faceLocations[i]);
			return; // Exit early if texture loading fails
		}

		// Update the cube map face with the new texture data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

		// Free the image data after loading
		stbi_image_free(texData);
	}

	// Optionally, reapply texture parameters to ensure they are still set correctly
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	printf("Skybox textures updated.\n");
}

std::vector<const char*> Skybox::GetSkyboxPaths(SkyboxType type)
{
	switch (type)
	{
	case SkyboxType::StylizedCloud1:
		return {
			Textures::Skybox::StylizedCloud1::RIGHT,
			Textures::Skybox::StylizedCloud1::LEFT,
			Textures::Skybox::StylizedCloud1::TOP,
			Textures::Skybox::StylizedCloud1::DOWN,
			Textures::Skybox::StylizedCloud1::BACK,
			Textures::Skybox::StylizedCloud1::FRONT
		};
	case SkyboxType::StylizedCloudNight:
		return {
			Textures::Skybox::StylizedCloudNight::RIGHT,
			Textures::Skybox::StylizedCloudNight::LEFT,
			Textures::Skybox::StylizedCloudNight::TOP,
			Textures::Skybox::StylizedCloudNight::DOWN,
			Textures::Skybox::StylizedCloudNight::BACK,
			Textures::Skybox::StylizedCloudNight::FRONT
		};
	case SkyboxType::Cupertin:
		return {
			Textures::Skybox::Cupertin::RIGHT,
			Textures::Skybox::Cupertin::LEFT,
			Textures::Skybox::Cupertin::TOP,
			Textures::Skybox::Cupertin::DOWN,
			Textures::Skybox::Cupertin::BACK,
			Textures::Skybox::Cupertin::FRONT
		};
	case SkyboxType::Minecraft:
		return {
			Textures::Skybox::Minecraft::RIGHT,
			Textures::Skybox::Minecraft::LEFT,
			Textures::Skybox::Minecraft::TOP,
			Textures::Skybox::Minecraft::DOWN,
			Textures::Skybox::Minecraft::BACK,
			Textures::Skybox::Minecraft::FRONT
		};
	default:
		throw std::invalid_argument("Unknown SkyboxType");
	}
}
