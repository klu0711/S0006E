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


vec4 ray::intersectQuad(quad hitQuad) const
{
    //Möller-Trumbore instersection algorithm modified by adding a normal direction check
    vec4 edge1, edge2, h, s, q;
    float a, f, u, v;
    vec4 objSpacePoint = mat4::transpose(mat4::inverse((hitQuad.transform))) * this->startPoint;
    vec4 objSpaceDirection = mat4::transpose(mat4::inverse((hitQuad.transform))) * this->direction;
    //objSpaceDirection = objSpaceDirection.normalize3();

    edge1 = hitQuad.v1 - hitQuad.v0;
    edge2 = hitQuad.v2 - hitQuad.v0;
    if(objSpaceDirection.dot3(hitQuad.quadPlane.normal) < 0)
        return vec4(0,0,0,-1);

    h = objSpaceDirection.crossProduct(edge2);
   // vec4 temp = objSpaceDirection.crossProduct(vec4(e2[0], e2[1], e2[2], 1));

    a = edge1.dot3(h);

    if(a > -0.00001 && a < 0.00001)
        return vec4(0,0,0,-1);
    f = 1/a;

    s = objSpacePoint - hitQuad.v0;

    u = f * (s).dot3(h);

    if(u < 0.0f || u > 1.0)
        return vec4(0,0,0,-1);

    q = (s).crossProduct(edge1);


    v = f * (objSpaceDirection.dot3(q));

    if (v < 0.0 || u + v > 1.0)
        return vec4(0,0,0,-1);


    float t = f * (edge2).dot3(q);

    if(t > 0.00001)
    {
        //Intersection happened
        vec4 returnValue = this->startPoint + (this->direction * t);
        returnValue[3]= t;
        return returnValue;
    }


}

vec4 ray::intersectCube(const debugCube & parentClass, const cube &hitCube) const
{
    vec4 lowestDist(0,0,0,10000);
    if(hitCube.childMesh == nullptr)
        return vec4(0,0,0,-1);
    bool hit = false;
    std::shared_ptr<MeshResource> m = parentClass.meshRes;
    for (int i = 0; i < m->indexBuffer.size(); i += 3)
    {
        quad q;
        q.transform = hitCube.transform;
        q.v0 = vec4(m->vertexBuffer[m->indexBuffer[i]].pos[0],
                    m->vertexBuffer[m->indexBuffer[i]].pos[1],
                    m->vertexBuffer[m->indexBuffer[i]].pos[2], 1);
        q.v1 = vec4(m->vertexBuffer[m->indexBuffer[i + 1]].pos[0],
                    m->vertexBuffer[m->indexBuffer[i + 1]].pos[1],
                    m->vertexBuffer[m->indexBuffer[i + 1]].pos[2], 1);
        q.v2 = vec4(m->vertexBuffer[m->indexBuffer[i + 2]].pos[0],
                    m->vertexBuffer[m->indexBuffer[i + 2]].pos[1],
                    m->vertexBuffer[m->indexBuffer[i + 2]].pos[2], 1);
        vec4 u = q.v1 - q.v0;
        vec4 v = q.v2 - q.v0;

        vec4 normal = (v.crossProduct(u)).normalize3();
        q.quadPlane.normal = normal;

        vec4 returnvalue = intersectQuad(q);
        if(returnvalue[3] != -1)
        {
           hit = true;
           //return returnvalue;
        }
    }
    std::shared_ptr<MeshResource> childMesh = hitCube.childMesh;
    if(hit) {
        for (int j = 0; j < childMesh->indexBuffer.size(); j += 3) {
            quad q;
            //TODO: set transform to the transform from the meshd
            q.transform = mat4();
            q.v0 = vec4(childMesh->vertexBuffer[childMesh->indexBuffer[j]].pos[0],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j]].pos[1],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j]].pos[2], 1);
            q.v1 = vec4(childMesh->vertexBuffer[childMesh->indexBuffer[j + 1]].pos[0],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j + 1]].pos[1],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j + 1]].pos[2], 1);
            q.v2 = vec4(childMesh->vertexBuffer[childMesh->indexBuffer[j + 2]].pos[0],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j + 2]].pos[1],
                        childMesh->vertexBuffer[childMesh->indexBuffer[j + 2]].pos[2], 1);
            vec4 u = q.v1 - q.v0;
            vec4 v = q.v2 - q.v0;

            vec4 normal = (v.crossProduct(u)).normalize3();
            q.quadPlane.normal = normal;//childMesh->vertexBuffer[childMesh->indexBuffer[i]].normal;

            vec4 returnValue = intersectQuad(q);
            if (returnValue[3] != -1) {
                if(returnValue[3] < lowestDist[3])
                    lowestDist = returnValue;
            }
        }
    }
    if(lowestDist[3] != -1)
    {
        lowestDist[3] = 1;
        return lowestDist;

    }
    return vec4(0,0,0,-1);
}