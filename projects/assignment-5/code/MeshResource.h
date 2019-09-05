#pragma once
#include "core/app.h"
#include "config.h"
#include <vector>
#include "Vector4D.h"
#include <string.h>
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
	void bindIndexBuffer(const std::vector<int>& buffer);
	void unBindBuffers();
	void bindAttrPointer();
	void bind();
	void setupHandles();
	void unbindVAO();
	void destroy();
	void setupMesh(char* filename);
	




};


