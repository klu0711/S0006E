#ifndef GSCEPT_LAB_ENV_ANIMATION_H
#define GSCEPT_LAB_ENV_ANIMATION_H
#include <fstream>
#include "Vector4D.h"
#include <cstring>
namespace CoreAnimation
{
#pragma pack(push, 1)

#define NEBULA3_NAX3_MAGICNUMBER 'NA01'

//------------------------------------------------------------------------------
/**
    NAX3 file format structs.
    NOTE: keep all header-structs 4-byte aligned!
*/
struct Nax3Header
{
    unsigned int magic;
    unsigned int numClips;
    unsigned int numKeys;
};

struct Nax3Clip
{
    unsigned short numCurves;
    unsigned short startKeyIndex;
    unsigned short numKeys;
    unsigned short keyStride;
    unsigned short keyDuration;
    unsigned char preInfinityType;          // CoreAnimation::InfinityType::Code
    unsigned char postInfinityType;         // CoreAnimation::InfinityType::Code
    unsigned short numEvents;
    char name[50];                  // add up to 64 bytes size for Nax3Clip
};

struct Nax3AnimEvent
{
    char name[47];
    char category[15];
    unsigned short keyIndex;
};

struct Nax3Curve
{
    unsigned int firstKeyIndex;
    unsigned char isActive;                 // 0 or 1
    unsigned char isStatic;                 // 0 or 1
    unsigned char curveType;                // CoreAnimation::CurveType::Code
    unsigned char _padding;                 // padding byte
    float staticKeyX;
    float staticKeyY;
    float staticKeyZ;
    float staticKeyW;

};

#pragma pack(pop)
} // namespace CoreAnimation


class curve
{
public:
    unsigned int firstKeyIndex;
    bool isActive;                 // 0 or 1
    bool isStatic;                 // 0 or 1
    unsigned char curveType;                // CoreAnimation::CurveType::Code
    Vector4D staticKey;
};

class clip
{
public:
    unsigned short numCurves;
    unsigned short startKeyIndex;
    unsigned short numKeys;
    unsigned short keyStride;
    unsigned short keyDuration;
    unsigned char preInfinityType;          // CoreAnimation::InfinityType::Code
    unsigned char postInfinityType;         // CoreAnimation::InfinityType::Code
    unsigned short numEvents;
    char* name;
    curve* curves;

};
typedef void* keys;

class animation {
public:
    animation() {};
    ~animation(){};

    void loadAnimations(char* filename);
    CoreAnimation::Nax3Header* header;
    clip* clips;
    keys keyBuffer;

    //use ifstream to load file.
};


#endif //GSCEPT_LAB_ENV_ANIMATION_H
