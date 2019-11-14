#pragma once
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include <memory>
#include "MeshResource.h"
struct quad
{
    mat4 transform;
    uint lifetime;
    bool wireframe;
};
class debugQuad {
public:
    debugQuad();

    ~debugQuad();

    void init(const char *vertexShader, const char *fragmentShader);

    void addCube(vec4 scale, vec4 point, bool drawWireFrame);

    void draw(mat4 transform);

    void addMesh(std::shared_ptr<MeshResource> ptr);

    void bindAttrPointers();

private:
    std::vector<quad> quads;
    std::shared_ptr<MeshResource> meshRes;

    void loadVertexShader(const char *vertexShader);

    void loadFragmentShader(const char *fragmentShader);

    void linkShaders();

    uint program;
    uint vertexShaderHandle;
    uint fragmentShaderHandle;
    const char *vertexShader;
    const char *fragmentShader;


};