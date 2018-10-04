#pragma once
#include "TextureResource.h"
#include "Shader.h"
#include "MeshResource.h"
#include <memory>

	class GraphicsNode {
	public:
		GraphicsNode();
		~GraphicsNode();
		void setShaderClass(Shader* shader);
		void setTextureclass(TextureResource* textureResource);
		void setMeshCLass(MeshResource * meshresource);
		Shader* getShader();
		TextureResource* getTextureResource();
		MeshResource* getMeshResource();
		Matrix4 getTransform();
		void loadTexture(std::string filename);
		void draw(std::string vertexShaderName, std::string fragmentShaderName, std::string textureName);
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<TextureResource> textureResource;
		std::shared_ptr<MeshResource> meshResource;
		Matrix4 transform;
		float rotaion = 0;
	};
