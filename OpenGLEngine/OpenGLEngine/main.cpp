#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "KeyMappings.h"
#include "ResourcesPaths.h"

#include "Skybox.h"


const float toRadians = 3.14159265f / 180.0f;

// Global variables for storing shader uniform locations
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

// Window object and mesh list
Window mainWindow;
std::vector<Mesh*> meshList;

// List of shaders and shadow shaders
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

// Camera, textures, materials, models, and lights
Camera camera;

Texture brickTexture, dirtTexture, plainTexture;
Material shinyMaterial, dullMaterial;
Model xwing, blackhawk_1, blackhawk_2, mech;
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Skybox skybox;

// Light count and timing variables
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat blackhawkAngle = 0.0f;
GLfloat rotationSpeed = 0.0f;
GLfloat rotationSpeedBlackhawk = 0.0f;
GLfloat rotationSpeedMech = 0.0f;

// Skybox texture faces
std::vector<std::string> skyboxFaces;


// Vertex and Fragment Shader file paths
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

// Calculate average normals for the mesh
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	// Loop through the triangles of the mesh to calculate normal vectors
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		// Calculate two vectors from the vertices of the triangle
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		// Calculate the normal vector using cross product
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		// Add the calculated normal to each vertex
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	// Normalize the normals to unit length
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}
/// Create Sphere, Create Objects and Create Shaders
#pragma region Create Elements

void CreateSphere(float radius, unsigned int rings, unsigned int sectors) 
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

	// Calculate the spherical coordinates
    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);
    int r, s;

	// Generate vertices and texture coordinates
    for (r = 0; r < rings; ++r) {
        for (s = 0; s < sectors; ++s) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);
            vertices.push_back(s * S);
            vertices.push_back(r * R);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
	// Generate indices for the triangles that make up the sphere
    for (r = 0; r < rings - 1; ++r) {
        for (s = 0; s < sectors - 1; ++s) {
            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + s);
            indices.push_back(r * sectors + (s + 1));

            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));
        }
    }

	// Create mesh for the sphere and add it to the mesh list
    Mesh* sphereMesh = new Mesh();
    sphereMesh->CreateMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
    meshList.push_back(sphereMesh);
}


void CreateObjects() 
{
	// Indices for creating the triangles of the object
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Vertices defining positions, texture coordinates, and normals for each vertex
	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	// Indices for creating the floor object
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	// Vertices for the floor, including positions, texture coordinates, and normals
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	// Calculate average normals for the objects
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	// Create and push objects to the mesh list
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	CreateSphere(1.0f, 20, 20);
}

void CreateShaders()
{
	// Create a shader from vertex and fragment files
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	// Create shadow shaders for directional and omnidirectional lights
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

#pragma endregion 

#pragma region Render
// Apply transformation and materials to an object
void ApplyTransformAndMaterial(glm::mat4 model, Texture& texture, Material& material) {
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	texture.UseTexture();
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
}

// Render an object with transformation and materials
void RenderObject(glm::vec3 position, Texture& texture, Material& material, Mesh* mesh) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	ApplyTransformAndMaterial(model, texture, material);
	mesh->RenderMesh();
}

void Render3DModelObject(Model modelInfo,glm::mat4 model, glm::vec3 pos, glm::float32 angle, glm::vec3 axis, glm::vec3 scale)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::rotate(model, angle * toRadians, axis);
	model = glm::scale(model, scale);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	modelInfo.RenderModel();
}


void RenderScene()
{
	glm::mat4 model(1.0f);

	// Rotate Part
	rotationSpeedBlackhawk += 0.1f;
	rotationSpeedMech += 0.05f;

	if (rotationSpeedBlackhawk > 360.0f)
	{
		rotationSpeedBlackhawk = 0.1f;
	}
	if (rotationSpeedMech > 360.0f)
	{
		rotationSpeedMech = 0.05f;
	}

	// Render objects with their transformations, textures, and materials
	RenderObject(glm::vec3(5.0f, 0.0f, -2.5f), brickTexture, shinyMaterial, meshList[0]);
	RenderObject(glm::vec3(-5.0f, 0.0f, -2.5f), dirtTexture,  dullMaterial, meshList[1]);
	RenderObject(glm::vec3(0.0f, -2.0f, 0.0f), dirtTexture,  shinyMaterial, meshList[2]); // Floor
	RenderObject(glm::vec3(-5.0f, 0.0f, 2.5f), plainTexture, shinyMaterial, meshList[3]); 

	////Blackhawk Battle Helicopter
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -rotationSpeedBlackhawk * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, 20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk_1.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::rotate(model, -rotationSpeedBlackhawk * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk_2.RenderModel();

	// X-Wing Model
	//TODO : Fix the X-Wing Model
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	//xwing.RenderModel();

	// Mech Model
	Render3DModelObject(mech, model, glm::vec3(0.0f, -2.0f, 0.0f), rotationSpeedMech, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

#pragma endregion 

#pragma region Initialize 

void CreatePointLights()
{
	pointLights[pointLightCount] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
}


void InitializeSkybox()
{
	
	// Initialize skybox with textures
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::RIGHT);
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::LEFT);
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::TOP);
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::DOWN);
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::BACK);
	skyboxFaces.push_back(Textures::Skybox::StylizedCloudNight::FRONT);

	skybox = Skybox(skyboxFaces);    // Create the Skybox
}

void InitializeModels()
{
	// Load 3D models (e.g., X-wing, Blackhawk)
	xwing = Model();
	xwing.LoadModel(Models::XWING);

	blackhawk_1 = Model();
	blackhawk_1.LoadModel(Models::BLACKHAWK);

	blackhawk_2 = Model();
	blackhawk_2.LoadModel(Models::BLACKHAWK);

	mech = Model();
	mech.LoadModel(Models::MECH);
}
#pragma endregion

#pragma region ShadowMapPass

// Render the shadow map for the directional light (Main Light or directionnal light)
void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	// Set the viewport size based on the shadow map resolution of the directional light
	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	// Write to the shadow map, essentially starting the depth pass
	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Get the model location in the shader
	uniformModel = directionalShadowShader.GetModelLocation();

	// Calculate the transformation matrix for the directional light's shadow
	glm::mat4 directionalLightTransform = light->CalculateLightTransform();

	// Set the directional light's transformation matrix for shadow mapping
	directionalShadowShader.SetDirectionalLightTransform(&directionalLightTransform);

	directionalShadowShader.Validate();

	// Render the scene to populate the shadow map
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Render the shadow map for the omni light (point lights or spotlights)
void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	// Set the viewport based on the shadow map resolution of the point light
	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	// Write to the shadow map (start depth pass)
	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Get the uniform locations for the point light shader
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	// Set the light's position and far plane in the shader
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	// Calculate and set the light transformation matrices
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	// Render the scene to populate the shadow map
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer after rendering
}

#pragma endregion


void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	// Set the viewport for rendering the scene to the main window
	glViewport(0, 0, 1366, 768);

	// Clear the color and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the skybox first, since it's a background
	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	// Use the main shader for rendering models
	shaderList[0].UseShader();

	// Set uniform locations for model transformations, camera view
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	// Pass the camera's projection and view matrices to the shader
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	// Pass the camera position to the shader for lighting calculations
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	// Set up lighting for the scene
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

	// Calculate the directional light's transformation matrix
	glm::mat4 directionalLightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&directionalLightTransform);

	// Set the shadow map for the directional light
	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	// Adjust the spotlights based on camera position
	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();			

	// Render the scene (all models, lights)
	RenderScene();
}



#pragma region UpdateControl
	void SwitchSkybox()
	{
		if (mainWindow.getsKeys()[KeyMappings::SKYBOX_CLOUD])
		{
			skybox.UpdateSkyboxTextures(SkyboxType::StylizedCloud1);
			mainWindow.getsKeys()[KeyMappings::SKYBOX_CLOUD] = false;
		}

		if (mainWindow.getsKeys()[KeyMappings::SKYBOX_CLOUD_NIGHT])
		{
			skybox.UpdateSkyboxTextures(SkyboxType::StylizedCloudNight);
			mainWindow.getsKeys()[KeyMappings::SKYBOX_CLOUD_NIGHT] = false;
		}

		if (mainWindow.getsKeys()[KeyMappings::SKYBOX_CUPERTIN])
		{
			skybox.UpdateSkyboxTextures(SkyboxType::Cupertin);
			mainWindow.getsKeys()[KeyMappings::SKYBOX_CUPERTIN] = false;
		}	
		
		if (mainWindow.getsKeys()[KeyMappings::SKYBOX_MINECRAFT])
		{
			skybox.UpdateSkyboxTextures(SkyboxType::Minecraft);
			mainWindow.getsKeys()[KeyMappings::SKYBOX_MINECRAFT] = false;
		}
	}
#pragma endregion

int main() 
{
	mainWindow = Window(1366, 768);  // Initialize the window with specified dimensions
	mainWindow.Initialise();

	// Create objects and shaders used in the scene
	CreateObjects();
	CreateShaders();

	// Initialize the camera at the origin with a specific direction and parameters
	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	// Load textures for materials and objects
	brickTexture = Texture(Textures::Material::BRICK);
	brickTexture.LoadTextureA();
	dirtTexture = Texture(Textures::Material::DIRT);
	dirtTexture.LoadTextureA();
	plainTexture = Texture(Textures::Material::PLAIN);
	plainTexture.LoadTextureA();

	// Set material properties for shiny and dull materials
	shinyMaterial = Material(4.0f, 256);
	dullMaterial  = Material(0.3f, 4);

	// Initialize Loading Model
	InitializeModels();

	// Initialize main directional light
	mainLight = DirectionalLight(2048, 2048,
								1.0f, 0.53f, 0.3f, 
								0.1f, 0.9f,
								-10.0f, -12.0f, 18.5f);
	// Initialize point lights
	pointLights[0] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 3.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Initialize spotlights
	spotLights[0] = SpotLight(1024, 1024,
						0.01f, 100.0f, 
						1.0f, 1.0f, 1.0f,
						0.0f, 2.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
						0.01f, 100.0f, 
						1.0f, 1.0f, 1.0f,
						0.0f, 1.0f,
						0.0f, -1.5f, 0.0f,
						-100.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	spotLightCount++;

	// Initialize the skybox
	InitializeSkybox();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	// Define the perspective projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime;  // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Toggle spotlight based on user input
		if (mainWindow.getsKeys()[KeyMappings::ENABLE_LIGHT])
		{
			spotLights[0].Toggle();
			mainWindow.getsKeys()[KeyMappings::ENABLE_LIGHT] = false;
		}

		// Update Skybox Renderer
		SwitchSkybox();

		// Perform shadow map rendering for directional and omni lights
		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}

		// Perform the main rendering pass
		RenderPass(camera.calculateViewMatrix(), projection);

		// Swap buffers for the next frame
		mainWindow.swapBuffers();
	}

	return 0;
}