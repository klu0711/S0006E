#pragma once
#include "TextureResource.h"
#include "Shader.h"
#include "MeshResource.h"
#include <memory>
#include "LightingNode.h"


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
        mat4 getTransform();
		void setTransform(mat4 mat);
		uint load(std::string filename, std::string vertexShaderName, std::string fragmentShaderName, int texture);
		void draw();
		LightingNode light;
	private:

		std::shared_ptr<Shader> shader;
		std::shared_ptr<TextureResource> textureResource;
		std::shared_ptr<MeshResource> meshResource;
        mat4 transform;
		float rotaion = 0;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		std::vector<vec4> uv;
		std::vector<vec4> normals;
		std::vector<vec4> combinedbuffer;


	};
