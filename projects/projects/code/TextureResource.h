#pragma once
#include "core/app.h"
#include "config.h"
#include "stb_image.h"
#include "Renderer.h"

class TextureResource {
private:
	uint32 texture;
	unsigned char*  imageData;
	int width;
	int heigth;
public:
	TextureResource();
	~TextureResource();

	void loadFromFile(const char* filename);
	void loadBuffer();
	void loadFromArray(pixel* buffer, int width, int height);
	pixel* getTextureBuffer();
	int getWidth();
	int getHeigth();



	void bind(unsigned int unit);



};