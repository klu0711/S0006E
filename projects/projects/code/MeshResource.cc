#include "MeshResource.h"
#include <map>
#include <string>


MeshResource::MeshResource()
{
	
}


MeshResource::~MeshResource()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
/// Load an obj file
bool MeshResource::loadOBJ(char* filename)
{

	std::map<std::string, int> indexMap;
	std::vector<int> indexBuffer;
	std::vector<Vertex> vertexBuffer;
	int index = 0;
	bool quad = false;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector4D> temp_vertices;
	std::vector<Vector4D> temp_indicies;
	std::vector<Vector4D> temp_normals;
	char* path = filename;
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
			unsigned int vIndex[4], uIndex[4], nIndex[4];
			/// The changes the operation depending on the number of faces in the obj file
			if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &uIndex[0], &nIndex[0], &vIndex[1], &uIndex[1], &nIndex[1], &vIndex[2], &uIndex[2], &nIndex[2], &vIndex[3], &uIndex[3], &nIndex[3]) == 12)
			{
				quad = true;
				vertexIndices.push_back(vIndex[0]);
				vertexIndices.push_back(vIndex[1]);
				vertexIndices.push_back(vIndex[2]);
				uvIndices.push_back(uIndex[0]);
				uvIndices.push_back(uIndex[1]);
				uvIndices.push_back(uIndex[2]);
				normalIndices.push_back(nIndex[0]);
				normalIndices.push_back(nIndex[1]);
				normalIndices.push_back(nIndex[2]);

				vertexIndices.push_back(vIndex[0]);
				vertexIndices.push_back(vIndex[2]);
				vertexIndices.push_back(vIndex[3]);
				uvIndices.push_back(uIndex[0]);
				uvIndices.push_back(uIndex[2]);
				uvIndices.push_back(uIndex[3]);
				normalIndices.push_back(nIndex[0]);
				normalIndices.push_back(nIndex[2]);
				normalIndices.push_back(nIndex[3]);

			}
			else
			{
				vertexIndices.push_back(vIndex[0]);
				vertexIndices.push_back(vIndex[1]);
				vertexIndices.push_back(vIndex[2]);
				uvIndices.push_back(uIndex[0]);
				uvIndices.push_back(uIndex[1]);
				uvIndices.push_back(uIndex[2]);
				normalIndices.push_back(nIndex[0]);
				normalIndices.push_back(nIndex[1]);
				normalIndices.push_back(nIndex[2]);
			}

			/*int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &uIndex[0], &nIndex[0], &vIndex[1], &uIndex[1], &nIndex[1], &vIndex[2], &uIndex[2], &nIndex[2]);
			if (matches != 9) {
				printf("This can only read files with nine face indices");
				return false;
			}*/

			

		}


	}
	
	for (unsigned int  i = 0; i <vertexIndices.size(); i++	)
	{
		std::string faceVertex = std::to_string(vertexIndices[i]) + "/" + std::to_string(uvIndices[i]) + "/" + std::to_string(normalIndices[i]);

		if (indexMap.count(faceVertex))
		{
			indexBuffer.push_back(indexMap[faceVertex]);
		}
		else
		{
			unsigned int vIndex = vertexIndices[i];
			Vertex v;
			v.pos[0] = temp_vertices[vIndex - 1][0];
			v.pos[1] = temp_vertices[vIndex - 1][1];
			v.pos[2] = temp_vertices[vIndex - 1][2];

			unsigned int uIndex = uvIndices[i];
			v.uv[0] = temp_indicies[uIndex - 1][0];
			v.uv[1] = temp_indicies[uIndex - 1][1];

			unsigned int nIndex = normalIndices[i];
			v.normal[0] = temp_normals[nIndex - 1][0];
			v.normal[1] = temp_normals[nIndex - 1][1];
			v.normal[2] = temp_normals[nIndex - 1][2];

			vertexBuffer.push_back(v);
			indexBuffer.push_back(index);
			indexMap[faceVertex] = index;
			index++;
		}
	}
	sizeOfIndexBuffer = indexBuffer.size();
	sizeOfVertexBuffer = vertexBuffer.size();
	faces = vertexBuffer;
	indices = indexBuffer;
	bindVertexBuffer(vertexBuffer);
	bindIndexBuffer(indexBuffer);

}

int MeshResource::getVertexSize()
{
	return sizeOfVertexBuffer;
}

int MeshResource::getIndexSize()
{
	return sizeOfIndexBuffer;
}

void MeshResource::setupBuffers()
{
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
}


/// Setup the buffer which contains the verticies and the colors
void MeshResource::bindVertexBuffer(const std::vector<Vertex> &vector)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer * sizeof(Vertex), &vector[0], GL_STATIC_DRAW);
}
/// Setup the buffer which contains the indexes which will become the triangles
void MeshResource::bindIndexBuffer(const std::vector<int>& buffer)
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	int test = sizeOfIndexBuffer * sizeof(int);
	int test2 = buffer[0];
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexBuffer * sizeof(int), &buffer[0], GL_STATIC_DRAW);

	
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
	//glBindVertexArray(VAO);
	//verticies
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//Colors
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);	
	//uv cordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);
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



void MeshResource::unbindVAO()
{
	glBindVertexArray(0);
}


/// Does all the setup requierd setup in order to draw something to the screen
void MeshResource::setupMesh(char* filename)
{
	setupHandles();
	setupBuffers();
	bind();
	loadOBJ(filename);
	bindAttrPointer();
	unBindBuffers();
}

std::vector<Vertex> MeshResource::getFaces()
{
	return faces;
}

std::vector<int> MeshResource::getIndicies()
{
	return indices;
}



