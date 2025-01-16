#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
}

ShadowMap::~ShadowMap()
{
	// Free the ressource, delete the framebuffer object.
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	// Free the ressource, delete shadow map texture.
	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}

// Initializes the shadow map by creating the FBO and shadow map texture.
bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth = width; 
	shadowHeight = height;

	// Generate a framebuffer object.
	glGenFramebuffers(1, &FBO);

	// Generate a texture to hold the shadow map.
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	// Create a 2D depth texture for the shadow map.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Set texture parameters for the shadow map.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Set the border color for the texture (used in case of out-of-bounds access).
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Bind the framebuffer and attach the shadow map texture to it.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	// Disable the color attachment, as we're only using depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check the framebuffer status to ensure it was correctly created.
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
		return false;
	}

	return true;
}

// Binds the framebuffer so that we can write to the shadow map.
void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

// Binds the shadow map texture to a texture unit so that we can read from it.
void ShadowMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}


