#ifndef GSCEPT_LAB_ENV_SKELETON_H
#define GSCEPT_LAB_ENV_SKELETON_H
#include "joint.h"
#include <vector>
#include "tinyxml.h"
#include <iostream>

class skeleton {

public:
    std::vector<joint>* joints = new std::vector<joint>();
    void loadSkeleton(char* fileName);
};


#endif //GSCEPT_LAB_ENV_SKELETON_H
