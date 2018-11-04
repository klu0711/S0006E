#pragma once
#include "core/app.h"
#include "config.h"
#include "stb_image.h"
#include "Vector4D.h"

class TextureResource {
private:
	uint32 texture;
	unsigned char*  imageData;
	int width;
	int heigth;
	int numComponents;
public:
	TextureResource();
	~TextureResource();

	void loadFromFile(const char* filename);
	void loadBuffer();
	void loadFromArray(pixel* buffer, int width, int height);
	float* getColor(float x, float y);
	int getWidth();
	int getHeigth();



	void bind(unsigned int unit);



};