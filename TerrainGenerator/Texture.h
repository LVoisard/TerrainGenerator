#pragma once

#include <GL/glew.h>
#include "CommonIncludes.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	void Load();
	void Use();
	void Use(GLenum activeTexture);
	void Clear();

private:
	GLuint ID;
	int width, height, bitDepth;

	const char* fileLocation;
};

