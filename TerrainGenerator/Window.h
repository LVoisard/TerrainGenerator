#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


enum class State
{
	Explore,
	Trackball,
	Menu
};

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat GetBufferWidth();
	GLfloat GetBufferHeight();

	bool GetWindowShouldClose();
	bool takeInput;

	bool* GetKeys();
	State GetInputState();
	GLfloat GetPosChangeX();
	GLfloat GetPosChangeY();
	GLfloat GetScrollChange();

	GLfloat GetLastFrameMousePosX();
	GLfloat GetLastFrameMousePosY();
	GLfloat GetMousePosX();
	GLfloat GetMousePosY();


	void SetCursorMode(GLenum mode);

	void SwapBuffers();

	// IM GUI
	void ImGui_NextFrame();
	void ImGui_Render();
	void ImGui_CreateWindow();
	void PerlinEditWindow(bool* edited, float* terrainScale, float* elevation, float* frequency, float* lacunarity, int* octaveCount, float* persistence, float* seaLevel, float* lightHorizontal, float* lightVertical);
	void ImGui_Shutdown();

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // ASCII codes

	State inputState;

	GLfloat lastFramePosX;
	GLfloat lastFramePosY;
	GLfloat posChangeX;
	GLfloat posChangeY;

	GLfloat scrollChange;

	bool mouseMoved;
	bool mouseScrolled;

	bool show_demo_window;
	bool show_another_window;
	float* clear_color;

	void DrawPerlinTools(bool* isOpen,bool* edited, float* terrainScale, float* elevation, float* frequency, float* lacunarity, int* octaveCount, float* persistence, float* seaLevel, float* lightHorizontal, float* lightVertical);
	// register callback methods
	void CreateCallbacks();
	// input callback
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double x, double y);
	static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mods);
	static void HandleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
};

