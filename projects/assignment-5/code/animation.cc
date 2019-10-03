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
    header = h;
    ptr += sizeof(Nax3Header);
    keyBuffer = (Vector4D*)malloc(h->numKeys* sizeof(Vector4D));

    clips = new clip[h->numClips];
    for (int i = 0; i < h->numClips; ++i) {
        Nax3Clip *naxClip = (Nax3Clip *) ptr;
        ptr += sizeof(Nax3Clip);
        clip cl;
        cl.numCurves = naxClip->numCurves;
        cl.startKeyIndex = naxClip->startKeyIndex;
        cl.numKeys = naxClip->numKeys;
        cl.keyStride = naxClip->keyStride;
        cl.keyDuration = naxClip->keyDuration;
        cl.preInfinityType = naxClip->preInfinityType;
        cl.postInfinityType = naxClip->postInfinityType;
        cl.name = naxClip->name;


        // move the pointer to skip the events since they are not used
        for (int j = 0; j < naxClip->numEvents; ++j) {
            ptr += sizeof(Nax3AnimEvent);
        }


        // Add the curves to the clips
        unsigned int curveIndex;
        cl.curves = new curve[cl.numCurves];
        for (curveIndex = 0; curveIndex < cl.numCurves; curveIndex++)
        {
            Nax3Curve* naxCurve = (Nax3Curve*)ptr;
            ptr += sizeof(Nax3Curve);
            curve cu;
            cu.firstKeyIndex = naxCurve->firstKeyIndex;
            cu.isActive = naxCurve->isActive;
            cu.isStatic = naxCurve->isStatic;
            cu.curveType = naxCurve->curveType;
            cu.staticKey = Vector4D(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
            cl.curves[curveIndex] = cu;

        }
        clips[i] = cl;
        memcpy(keyBuffer, ptr, h->numKeys*sizeof(Vector4D));
        // load the blob of data that contains the keyframes



    }

    printf("Hello");
}