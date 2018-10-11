#include "GraphicsNode.h"




GraphicsNode::GraphicsNode()
{
	transform = Matrix4::rotY(1);
}

GraphicsNode::~GraphicsNode()
{
	this->shader = 0;
	this->meshResource = 0;
	this->textureResource = 0;
}

void GraphicsNode::setShaderClass(std::shared_ptr<Shader> shader)
{
	this->shader = shader;
}

void GraphicsNode::setTextureclass(std::shared_ptr<TextureResource> textureResource)
{
	this->textureResource = textureResource;
}

void GraphicsNode::setMeshCLass(std::shared_ptr<MeshResource> meshResource)
{
	this->meshResource =meshResource;
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

void GraphicsNode::setTransform(Matrix4 mat)
{
	transform = mat;
}

void GraphicsNode::load(std::string filename, std::string vertexShaderName, std::string fragmentShaderName, int texture)
{
	textureResource.get()->loadFromFile(filename.c_str());
	shader.get()->loadVertexShader(vertexShaderName.c_str());
	shader.get()->loadFragmentShader(fragmentShaderName.c_str());
	shader.get()->linkShaders();
	meshResource.get()->setupHandles();
	meshResource.get()->loadOBJFile(vertices, uv, normals);
	combinedbuffer = meshResource.get()->combineBuffers(vertices, uv);
	//meshResource.get()->convertToFloatPointer(vertices, indicies, normals);
	textureResource.get()->bind(texture);


}

void GraphicsNode::draw()
{

	meshResource.get()->bindVertexBuffer(combinedbuffer);
	//meshResource.get()->bindIndexBuffer();
	
	//textureResource.get()->loadFromFile(textureName.c_str());
	meshResource.get()->bindAttrPointer();

	shader.get()->useProgram();
	shader.get()->modifyUniformMatrix("transform", transform.getPointer());

	
	meshResource.get()->bind();
	//meshResource.get()->bindVAO();
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	meshResource.get()->unBindBuffers();
	//transform = Matrix4::rotY(rotaion) * Matrix4::rotX(rotaion);
	//rotaion += 0.01;
}
