#pragma once
#include "core/app.h"
#include "config.h"
#include <vector>
#include "Vector4D.h"
#include <string.h>
namespace SkinningStructs
{

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
    struct vertexComponent
    {
        SemanticName name;
        uint index;
        Format format;
    };
}

struct Vertex
{
	GLfloat pos[3];
	GLfloat uv[2];
	GLfloat normal[3];
};




class MeshResource {
private:
	int sizeOfIndexBuffer;
	int sizeOfVertexBuffer;

	uint32 EBO;
	uint32 VBO;
	uint32 VAO;
public:



	MeshResource();
	~MeshResource();
	bool loadOBJ(char* filename);
	int getVertexSize();
	int getIndexSize();
	void setupBuffers();
	void bindVertexBuffer(const std::vector<Vertex>& vector);
	void bindVertexBufferSkin(const void* vb, uint vbSize);
	void bindIndexBuffer(const std::vector<int>& buffer);
	void bindIndexBufferSkin(const void* ib, uint ibSize);
	void loadSkeletonMeshBuffers();
	void unBindBuffers();
	void bindAttrPointer();
	void bind();
	void setupHandles();
	void unbindVAO();
	void destroy();
	void setupMesh(char* filename);
	void setupMeshSkin(void* ib, void *vb,  int ibSize, int vbSize, int numVerticies, int numIndicies);
	




};


