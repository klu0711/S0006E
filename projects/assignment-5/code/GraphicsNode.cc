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
/// Set the shader pointer to an instance
void GraphicsNode::setShaderClass(std::shared_ptr<Shader> shader)
{
	this->shader = shader;
}
/// Set the tecture pointer to an instance
void GraphicsNode::setTextureclass(std::shared_ptr<TextureResource> textureResource)
{
	this->textureResource = textureResource;
}
/// Set the mesh pointer to an instance
void GraphicsNode::setMeshCLass(std::shared_ptr<MeshResource> meshResource)
{
	this->meshResource = meshResource;
}
/// Return the shader pointer
Shader* GraphicsNode::getShader()
{
	return shader.get();
}
/// Return the texture pointer
TextureResource* GraphicsNode::getTextureResource()
{
	return textureResource.get();
}
/// Return the mesh pointer
MeshResource* GraphicsNode::getMeshResource()
{
	return meshResource.get();
}
/// return the stored transform
Matrix4 GraphicsNode::getTransform()
{
	return transform;
}
/// Change the stored transform
void GraphicsNode::setTransform(Matrix4 mat)
{
	transform = mat;
}
/// Run all the functions that need to be run to draw something but only the functions that don't need to be ran every frame
uint GraphicsNode::load(std::string filename, std::string vertexShaderName, std::string fragmentShaderName, int texture)
{
    uint texI;
    if(texture >= 0)
    {
        texI = textureResource.get()->loadFromFile(filename.c_str());
    }
	shader.get()->loadVertexShader(vertexShaderName.c_str());
	shader.get()->loadFragmentShader(fragmentShaderName.c_str());
	shader.get()->linkShaders();
	//meshResource.get()->setupHandles();
	//meshResource.get()->loadOBJFile(vertices, uv, normals);
	//combinedbuffer = meshResource.get()->combineBuffers(vertices, uv, normals);
	//meshResource.get()->convertToFloatPointer(vertices, indicies, normals);
    if(texture >= 0)
    {
        textureResource.get()->bind(texture);
    }
	light = LightingNode(Vector4D(5, 2, 0, 1), Vector4D(0.5f, 0.5f, 0.5f, 1), 1);
	shader.get()->useProgram();

	if(texI >= 0)
	    return texI;
	return -1;

}
/// This functions runs every frame to draw something to the screen
void GraphicsNode::draw()
{


	//textureResource.get()->loadFromFile(textureName.c_str());



	shader.get()->useProgram();
	shader.get()->modifyUniformMatrix("transform", transform.getPointer());


	shader->modifyUniformVector("lightPos", light.getPosition());
	//shader->modifyUniformVector("lightColor", light.getColor());
	//shader->modifyUniformFloat("intensity", light.getIntensity());

	meshResource.get()->bind();
	glDrawElements(GL_TRIANGLES, meshResource->getIndexSize(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	meshResource.get()->unBindBuffers();
}