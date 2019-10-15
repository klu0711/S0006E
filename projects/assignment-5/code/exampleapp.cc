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

	Vector4D cameraPos = Vector4D(0.0f, 1.0f, 5.0f, 1);
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
				clipToPlay = key - GLFW_KEY_1;
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

			std::shared_ptr<TextureResource> footmanDiffuse = std::make_shared<TextureResource>();
			std::shared_ptr<MeshResource> mesh1 = std::make_shared<MeshResource>();

            s.loadSkeleton("Unit_Footman.constants");
            s.loadMesh("Unit_Footman.nvx2");
            glEnable(GL_DEPTH_TEST);
            mesh->setupMesh("sphere.obj");
            mesh1->setupMeshSkin(s.indexDataPtr, s.vertexDataPtr, s.indexDataSize, s.vertexDataSize, s.numVertices, s.header->numIndices);
            a.loadAnimations("Unit_Footman.nax3");




            //Bind the normal map the the gpu

            //Bind all the classes to the GraphicsNode for the footman
            this->node2.setShaderClass(shader1);
            this->node2.setMeshCLass(mesh1);
            this->node2.setTextureclass(footmanDiffuse);
            dMap = this->node2.load("Footman_Diffuse.tga", "customVertexShader.ver", "fragmentShader.frag", 0);
            this->node2.light = LightingNode(Vector4D(0,2,-5,1), Vector4D(1,1,1,1), 1);
            nMap = footmanNormalMap.get()->loadFromFile("Footman_Normal.tga");
            footmanNormalMap.get()->bind(1);
            // Change what texture is being used
            shader1.get()->modifyUniformInt("diffuser", 0);
            shader1.get()->modifyUniformInt("normalMap", 1);

            for (int i = 0; i < s.joints->size(); ++i)
            {
                GraphicsNode* n = &s.joints->at(i).node;
                n->setShaderClass(shader);
                n->setMeshCLass(mesh);
                n->setTextureclass(tex);
                n->load("Footman_Diffuse.tga", "vertexShader.ver", "fragmentShader.frag", -1);

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
        std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
        Matrix4 ideMat = Matrix4();
		auto start = clock.now();
		while (this->window->IsOpen())
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, dMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, nMap);
			Matrix4 view = (Matrix4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
            // used to play animations
            using ms = std::chrono::duration<float, std::milli>;
            float animationSpeed = std::chrono::duration_cast<ms>(clock.now() - start).count() / a.clips[clipToPlay].keyDuration;
            Matrix4 jointMats[21];
            Vector4D scaleBalls(0.3, 0.3, 0.3, 1);
            for (int k = 0; k < s.joints->size() ; ++k)
            {
                //Load animation data for one key in a clip
                Vector4D pos = a.getKey(clipToPlay, animationSpeed, k*4, 0);
                Matrix4 po = Matrix4::getPositionMatrix(pos);
                Vector4D rot = a.getKey(clipToPlay, animationSpeed, k*4 + 1, 1);
                Matrix4 ro = Matrix4::getQmat(rot);
                Vector4D scale = a.getKey(clipToPlay, animationSpeed, k*4 + 2, 0);
                Matrix4 sc = Matrix4::scaleMat(scale);
                Vector4D vel = a.getKey(clipToPlay, animationSpeed, k*4 + 3, 0);
                Matrix4 res = po*ro*sc;
                s.joints->at(k).localTransform = res;

                // Draw balls
                GraphicsNode* n = &s.joints->at(k).node;
                n->setTransform(Matrix4::transpose(perspectiveProjection) * view * s.joints->at(k).transform * Matrix4::scaleMat(scaleBalls));
                // Update the joint matricies
                s.updateJoints(0);
                // reset joints to bind pose
                jointMats[k] = s.joints->at(s.skinJoints[k]).transform * s.joints->at(s.skinJoints[k]).inverseBindPose ;

                n->draw();

            }

            glUseProgram(node2.getShader()->getProgram());
            node2.getShader()->modifyUniformMatrix("model", &ideMat[0]);
            node2.getShader()->modifyUniformMatrix("view", &view[0]);
            node2.getShader()->modifyUniformMatrix("projection", &Matrix4::transpose(perspectiveProjection)[0]);
            node2.getShader()->modifyUniformVector("cameraPosition", cameraPos);
            this->node2.draw();
            node2.getShader()->modifyUniformMats(21, jointMats);
            this->node2.setTransform(Matrix4::transpose(perspectiveProjection) * view);

            Vector4D pos(0,0,0,0);
            Vector4D x(0,cos(rotation)*0.1f,0,1);

			Matrix4 worldToScreenSpaceMat = view * Matrix4::transpose(perspectiveProjection);

            // draw skeleton lines using the old openGL pipeline
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
            //std::this_thread::sleep_for(std::chrono::milliseconds(150));
			this->window->SwapBuffers();
		}
	}
}
