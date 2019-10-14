#include "Shader.h"

Shader::Shader()
{
	//this->program = glCreateProgram();
}

Shader::~Shader()
{
}
/// Create the program handle
void Shader::createProgram()
{
	this->program = glCreateProgram();
}
/// Load the shader file with the provided filename
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
/// Load the shader file with the provided filename
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
/// Create the vertex shader handle and compile the shader
void Shader::setupVertexShader()
{
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	int length = strlen(this->vertexShader);
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
/// Create the fragment shader handle and compile the shader
void Shader::setupFragmentShader()
{
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	int length = strlen(this->fragmentShader);
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
/// Link the shaders with the program
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
/// Modify a uniform matric in the shaders
void Shader::modifyUniformMatrix(std::string name, float * mat)
{
	unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
	glUniformMatrix4fv(uniform, 1, GL_TRUE, mat);

}
/// Modify a uniform vector in the shader
void Shader::modifyUniformVector(std::string name, Vector4D vec)
{
	unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
	glUniform4fv(uniform,1,vec.getPointer());

}
/// Modify a uniform float in the shader
void Shader::modifyUniformFloat(std::string name, float value)
{
	unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
	glUniform1f(uniform, value);
}
void Shader::modifyUniformInt(std::string name, int value)
{
    unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
    glUniform1i(uniform, value);
}

void Shader::modifyUniformMats(int count, Matrix4* jointMats)
{
    unsigned int uniform = glGetUniformLocation(this->program, "bones");
    glUniformMatrix4fv(uniform, count, GL_TRUE, &jointMats[0][0]);
}
void Shader::useProgram()
{

	glUseProgram(this->program);

}
