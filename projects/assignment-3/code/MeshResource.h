#pragma once
#include "core/app.h"
#include "config.h"
#include <vector>

class MeshResource {
private:



	// Buffer containing the verticies and the colors
	float* vertexBuffer;
	int* indexBuffer;

	int sizeBuffer;
	int sizeIndexBuffer;
	uint32 EBO;
	uint32 VBO;
	uint32 VAO;
public:


	MeshResource(float *buffer, int *indexBuffer, int sizeBuffer, int sizeIndexBuffer);
	MeshResource();
	~MeshResource();
	void bindVertexBuffer();
	void bindIndexBuffer(std::vector<int> buffer, <unknown>);
	void unBindBuffers();
	void bindPointer();
	void bind();
	void setup();
	void destroy();




};
