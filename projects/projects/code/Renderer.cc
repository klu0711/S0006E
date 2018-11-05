#include "Renderer.h"
#include <iostream>

Renderer::Renderer(const int& xSize, const int& ySize)
{
	frameBufferSize = xSize * ySize;
	width = xSize; height = ySize;
	frameBuffer = new pixel[frameBufferSize];
	zBuffer = new float[frameBufferSize];



	std::fill_n(zBuffer, frameBufferSize, -20000);
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

void Renderer::setFragmentShader(std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, TextureResource tex)> shader)
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

void Renderer::loadTexture(char* filename)
{
	texture.loadFromFile(filename);
}

void Renderer::setBuffers()
{
	MeshResource mesh;
	mesh.loadOBJ("tractor.obj");
	texture.loadFromFile("tractor.png");
	faces = mesh.getFaces();
	indices = mesh.getIndicies();

}

void Renderer::clearZbuffer()
{
	std::fill_n(zBuffer, frameBufferSize, -20000);
	pixel p;
	std::fill_n(frameBuffer, frameBufferSize, p);
}

void Renderer::rastTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	w1 = 1 / v1.pos[3];
	w2 = 1 / v2.pos[3];
	w3 = 1 / v3.pos[3];
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
	v1.pos[0] = std::roundf(v1.pos[0] * width +1 / 2 + width / 2);
	v1.pos[1] = std::roundf(-v1.pos[1] * height +1 / 2 + height / 2);

	v2.pos[0] = std::roundf(v2.pos[0] * width +1 / 2 + width / 2);
	v2.pos[1] = std::roundf(-v2.pos[1] * height +1 / 2 + height / 2);
	
	v3.pos[0] = std::roundf(v3.pos[0] * width+1  / 2 + width / 2);
	v3.pos[1] = std::roundf(-v3.pos[1] * height+1  / 2 + height / 2);
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
		if (temp == 58053)
		{
			printf("");
		}
		float u, v, w;
		barycentric(Vector4D(x, y, 0, 1), Vector4D(v1.pos[0], v1.pos[1], 1.0f, 1), Vector4D(v2.pos[0], v2.pos[1], 1.0f, 1), Vector4D(v3.pos[0], v3.pos[1], 1.0f,1), u, v, w);
		//float wCorrection = (u,)

		float zIndex = (v1.pos[2] * u) + (v2.pos[2] * v) + (v3.pos[2] * w);
		//Vector4D b = getBary(x, y, v1, v2, v3);
		//interpolated positions
		Vertex temp2;
		Vertex coordinates;
		temp2.pos[0] = (u * coordinateSpace[0] + v * coordinateSpace[2] + w * coordinateSpace[4]);
		temp2.pos[1] = (u * coordinateSpace[1] + v * coordinateSpace[3] + w * coordinateSpace[5]);
		//temp2.pos[2] = (u * v1.pos[2] + v * v2.pos[2] + w * v3.pos[2]);
		// Interpolated texture coordinates
		temp2.uv[0] = ((u * v1.uv[0]) + (v * v2.uv[0]) + (w * v3.uv[0]));
		temp2.uv[1] = ((u * v1.uv[1]) + (v * v2.uv[1]) + (w * v3.uv[1]));
		// Interpolated normals
		temp2.normal[0] = ((u * v1.normal[0]) + (v * v2.normal[0]) + (w * v3.normal[0]));
		temp2.normal[1] = ((u * v1.normal[1]) + (v * v2.normal[1]) + (w * v3.normal[1]));
		temp2.normal[2] = ((u * v1.normal[2]) + (v * v2.normal[2]) + (w * v3.normal[2]));


		/*temp2.pos[0] = (b[0] * coordinateSpace[0] + b[1] * coordinateSpace[2] + b[2] * coordinateSpace[4]);
		temp2.pos[1] = (b[0] * coordinateSpace[1] + b[1] * coordinateSpace[3] + b[2] * coordinateSpace[5]);
		//temp2.pos[2] = (b[0] * v1.pos[2] + b[1] * v2.pos[2] + b[2] * v3.pos[2]);
		// Interpolated texture coordinates
		temp2.uv[0] = (b[0] * v1.uv[0] + b[1] * v2.uv[0] + b[2] * v3.uv[0]);
		temp2.uv[1] = (b[0] * v1.uv[1] + b[1] * v2.uv[1] + b[2] * v3.uv[1]);
		// Interpolated normals
		temp2.normal[0] = (b[0] * v1.normal[0] + b[1] * v2.normal[0] + b[2] * v3.normal[0]);
		temp2.normal[1] = (b[0] * v1.normal[1] + b[1] * v2.normal[1] + b[2] * v3.normal[1]);
		temp2.normal[2] = (b[0] * v1.normal[2] + b[1] * v2.normal[2] + b[2] * v3.normal[2]);*/
		

		if (zBuffer[temp] < zIndex)
		{
			Vector4D color = fragmentShader(temp2, cameraPosition, texture);
			zBuffer[temp] = zIndex;
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

void Renderer::barycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3)
{
	//std::cout << vec1[0] <<" " << vec1[1] << " " <<std::endl;
	//std::cout << vec2[0] <<" " << vec2[1] << " " <<std::endl;
	//std::cout << vec3[0] <<" " << vec3[1] << " " <<std::endl;
	Vector4D temp1 = vec2 - vec1, temp2 = vec3 - vec1, temp3 = point - vec1;
	float daa = temp1.dotProduct(temp1);
	float dab = temp1.dotProduct(temp2);
	float dbb = temp2.dotProduct(temp2);
	float dca = temp3.dotProduct(temp1);
	float dcb = temp3.dotProduct(temp2);
	float denominator = (daa * dbb) - (dab * dab);
	p2 = (dbb * dca - dab * dcb) / denominator;
	p3 = (daa * dcb - dab * dca) / denominator;
	p1 = 1.0f - p2 - p3;
}

float Renderer::areaOfTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	float area = std::abs(((v1.pos[0] * (v2.pos[1] - v3.pos[1])) + v2.pos[0] * (v3.pos[1] - v1.pos[1]) + v3.pos[0] * (v1.pos[1] - v2.pos[1])) / 2);
	return area;
}
