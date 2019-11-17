#include "debugQuad.h"
#include "core/app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
float buffer[] = {
        -0.5f,	-0.5f,	0,			// pos 0
        1,		0,		0,		1,	// color 0
        -0.5f,	0.5f,	0,			// pos 1
        0,		1,		0,		1,	// color 0
        0.5f,	0.5f,	0,			// pos 2
        0,		0,		1,		1,
        0.5f,   -0.5f,   0,
        1,       0,      1,     1// color 0
};

int iBuffer[] =
        { 2,1,0,
          3,2,0};
debugQuad::debugQuad()
{


    this->vertexBuffer = buffer;

    this->indexBuffer = iBuffer;
    this->sizeBuffer = sizeof(buffer);
    this->sizeIndexBuffer = sizeof(iBuffer);

}

debugQuad::~debugQuad()
{

}

void debugQuad::init(const char *vertexShader, const char *fragmentShader)
{

    //Create program handle
    this->program = glCreateProgram();
    this->loadVertexShader(vertexShader);
    this->loadFragmentShader(fragmentShader);
    this->linkShaders();

    //Create VAO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    //Bind vertex buffer
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, vertexBuffer, GL_STATIC_DRAW);
    //Bind index buffer
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexBuffer, indexBuffer, GL_STATIC_DRAW);
    //Bind attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Unbind all the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void debugQuad::loadVertexShader(const char* vertexShader)
{    //Load vertex Shader

    std::ifstream inFile;
    inFile.open("lineShader.ver");

    if (inFile.fail())
    {
        std::cout << "File failed to load" << std::endl;
        exit(1);
    }
    else {
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string shader = buffer.str();
        this->vertexShader = (shader.c_str());

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

    }
    inFile.close();
}

void debugQuad::loadFragmentShader(const char* fragmentShader)
{
    std::ifstream inFile;
    inFile.open(fragmentShader);

    if (inFile.fail())
    {
        std::cout << "File failed to load" << std::endl;
        exit(1);
    }
    else {
        std::stringstream buffer;
        buffer << inFile.rdbuf();

        std::string shader = buffer.str();
        this->fragmentShader = (shader.c_str());

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
    }

}

void debugQuad::linkShaders()
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

void debugQuad::addQuad(vec4 position, vec4 scale, mat4 rotation)
{

    mat4 positionMat = mat4::getPositionMatrix(position);
    mat4 scaleMat = mat4::scaleMat(scale);
    mat4 rotMat = mat4();
    quad q;
    q.transform = mat4::transpose(positionMat) * scaleMat;
    //q.transform[15] = 0;
    q.v0 = vec4(-0.5f,	-0.5f,	0,1);
    q.v1 = vec4( -0.5f,	0.5f,	0,1);
    q.v2 = vec4( 0.5f,	0.5f,	0,1);
    q.v3 = vec4( 0.5f,	-0.5f,	0,1);
    vec4 v = vec4(vertexBuffer[7] - vertexBuffer[0], vertexBuffer[8] - vertexBuffer[1], vertexBuffer[9] - vertexBuffer[2], 1);
    vec4 u = vec4(vertexBuffer[14] - vertexBuffer[0], vertexBuffer[15] - vertexBuffer[1], vertexBuffer[16] - vertexBuffer[2], 1);

    vec4 normal(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0], 1);
    q.quadPlane.normal = normal;
    q.quadPlane.point = position;
    this->quads.push_back(q);


}


void debugQuad::bindAttrPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
}

void debugQuad::draw(mat4 transform)
{

    glUseProgram(this->program);
    glBindVertexArray(this->VAO);
    unsigned int uniform = glGetUniformLocation(this->program, "transform");
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &transform[0]);
    uint uniformColor = glGetUniformLocation(this->program, "color");
    glUniform4fv(uniformColor, 1, vec4(1,0,0,1).getPointer());
    for (int i = 0; i < quads.size(); ++i)
    {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &((quads[i].transform) * transform)[0]);
        glDrawElements(GL_TRIANGLES, this->sizeIndexBuffer, GL_UNSIGNED_INT, 0);

    }
    glBindVertexArray(0);

}