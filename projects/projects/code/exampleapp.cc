//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "MeshResource.h"
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_X 120



using namespace Display;
namespace Example
{
	/// Buffer containing verticies, colors, and texture coordinates
	float buffer[108]{
		// positions            // colors                    // texture coords
		 0.5f,  0.5f, 0.5f,        1.0f, 0.0f, 0.0f, 1,        1.0f, 1.0f,    //0    // top right
		 0.5f, -0.5f, 0.5f,        0.0f, 1.0f, 0.0f, 1,        1.0f, 0.0f,    //1    // bottom right
		-0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f, 1,        0.0f, 0.0f,    //2    // bottom left
		-0.5f,  0.5f, 0.5f,        1.0f, 1.0f, 0.0f, 1,        0.0f, 1.0f,    //3    // top left

		 0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1,        0.0f, 1.0f,    //4    // top right
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1,        0.0f, 0.0f,    //5    // bottom right
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 1,        1.0f, 0.0f,    //6    // bottom left
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f, 1,        1.0f, 1.0f,    //7    // top left

		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f, 1,        1.0f, 0.0f,    //8    // top right
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1,        1.0f, 1.0f,    //9    // bottom right
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 1,        0.0f, 1.0f,    //10// bottom left
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f, 1,        0.0f, 0.0f    //11// top left
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
			float speed = 0.05f;
			float cameraSpeed = 0.05f;
			if (key == GLFW_KEY_W)
			{
				position[1] = position[1] + speed;
			}
			if (key == GLFW_KEY_A) {
				position[0] = position[0] - speed;
			}
			if (key == GLFW_KEY_S)
			{
				position[1] = position[1] - speed;
			}
			if (key == GLFW_KEY_D)
			{
				position[0] = position[0] + speed;
			}
			if (key == GLFW_KEY_ESCAPE)
			{
				this->window->Close();
			}
//		meshResource->destroy();


	});



	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		float n = 0.1, f = 10, r = 0.2, l = -0.2, t = 0.2, b = -0.2;
		perspectiveProjection = Matrix4(
			2 * n / (r - l), 0, 0, 0,
			0, 2 * n / (t - b), 0, 0,
			((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
			0, 0, -((2 * f*n) / (f - n)), 0
		);
		

		std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));

		glEnable(GL_DEPTH_TEST);
		node.setShaderClass(shader);
		node.setMeshCLass(mesh);
		node.setTextureclass(tex);

		node.load("texture.jpg", "vertexShader.txt", "fragmentShader.txt");

		

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
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		node.draw();

	
		Matrix4 mat = Matrix4::rotX(rotation);
		Matrix4 mat1 = Matrix4::rotY(rotation);
		float posY, posX, posZ;

		//Vector4D cameraPos = Vector4D(posX, 0, posZ, 1.0f);
		Vector4D cameraTarget = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
		Vector4D cameraReverseDirection = (cameraPos - cameraTarget).normalize();
		Vector4D upX = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4D cameraRight = (upX.crossProduct(cameraReverseDirection)).normalize();
		Vector4D cameraUp = cameraReverseDirection.crossProduct(cameraRight);


		Matrix4 bounce = Matrix4::getPositionMatrix(position);
		
		node.setTransform(Matrix4::transpose(perspectiveProjection)*bounce * mat * mat1);


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