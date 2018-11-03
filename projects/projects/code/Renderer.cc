#include "Renderer.h"

Renderer::Renderer(const int& xSize, const int& ySize)
{
	frameBufferSize = xSize * ySize;
	width = xSize; height = ySize;
	frameBuffer = new pixel[frameBufferSize];
	zBuffer = new float[frameBufferSize];



	std::fill_n(zBuffer, frameBufferSize, 20000);
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	//delete[] frameBuffer;
	//delete[] zBuffer;
}


pixel* Renderer::getFrameBuffer()
{
	return frameBuffer;
}

int Renderer::getFrameBufferSize()
{
	return frameBufferSize;
}

void Renderer::setVertexShader(std::function<Vertex(Vertex vertex, Matrix4 lookat, Matrix4 modelMatrix)> shader)
{
	this->vertexShader = shader;
}

void Renderer::setFragmentShader(std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, pixel* tex, int width, int height)> shader)
{
	this->fragmentShader = shader;
}

void Renderer::setTransform( Matrix4 mat)
{
	transform = mat;
}

void Renderer::setLookat(Matrix4 mat)
{
	lookAt = mat;
}

void Renderer::setCameraPsition(const Vector4D& vec)
{
	cameraPosition = vec;
}

void Renderer::setTexture(pixel* p, int width, int height)
{
	drawTexture = p;
	drawTextureHeigth = height;
	drawTextureWidth = width;
}

void Renderer::setBuffers()
{
	MeshResource mesh;
	mesh.loadOBJ("tractor.obj");
	faces = mesh.getFaces();
	indices = mesh.getIndicies();

}

void Renderer::clearZbuffer()
{
	std::fill_n(zBuffer, frameBufferSize, 20000);
	pixel p;
	std::fill_n(frameBuffer, frameBufferSize, p);
}

void Renderer::rastTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	v1 = vertexShader(v1, transform, lookAt);
	v2 = vertexShader(v2, transform, lookAt);
	v3 = vertexShader(v3, transform, lookAt);
	/// Sort the verticeis so that they are sorted top to bottom
	if (v1.pos[1] < v2.pos[1])
	{
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}

	if (v2.pos[1] < v3.pos[1])
	{
		Vertex temp = v2;
		v2 = v3;
		v3 = temp;
	}

	if(v1.pos[1] < v2.pos[1])
	{
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}
	/// Save the coordinates before the pixel space conversion
	float coordianteSpace[6];
	coordianteSpace[0] = v1.pos[0];
	coordianteSpace[1] = v1.pos[1];
	coordianteSpace[2] = v2.pos[0];
	coordianteSpace[3] = v2.pos[1];
	coordianteSpace[4] = v3.pos[0];
	coordianteSpace[5] = v3.pos[1];
	/// Convert the x and y coordiantes to pixel space
	v1.pos[0] = v1.pos[0] * width / 2 + width / 2;
	v1.pos[1] = -v1.pos[1] * height / 2 + height / 2;
	v2.pos[0] = v2.pos[0] * width / 2 + width / 2;
	v2.pos[1] = -v2.pos[1] * height / 2 + height / 2;
	v3.pos[0] = v3.pos[0] * width / 2 + width / 2;
	v3.pos[1] = -v3.pos[1] * height / 2 + height / 2;
	/// Draw all the lines between the verticeis
	Line edge1 = createLine2(v1, v3);
	Line edge2 = createLine2(v1, v2);
	Line edge3 = createLine2(v2, v3);
	int vert1Y = v1.pos[1];
	int vert2Y = v2.pos[1];
	/// Fill in the pixels between the lines
	Line lineArray[] = { edge2, edge3 };
	int u = 0;
	bool isSwapped = false;
	int z = 0;
	for (int i = 0; i < edge1.size; i++)
	{
		if (!isSwapped && u >= edge2.size) {
		

			u = 1;
			z = 1;
			isSwapped = true;
			vert2Y = v3.pos[1];
		}

		linescan(edge1.pixels[i], lineArray[z].pixels[u], vert1Y + i, v1, v2, v3, coordianteSpace);

		u++;
	}



	/// Delete all the memory that was allocated with new
	delete[] edge1.pixels;
	delete[] edge2.pixels;
	delete[] edge3.pixels;

}

Line Renderer::createLine(Vertex v1, Vertex v2)
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


	Line pixels;

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
	if (std::abs(deltaX) >= std::abs(deltaY) && ((deltaX > 0 && deltaY < 0) || (deltaX < 0 && deltaY > 0)))
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

			//save Line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}
	else if (std::abs(deltaX) <= std::abs(deltaY) && ((deltaX > 0 && deltaY < 0) || (deltaX < 0 && deltaY > 0)))
	{
		// Quadrant 6 and 2
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
				vert1 -= 1;
				F += temp;
			}
			pixels.pixels[i + 1] = vert1;

			//save Line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}
	else if (std::abs(deltaX) > std::abs(deltaY) && ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)))
	{
		// Quadrant 0 and 4
		pixels.pixels[0] = vert1;
		for (int i = 0; i < deltaX; i++)
		{
			std::cout << vert1 << std::endl;
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

			//save Line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = deltaX + 1;
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

			//save Line where index is y and value is x
			//add vertex pixel coordiante to y when accessing
		}
		pixels.size = size + 1;
		return pixels;
	}

}

Line Renderer::createLine2(Vertex v1, Vertex v2)
{
	/// Sort the verticies top to bottom
	/// This may be redundant but i am scared to remove it 
	Vertex firstVertex = v1;
	Vertex secondVertex = v2;
	if (v1.pos[1] < v2.pos[1])
	{
		
	}
	else
	{
		firstVertex = v2;
		secondVertex = v1;
	}


	int vert1X = firstVertex.pos[0];
	int vert1Y = firstVertex.pos[1];
	int vert2X = secondVertex.pos[0];
	int vert2Y = secondVertex.pos[1];

	int dx, dy, F;
	
	dx = std::abs(vert1X - vert2X);
	dy = std::abs(vert1Y - vert2Y);
	Line line;
	line.pixels = new int[dy+1];
	line.size = dy + 1;
	/// Change which direction the algorithm draws the line depending on the pixels positions
	int dir = vert1X < vert2X ? 1 : -1;
	int y = 0;
	int h2 = 2 * dy;
	int wh2 = -2 * (dx - dy);
	/// If the direction is less than abs(45) degrees from the x line in the screenspace or abs(45) degrees exactly
	if(dx >= dy)
	{
		F = 2*dy - dx;
		while (vert1X != vert2X)
		{
			line.pixels[y] = vert1X;
			if(F < 0)
			{
				F += h2;
			}
			else
			{
				y += 1;
				F += wh2;
			}
			vert1X += dir;
		}
	}
	/// If the direction is more than abs(45) degrees from the x line in the screenspace
	else
	{
		h2 = 2 * dx;
		wh2 = -2 * (dy - dx);
		F = 2 * dx - dy;
		while (y < dy)
		{
			line.pixels[y] = vert1X;
			if (F < 0)
			{
				F += h2;
			}
			else
			{
				vert1X += dir;
				F += wh2;
			}
			y += 1;
		}
	}
	line.pixels[y] = vert1X;


	return line;
}

void Renderer::putPixel(int index, Vector4D color)
{
	/// Put a pixel into the framebuffer

	frameBuffer[index].red = color[0];
	frameBuffer[index].green = color[1];
	frameBuffer[index].blue = color[2];

}
/// @param x1 the starting x coordiante
/// @param x2 the end x coordinate
/// @param y the y index of the horizontal line to be drawn between two x coordiantes
/// @param v1 
/// @param v2
/// @param v3
/// @param coordinatespace the values of all three verticeis before they were converted to pixel space 
void Renderer::linescan(int x1, int x2, int y, const Vertex &v1, const Vertex &v2, const Vertex &v3, float* coordinateSpace)
{
	if (x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	/// Check so that coordiantes arent outside framebuffer
	if (x1 <= 0)
	{
		x1 = 0;
	}
	if (x2 >= width)
	{
		x2 = width;
	}
	if ( y <= 0 || y >= height)
	{
		return;
	}


	int temp = y * width + x1 + 1;
	for (int x = x1; x <= x2; x++)
	{	
		Vector4D b = getBary(x, y, v1, v2, v3);
		//interpolated positions
		Vertex temp2;
		Vertex coordinates;
		temp2.pos[0] = (b[0] * coordinateSpace[0] + b[1] * coordinateSpace[2] + b[2] * coordinateSpace[4]);
		temp2.pos[1] = (b[0] * coordinateSpace[1] + b[1] * coordinateSpace[3] + b[2] * coordinateSpace[5]);
		temp2.pos[2] = (b[0] * v1.pos[2] + b[1] * v2.pos[2] + b[2] * v3.pos[2]);
		// Interpolated texture coordinates
		temp2.uv[0] = (b[0] * v1.uv[0] + b[1] * v2.uv[0] + b[2] * v3.uv[0]);
		temp2.uv[1] = (b[0] * v1.uv[1] + b[1] * v2.uv[1] + b[2] * v3.uv[1]);
		// Interpolated normals
		temp2.normal[0] = (b[0] * v1.normal[0] + b[1] * v2.normal[0] + b[2] * v3.normal[0]);
		temp2.normal[1] = (b[0] * v1.normal[1] + b[1] * v2.normal[1] + b[2] * v3.normal[1]);
		temp2.normal[2] = (b[0] * v1.normal[2] + b[1] * v2.normal[2] + b[2] * v3.normal[2]);
		

		if (zBuffer[temp] < temp2.pos[2])
		{
			
		}
		else
		{
			Vector4D color = fragmentShader(temp2, cameraPosition, drawTexture, drawTextureWidth, drawTextureHeigth );
			zBuffer[temp] = temp2.pos[2];
			//std::cout << temp2.pos[2] << std::endl;
			putPixel(temp, color);
		}
		temp += 1;
	}
}

Vector4D Renderer::getBary(int x, int y, Vertex v1, Vertex v2, Vertex v3)
{
	float area = areaOfTriangle(v1, v2, v3);
	Vertex temp;
	temp.pos[0] = x;
	temp.pos[1] = y;
	Vector4D triangles;
	triangles[0] = areaOfTriangle(temp, v2, v3)/area;
	triangles[1] = areaOfTriangle(temp, v1, v3)/area;
	triangles[2] = areaOfTriangle(temp, v2, v1)/area;
	return triangles;
}

float Renderer::areaOfTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	float area = std::abs(((v1.pos[0] * (v2.pos[1] - v3.pos[1])) + v2.pos[0] * (v3.pos[1] - v1.pos[1]) + v3.pos[0] * (v1.pos[1] - v2.pos[1])) / 2);
	return area;
}
