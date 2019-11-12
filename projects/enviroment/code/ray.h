#pragma once
#include "plane.h"

class ray
{
public:
    ray();
    ~ray();

    vec4 intersect(const plane &p) const;

    vec4 startPoint, direction;
};