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

	Matrix4 perspectiveProjection;

	Vector4D cameraPos = Vector4D(0.0f, 3.0f, 3.0f, 1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Matrix4 rotX = Matrix4::rotX(0);
	Matrix4 rotY = Matrix4::rotY(0);
	bool click = false;
	bool firstMouse = true;

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
			float cameraSpeed = 0.05;
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
        s.loadSkeleton("Unit_Footman.constants");

		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			float n = 0.1, f = 1000, r = 0.1, l = -0.1, t = 0.1, b = -0.1;
			perspectiveProjection = Matrix4(
				2 * n / (r - l), 0, 0, 0,
				0, 2 * n / (t - b), 0, 0,
				((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
				0, 0, -((2 * f * n) / (f - n)), 0
			);


			std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
			std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();

			std::shared_ptr<TextureResource> tex1 = std::make_shared<TextureResource>();
			std::shared_ptr<MeshResource> mesh1 = std::make_shared<MeshResource>();


			glEnable(GL_DEPTH_TEST);
			mesh->setupMesh("sphere.obj");
			/*node.setShaderClass(shader);
			node.setMeshCLass(mesh);
			node.setTextureclass(tex);
			node.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);

			node2.setShaderClass(shader);
			node2.setMeshCLass(mesh);
			node2.setTextureclass(tex);
			node2.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);*/


            a.loadAnimations("Unit_Footman.nax3");




            for (int j = 0; j < 10; ++j) {

            }

            for (int i = 0; i < s.joints->size(); ++i) {
                GraphicsNode* n = &s.joints->at(i).node;
                n->setShaderClass(shader);
                n->setMeshCLass(mesh);
                n->setTextureclass(tex);
                n->load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);

            }

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

			glDisable(GL_FRAMEBUFFER_SRGB);
			return true;
		}
		return false;
	}


	void ExampleApp::Run()
	{
		float rotation = 0;
		float movementn = 0;
		while (this->window->IsOpen())
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			/*Matrix4 move = Matrix4(1, 0, 0, 10,
			                       0, 1, 0, 0,
			                       0, 0, 1, 0,
			                       0, 0, 0, 1);*/
			Matrix4 view = Matrix4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            // currentTime - StartTime * speedFactor
            for (int k = 0; k < s.joints->size() ; ++k)
            {
                Vector4D pos = a.getKey(clipToPlay, 0, k*4);
                Matrix4 po = Matrix4::getPositionMatrix(pos);
                Vector4D rot = a.getKey(clipToPlay, 0, k*4 + 1);
                Matrix4 ro = Matrix4::getQmat(rot);
                Vector4D scale = a.getKey(clipToPlay, 0, k*4 + 2);
                Matrix4 sc = Matrix4::scaleMat(scale);
                Vector4D vel = a.getKey(clipToPlay, 0, k*4 + 3);
                Matrix4 res = po*ro*sc;
                s.joints->at(k).localTransform = res;

            }
            s.updateJoints(0);

			//node.setTransform(Matrix4::transpose(perspectiveProjection) * view);
			//node2.setTransform(Matrix4::transpose(perspectiveProjection) * view * move);

			//shader->modifyUniformMatrix("objPosition", Matrix4().getPointer());
			//shader->modifyUniformVector("cameraPosition", cameraPos);
			//node.draw();
			//shader->modifyUniformMatrix("objPosition", move.getPointer());
			//node2.draw();

			//Vector4D line1(0,0,0,1);
			//Vector4D line2(1,1,1,1);

            for (int i = 0; i <  s.joints->size(); ++i)
            {
                GraphicsNode* n = &s.joints->at(i).node;
                n->setTransform(Matrix4::transpose(perspectiveProjection) * view * s.joints->at(i).transform * Matrix4::scaleMat(scale));

                n->draw();
            }
            Vector4D x(0,cos(rotation)*0.1f,0,1);

			Matrix4 worldToScreenSpaceMat = view * Matrix4::transpose(perspectiveProjection);


			glUseProgram(0);
			glMatrixMode(GL_MODELVIEW);
			auto viewMat = Matrix4::transpose(view);
			glLoadMatrixf((GLfloat*)&viewMat);
            glMatrixMode(GL_PROJECTION);
            auto dood = (perspectiveProjection);
            glLoadMatrixf((GLfloat*)&dood);
			glBegin(GL_LINES);
			glColor3f(255, 0, 0);
            for (int i = 0; i < s.joints->size(); ++i) {
                joint Joint = s.joints->at(i);
                if(Joint.parent != -1)
                {
                    Vector4D a = Joint.transform.getPositionVec();
                    Vector4D b = s.joints->at(Joint.parent).transform.getPositionVec();
                    glVertex3f(a[0], a[1], a[2]);
                    glVertex3f(b[0], b[1], b[2]);
                }

            }

			glEnd();
            rotation += 0.1;
            //s.moveJoint(Matrix4::getPositionMatrix(x), 2);

			this->window->SwapBuffers();
		}
	}
}
