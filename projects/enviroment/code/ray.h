#pragma once
#include "plane.h"
#include "debugQuad.h"

class ray
{
public:
    ray(vec4 start, vec4 direction);
    ~ray();

    vec4 intersectPlane(const plane &p) const;

    vec4 intersectQuad(const quad &hitQuad) const;

    vec4 startPoint, direction;
};