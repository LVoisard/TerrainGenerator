#include "Light.h"
#include <iostream>

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(float R, float G, float B, float ai,
	float xDir, float yDir, float zDir, float di)
{
	color = glm::vec3(R, G, B);
	ambientIntensity = ai;
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	diffuseIntensity = di;
}

void Light::Illuminate(GLuint ambientintensityLocation, GLuint colorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(ambientintensityLocation, ambientIntensity);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

void Light::Rotate(float xRotation, float yRotation)
{
	glm::mat3 rotx = glm::mat3
		(1, 0, 0,
		0, cos(xRotation), -sin(xRotation),
		0, sin(xRotation), cos(xRotation));

	glm::mat3 roty = glm::mat3(cos(yRotation), 0, sin(yRotation),
		0, 1, 0,
		-sin(yRotation), 0, cos(yRotation));

	glm::vec3 pos = glm::vec3(0, 1, 0);
	
	glm::vec3 newPos = rotx * pos * roty;

	color.r = std::max(0.8f , logf(newPos.y)/5 + 1);
	color.g = std::max(0.6f , logf(newPos.y)/5 + 1);
	color.b = std::max(0.6f , logf(newPos.y)/5 + 1);

	direction = -glm::normalize(newPos);
}
