#pragma once
#include "plane.h"

class ray
{
public:
    ray(vec4 start, vec4 direction);
    ~ray();

    vec4 intersect(const plane &p) const;

    vec4 startPoint, direction;
};