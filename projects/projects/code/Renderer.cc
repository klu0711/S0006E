#include "Renderer.h"

Renderer::Renderer(const int& xSize, const int& ySize)
{
	frameBufferSize = xSize * ySize;
	width = xSize; height = ySize;
	frameBuffer = new pixel[frameBufferSize];
	zBuffer = new float[frameBufferSize];
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

pixel* Renderer::getFrameBuffer()
{
	return frameBuffer;
}

int Renderer::getFrameBufferSize()
{
	return frameBufferSize;
}

void Renderer::setVertexShader(vertexShaderOuput(* vertexShader)(vertexShaderInput))
{
	vertexShader = vertexShader;
}

void Renderer::setFragmentShader(void(* fragmentShader)(vertexShaderOuput))
{
	this->fragmentShader = fragmentShader;
}

void Renderer::setTransform(const Matrix4 &mat)
{
	transform = mat;
}

void Renderer::setBuffers()
{
	MeshResource mesh;
	mesh.loadOBJ("triangle.obj");
	faces = mesh.getFaces();
	indices = mesh.getIndicies();

}

void Renderer::rastTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	Vertex firstVertex = v1;
	Vertex secondVertex;
	
	if (firstVertex.pos[0] > v2.pos[0])
	{
		secondVertex = firstVertex;
		firstVertex = v2;
	}
	if (firstVertex.pos[0] > v3.pos[0])
	{
		secondVertex = firstVertex;
		firstVertex = v3;
	}

	// Calculate center and the pixel of each vertex.
	int center = (width * height / 2) - 1;
	int vert1 = center - ((firstVertex.pos[1] * height / 2)* width) + v1.pos[0] * width / 2;
	int vert2 = center - ((secondVertex.pos[1] * height / 2)* width) + v2.pos[0] * width / 2;
	/*int vert3 = center - ((v3.pos[1] * height / 2)* width) + v3.pos[0] * width / 2;*/


	// Calculate the diffrense in x- y- pos between two vertex.
	int deltaX = (secondVertex.pos[0] - firstVertex.pos[0]) * width;
	int deltaY = (secondVertex.pos[1] - firstVertex.pos[1]) * height;
	int* lineOne = new int[deltaY + 1];
	int* lineTwo = new int[deltaY + 1];
	int* lineThree = new int[deltaY + 1];
	if(deltaY = 0)
	{
			
	}
	else if(deltaX = 0)
	{
		
	
	}
	else if(deltaX > deltaY)
	{
		float move_Y = deltaY / deltaX;
		int twoH = 2 * deltaY;
		int temp = -2 * (deltaX - deltaY);
		int F = twoH - deltaX;
		
		for(int i = 0; i < deltaX; i++)
		{
			if (F < 0)
			{
				F += twoH;
			}
			else
			{
				vert1 += width;
				F += temp;
			}
			vert1 += 1;
			//save line where index is y and value is x
			//add vertex pixel coordiante to y when accessing

		}
	}
	else if (deltaY > deltaX)
	{
		float move_X = deltaX / deltaY;

		
	}
	else if (deltaX = deltaY)
	{
		
	}



	//float W = secondVertex.pos[0] - firstVertex.pos[0];
	//float H = secondVertex.pos[1] - firstVertex.pos[1];
	


	// vertex 1 pixel space x

	//Pixel space conversion
	
	//Convert vectors to pixel space

	//Draw line and put pixels in buffer

	//

}

void Renderer::putPixel(int x, int y)
{
	int tempX = floor(x * width/2);
	int tempY = floor(y * height/2);

	frameBuffer[tempX+(width*tempY)];

}

