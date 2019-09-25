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
    GraphicsNode node;
};

class skeleton {

public:
    std::vector<joint>* joints = new std::vector<joint>();
    void loadSkeleton(char* fileName);
};


#endif //GSCEPT_LAB_ENV_SKELETON_H
