#include "Texture.h"

Texture::Texture()
{
	ID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLocation)
{
	ID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->fileLocation = fileLocation;
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!textureData)
	{
		printf("Failed to load the texture at %s\n", fileLocation);
		return;
	}


	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
}

void Texture::Use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Use(GLenum activeTexture)
{
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Clear()
{
	glDeleteTextures(1, &ID);
	ID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}
