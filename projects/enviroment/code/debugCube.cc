#include "debugCube.h"
#include "core/app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>


debugCube::debugCube()
{



}

debugCube::~debugCube()
{

}

void debugCube::init(const char *vertexShader, const char *fragmentShader)
{
    //Create program handle
    this->program = glCreateProgram();
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    this->loadVertexShader(vertexShader);
    this->loadFragmentShader(fragmentShader);
    this->linkShaders();


}

void debugCube::loadVertexShader(const char* vertexShader)
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

void debugCube::loadFragmentShader(const char* fragmentShader)
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

void debugCube::linkShaders()
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

void debugCube::addCube(vec4 scale,  vec4 point, uint lifetime)
{

    mat4 scalemat = mat4::scaleMat(scale);
    mat4 moveMat = mat4::getPositionMatrix(point);
    cube c;
    c.transform = scalemat*moveMat;
    c.lifetime = lifetime;
    cubes.push_back(c);

}

void debugCube::addMesh(std::shared_ptr<MeshResource> ptr)
{
    this->meshRes = ptr;
}

void debugCube::draw(mat4 transform)
{
    glUseProgram(this->program);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    unsigned int uniform = glGetUniformLocation(this->program, "transform");
    glUniformMatrix4fv(uniform, 1, GL_TRUE, &transform[0]);
    meshRes.get()->bind();

    for (int i = 0; i < 1; ++i)
    {
        if(cubes[i].lifetime == 0)
        {
            //cubes.erase(cubes.begin() + i);
        }
        glUniformMatrix4fv(uniform, 1, GL_TRUE, &(transform * cubes[i].transform)[0]);
        glDrawElements(GL_TRIANGLES, meshRes->getIndexSize(), GL_UNSIGNED_INT, 0);

        //cubes[i].lifetime -= 1;

    }
    meshRes.get()->unBindBuffers();

}