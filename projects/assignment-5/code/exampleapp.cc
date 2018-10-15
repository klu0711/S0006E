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
	float buffer[108]{
		// positions            // colors                    // texture coords
		 0.25f,  0.25f, 0.25f,        1.0f, 0.0f, 0.0f, 1,        1.0f, 1.0f,    //0    // top right
		 0.25f, -0.25f, 0.25f,        0.0f, 1.0f, 0.0f, 1,        1.0f, 0.0f,    //1    // bottom right
		-0.25f, -0.25f, 0.25f,        0.0f, 0.0f, 1.0f, 1,        0.0f, 0.0f,    //2    // bottom left
		-0.25f,  0.25f, 0.25f,        1.0f, 1.0f, 0.0f, 1,        0.0f, 1.0f,    //3    // top left

		 0.25f,  0.25f, -0.25f,    1.0f, 0.0f, 0.0f, 1,        0.0f, 1.0f,    //4    // top right
		 0.25f, -0.25f, -0.25f,    0.0f, 1.0f, 0.0f, 1,        0.0f, 0.0f,    //5    // bottom right
		-0.25f, -0.25f, -0.25f,    0.0f, 0.0f, 1.0f, 1,        1.0f, 0.0f,    //6    // bottom left
		-0.25f,  0.25f, -0.25f,    1.0f, 1.0f, 0.0f, 1,        1.0f, 1.0f,    //7    // top left

		 0.25f,  0.25f, -0.25f,    0.0f, 0.0f, 0.0f, 1,        1.0f, 0.0f,    //8    // top right
		 0.25f, -0.25f, -0.25f,    0.0f, 1.0f, 0.0f, 1,        1.0f, 1.0f,    //9    // bottom right
		-0.25f, -0.25f, -0.25f,    0.0f, 0.0f, 1.0f, 1,        0.0f, 1.0f,    //10// bottom left
		-0.25f,  0.25f, -0.25f,    1.0f, 1.0f, 0.0f, 1,        0.0f, 0.0f    //11// top left
	};
	int indexBuffer[36]{
		0,1,3,        //first triangle        Front
		1,2,3,        //second triangle

		4,5,7,        //third triangle        Back
		5,6,7,        //fourth triangle

		0,5,4,        //fifth triangle        Left
		0,1,5,        //sixth triangle

		3,7,2,        //seventh triangle        Right
		2,7,6,        //eigth triangle

		0,8,11,        //nineth triangle        Top
		0,11,3,        //tenth triangle

		1,2,9,        //eleventh triangle        Bottom
		2,10,9        //twelth triangle
	};

	
	//MeshResource * meshResource = new MeshResource(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));
	TextureResource tex;

	//TextureResource tex;
	Matrix4 perspectiveProjection;

	Vector4D cameraPos = Vector4D(0.0f, 0.0f, 3.0f,1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Matrix4 rotX = Matrix4::rotX(0);
	Matrix4 rotY = Matrix4::rotY(0);
	bool click = false;
	bool firstMouse = true;

	float lastX, lastY, yaw = -90.0f, pitch = 0.0f;
	
	float radianConversion = 3.14159265 / 180;

	
//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{


}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/

void fps()
{
	static float fps = 0.0f;
	static float before = 0.0f;
	static char strFPS[20] = { 0 };
	static float now = (GetTickCount()*0.001f);

	++fps;

	if (now - before > 1.0f)
	{
		before = now;
		sprintf(strFPS, "FPS: %d", int(fps));
		fps = 0.0f;
	}
	std::cout << strFPS << std::endl;
}
void ExampleApp::keyListener() {
	char key = getch();
	int value = key;

	while (key != KEY_X) {
		switch (getch())
		{
		case KEY_UP:
			//do something
			break;
		case KEY_DOWN:
			//do something
			break;
		case KEY_LEFT:
			//do something
			break;
		case KEY_RIGHT:
			//do something
			break;

		}
		key = getch();
		value = key;
	}
}
bool ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;


	window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
	{
			float speed = 0.5f;
			float cameraSpeed = 0.3f;
			if (key == GLFW_KEY_W)
			{
				cameraPos = cameraPos + (cameraFront * cameraSpeed);
				//position[1] = position[1] + speed;
			}
			if (key == GLFW_KEY_A) {
				cameraPos = cameraPos - ((cameraFront.crossProduct(cameraUp)).normalize())*cameraSpeed;
				//position[0] = position[0] - speed;
			}
			if (key == GLFW_KEY_S)
			{
				cameraPos = cameraPos - (cameraFront * cameraSpeed);
				//position[1] = position[1] - speed;
			}
			if (key == GLFW_KEY_D)
			{
				cameraPos = cameraPos + ((cameraFront.crossProduct(cameraUp)).normalize())*cameraSpeed;
				//position[0] = position[0] + speed;
			}
			if (key == GLFW_KEY_ESCAPE)
			{
				this->window->Close();
			}
//		meshResource->destroy();


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

	window->SetMouseMoveFunction([this](float x, float y) {
		if (click)
		{
			if (firstMouse) {
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
			front[0] = cos(yaw*radianConversion) * cos(pitch*radianConversion);
			front[1] = sin(pitch*radianConversion);
			front[2] = sin(yaw*radianConversion) * cos(pitch*radianConversion);
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
			0, 0, -((2 * f*n) / (f - n)), 0
		);
		

		std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
		std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));
		
		std::shared_ptr<TextureResource> tex1 = std::make_shared<TextureResource>();
		std::shared_ptr<MeshResource> mesh1 = std::make_shared<MeshResource>(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));


		glEnable(GL_DEPTH_TEST);




		node.setShaderClass(shader);
		node.setMeshCLass(mesh);
		node.setTextureclass(tex);
		node.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);



		node2.setShaderClass(shader);
		node2.setMeshCLass(mesh);
		node2.setTextureclass(tex);
		node2.load("tractor.png", "vertexShader.ver", "fragmentShader.frag", 0);

		glDisable(GL_FRAMEBUFFER_SRGB);
		return true;
		
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void ExampleApp::Run()
{
	float rotation = 0;
	float movementn = 0;
	auto clock = std::chrono::high_resolution_clock();     auto start = clock.now();     int frames = 0;
	while (this->window->IsOpen())
	{
		frames++;
		if (std::chrono::duration_cast<std::chrono::seconds>(clock.now() - start).count() > 1)
		{
			auto end = clock.now();
			auto t = ((std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / (double)frames) / 1000.0);
			std::cout << t << "\t" << 1 / (t / 1000.0) << "\t" << frames << "\r\n";
			//this->window->SwapBuffers();
			start = clock.now();
			frames = 0;
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		Matrix4 move = Matrix4(1, 0, 0, 10,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		Matrix4 lookAt = Matrix4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		node.setTransform(Matrix4::transpose(perspectiveProjection)*lookAt);
		node2.setTransform(Matrix4::transpose(perspectiveProjection)*lookAt * move);

		shader->modifyUniformMatrix("objPosition",Matrix4().getPointer());
		shader->modifyUniformVector("cameraPosition", cameraPos);
		node.draw();
		shader->modifyUniformMatrix("objPosition", move.getPointer());
		node2.draw();


		
		



	
		Matrix4 mat = Matrix4::rotX(rotation);
		Matrix4 mat1 = Matrix4::rotY(rotation);
		float posY, posX, posZ;

		//Vector4D cameraPos = Vector4D(posX, 0, posZ, 1.0f);
		/*Vector4D cameraTarget = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
		Vector4D cameraReverseDirection = (cameraPos - cameraTarget).normalize();
		Vector4D upX = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4D cameraRight = (upX.crossProduct(cameraReverseDirection)).normalize();
		Vector4D cameraUp = cameraReverseDirection.crossProduct(cameraRight);*/


		Matrix4 bounce = Matrix4::getPositionMatrix(position);
		
	//	node.setTransform(Matrix4::transpose(perspectiveProjection)*bounce * mat * mat1);



		

/*
		glUseProgram(this->program);
		// do stuff
		meshResource.bind();

		/// This code sets a mat4 in the vertex shader to a matrix that rotates and moves the quadrilateral
		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		/// Create rotations matricies
		Matrix4 mat = Matrix4::rotX(rotation);
		Matrix4 mat1 = Matrix4::rotY(rotation);
		Matrix4 mat2 = Matrix4::getPositionMatrix(Vector4D(sinf(movementn), 0, 0, 1));
		/// Code for creating all of the verticies and matricies that are needed to create the look at matrix
		float radius = 1.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;




		glUniformMatrix4fv(transformLoc, 1, GL_TRUE,(Matrix4::transpose(perspectiveProjection)*lookAt*mat).getPointer());
		tex.bind(0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		meshResource.unBindBuffers();*/
		rotation += 0.01;
		movementn += 0.01;
		this->window->SwapBuffers();
	}
}

} // namespace Example