#pragma once
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include <memory>
#include "MeshResource.h"
struct cube
{
    mat4 transform;
    uint lifetime;
};
class debugCube {
    debugCube();

    ~debugCube();

    void init(const char *vertexShader, const char *fragmentShader);

    void addCube(vec4 scale, vec4 point, uint lifetime);

    void draw(mat4 transform);

private:
    std::vector<cube> cubes;
    std::shared_ptr<MeshResource> meshRes;

    void loadVertexShader(const char *vertexShader);

    void loadFragmentShader(const char *fragmentShader);

    void linkShaders();

    uint program;
    uint vertexShaderHandle;
    uint fragmentShaderHandle;
    const char *vertexShader;
    const char *fragmentShader;
    uint VBO;


};