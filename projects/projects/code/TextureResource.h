#pragma once
#include "core/app.h"
#include "config.h"
#include "stb_image.h"
#include "Vector4D.h"

struct pixel {
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    pixel(unsigned char r, unsigned char g, unsigned char b) {
        red = r;
        green = g;
        blue = b;
    }
    pixel() {

    }
};
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