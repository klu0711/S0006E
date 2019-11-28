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
    this->loadVertexShader(vertexShader);
    this->loadFragmentShader(fragmentShader);
    this->linkShaders();
    glBindVertexArray(0);

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

void debugCube::addCube(vec4 scale,  vec4 point, uint lifetime, bool drawWireFrame, std::shared_ptr<MeshResource> childMesh, mat4 childMeshTransform)
{

    mat4 scaleMat = mat4::scaleMat(scale);
    mat4 moveMat = mat4::getPositionMatrix(point);
    cube c;
    c.childMeshTransform = childMeshTransform;
    c.transform = scaleMat * mat4::transpose(moveMat);
    c.lifetime = lifetime;
    c.wireframe = drawWireFrame;
    c.childMesh = childMesh;
    cubes.push_back(c);

}

void debugCube::recalculateBoundingBox(uint index, mat4 transform)
{
    this->cubes[index].childMeshTransform = transform;
    transform = mat4::transpose(transform);
    vec4 preMin = this->cubes[index].childMesh->min;
    vec4 preMax = this->cubes[index].childMesh->max;
    vec4 corners[8];
    corners[0] = preMin;
    corners[1] = vec4(preMin[0], preMax[1], preMin[2], 1);
    corners[2] = vec4(preMin[0], preMax[1], preMax[2], 1);
    corners[3] = vec4(preMin[0], preMin[1], preMax[2], 1);
    corners[4] = vec4(preMax[0], preMin[1], preMin[2], 1);
    corners[5] = vec4(preMax[0], preMax[1], preMin[2], 1);
    corners[6] = preMax;
    corners[7] = vec4(preMax[0], preMin[1], preMax[2], 1);

    vec4 tmin = (transform * corners[0]);
    tmin[3] = 1;
    vec4 tmax = tmin;

    for (int i = 0; i < 8; ++i)
    {
        vec4 point = (transform * corners[i]);
        point[3] = 1;

        tmin = vec4::min(tmin, point);
        tmax = vec4::max(tmax, point);
    }

    vec4 midPoint =  (tmax + tmin) * 0.5f;
    vec4 sizeAABB =  (tmax - tmin);

    mat4 scaleMat = mat4::scaleMat(sizeAABB);
    mat4 moveMat = mat4::getPositionMatrix(midPoint);

    this->cubes[index].transform = scaleMat * mat4::transpose(moveMat);

    /*
    float a, b;
    float Amin[3], Amax[3];
    float Bmin[3], Bmax[3];
    vec4 translation(transform[12], transform[13], transform[14], 1);
    mat4 rotation = transform;
    rotation[3] = 0;
    rotation[7] = 0;
    rotation[11] = 0;
    rotation[15] = 1;
    rotation[12] = 0;
    rotation[13] = 0;
    rotation[14] = 0;
    Amin[0] = this->cubes[index].childMesh->min[0]; Amax[0] = this->cubes[index].childMesh->max[0];
    Amin[1] = this->cubes[index].childMesh->min[1]; Amax[1] = this->cubes[index].childMesh->max[1];
    Amin[2] = this->cubes[index].childMesh->min[2]; Amax[2] = this->cubes[index].childMesh->max[2];

    rotation = mat4::transpose(rotation);
    Bmin[0] = Bmax[0] = translation[0];
    Bmin[1] = Bmax[1] = translation[1];
    Bmin[2] = Bmax[2] = translation[2];

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            a = (float)(rotation[4 * i + j] * Amin[j]);
            b = (float)(rotation[4 * i + j] * Amax[j]);
            if (a < b)
            {
                Bmin[i] += a;
                Bmax[i] += b;
            }
            else
            {
                Bmin[i] += b;
                Bmax[i] += a;
            }
        }
    }

    vec4 midPoint =  ((vec4(Bmax[0], Bmax[1], Bmax[2], 0) + vec4(Bmin[0], Bmin[1], Bmin[2], 0))) * 0.5f;
    vec4 sizeAABB =  (vec4(Bmax[0], Bmax[1], Bmax[2], 0) - vec4(Bmin[0], Bmin[1], Bmin[2], 0));

    mat4 scaleMat = mat4::scaleMat(sizeAABB);
    mat4 moveMat = mat4::getPositionMatrix(midPoint);

    this->cubes[index].transform = scaleMat * mat4::transpose(moveMat);*/
}

void debugCube::addMesh(std::shared_ptr<MeshResource> ptr)
{
    this->meshRes = ptr;
}

void debugCube::bindAttrPointers()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
}

void debugCube::draw(mat4 transform)
{

    glUseProgram(this->program);
    this->meshRes->bind();
    unsigned int uniform = glGetUniformLocation(this->program, "transform");
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &transform[0]);
    uint uniformColor = glGetUniformLocation(this->program, "color");
    glUniform4fv(uniformColor, 1, vec4(0,0,1,1).getPointer());
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int i = 0; i < cubes.size(); ++i)
    {
        if(this->cubes[i].lifetime < 1)
        {
            cubes.erase(cubes.begin() + i);
        }
        (cubes[i].wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &((cubes[i].transform) * transform)[0]);
        glDrawElements(GL_TRIANGLES, this->meshRes->getIndexSize(), GL_UNSIGNED_INT, 0);


        cubes[i].lifetime -= 1;

    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->meshRes->unBindBuffers();

}