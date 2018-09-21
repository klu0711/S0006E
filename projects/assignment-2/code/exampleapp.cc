//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>



using namespace Display;
namespace Example
{

	float buffer[] =
	{
		-0.5f,	-0.5f,	-1,			// pos 0
		1,		0,		0,		1,	// color 0
		-0.5f,	0.5f,	-1,			// pos 1
		0,		1,		0,		1,	// color 0
		0.5f,	0.5f,	-1,			// pos 2
		0,		0,		1,		1,
		0.5f,   -0.5f,   1,
		1,       0,      1,     1// color 0
	};
	int indexBuffer[] =
	{ 0,1,2,
	 0,2,3};

	MeshResource meshResource(buffer, indexBuffer, sizeof(buffer), sizeof(indexBuffer));
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

		

		// setup
		meshResource.setup();
		meshResource.bindVertexBuffer();
		meshResource.bindIndexBuffer();
		meshResource.bindPointer();
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
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		glUseProgram(this->program);
		// do stuff
		meshResource.bind();
		/// This code sets a mat4 in the vertex shader to a matrix that rotates and moves the quadrilateral
		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		Matrix4 mat = Matrix4().rotZ(rotation);
		Matrix4 mat2 = Matrix4::getPositionMatrix(Vector4D(sinf(movementn), 0, 0, 1));
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, (mat2 * mat).getPointer());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		meshResource.unBindBuffers();
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		rotation += 0.01;
		movementn += 0.01;
		this->window->SwapBuffers();
	}
}

} // namespace Example