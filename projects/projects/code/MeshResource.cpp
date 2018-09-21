#include "MeshResource.h"

MeshResource::MeshResource()
{
	
}

MeshResource::MeshResource(float *buffer, int *indexBuffer, int sizeBuffer, int sizeIndexBuffer)
{
	vertexBuffer = buffer;
	this->indexBuffer = indexBuffer;
	this->sizeBuffer = sizeBuffer;
	this->sizeIndexBuffer = sizeIndexBuffer;


}

MeshResource::~MeshResource()
{
}

void MeshResource::bindVertexShader()
{

	
}

void MeshResource::bindPixelShader()
{


}

void MeshResource::createProgram()
{

}
/// Setup Vertex Buffer Object
void MeshResource::bindVertexBuffer()
{

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeBuffer, vertexBuffer, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void MeshResource::bindIndexBuffer()
{
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexBuffer, indexBuffer, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshResource::unBindBuffers()
{
	glBindVertexArray(0);
}

void MeshResource::bindPointer()
{


	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeBuffer, vertexBuffer, GL_STATIC_DRAW);

//	glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexBuffer, indexBuffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
}

void MeshResource::bind()
{
	glBindVertexArray(VAO);
}

void MeshResource::setup()
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}


