#pragma once
#include "core/app.h"
#include "config.h"
#include "stb_image.h"

class TextureResource {
private:
	uint32 texture;
public:
	TextureResource();
	~TextureResource();

	void loadFromFile(const char* filename);
	void bind(unsigned int unit);

};