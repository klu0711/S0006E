#pragma once

#include "Vector4D.h"
#include "Matrix4.h"

class rigidBody
{

public:

    rigidBody();
    ~rigidBody();

    float mass;
    vec4 position;
    vec4 velocity;
    vec4 orientation;
    vec4 rotation;
    mat4 transformMatrix;

private:

};