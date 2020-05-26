#include "rigidBody.h"


rigidBody::rigidBody()
{

}

rigidBody::~rigidBody()
{

}

void rigidBody::integrate(float duration)
{
    vec4 newDeltaPos = velocity * duration;

    position += newDeltaPos;

    vec4 resAcc = acceleration;
    vec4 newDeltaAcc = forceAccumulated * inverseMass;
    resAcc += forceAccumulated;

    vec4 newDeltaVel = resAcc * duration;
    resAcc += newDeltaVel;

}

