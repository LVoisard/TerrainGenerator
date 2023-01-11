#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(float R, float G, float B, float ai, 
		float xDir, float yDir, float zDir, float di);

	void Illuminate(GLuint intensity, GLuint colorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	void Rotate(float xRotation, float yRotation);

	glm::vec3 direction;
private:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

