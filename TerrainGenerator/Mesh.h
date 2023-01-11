#pragma once

#include <noise/noise.h>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

using std::vector;
using namespace glm;

class Vertex
{
public:
	Vertex();
	Vertex(vec3 pos);
	Vertex(vec3 pos, vec3 normal);
	Vertex(vec3 pos, vec3 normal, vec2 uvs);
	vec3 pos;
	vec3 normal;
	vec2 uvs;
};

class Mesh
{
public:
	Mesh(int x, int z, float scale, noise::module::RidgedMulti*, float elevation);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void Draw();
	void UpdateVertices();
	void CalculateNormals();

	Vertex* vertices;	
	unsigned int numVertices;

	unsigned int* indices;
	GLsizei indexCount;

	vec3* normals;
	unsigned int numNormals;

	virtual ~Mesh();

private:
	GLuint VAO, VBO, EBO;
	noise::module::Module* terrainModule;
	int sizeX, sizeZ;
};

