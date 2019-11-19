#pragma once
#include "plane.h"
#include "debugQuad.h"
#include "debugCube.h"

class ray
{
public:
    ray(vec4 start, vec4 direction);
    ~ray();

    vec4 intersectPlane(const plane &p) const;

    vec4 intersectQuad(quad hitQuad) const;

    vec4 intersectCube(const debugCube & parentClass, const cube& hitCube) const;

    vec4 startPoint, direction;
};