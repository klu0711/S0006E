#include "Shader.h"

void Shader::loadVertexShader(const char * filename)
{
	std::ifstream inFile;
	inFile.open(filename);

	if (inFile.fail())
	{
		std::cout << "File failed to load" << std::endl;
		exit(1);
	}
	else {
		std::stringstream buffer;
		buffer << inFile.rdbuf();
		std::string shader = buffer.str();

		vertexShader = shader.c_str();
		setupVertexShader();
	}


}

void Shader::loadFragmentShader(const char * filename)
{
	std::ifstream inFile;
	inFile.open(filename);

	if (inFile.fail())
	{
		std::cout << "File failed to load" << std::endl;
		exit(1);
	}
	else {
		std::stringstream buffer;
		buffer << inFile.rdbuf();

		std::string shader = buffer.str();
		fragmentShader = shader.c_str();
		setupFragmentShader();

	}
		

}

void Shader::setupVertexShader()
{
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	int length = std::strlen(this->vertexShader);
	glShaderSource(vertexShaderHandle, 1, &this->vertexShader, &length);
	glCompileShader(vertexShaderHandle);

	int shaderLogSize;
	glGetShaderiv(this->vertexShaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->vertexShaderHandle, shaderLogSize, NULL, buf);
		printf(buf);
		delete[] buf;
	}
	//delete vertexShader;
}

void Shader::setupFragmentShader()
{
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	int length = std::strlen(this->fragmentShader);
	glShaderSource(fragmentShaderHandle, 1, &this->fragmentShader, &length);
	glCompileShader(fragmentShaderHandle);

	// get error log
	int shaderLogSize;
	glGetShaderiv(this->fragmentShaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->fragmentShaderHandle, shaderLogSize, NULL, buf);
		printf(buf);
		delete[] buf;
	}
	//delete fragmentShader;
}

void Shader::linkShaders()
{

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShaderHandle);
	glAttachShader(this->program, fragmentShaderHandle);
	glLinkProgram(this->program);

	int shaderLogSize;
	glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
		printf(buf);
		delete[] buf;
	}
}

void Shader::modifyUniformMatrix(std::string name, float * mat)
{
	unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
	glUniformMatrix4fv(uniform, 1, GL_TRUE, mat);

}

void Shader::modifyUniformVector(std::string name, Vector4D vec)
{
	unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
	glUniform4fv(uniform,1,vec.getPointer());

}

void Shader::useProgram()
{

	glUseProgram(this->program);

}
