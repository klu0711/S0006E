#include "animation.h"

void animation::loadAnimations(char *filename)
{
    std::ifstream file;
    file.open(filename, std::ifstream::in | std::ifstream::binary);
    CoreAnimation::Nax3Header h;
    while(!file.eof())
    {
        file.read(reinterpret_cast<char*>(&h), sizeof(h));
        break;
    }
    printf("Hello");
}