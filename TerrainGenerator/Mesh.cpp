#include "Mesh.h"
#include <iostream>


Mesh::Mesh(int x, int z,float scale, noise::module::RidgedMulti* terrainModule, float elevation)
{
	int cols = x, rows = z;

	int realSizeX = cols + 1, realSizeY = rows + 1;
	float offsetx = cols * scale / 2;
	float offsetz = rows * scale / 2;

	unsigned int vertexAndNormalCount = realSizeX * realSizeY;
	Vertex* vertices = new Vertex[vertexAndNormalCount];
	int indexCount = 6 * rows * cols;
	unsigned int* indices = new unsigned int[indexCount];

	for (int z = 0; z < realSizeY; z++)
	{
		for (int x = 0; x < realSizeX; x++)
		{
			vec3 position = glm::vec3(scale * x - offsetx, 1.0f, scale * z - offsetz);
			double val = terrainModule->GetValue(position.x, 1, position.z);
			vertices[z * realSizeX + x] = Vertex(vec3(position.x, elevation * val, position.z), vec3(0.0f, 0.0f, 0.0f), vec2((float) x / (250 * scale), (float) z / (250 * scale)));
		}
	}

	int indicesIndex = 0;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			// bottom triangle
			indices[indicesIndex + y * cols + x] = y * realSizeX + x; // 0
			indices[indicesIndex + y * cols + x + 1] = (y + 1) * realSizeX + x; // 2
			indices[indicesIndex + y * cols + x + 2] = y * realSizeX + x + 1; // 1
			

			// top triangle
			indices[indicesIndex + y * cols + x + 3] = (y + 1) * realSizeX + x + 1; // 3
			indices[indicesIndex + y * cols + x + 4] = y * realSizeX + x + 1; // 1
			indices[indicesIndex + y * cols + x + 5] = (y + 1) * realSizeX + x; // 2

			indicesIndex += 5;
		}
	}

	this->vertices = vertices;
	this->numVertices = vertexAndNormalCount;
	this->indices = indices;
	this->indexCount = indexCount;
	this->sizeX = x;
	this->sizeZ = z;

	// calculate normals
	CalculateNormals();

	//write mesh to buffer
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * numVertices, vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(sizeof(float) * 6));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	indexCount = numIndices;
	this->vertices = vertices;
	this->numVertices = numVertices;
	this->normals = normals;
	this->numNormals = numNormals;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * numVertices, vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const GLvoid*)(sizeof(float) * 6));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::UpdateVertices()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(this->vertices[0]) * numVertices, this->vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 1, sizeof(this->normals[0]) * numNormals, this->normals);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::CalculateNormals()
{
	for (int y = 1; y < sizeZ; y++)
	{
		for (int x = 1; x < sizeX; x++)
		{
			vec3 center = vertices[y * (sizeX + 1) + x].pos;

			vec3 right = vertices[y * (sizeX + 1) + x + 1].pos;
			vec3 up = vertices[(y + 1) * (sizeX + 1) + x].pos;
				
			vec3 left = vertices[y * (sizeX + 1) + x - 1].pos;
			vec3 bottom = vertices[(y - 1) * (sizeX + 1) + x].pos;


			vec3 v1 = normalize(right - center);
			vec3 v2 = normalize(up - center);
			vec3 v3 = normalize(left - center);
			vec3 v4 = normalize(bottom - center);

			vec3 normal = cross(v2,v1);
			normal += cross(v3,v2);
			normal += cross(v4,v3);
			normal += cross(v1,v4);

			vertices[y * (sizeX + 1) + x].normal = normalize(normal);
		}
	}
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Vertex::Vertex()
{
}

Vertex::Vertex(vec3 pos)
{
	this->pos = pos;
}

Vertex::Vertex(vec3 pos, vec3 normal)
{
	this->pos = pos;
	this->normal = normal;
}

Vertex::Vertex(vec3 pos, vec3 normal, vec2 uvs)
{
	this->pos = pos;
	this->normal = normal;
	this->uvs = uvs;
}
