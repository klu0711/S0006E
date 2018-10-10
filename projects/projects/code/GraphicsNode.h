#pragma once
#include "TextureResource.h"
#include "Shader.h"
#include "MeshResource.h"
#include <memory>


	class GraphicsNode {
	public:
		GraphicsNode();
		~GraphicsNode();
		void setShaderClass(std::shared_ptr<Shader> shader);
		void setTextureclass(std::shared_ptr<TextureResource> textureResource);
		void setMeshCLass(std::shared_ptr<MeshResource> meshresource);
		Shader* getShader();
		TextureResource* getTextureResource();
		MeshResource* getMeshResource();
		Matrix4 getTransform();
		void setTransform(Matrix4 mat);
		void load(std::string filename, std::string vertexShaderName, std::string fragmentShaderName, int texture);
		void draw();
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<TextureResource> textureResource;
		std::shared_ptr<MeshResource> meshResource;
		Matrix4 transform;
		float rotaion = 0;
		std::vector<Vector4D> vertices;
		std::vector<Vector4D> indicies;
		std::vector<Vector4D> normals;
	};
