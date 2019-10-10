#pragma once
#include <vector>
#include "tinyxml.h"
#include <iostream>
#include "Matrix4.h"
#include "GraphicsNode.h"

namespace CoreGraphics
{
    struct Nvx2Header
    {
        uint magic;
        uint numGroups;
        uint numVertices;
        uint vertexWidth;
        uint numIndices;
        uint numEdges;
        uint vertexComponentMask;
    };

    struct Nvx2Group
    {
        uint firstVertex;
        uint numVertices;
        uint firstTriangle;
        uint numTriangles;
        uint firstEdge;
        uint numEdges;
    };

    struct PrimitiveGroup
    {
        uint numVerticies;
        uint baseIndex;
        uint numIndicies;
        uint primitiveTopology;
    };

    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
    };

    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };
} // namespace CoreGraphics

struct joint
{
    int parent = -1;
    int index;
    std::vector<int> children;
    Matrix4 transform;
    Matrix4 localTransform;
    GraphicsNode node;
};

struct vertexComponent
{
    CoreGraphics::SemanticName name;
    uint index;
    CoreGraphics::Format format;
};
class skeleton {

public:
    std::vector<joint>* joints = new std::vector<joint>();
    std::vector<int> skinJoints;
    skeleton();
    ~skeleton();
    void moveJoint(Matrix4 transform, int joint);
    void loadSkeleton(char* fileName);
    void updateJoints(int joint);
    void worldSpaceConversion();
    void loadMesh(char* fileName);
    CoreGraphics::Nvx2Header* header;

    std::vector<CoreGraphics::PrimitiveGroup> primGroups;
    std::vector<vertexComponent> vertexComponents;

    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    size_t groupDataSize;
    size_t vertexDataSize;
    size_t indexDataSize;

    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
};
