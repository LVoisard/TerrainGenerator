#pragma once


#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	Camera();
	Camera(vec3 position, vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat mouseSensibility, GLfloat panSensibility);
	
	void KeyControl(bool* keys, GLfloat detaTime);
	void MouseControl(GLfloat x, GLfloat y);
	void TrackballControl(bool* keys, GLfloat deltaTime, GLfloat x, GLfloat y, GLfloat scrollChange);

	vec3 GetCameraPosition();

	mat4 CalculateViewMatrix();
	
	~Camera();
private:
	vec3 position;
	vec3 lookAt;
	vec3 up;
	vec3 right;
	vec3 worldUp;
	vec3 front;

	vec3 pivotPoint;

	GLfloat yaw; // left and right, y axis rotation
	GLfloat pitch; // front back, x axis rotation

	GLfloat yawLastFrame;
	GLfloat pitchLastFrame;

	GLfloat cameraOriginRadius;

	GLfloat movementSpeed;
	GLfloat panSensibility;
	GLfloat mouseSensibility;

	void Update();
	void UpdateTrackball();
};

