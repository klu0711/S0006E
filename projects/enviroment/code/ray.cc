#include "ray.h"

ray::ray(vec4 start, vec4 direction)
{
    this->startPoint = start;
    this->direction = direction;
}

ray::~ray()
{

}

vec4 ray::intersectPlane(const plane &p) const
{
    vec4 diff = startPoint - p.point;

    double p1 = diff.dot3(p.normal);
    double p2 = direction.dot3(p.normal);
    if(p2 >= 0)
    {
        return vec4(0,0,0,-1);
    }
    double p3 = p1 / p2;
    return startPoint - direction * p3;
}


vec4 ray::intersectQuad(const quad &hitQuad) const
{
    float e1[3], e2[3], h[3], s[3], q[3];
    float a, f, u, v;
    vec4 objSpacePoint = mat4::transpose(mat4::inverse((hitQuad.transform))) * this->startPoint;
    vec4 objSpaceDirection = mat4::transpose(mat4::inverse((hitQuad.transform))) * this->direction;
    //objSpaceDirection = objSpaceDirection.normalize3();

    e1[0] = hitQuad.v1[0] - hitQuad.v0[0];
    e1[1] = hitQuad.v1[1] - hitQuad.v0[1];
    e1[2] = hitQuad.v1[2] - hitQuad.v0[2];

    e2[0] = hitQuad.v2[0] - hitQuad.v0[0];
    e2[1] = hitQuad.v2[1] - hitQuad.v0[1];
    e2[2] = hitQuad.v2[2] - hitQuad.v0[2];

    vec4 temp = objSpaceDirection.crossProduct(vec4(e2[0], e2[1], e2[2],1));
    h[0] = temp[0];
    h[1] = temp[1];
    h[2] = temp[2];
    a = vec4(e1[0], e1[1], e1[2], 1).dot3(vec4(h[0], h[1], h[2], 1));

    if(a > -0.00001 && a < 0.00001)
        return vec4(0,0,0,-1);
    f = 1/a;

    s[0] = objSpacePoint[0] - hitQuad.v0[0];
    s[1] = objSpacePoint[1] - hitQuad.v0[1];
    s[2] = objSpacePoint[2] - hitQuad.v0[2];

    u = f * (vec4(s[0], s[1], s[2], 1).dot3(vec4(h[0], h[1], h[2], 1)));

    if(u < 0.0f || u > 1.0)
        return vec4(0,0,0,-1);

    temp = vec4(s[0], s[1], s[2], 1).crossProduct(vec4(e1[0], e1[1], e1[2], 1));
    q[0] = temp[0];
    q[1] = temp[1];
    q[2] = temp[2];

    v = f * (objSpaceDirection.dot3(vec4(q[0], q[1], q[2], 1)));

    if (v < 0.0 || u + v > 1.0)
        return vec4(0,0,0,-1);


    float t = f * (vec4(e2[0], e2[1], e2[2], 1).dot3(vec4(q[0], q[1], q[2], 1)));

    if(t > 0.00001)
    {
        //Intersection happened
        return this->startPoint + (this->direction * t);
    }


}