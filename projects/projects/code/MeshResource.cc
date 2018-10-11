#include "MeshResource.h"

MeshResource::MeshResource()
{
	
}
/// Constructor that takes two pointers to the index and the vertex arrays, it also accepts the "sizeof" both of them
MeshResource::MeshResource(float *buffer, int *indexBuffer, int sizeBuffer, int sizeIndexBuffer)
{
	//vertexBuffer = buffer;
	//this->indexBuffer = indexBuffer;
	//this->sizeBuffer = sizeBuffer;
	//this->sizeIndexBuffer = sizeIndexBuffer;


}

MeshResource::~MeshResource()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool MeshResource::loadOBJFile(std::vector<Vector4D> &vertices, std::vector<Vector4D>& uv, std::vector<Vector4D>& normals)
{
	std::vector<unsigned int> vertexIndices, indexIndices, normalIndices;
	std::vector<Vector4D> temp_vertices;
	std::vector<Vector4D> temp_indicies;
	std::vector<Vector4D> temp_normals;
	char* path = "tractor.obj";
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("File failed to load\n");
		return false;
	}
	while (1) {
		char lineheader[128];
		int res = fscanf(file, "%s", lineheader);
		if (res == EOF) {
			break;
		}
		// Compare the first char of each string
		if (strcmp(lineheader, "v") == 0) {
			Vector4D position;
			fscanf(file, "%f %f %f\n", &position[0], &position[1], &position[2]);
			temp_vertices.push_back(position);
		}
		else if (strcmp(lineheader, "vt") == 0) {
			Vector4D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_indicies.push_back(uv);
		}
		else if (strcmp(lineheader, "vn") == 0) {
			Vector4D normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineheader, "f") == 0) {

			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			indexIndices.push_back(uvIndex[0]);
			indexIndices.push_back(uvIndex[1]);
			indexIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}


	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		Vector4D vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < indexIndices.size(); i++)
	{
		unsigned int indexIndex = indexIndices[i];
		Vector4D index = temp_indicies[indexIndex - 1];
		uv.push_back(index);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		Vector4D normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}
	sizeBuffer = vertexIndices.size();

	


	return true;
}

void MeshResource::convertToFloatPointer(std::vector<Vector4D>& verticies, std::vector<Vector4D>& indices, std::vector<Vector4D>& normals)
{
	int j = 0;
	float *vertexAndTextureBuf = new float[(verticies.size()*3 + indices.size() * 3)];
	for (int i = 0; i < verticies.size(); i++)
	{
		for (int x = 0; x < 3; x++)
		{
			vertexAndTextureBuf[(i*3)+x] = verticies[i].getVectorValue(x);
		}
	}
	for (int i = verticies.size(); i < (verticies.size() + indices.size()); i++)
	{
		for (int x = 0; x < 3; x++)
		{
			vertexAndTextureBuf[(i * 3) + x] = verticies[i-verticies.size()].getVectorValue(x);
		}
	}
	vertexAndIndexBuffer = vertexAndTextureBuf;
}

void MeshResource::setupBuffers()
{
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
}


/// Setup the buffer which contains the verticies and the colors
void MeshResource::bindVertexBuffer(std::vector<Vector4D> vector)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(Vector4D), &vector[0], GL_STATIC_DRAW);
}
/// Setup the buffer which contains the indexes which will become the triangles
void MeshResource::bindIndexBuffer()
{

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexBuffer, indexBuffer, GL_STATIC_DRAW);

	
}
/// Unbind the vertex array object
void MeshResource::unBindBuffers()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
/// Tell the shaders how the buffers are layed out and enable both of them
void MeshResource::bindAttrPointer()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);	
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeBuffer * sizeof(Vector4D)));
	glEnableVertexAttribArray(2);
}	
/// Bind the vertex array object
void MeshResource::bind()
{
	glBindVertexArray(VAO);
}
/// Create the vertex array object
void MeshResource::setupHandles()
{
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->VBO);

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}

void MeshResource::bindVAO()
{
	glBindVertexArray(this->VAO);
}

void MeshResource::unbindVAO()
{
	glBindVertexArray(0);
}

void MeshResource::destroy()
{

}

std::vector<Vector4D> MeshResource::combineBuffers(std::vector<Vector4D> vertices, std::vector<Vector4D> indices)
{
	std::vector<Vector4D> combinedBuffers;
	combinedBuffers.insert(combinedBuffers.end(), vertices.begin(), vertices.end());
	combinedBuffers.insert(combinedBuffers.end(), indices.begin(), indices.end());
	return combinedBuffers;
}


