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
#include "Renderer.h"
#include <algorithm>
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

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	std::shared_ptr<Shader> shader1 = std::make_shared<Shader>();

};
} // namespace Example