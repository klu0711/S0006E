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
#include "debugQuad.h"
#include "debugCube.h"
#include "debugLine.h"
#include "ray.h"
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
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	debugCube cubes;
    debugLine lines;
    debugQuad quads = debugQuad();
    std::vector<ray> rays;
    int screenWidth;
    int screenHeight;

    float convertedMouseX;
    float convertedMouseY;




};
} // namespace Example