#pragma once

#include <GL\glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);
	~Texture();

	// load the texture from a file
	bool LoadTexture();
	// load the texture with alpha (transparency) support
	bool LoadTextureA();

	// Bind the texture to OpenGL
	void UseTexture();
	// Clear the texture (unbind and free resource)
	void ClearTexture();


private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

