#include "debugQuad.h"
#include "core/app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>


debugQuad::debugQuad()
{



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
    glBindVertexArray(0);

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

void debugQuad::addCube(vec4 scale,  vec4 point, bool drawWireFrame)
{

    mat4 scalemat = mat4::scaleMat(scale);
    mat4 moveMat = mat4::getPositionMatrix(point);
    quad q;
    q.transform = scalemat*moveMat;
    q.lifetime = lifetime;
    q.wireframe = drawWireFrame;
    quads.push_back(q);

}

void debugQuad::addMesh(std::shared_ptr<MeshResource> ptr)
{
    this->meshRes = ptr;
}

void debugQuad::bindAttrPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
}

void debugQuad::draw(mat4 transform)
{

    glUseProgram(this->program);
    this->meshRes->bind();
    unsigned int uniform = glGetUniformLocation(this->program, "transform");
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &transform[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int i = 0; i < quads.size(); ++i)
    {
        if(this->quads[i].lifetime < 1)
        {
            quads.erase(quads.begin() + i);
        }
        (quads[i].wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &(mat4::transpose(quads[i].transform) * transform)[0]);
        glDrawElements(GL_TRIANGLES, this->meshRes->getIndexSize(), GL_UNSIGNED_INT, 0);


        quads[i].lifetime -= 1;

    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->meshRes->unBindBuffers();

}