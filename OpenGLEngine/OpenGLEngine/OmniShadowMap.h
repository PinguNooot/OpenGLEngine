#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
	public ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap();

	// Initializes the omni - directional shadow map with the specified width and height.
	bool Init(unsigned int width, unsigned int height);

	// Binds the shadow map for rendering and prepares it for writing depth information.
	void Write();
	// Reads the shadow map texture from the specified texture unit.
	void Read(GLenum TextureUnit);

};

