#pragma once
#include <iostream>
#include "core/app.h"
#include <string>

class Shader {
private:
	GLchar* vertexShader;

public:
	void loadVertexShader();
	void loadFragmentShader();
};