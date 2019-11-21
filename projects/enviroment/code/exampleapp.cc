//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "MeshResource.h"
#include <chrono>
#include <imgui.h>





using namespace Display;

namespace Example
{
	/// Buffer containing verticies, colors, and texture coordinates


	TextureResource tex;

	mat4 perspectiveProjection;
    mat4 view;

	vec4 cameraPos = vec4(0.0f, 0.0f, 20.0f, 1);
	vec4 cameraFront = vec4(0.0f, 0.0f, -1.0f, 1);
	vec4 cameraUp = vec4(0.0f, 1.0f, 0.0f, 1);

	vec4 position = vec4(0.0f, 0.0f, -1.0f, 1.0f);
	mat4 rotX = mat4::rotX(0);
	mat4 rotY = mat4::rotY(0);
	bool click = false;
	bool rightClick = false;
	bool firstMouse = true;

	int windowSizeX;
	int windowSizeY;

	float fov = 90;


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
			float cameraSpeed = 1;
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
                //node.light.setPosition(cameraPos);
                lines.clearLines();
                rightClick = true;
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

			if(rightClick == true)
            {
			    this->convertedMouseX = ((x - ((float)this->screenWidth/(float)2)) / (float)this->screenWidth) * 2;
			    this->convertedMouseY = ((y - ((float)this->screenHeight/(float)2)) / (float)this->screenHeight * -2);
                //this->convertedMouseX = 0;
			    //this->convertedMouseY = 0;

			    vec4 ray_clip(this->convertedMouseX, this->convertedMouseY, -1.0f, 1.0f);

			    vec4 ray_eye = mat4::inverse((perspectiveProjection)) * ray_clip;

			    ray_eye = vec4(ray_eye[0], ray_eye[1], -1, 0.0);
			    vec4 ray_world = (mat4::inverse(mat4::transpose(view))) * ray_eye;

			    ray_world = ray_world.normalize3();
			    cameraPos[3] = 1;
			    ray_world[3] = 0;//APPARENTLY THIS IS NECESSARY OTHERWISE NOTHING WORKS????????? WHY

			    ray r(cameraPos, ray_world);
                for (int i = 0; i < quads.quads.size(); ++i)
                {
                    vec4 quadRes = r.intersectQuad(quads.quads[i]);
                    if(quadRes[3] != -1)
                    {
                        quadRes[3] = 1;
                        cubes.addCube(vec4(0.1,0.1,0.1,1), quadRes , 120, true);
                        break;
                    }
                }

                for (int j = 0; j < cubes.cubes.size(); ++j)
                {
                    vec4 cubeRes = r.intersectCube(cubes ,cubes.cubes[j]);

                    if(cubeRes[3] != -1)
                    {
                        cubeRes[3] = 1;
                        cubes.addCube(vec4(0.5, 0.5, 0.5, 1), cubeRes, 200, true);
                        break;
                    }
                }
			    vec4 result  = r.intersectPlane(quads.quads[0].quadPlane);
                lines.addLine(r);

			    rightClick = false;
            }
		});


		if (this->window->Open())
		{
		    window->GetSize(screenWidth, screenHeight);
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			this->window->GetSize(windowSizeX, windowSizeY);

			perspectiveProjection = mat4::Perspective(nvgDegToRad(fov), (float)windowSizeX/(float)windowSizeY, 0.1f, 1000);


			std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
			std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();
            std::shared_ptr<MeshResource> cubeMesh = std::make_shared<MeshResource>();
            cubeMesh->loadOBJ("cube.obj");
            cubes.bindAttrPointers();
            cubes.addMesh(cubeMesh);
            cubes.init("lineShader.ver", "lineShader.frag");
            //cube.addCube(vec4(5,1,1,1), vec4(1,1,1,1), 1000, false);
            //cube.addCube(vec4(5,1,1,1), vec4(3,3,3,1), 1001, true);
            cubeMesh->unBindBuffers();

            mesh->loadOBJ("tractor.obj");
            node.setShaderClass(shader);
            node.setMeshCLass(mesh);
            node.setTextureclass(tex);
            node.load("tractor.png", "vertexShader.ver", "fShader.frag", 0 );
            node.light.setPosition(cameraPos);
            node.getShader()->modifyUniformInt("diffuser", 0);

            // AABB
            vec4 midPoint =  (mesh->max + mesh->min) * 0.5f;
            vec4 sizeAABB =  (mesh->max - mesh->min);


            cubes.addCube(sizeAABB, midPoint, 100000, true, mesh);


            quads.init("lineShader.ver", "lineShader.frag");
            quads.addQuad(vec4(0,2,0,1), vec4(2,2,1,1), mat4());


            //Back face culling
            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_BACK);
			glDisable(GL_FRAMEBUFFER_SRGB);
			//Depth buffer
			glEnable(GL_DEPTH_TEST);





        window->SetUiRender([]
        {
            //ImGui::ShowDemoWindow();
            ImGui::Begin("Test Tool", nullptr, ImGuiWindowFlags_MenuBar);
            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("File"))
                {
                    if(ImGui::MenuItem("clear lines", "press this")) {/*Do Stuff*/}
                    if(ImGui::MenuItem("Save", "press this")) {/*Do Stuff*/}
                    if(ImGui::MenuItem("Close", "press this")) {/*Do Stuff*/}
                    if(ImGui::MenuItem("Test", "press this")) {/*Do Stuff*/}
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            vec4 vec;
            ImGui::ColorEdit4("Color", &vec[0]);

            ImGui::End();
        });


			return true;
		}
		return false;
	}


	void ExampleApp::Run()
	{

        mat4 ideMat = mat4();
        mat4 rotModel;
        lines.init("lineShader.ver", "lineShader.frag");
        //line.addLine(vec4(0,0,0,1), vec4(4,2,-10,1));
        //line.addLine(vec4(0,0,0,1), vec4(0,2,-10,1));
		while (this->window->IsOpen())
		{

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->window->Update();

            view = (mat4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
            mat4 transform = view * perspectiveProjection;
            lines.draw(transform);
            cubes.draw(transform);
            node.getShader()->useProgram();
            node.setTransform(transform);
            node.getShader()->modifyUniformMatrix("objPosition", &ideMat[0]);
            node.draw();
            quads.draw(transform);




            this->window->SwapBuffers();
        }
    }
}
