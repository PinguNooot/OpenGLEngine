#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();
	~Model();

	// Loads a model from a specified file
	void LoadModel(const std::string& fileName);
	// Renders the model by rendering each mesh with its associated texture
	void RenderModel();
	// Clears loaded data (like meshes and textures) 
	void ClearModel();
private:
	// Recursive function to process nodes in the scene graph
	void LoadNode(aiNode *node, const aiScene *scene);
	// Processes individual meshes in the scene
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	// Loads materials (textures) from the scene and maps them to meshes
	void LoadMaterials(const aiScene *scene);

	std::vector<Mesh*> meshList;				// Hold all meshes loaded from the model file
	std::vector<Texture*> textureList;			// Hold all textures (e.g., diffuse, specular) used by the model
	std::vector<unsigned int> meshToTex;		// Mapping of mesh indices to corresponding texture indices
};

