#pragma once
#include <functional>
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include "MeshResource.h"
#include <math.h>
#include <iostream>
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

struct line
{
	int* pixels;
	int size;
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



public:
	std::vector<Vertex> faces;
	std::vector<int> indices;
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
	line createLine(Vertex v1, Vertex v2);
	void putPixel(int index);

};

