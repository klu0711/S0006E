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
	
	stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load(filename, &width, &heigth, &numComponents, 3);

	if (imageData == NULL)
	{
		std::cerr << "Texture loading failed at: " << filename << std::endl;
	}
	 
/*	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(imageData);*/

	
}

/// Load a texture buffer to the gpu and allocates memory for it
void TextureResource::loadBuffer()
{


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureResource::loadFromArray(pixel* buffer, int w, int h)
{


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);

	
}


float* TextureResource::getColor(float x, float y)
{
	//std::cout << "Functions recvd " << x << ":" << y << std::endl;
	if(x > 1)
	{
		x = 1;
	}
	else if (x < 0)
	{
		x = 0;
	}
	if (y > 1)
	{
		y = 1;
	}
	else if (y < 0)
	{
		y = 0;
	}
	int xCord = width * x;
	int yCord = (heigth - 1) * y;
	/// Calculate the uv coordiante based on the number of components ex (rgb, rgba) and based on thw width and height of the image;
	int uvCoordinate =(xCord * numComponents) + (yCord * width * numComponents);
	float rgb[] = 
	{
		imageData[uvCoordinate++],
		imageData[uvCoordinate++],
		imageData[uvCoordinate++]
	};
	return rgb;
}

int TextureResource::getWidth()
{
	return width;
}

int TextureResource::getHeigth()
{
	return heigth;
}

/// choose and active texture to use for drawing textures from
void TextureResource::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}
