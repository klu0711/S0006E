#pragma once
#include <functional>
#include "Vector4D.h"
#include "Matrix4.h"
#include <vector>
#include "MeshResource.h"
#include "TextureResource.h"




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
	std::function<Vertex(Vertex vertex, Matrix4 lookat, Matrix4 modelMatrix)> vertexShader;
	std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, TextureResource tex)> fragmentShader;
	Matrix4 transform;
	Matrix4 lookAt;
	Vector4D cameraPosition;
	pixel* drawTexture;
	int drawTextureWidth;
	int drawTextureHeigth;
	TextureResource texture;
	float w1, w2, w3;



public:
	std::vector<Vertex> faces;
	std::vector<int> indices;
	Renderer(const int &xSize, const int &ySize);
	Renderer();
	~Renderer();
	void setTransform(Matrix4 mat);
	void setLookat(Matrix4 mat);
	void setCameraPsition(const Vector4D& vec);
	void setTexture(pixel* p, int width, int height);
	void loadTexture(char* filename);
	pixel* getFrameBuffer();
	int getFrameBufferSize();
	void setVertexShader(std::function<Vertex(Vertex vertex, Matrix4 lookat, Matrix4 modelMatrix)> shader);
	void setFragmentShader(std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, TextureResource tex)> shader);
	void setBuffers();
	void clearZbuffer();
	void rastTriangle(Vertex v1, Vertex v2, Vertex v3);
	Line createLine2(Vertex v1, Vertex v2);
	void putPixel(int index, Vector4D color);
	void linescan(int x1, int x2, int y, const Vertex &v1, const Vertex &v2, const Vertex &v3, float* coordinateSpace);
	Vector4D getBary(int x, int y, Vertex v1, Vertex v2, Vertex v3);
	void barycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3);
	float areaOfTriangle(Vertex v1, Vertex v2, Vertex v3);

};

