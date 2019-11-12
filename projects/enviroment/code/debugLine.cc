#include "debugLine.h"
#include "core/app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>


debugLine::debugLine(const char *vertexShader,const char *fragmentShader)
{
    //Create program handle
    this->program = glCreateProgram();
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    this->loadVertexShader(vertexShader);
    this->loadFragmentShader(fragmentShader);
    this->linkShaders();


}

debugLine::~debugLine()
{

}

void debugLine::loadVertexShader(const char* vertexShader)
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

void debugLine::loadFragmentShader(const char* fragmentShader)
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

void debugLine::linkShaders()
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

void debugLine::addLine(vec4 p1, vec4 p2)
{
    points.push_back(p1[0]);
    points.push_back(p1[1]);
    points.push_back(p1[2]);

    points.push_back(p2[0]);
    points.push_back(p2[1]);
    points.push_back(p2[2]);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW );

}

void debugLine::draw(mat4 perspective, mat4 view)
{
    glUseProgram(this->program);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);

    unsigned int uniform = glGetUniformLocation(this->program, "projection");
    glUniformMatrix4fv(uniform, 1, GL_TRUE, &perspective[0]);
    uniform = glGetUniformLocation(this->program, "view");
    glUniformMatrix4fv(uniform, 1, GL_TRUE, &view[0]);
    uniform = glGetUniformLocation(this->program, "model");
    glUniformMatrix4fv(uniform, 1, GL_TRUE, &mat4()[0]);

    for (int i = 0; i < points.size()/6; ++i)
    {
        //glDrawArrays(GL_LINES, i*2, i*2 + 2);
    }
    glLineWidth(3.f);
    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}