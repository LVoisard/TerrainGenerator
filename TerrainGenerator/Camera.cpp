#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

Camera::Camera(vec3 position, vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat mouseSensibility, GLfloat panSensibility)
{
	this->position = position;
	this->worldUp = worldUp;

	this->yaw = yaw;
	this->pitch = pitch;
	this->lookAt = vec3(0.0f, 0.0f, -1.0f);

	
	this->right = glm::normalize(glm::cross(lookAt, worldUp));
	this->up = glm::normalize(glm::cross(right, lookAt));
	this->front = glm::cross(worldUp, right);


	this->movementSpeed = movementSpeed;
	this->mouseSensibility = mouseSensibility;
	this->panSensibility = panSensibility;

	this->pivotPoint = vec3(0, 0, 0);

	Update();
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = deltaTime * movementSpeed;

	if (keys[GLFW_KEY_W])
	{
		position += lookAt * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= lookAt * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		position += worldUp * velocity;
	}
	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		position -= worldUp * velocity;
	}
}

void Camera::MouseControl(GLfloat x, GLfloat y)
{
	x *= mouseSensibility;
	y *= mouseSensibility;

	yaw += x;
	pitch += y;

	if (pitch > 89)
		pitch = 89;
	if (pitch < -89)
		pitch = -89;

	Update();
}

void Camera::TrackballControl(bool* keys, GLfloat deltaTime, GLfloat x, GLfloat y, GLfloat scrollChange)
{

	float panX = x * panSensibility;
	float panY = y * panSensibility;

	float rotX = x * mouseSensibility;
	float rotY = y * mouseSensibility;

	if (scrollChange != 0) 
	{
		position += lookAt * scrollChange * movementSpeed;
	}

	if (keys[GLFW_MOUSE_BUTTON_LEFT])
	{
		yaw += rotX;
		pitch += rotY;

		if (pitch > 89)
			pitch = 89;
		if (pitch < -89)
			pitch = -89;
	}
	else if (keys[GLFW_MOUSE_BUTTON_RIGHT])
	{
		position -= panX * right;
		position -= panY * front;

		pivotPoint -= panX * right; 
		pivotPoint -= panY * front;
	}

	UpdateTrackball();
}

vec3 Camera::GetCameraPosition()
{
	return position;
}

mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + lookAt, worldUp);
}

void Camera::Update()
{
	lookAt.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	lookAt.y = sin(glm::radians(pitch));
	lookAt.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	lookAt = glm::normalize(lookAt);

	right = glm::normalize(glm::cross(lookAt, worldUp));
	up = glm::normalize(glm::cross(right, lookAt)); 
	front = glm::cross(worldUp, right);

	yawLastFrame = yaw;
	pitchLastFrame = pitch;
}

void Camera::UpdateTrackball()
{
	mat4 xRot = glm::rotate(glm::radians(yaw), vec3(0, 1, 0));
	mat4 yRot = glm::rotate(glm::radians(pitch), vec3(0, 0, 1));

	vec3 trans = vec3(0, 0, 0) - pivotPoint;

	float distance = glm::distance(position, pivotPoint);

	vec3 posXZ = yRot * glm::normalize(glm::vec4(-1, 0, 0, 0)) * xRot;
	position = distance * (posXZ)-trans;

	Update();
}

Camera::~Camera()
{
}

