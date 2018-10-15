#pragma once
#include "core/app.h"
#include "config.h"
#include <vector>
#include "Vector4D.h"
class MeshResource {
private:



	// Buffer containing the vertices and the colors
	float* vertexAndIndexBuffer;
	int* indexBuffer;
	

	int sizeBuffer;
	int sizeOfNormals;
	int sizeIndexBuffer;
	int temp;
	uint32 EBO;
	uint32 VBO;
	uint32 VAO;
public:


	MeshResource(float *buffer, int *indexBuffer, int sizeBuffer, int sizeIndexBuffer);
	MeshResource();
	~MeshResource();
	bool loadOBJFile(std::vector<Vector4D> &vertices, std::vector<Vector4D>& uv, std::vector<Vector4D>& normals);
	void convertToFloatPointer(std::vector<Vector4D> &verticies, std::vector<Vector4D>& indices, std::vector<Vector4D>& normals);
	void setupBuffers();
	void bindVertexBuffer(std::vector<Vector4D> vector);
	void bindIndexBuffer();
	void unBindBuffers();
	void bindAttrPointer();
	void bind();
	void setupHandles();
	void bindVAO();
	void unbindVAO();
	void destroy();
	std::vector<Vector4D> combineBuffers(std::vector<Vector4D> vertices, std::vector<Vector4D> indices, std::vector<Vector4D> normals);





};


