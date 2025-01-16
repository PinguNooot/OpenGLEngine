#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

// Render the model: This function loops through each mesh in the model's mesh list,
// applies the associated texture, and renders it.
void Model::RenderModel()
{
	// Iterate over all meshes in the model
	for (size_t i = 0; i < meshList.size(); i++)
	{
		// Get the material index for the current mesh
		unsigned int materialIndex = meshToTex[i];

		// If a valid texture is available for the material, use it
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		// Render the mesh
		meshList[i]->RenderMesh();
	}
}
// Load a model from the given file
// Using Assimp to load the model's data (meshes, textures, materials)
void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

// Recursive function to load nodes in the scene graph
void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	// Process each mesh in the current node
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	// Recursively process each child node
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}
// Load the mesh data (Positions, textures coordinates and indices) and store it in the model
void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// Loop through all vertices and extract their data
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Insert vertex position data (x, y, z)
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		// Insert texture coordinates (if they exist, if not use default value)
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		// Insert normal vector(x, y, z), inverted to match OpenGL convention(Y - up vs.Z - up)
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	// Loop through all faces in the mesh
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create a new Mesh object with the loaded vertex and index data
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);															// Add the new mesh to the list of meshes in the model
	meshToTex.push_back(mesh->mMaterialIndex);												// Add the corresponding texture index to the mesh-to-texture mapping
}

// Load the materials (textures) for the model
void Model::LoadMaterials(const aiScene * scene)
{
	// Resize the texture list to match the number of materials in the scene
	textureList.resize(scene->mNumMaterials);
	
	// Loop through each material in the scene
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		// Check if the material has a diffuse texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Extract the file name from the path
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				// Build the full texture path and load the texture
				std::string texPath = std::string("Textures/") + filename;
				textureList[i] = new Texture(texPath.c_str());

				// Attempt to load the texture, and report if it fails
				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		// If no texture is found for the material, load a default texture
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}
// Clear the model's meshes and textures from memory (Free the ressources).
void Model::ClearModel()
{
	// Delete all meshes from memory
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	// Delete all textures from memory
	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}
