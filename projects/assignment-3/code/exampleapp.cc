//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "TextureResource.h"




using namespace Display;
namespace Example
{
	/// Buffer containing verticies, colors, and texture coordinates
	float buffer[72]{
		-0.25f, -0.25f, 0.25f,
		 1, 1, 1,1,
		 0,0,

		0.25f,  -0.25f, 0.25f,
		 1, 1, 1,1,
		 1,0,

		 0.25f,  0.25f, 0.25f,
		 1, 0, 1,1,
		 1,1,

		 -0.25f, 0.25f, 0.25f,
		 1, 0, 1,1,
		 0,1,

		-0.25f, -0.25f, -0.25f,
		 0, 1, 0,1,
		 0,1,

		0.25f,  -0.25f, -0.25f,
		 0, 1, 0,1,
		 1,0, 

		 0.25f,  0.25f, -0.25f,
		 0, 0, 1,1,
		 0,1, 

		 -0.25f, 0.25f, -0.25f,
		 0, 0, 1,1,
		 1,1
	};
	int indexBuffer[36]{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	MeshResource meshResource(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));


	Matrix4 perspectiveProjection;

	TextureResource tex;

	
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
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
		meshResource.destroy();


	});



	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


		// setup vertex shader


		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		int32 length = std::strlen(this->vs);
		glShaderSource(vertexShader, 1, &this->vs, &length);
		glCompileShader(vertexShader);



		// setup pixel shader
		pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(this->ps);
		glShaderSource(pixelShader, 1, &this->ps, &length);
		glCompileShader(pixelShader);




		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, vertexShader);
		glAttachShader(this->program, pixelShader);
		glLinkProgram(this->program);


		glUseProgram(program);

		
		glEnable(GL_DEPTH_TEST);
		// setup
		meshResource.setup();
		meshResource.bindVertexBuffer();
		meshResource.bindIndexBuffer();
		tex.loadFromFile("texture.jpg");
		meshResource.bindPointer();

		float n = 0.5, f = 5, r = 0.3, l = -0.3, t = 0.3, b = -0.3;
		perspectiveProjection = Matrix4(
			2 * n / (r - l), 0, 0, 0,
			0, 2 * n / (t - b), 0, 0,
			((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
			0, 0, -((2 * f*n) / (f - n)), 0
		);
		


		

		return true;
		
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	float rotation = 0;
	float movementn = 0;
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		this->window->Update();

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
		Vector4D cameraPos = Vector4D(camX, 0, camZ, 1.0f);
		Vector4D cameraTarget = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
		Vector4D cameraReverseDirection = (cameraPos - cameraTarget).normalize();
		Vector4D upX = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4D cameraRight = (upX.crossProduct(cameraReverseDirection)).normalize();
		Vector4D cameraUp = cameraReverseDirection.crossProduct(cameraRight);



		Matrix4 lookAt = Matrix4::lookAt(cameraRight, upX, cameraReverseDirection, cameraPos);



		glUniformMatrix4fv(transformLoc, 1, GL_TRUE,(Matrix4::transpose(perspectiveProjection)*lookAt*mat).getPointer());
		tex.bind(0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		meshResource.unBindBuffers();
		rotation += 0.01;
		movementn += 0.01;
		this->window->SwapBuffers();
	}
}

} // namespace Example