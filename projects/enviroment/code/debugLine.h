#pragma once
#include <vector>
#include "Vector4D.h"
#include "Matrix4.h"
#include "ray.h"
class debugLine
{
public:

    debugLine();
    ~debugLine();
    void init(const char* vertexShader,const char* fragmentShader);
    void addLine(ray r);
    void draw(mat4 transform);

    void clearLines();


    std::vector<float> points;
    std::vector<int> indices;
    int currentIndex = 0;


private:

    void loadVertexShader(const char* vertexShader);
    void loadFragmentShader(const char* fragmentShader);
    void linkShaders();
    uint program;
    uint vertexShaderHandle;
    uint fragmentShaderHandle;
    const char* vertexShader;
    const char* fragmentShader;
    uint VBO;
    uint length = 30;
};