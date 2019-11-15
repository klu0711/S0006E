#pragma once
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include <memory>
#include "plane.h"
struct quad
{
    plane quadPlane;
    mat4 transform;
    vec4 v0,v1,v2,v3;
};
class debugQuad {
public:
    debugQuad();

    ~debugQuad();

    void init(const char *vertexShader, const char *fragmentShader);

    void addQuad(vec4 position, vec4 scale, mat4 rotation);

    void draw(mat4 transform);

    void bindAttrPointers();

    std::vector<quad> quads;

private:

    void loadVertexShader(const char *vertexShader);

    void loadFragmentShader(const char *fragmentShader);

    void linkShaders();
    float* vertexBuffer;
    int* indexBuffer;

    int sizeBuffer;
    int sizeIndexBuffer;
    uint EBO;
    uint VBO;
    uint VAO;

    uint program;
    uint vertexShaderHandle;
    uint fragmentShaderHandle;
    const char *vertexShader;
    const char *fragmentShader;


};