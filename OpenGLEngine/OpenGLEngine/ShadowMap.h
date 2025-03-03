#pragma once

#include <stdio.h>

#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	// Initializes the shadow map with a specific width and height.
	virtual bool Init(unsigned int width, unsigned int height);
	// Binds the framebuffer to write depth data to the shadow map.
	virtual void Write();
	// Binds the shadow map texture to the specified texture unit for use in shaders.
	virtual void Read(GLenum TextureUnit);

	// Getter methods to retrieve the width and height of the shadow map.
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

protected:
	GLuint FBO, shadowMap; // FBO => FrameBuffer Object
	GLuint shadowWidth, shadowHeight;
};