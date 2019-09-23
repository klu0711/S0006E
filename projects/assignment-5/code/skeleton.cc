
#include "skeleton.h"


void skeleton::loadSkeleton(char *fileName)
{
    TiXmlDocument doc(fileName);
    bool loadOkay = doc.LoadFile();
    if(!loadOkay)
    {
        std::cout << "XML load failed" << std::endl;
    }

}