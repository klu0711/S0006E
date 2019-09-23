#ifndef GSCEPT_LAB_ENV_JOINT_H
#define GSCEPT_LAB_ENV_JOINT_H
#include "Matrix4.h"
#include <string>
#include <vector>

class joint {

public:
    std::string name;
    int index;
    int parentIndex;
    Matrix4 transform;
};


#endif //GSCEPT_LAB_ENV_JOINT_H
