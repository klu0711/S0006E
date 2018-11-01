#pragma once
#include "TextureResource.h"
#include <cassert>
#include <iostream>

TextureResource::TextureResource()
{
}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
}
/// Load textures from a file using the stb_image library
/// The function also sets the parameters for the textures and clears the cpu side after it's done
void TextureResource::loadFromFile(const char * filename)
{
	int width, heigth, numComponents;
	stbi_set_flip_vertically_on_load(1);
	unsigned char*  imageData = stbi_load(filename, &width, &heigth, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Texture loading failed at: " << filename << std::endl;
	}
	 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);

	
}
void TextureResource::loadFromArray(pixel* buffer, int width, int heigth)
{
	if (buffer == NULL)
	{
		std::cerr << "Texture loading failed "<< std::endl;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(buffer);
}
/// choose and active texture to use for drawing textures from
void TextureResource::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}
