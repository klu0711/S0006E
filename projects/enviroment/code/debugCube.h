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
    bool wireframe;
    std::shared_ptr<MeshResource> childMesh = nullptr;
};
class debugCube {
public:
    debugCube();

    ~debugCube();

    void init(const char *vertexShader, const char *fragmentShader);

    void addCube(vec4 scale, vec4 point, uint lifetime, bool drawWireFrame, std::shared_ptr<MeshResource> childMesh = nullptr);

    void draw(mat4 transform);

    void addMesh(std::shared_ptr<MeshResource> ptr);

    void bindAttrPointers();



    std::vector<cube> cubes;
    std::shared_ptr<MeshResource> meshRes;
private:

    void loadVertexShader(const char *vertexShader);

    void loadFragmentShader(const char *fragmentShader);

    void linkShaders();

    uint program;
    uint vertexShaderHandle;
    uint fragmentShaderHandle;
    const char *vertexShader;
    const char *fragmentShader;


};