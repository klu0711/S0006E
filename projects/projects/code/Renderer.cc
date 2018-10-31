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
	line edge1 = createLine(v1, v2);
	line edge2 = createLine(v2, v3);
	line edge3 = createLine(v3, v1);

	for (int i = 0; i < edge1.size; i++)
		frameBuffer[edge1.pixels[i]].red = 1;
		
	for (int i = 0; i < edge2.size; i++)
		frameBuffer[edge2.pixels[i]].red = 1;

	for (int i = 0; i < edge3.size; i++)
		frameBuffer[edge3.pixels[i]].red = 1;



}

line Renderer::createLine(Vertex v1, Vertex v2)
{
	Vertex firstVertex = v1;
	Vertex secondVertex = v2;

	if (firstVertex.pos[1] < v2.pos[1])
	{
		secondVertex = firstVertex;
		firstVertex = v2;
	}

	// Calculate center and the pixel of each vertex.

	int center = (width * height) / 2 - 1;
	int vertx = center - (width / 2) + firstVertex.pos[0] * width/2;
	int verty = (firstVertex.pos[1] * -height/2) * width; 
	int vert1 = vertx + verty;
	//int center = (width * height) / 2 -1;
	//int vert1 = center - width/2 - ((firstVertex.pos[1] * (height/2))* width) + ((firstVertex.pos[0] * (width / 2)));

	// Calculate the diffrense in x- y- pos between two vertex.
	int deltaX = (secondVertex.pos[0] - firstVertex.pos[0]) * width / 2;
	int deltaY = (secondVertex.pos[1] - firstVertex.pos[1]) * -height / 2;


	line pixels;

	int twoH;
	int temp;
	int F;

	int size = std::abs(deltaY);
	int absY = std::abs(deltaY);
	int absX = std::abs(deltaX);
	if (std::abs(deltaX) > std::abs(deltaY))
	{
		twoH = 2 * absY;
		temp = -2 * (absX - absY);
		F = twoH - absX;
	}
	else
	{
		twoH = 2 * absX;
		temp = -2 * (absY - absX);
		F = twoH - absY;
	}

	if (deltaY == 0)
	{

		firstVertex = v1;
		secondVertex = v2;
		if (firstVertex.pos[0] > v2.pos[0])
		{
			secondVertex = firstVertex;
			firstVertex = v2;
			vert1 = center - ((firstVertex.pos[1] * height / 2)* width) + v1.pos[0] * width / 2;
			size = std::abs(deltaX);
		}
		pixels.pixels = new int[size + 1];
		pixels.pixels[0] = vert1;
		for (int i = 0; i < size; i++)
		{
			vert1++;
			pixels.pixels[i + 1] = vert1;
		}
		pixels.size = std::abs(deltaX) + 1;
		return pixels;
	}
	else if (deltaX == 0)
	{
		if (firstVertex.pos[1] > v2.pos[1])
		{
			secondVertex = firstVertex;
			firstVertex = v2;
		}
		pixels.pixels = new int[deltaY + 1];
		pixels.pixels[0] = vert1;
		for (int i = 0; i < deltaY; i++)
		{
			vert1 -= width;
			pixels.pixels[i + 1] = vert1;
		}
		pixels.size = deltaY + 1;
		return pixels;
	}

	pixels.pixels = new int[size + 1];
	if (std::abs(deltaX) > std::abs(deltaY) && ((deltaX > 0 && deltaY < 0) || (deltaX < 0 && deltaY > 0)))
	{
		// Quadrant 7 and 3
		

		pixels.pixels[0] = vert1;
		for (int i = 0; i < deltaY; i++)
		{
			vert1 += 1;
			if (F < 0)
			{
				F += twoH;
			}
			else
			{
				vert1 += width;
				F += temp;
			}
			pixels.pixels[i + 1] = vert1;

			//save line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}
	else if (std::abs(deltaX) < std::abs(deltaY) && ((deltaX > 0 && deltaY < 0) || (deltaX < 0 && deltaY > 0)))
	{
		// Quadrant 6 and 2
		pixels.pixels[0] = vert1;
		for (int i = 0; i < size; i++)
		{
			std::cout << vert1 << std::endl;
			vert1 += width;
			if (F < 0)
			{
				F += twoH;
			}
			else
			{
				vert1 -= 1;
				F += temp;
			}
			pixels.pixels[i + 1] = vert1;

			//save line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}
	else if (std::abs(deltaX) > std::abs(deltaY) && ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)))
	{
		// Quadrant 0 and 4
		pixels.pixels[0] = vert1;
		for (int i = 0; i < deltaY; i++)
		{
			vert1 += 1;
			if (F < 0)
			{
				F += twoH;
			}
			else
			{
				vert1 += width;
				F += temp;
			}
			pixels.pixels[i + 1] = vert1;

			//save line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}
	else if (std::abs(deltaX) < std::abs(deltaY) && ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)))
	{
		// Quadrant 1 and 5

		pixels.pixels[0] = vert1;
		for (int i = 0; i < size; i++)
		{
			//std::cout << vert1 << std::endl;
			vert1 += width;
			if (F < 0)
			{
				F += twoH;
			}
			else
			{
				vert1 += 1;
				F += temp;
			}
			pixels.pixels[i + 1] = vert1;

			//save line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}

}

void Renderer::putPixel(int index)
{


	frameBuffer[index];

}

