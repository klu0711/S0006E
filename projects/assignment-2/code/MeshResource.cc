#include "MeshResource.h"

MeshResource::MeshResource()
{

}
/// Constructor that takes two pointers to the index and the vertex arrays, it also accepts the "sizeof" both of them
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


/// Setup the buffer which contains the verticies and the colors
void MeshResource::bindVertexBuffer()
{

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, vertexBuffer, GL_STATIC_DRAW);


}
/// Setup the buffer which contains the indexes which will become the triangles
void MeshResource::bindIndexBuffer()
{
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexBuffer, indexBuffer, GL_STATIC_DRAW);

}
/// Unbind the vertex array object
void MeshResource::unBindBuffers()
{
    glBindVertexArray(0);
}
/// Tell the shaders how the buffers are layed out and enable both of them
void MeshResource::bindPointer()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
/// Bind the vertex array object
void MeshResource::bind()
{
    glBindVertexArray(VAO);
}
/// Create the vertex array object
void MeshResource::setup()
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
}
