#ifndef GSCEPT_LAB_ENV_SKELETON_H
#define GSCEPT_LAB_ENV_SKELETON_H
#include <vector>
#include "tinyxml.h"
#include <iostream>
#include "Matrix4.h"
struct joint
{
    int parent;
    int index;
    std::vector<int> children;
    Matrix4 transform;
};

class skeleton {

public:
    std::vector<joint>* joints = new std::vector<joint>();
    void loadSkeleton(char* fileName);
};


#endif //GSCEPT_LAB_ENV_SKELETON_H
