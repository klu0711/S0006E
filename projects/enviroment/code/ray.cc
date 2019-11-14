#include "ray.h"

ray::ray(vec4 start, vec4 direction)
{
    this->startPoint = start;
    this->direction = direction;
}

ray::~ray()
{

}

vec4 ray::intersect(const plane &p) const
{
    vec4 diff = startPoint - p.point;

    double p1 = diff.dot3(p.normal);
    double p2 = direction.dot3(p.normal);
    if(p2 >= 0)
    {
        return nullptr;
    }
    double p3 = p1 / p2;
    return startPoint - direction * p3;
}