#include "animation.h"
using namespace CoreAnimation;
void animation::loadAnimations(char *filename)
{
    std::ifstream file;
    file.open(filename, std::ifstream::in | std::ifstream::binary);
     FILE* f = fopen(filename, "r");
     file.seekg(0, file.end);
     unsigned int length = file.tellg();
     file.seekg(0, file.beg);
     char * ptr = new char[length];
     file.read(ptr , length);



    Nax3Header* h = (Nax3Header*) ptr;
    ptr += sizeof(Nax3Header);

    clips = new Nax3Clip[h->numClips];
    for (int i = 0; i < h->numClips; ++i) {
        Nax3Clip *clip = (Nax3Clip *) ptr;
        ptr += sizeof(Nax3Clip);
        clips[i] = *clip;
        //TODO: Add own class for storing clips



    }

    printf("Hello");
}