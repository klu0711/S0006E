//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "MeshResource.h"
#include <Windows.h>
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


	std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
	std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();

	std::shared_ptr<TextureResource> tex1 = std::make_shared<TextureResource>();
	std::shared_ptr<MeshResource> mesh1 = std::make_shared<MeshResource>();

	Matrix4 perspectiveProjection;
	Matrix4 lookAt;

	Vector4D cameraPos = Vector4D(0.0f, 0.0f, 0.0f, 1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Matrix4 rotX = Matrix4::rotX(0);
	Matrix4 rotY = Matrix4::rotY(0);
	bool click = false;
	bool firstMouse = true;

	float lastX, lastY, yaw = -90.0f, pitch = 0.0f;

	float radianConversion = 3.14159265 / 180;

	int width = 400;
	int height = 300;
	Renderer rend(width, height);



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
			float speed = 0.05f;
			float cameraSpeed = 0.055;
			if (key == GLFW_KEY_W)
			{
				cameraPos = cameraPos + (cameraFront * cameraSpeed);
			}
			if (key == GLFW_KEY_A)
			{
				cameraPos = cameraPos - ((cameraFront.crossProduct(cameraUp)).normalize()) * cameraSpeed;
			}
			if (key == GLFW_KEY_S)
			{
				cameraPos = cameraPos - (cameraFront * cameraSpeed);
			}
			if (key == GLFW_KEY_D)
			{
				cameraPos = cameraPos + ((cameraFront.crossProduct(cameraUp)).normalize()) * cameraSpeed;
			}
			if (key == GLFW_KEY_ESCAPE)
			{
				this->window->Close();
			}
		});

		window->SetMousePressFunction([this](int32 key, int32 action, int32)
		{
			float speed = 0.05f;
			float cameraSpeed = 0.01f;
			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				click = true;
			}
			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				click = false;
				firstMouse = true;
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

				Vector4D front;
				front[0] = cos(yaw * radianConversion) * cos(pitch * radianConversion);
				front[1] = sin(pitch * radianConversion);
				front[2] = sin(yaw * radianConversion) * cos(pitch * radianConversion);
				front[3] = 1;
				cameraFront = front.normalize();
			}
		});


		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			float n = 0.1, f = 1000, r = 1, l = -1, t = 1, b = -1;
			perspectiveProjection = Matrix4(
				2 * n / (r - l), 0, 0, 0,
				0, 2 * n / (t - b), 0, 0,
				((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
				0, 0, -((2 * f * n) / (f - n)), 0
			);





			glEnable(GL_DEPTH_TEST);

			mesh->setupMesh("quad.obj");
			node.setShaderClass(shader);
			node.setMeshCLass(mesh);
			node.setTextureclass(tex);
			node.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);

			/*node2.setShaderClass(shader);
			node2.setMeshCLass(mesh);
			node2.setTextureclass(tex);
			node2.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);*/

			

			//glDisable(GL_FRAMEBUFFER_SRGB);

			bool color = false;


			Matrix4 lookAt = Matrix4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			auto vertexShader = [](Vertex vertex, Matrix4 lookat, Matrix4 modelMatrix) -> Vertex 
			{
				Vector4D position =  lookat * Vector4D(vertex.pos[0], vertex.pos[1], vertex.pos[2], 1.0f);
				Vector4D normal = (Matrix4::inverse(modelMatrix)) * Vector4D(vertex.normal[0], vertex.normal[1], vertex.normal[2], 1.0f);

				Vertex returnVertex;

				returnVertex.pos[0] = position[0];
				returnVertex.pos[1] = position[1];
				returnVertex.pos[2] = position[2];

				returnVertex.uv[0] = vertex.uv[0];
				returnVertex.uv[1] = vertex.uv[1];

				returnVertex.normal[0] = normal[0];
				returnVertex.normal[1] = normal[1];
				returnVertex.normal[2] = normal[2];
				return returnVertex;


			};


			auto fragmentShader = [](Vertex vertex, Vector4D cameraPosition, TextureResource tex) -> Vector4D
			{
				//Constants
				Vector4D lightpos(0.0f, 5.0f, 0, 0);
				Vector4D ambient = Vector4D(0.1f, 0.1f, 0.1f, 1);
				Vector4D color(1,1,1,1);
				Vector4D normal(vertex.normal[0], vertex.normal[1], vertex.normal[2], 0);
				Vector4D position(vertex.pos[0], vertex.pos[1], vertex.pos[2], 1.0f);



				Vector4D posToLightDirVec = (lightpos - cameraPosition).normalize();
				float diffuse = posToLightDirVec.dotProduct(normal);

				if (diffuse < 0)
				{
					diffuse = 0;
				}
				else if (diffuse > 1)
				{
					diffuse = 1;
				}
				Vector4D finalDiffuse = color * diffuse;

				Vector4D lightToPosVec = (lightpos - position).normalize();
				Vector4D posToViewVec = (cameraPosition - position).normalize();

				Vector4D specFinal;
				if (normal.dotProduct((posToLightDirVec)) < 0)
				{
					specFinal = Vector4D(0, 0, 0, 0);

				}
				else
				{
					Vector4D halfWay = (lightToPosVec + posToViewVec).normalize();
					float specularConstant = normal.dotProduct(halfWay);


					if (specularConstant > 0.0)
					{
					    specularConstant = 0;
					}
					specularConstant = std::pow(specularConstant, 8);
					if (specularConstant < 0)
					{
						specularConstant = 0;
					}
					else if (specularConstant > 1)
					{
						specularConstant = 1;
					}
					specFinal = color * specularConstant * 0.25f;
					
				}
				//std::cout << "function is called with " << vertex.uv[0] << ":" << vertex.uv[1] << std::endl;
				float* texture = tex.getColor(vertex.uv[0], vertex.uv[1]);
				Vector4D texColor(texture[0], texture[1], texture[2], 1);
				return texColor.glProduct(ambient + finalDiffuse + specFinal);
				return Vector4D(255, 0, 0, 1);
			};
			rend.setVertexShader(vertexShader);
			rend.setFragmentShader(fragmentShader);
			rend.setBuffers();
			tex->loadBuffer();

			
			return true;
		}
		return false;
	}


	void ExampleApp::Run()
	{
		float rot = 0;
		while (this->window->IsOpen() && true)
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			Matrix4 move = Matrix4(1, 0, 0, 1000,
			                       0, 1, 0, 0,
			                       0, 0, 1, 0,
			                       0, 0, 0, 1);
			 lookAt = Matrix4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			rend.clearZbuffer();
			rend.setTransform(Matrix4::transpose(perspectiveProjection)* lookAt);
			rend.setLookat(Matrix4());
			rend.setCameraPsition(cameraPos);
			for (int i = 0; i < rend.indices.size(); i += 3)
			{

				rend.rastTriangle(rend.faces[rend.indices[i]], rend.faces[rend.indices[i + 1]], rend.faces[rend.indices[i + 2]]);
			}
			tex->loadFromArray(rend.getFrameBuffer(), width, height);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			node.draw();


			this->window->SwapBuffers();
		}
	}
}

