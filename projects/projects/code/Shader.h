#pragma once
#include <iostream>
#include "core/app.h"
#include <string>
#include <fstream>
#include <sstream>
#include "Matrix4.h"
#include "Vector4D.h"

	class Shader {
	private:
		const char* vertexShader;
		const char* fragmentShader;
		unsigned int fragmentShaderHandle;
		unsigned int vertexShaderHandle;
		unsigned int program;



	public:
		Shader();
		~Shader();
		void createProgram();
		void loadVertexShader(const char * filename);
		void loadFragmentShader(const char * filename);
		void setupVertexShader();
		void setupFragmentShader();
		void linkShaders();
		void modifyUniformMatrix(std::string name, float* mat);
		void modifyUniformVector(std::string name, Vector4D vec);
		void modifyUniformFloat(std::string name, float value);
		void useProgram();
	};
