#pragma once
#include <functional>
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include "MeshResource.h"

struct pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct vertexShaderInput
{
	Vector4D pos;
	Vector4D textureCoordinates;
	Vector4D normals;
};

struct  vertexShaderOuput
{
	Vector4D pos;
	Vector4D textureCoordinates;
	Vector4D normals;
	
};

class Renderer
{
private:
	pixel* frameBuffer;
	float* zBuffer;
	int frameBufferSize = 0;
	int width;
	int height;
	vertexShaderOuput(*vertexShader)(vertexShaderInput);
	void(*fragmentShader)(vertexShaderOuput);
	Matrix4 transform;

	std::vector<Vertex> faces;
	std::vector<int> indices;

public:
	Renderer(const int &xSize, const int &ySize);
	Renderer();
	~Renderer();

	float* getVertexHandle(float verticies[]);
	int* getIndexHandle(int indicies[]);
	pixel* getFrameBuffer();
	int getFrameBufferSize();
	void setVertexShader(vertexShaderOuput(*vertexShader)(vertexShaderInput));
	void setFragmentShader(void(*fragmentShader)(vertexShaderOuput));
	void setTransform(const Matrix4& mat);
	void setBuffers();
	void rastTriangle(Vertex v1, Vertex v2, Vertex v3);
	void putPixel(int x, int y);

};

