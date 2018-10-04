#include "GraphicsNode.h"




GraphicsNode::GraphicsNode()
{
	transform = Matrix4::rotY(1);
}

GraphicsNode::~GraphicsNode()
{
}

void GraphicsNode::setShaderClass(Shader *shader)
{
	this->shader.reset(shader);
}

void GraphicsNode::setTextureclass(TextureResource* textureResource)
{
	this->textureResource.reset(textureResource);
}

void GraphicsNode::setMeshCLass(MeshResource* meshResource)
{
	this->meshResource.reset(meshResource);
}

Shader* GraphicsNode::getShader()
{
	return shader.get();
}

TextureResource* GraphicsNode::getTextureResource()
{
	return textureResource.get();
}

MeshResource* GraphicsNode::getMeshResource()
{
	return meshResource.get();
}

Matrix4 GraphicsNode::getTransform()
{
	return transform;
}

void GraphicsNode::loadTexture(std::string filename)
{
	textureResource.get()->loadFromFile(filename.c_str());
}

void GraphicsNode::draw(std::string vertexShaderName, std::string fragmentShaderName, std::string textureName)
{
	shader.get()->loadVertexShader(vertexShaderName.c_str());
	shader.get()->loadFragmentShader(fragmentShaderName.c_str());
	shader.get()->linkShaders();
	
	meshResource.get()->setup();
	meshResource.get()->bindVertexBuffer();
	meshResource.get()->bindIndexBuffer();
	
	//textureResource.get()->loadFromFile(textureName.c_str());
	meshResource.get()->bindPointer();

	shader.get()->useProgram();
	shader.get()->modifyUniformMatrix("transform", transform.getPointer());

	textureResource.get()->bind(0);
	meshResource.get()->bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	meshResource.get()->unBindBuffers();
	//transform = Matrix4::rotY(rotaion) * Matrix4::rotX(rotaion);
	//rotaion += 0.01;
}
