#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "MeshResource.h"
#include "Matrix4.h"
#include "Vector4D.h"
#include <iostream>
#include "Shader.h"
#include "TextureResource.h"
#include "GraphicsNode.h"
#include "LightingNode.h"
#include <thread>
#include "skeleton.h"
#include "animation.h"
#include "animation.h"
#include <chrono>
namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();
	void keyListener();

	/// open app
	bool Open();
	/// run app
	void Run();
private:


	const GLchar* vs =
		"#version 430\n"
		"layout(location=0) in vec3 pos;\n"
		"layout(location=1) in vec4 color;\n"
		"layout(location=2) in vec2 texCords;\n"
		"out vec2 texCords2;\n"
		"out vec4 Color;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = transform * vec4(pos, 1.0f);\n"
		"	texCords2 = texCords;\n"	
		"	Color = color;\n"	
		"}\n";

	const GLchar* ps =
		"#version 430\n"
		"out vec4 fragColor;\n "
		"in vec2 texCords2;\n"
		"in vec4 Color;\n"
		"uniform sampler2D diffuse;\n"
		"void main()\n"
		"{\n"
		"	fragColor = texture(diffuse, texCords2);\n"
		"}\n";
	//GLuint program;
	//GLuint vertexShader;
	//GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
	GraphicsNode node;
	GraphicsNode node2;
	GraphicsNode node3;
	GraphicsNode node4;
	GraphicsNode node5;
	GraphicsNode node6;
	GraphicsNode node7;
	GraphicsNode node8;
	GraphicsNode node9;
	GraphicsNode node10;
	skeleton s;
    Vector4D scale = Vector4D(0.4,0.4,0.4,1);
    animation a;
    unsigned int clipToPlay = 0;

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	std::shared_ptr<Shader> shader1 = std::make_shared<Shader>();

};
} // namespace Example