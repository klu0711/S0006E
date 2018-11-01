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
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
};


struct Line
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
	Vertex(*vertexShader)(Vertex);
	Vector4D(*fragmentShader)(Vertex);
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
	void setVertexShader(Vertex(*vertexShader)(Vertex));
	void setFragmentShader(Vector4D(*fragmentShader)(Vertex));
	void setTransform(const Matrix4& mat);
	void setBuffers();
	void rastTriangle(Vertex v1, Vertex v2, Vertex v3);
	Line createLine(Vertex v1, Vertex v2);
	Line createLine2(Vertex v1, Vertex v2);
	void putPixel(int index, Vector4D color);
	void linescan(int x1, int x2, int y, const Vertex &v1, const Vertex &v2, const Vertex &v3);
	Vector4D getBary(int x, int y, Vertex v1, Vertex v2, Vertex v3);
	float areaOfTriangle(Vertex v1, Vertex v2, Vertex v3);

};

