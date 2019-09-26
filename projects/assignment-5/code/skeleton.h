#ifndef GSCEPT_LAB_ENV_SKELETON_H
#define GSCEPT_LAB_ENV_SKELETON_H
#include <vector>
#include "tinyxml.h"
#include <iostream>
#include "Matrix4.h"
#include "GraphicsNode.h"
struct joint
{
    int parent = -1;
    int index;
    std::vector<int> children;
    Matrix4 transform;
    Matrix4 localTransform;
    GraphicsNode node;
};

class skeleton {

public:
    std::vector<joint>* joints = new std::vector<joint>();
    skeleton();
    ~skeleton();
    void moveJoint(Matrix4 transform, int joint);
    void loadSkeleton(char* fileName);
private:
    void updateJoints(int joint);
    void worldSpaceConversion();
};


#endif //GSCEPT_LAB_ENV_SKELETON_H
