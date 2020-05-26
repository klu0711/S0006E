#pragma once

#include "Vector4D.h"
#include "Matrix4.h"

class rigidBody
{

public:

    rigidBody();
    ~rigidBody();


    float damping;
    float mass;
    float inverseMass;
    vec4 position;
    vec4 velocity;
    vec4 orientation;
    vec4 rotation;
    mat4 transformMatrix;
    vec4 forceAccumulated;
    vec4 acceleration;

    void integrate(float duration);



private:

};