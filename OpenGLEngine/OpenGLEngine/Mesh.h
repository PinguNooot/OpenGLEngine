#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	// Constructor and Destructor
	Mesh();
	~Mesh();

	//Creates a mesh with the given vertices and indices.
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	// Renders the mesh by binding the VAO and issuing the draw call
	void RenderMesh();
	// Frees the allocated GPU resources associated with the mesh
	void ClearMesh();

private:
	GLuint VAO, VBO, IBO; // VAO => Vertex Array Object, VBO => Vertex Buffer Object, IBO => Indice Buffer Object
	GLsizei indexCount;	  // Number of indices used for rendering the mesh
};

