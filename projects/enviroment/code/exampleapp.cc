//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "MeshResource.h"
#include <chrono>
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_X 120
//#define RADIANCON = 3.141592/180;


using namespace Display;

namespace Example
{
	/// Buffer containing verticies, colors, and texture coordinates


	TextureResource tex;

	mat4 perspectiveProjection;

	vec4 cameraPos = vec4(0.0f, 1.0f, 5.0f, 1);
	vec4 cameraFront = vec4(0.0f, 0.0f, -1.0f, 1);
	vec4 cameraUp = vec4(0.0f, 1.0f, 0.0f, 1);

	vec4 position = vec4(0.0f, 0.0f, -1.0f, 1.0f);
	mat4 rotX = mat4::rotX(0);
	mat4 rotY = mat4::rotY(0);
	bool click = false;
	bool firstMouse = true;

	int windowSizeX;
	int windowSizeY;

	float fov = 60;


	float lastX, lastY, yaw = -90.0f, pitch = 0.0f;

	float radianConversion = 3.14159265 / 180;

	ExampleApp::ExampleApp()
	{
	}

	ExampleApp::~ExampleApp()
	{
		// empty
	}


	bool ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;


		window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
		{
			float speed = 0.5f;
			float cameraSpeed = 0.1;
			if (key == GLFW_KEY_W)
			{
				cameraPos = cameraPos + (cameraFront * cameraSpeed);
			}
            else if (key == GLFW_KEY_A)
			{
				cameraPos = cameraPos - ((cameraFront.crossProduct(cameraUp)).normalize()) * cameraSpeed;
			}
            else if (key == GLFW_KEY_S)
			{
				cameraPos = cameraPos - (cameraFront * cameraSpeed);
			}
            else if (key == GLFW_KEY_D)
			{
				cameraPos = cameraPos + ((cameraFront.crossProduct(cameraUp)).normalize()) * cameraSpeed;
			}
            else if (key == GLFW_KEY_ESCAPE)
			{
				this->window->Close();
			}
            else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_8)
			{

			}

		});

		window->SetMousePressFunction([this](int32 key, int32 action, int32)
		{
			float speed = 0.05f;
			float cameraSpeed = 0.001f;
			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				click = true;
			}
			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				click = false;
				firstMouse = true;
			}
			if(key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            {
                node.light.setPosition(cameraPos);
            }
		});

		window->SetMouseMoveFunction([this](float x, float y)
		{
			if (click)
			{
				if (firstMouse)
				{
					lastX = x;
					lastY = y;
					firstMouse = false;
				}

				float xoffset = x - lastX;
				float yoffset = lastY - y;

				lastX = x;
				lastY = y;

				float sensitivity = 0.2;
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				yaw += xoffset;
				pitch += yoffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				vec4 front;
				front[0] = cos(yaw * radianConversion) * cos(pitch * radianConversion);
				front[1] = sin(pitch * radianConversion);
				front[2] = sin(yaw * radianConversion) * cos(pitch * radianConversion);
				front[3] = 1;
				cameraFront = front.normalize3();
			}
		});


		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			this->window->GetSize(windowSizeX, windowSizeY);

			perspectiveProjection = mat4::Perspective(nvgDegToRad(75.0f), (float)windowSizeX/(float)windowSizeY, 1000, 0.1);


			std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
			std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();


            mesh.get()->loadOBJ("tractor.obj");
            node.setShaderClass(shader);
            node.setMeshCLass(mesh);
            node.setTextureclass(tex);
            node.load("tractor.png", "vertexShader.ver", "fShader.frag", 0 );
            node.light.setPosition(cameraPos);
            node.getShader()->modifyUniformInt("diffuser", 0);
            //node.getShader()->modifyUniformFloat("intensity", 1);
            //Back face culling
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
			glDisable(GL_FRAMEBUFFER_SRGB);
			//Depth buffer
			glEnable(GL_DEPTH_TEST);
			return true;
		}
		return false;
	}


	void ExampleApp::Run()
	{
		float rotation = 0;
		float movementn = 0;
        std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
        mat4 ideMat = mat4();
		auto start = clock.now();
        mat4 rotModel;
		while (this->window->IsOpen())
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
            mat4 view = (mat4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
			node.setTransform(mat4::transpose(perspectiveProjection) * view);
			node.getShader()->modifyUniformMatrix("objPosition", &ideMat[0]);
			node.draw();


			this->window->SwapBuffers();
		}
	}
}
