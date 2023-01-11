#include "Window.h"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window()
{
	height = 800;
	width = 800;

	for (int i = 0; i < sizeof(keys); i++)
	{
		keys[i] = false;
	}

	posChangeX = 0.0f;
	posChangeY = 0.0f;
	scrollChange = 0.0f;
	mouseMoved = true;
	takeInput = true;

	inputState = State::Trackball;

	show_demo_window = true;
	show_another_window = false;
	clear_color = new float[4]{ 0.45f, 0.55f, 0.60f, 1.00f };
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (int i = 0; i < sizeof(keys); i++)
	{
		keys[i] = false;
	}

	posChangeX = 0.0f;
	posChangeY = 0.0f;
	scrollChange = 0.0f;
	mouseMoved = true;
	takeInput = true;

	inputState = State::Trackball;

	show_demo_window = true;
	show_another_window = false;
	clear_color = new float[4]{ 0.45f, 0.55f, 0.60f, 1.00f };
}

int Window::Initialize()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Terrain Generator", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information	
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	CreateCallbacks();

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// tell the callback that we are using this window
	glfwSetWindowUserPointer(mainWindow, this);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
	glfwSetScrollCallback(mainWindow, HandleMouseScroll);
	glfwSetMouseButtonCallback(mainWindow, HandleMouseButtons);
}

void Window::SetCursorMode(GLenum mode)
{
	glfwSetInputMode(mainWindow, GLFW_CURSOR, mode);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		currentWindow->inputState = State::Menu;
		currentWindow->SetCursorMode(GLFW_CURSOR_NORMAL);
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		currentWindow->inputState = State::Trackball;
		currentWindow->SetCursorMode(GLFW_CURSOR_NORMAL);
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		currentWindow->inputState = State::Explore;
		currentWindow->SetCursorMode(GLFW_CURSOR_DISABLED);
	}

	if (key >= 0 && key < sizeof(keys))
	{
		if (action == GLFW_PRESS)
		{
			currentWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			currentWindow->keys[key] = false;
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, double x, double y)
{
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (currentWindow->mouseMoved)
	{
		currentWindow->lastFramePosX = x;
		currentWindow->lastFramePosY = y;
		currentWindow->mouseMoved = false;
	}

	currentWindow->posChangeX = x - currentWindow->lastFramePosX;
	currentWindow->posChangeY = currentWindow->lastFramePosY - y;

	currentWindow->lastFramePosX = x;
	currentWindow->lastFramePosY = y;
}

void Window::HandleMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	currentWindow->scrollChange = yoffset;
}

void Window::HandleMouseButtons(GLFWwindow* window, int button, int action, int mods)
{
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
	{
		currentWindow->keys[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		currentWindow->keys[button] = false;
	}
}

GLfloat Window::GetBufferWidth()
{
	return bufferWidth;
}

GLfloat Window::GetBufferHeight()
{
	return bufferHeight;
}

bool Window::GetWindowShouldClose()
{
	return glfwWindowShouldClose(mainWindow);
}

bool* Window::GetKeys()
{
	return keys;
}

State Window::GetInputState()
{
	return inputState;
}

GLfloat Window::GetPosChangeX()
{
	GLfloat change = posChangeX;
	posChangeX = 0;
	return change;
}

GLfloat Window::GetPosChangeY()
{
	GLfloat change = posChangeY;
	posChangeY = 0;
	return change;
}

GLfloat Window::GetScrollChange()
{
	GLfloat change = scrollChange;
	scrollChange = 0;
	return change;
}

GLfloat Window::GetLastFrameMousePosX()
{
	return lastFramePosX;
}

GLfloat Window::GetLastFrameMousePosY()
{
	return lastFramePosY;
}

GLfloat Window::GetMousePosX()
{
	return lastFramePosX + posChangeX;
}

GLfloat Window::GetMousePosY()
{
	return lastFramePosY + posChangeY;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(mainWindow);
}

void Window::ImGui_NextFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Window::ImGui_Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::ImGui_CreateWindow()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void Window::PerlinEditWindow(bool* edited, float* terrainScale, float* elevation, float* frequency, float* lacunarity, int* octaveCount, float* persistence, float* seaLevel, float* lightHorizontal, float* lightVertical)
{
	static bool showPerlinTool = true;

	if (showPerlinTool)
	{
		DrawPerlinTools(&showPerlinTool, edited, terrainScale, elevation, frequency, lacunarity, octaveCount, persistence, seaLevel, lightHorizontal, lightVertical);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Perlin Noise Editor", NULL, &showPerlinTool);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


static float frequencyVal = 0.5f;

void Window::DrawPerlinTools(bool* isOpen, bool* edited, float* terrainScale, float* elevation, float* frequency, float* lacunarity, int* octaveCount, float* persistence, float* seaLevel, float* lightHorizontal, float* lightVertical)
{
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Perlin Noise Editor", isOpen))
	{
		if (ImGui::SliderFloat("Elevation", elevation, 0.000f, 100.0f, "%.3f"))
		{
			*edited = true;
		}
		if (ImGui::SliderFloat("Frequency", frequency, 0.0f, 1.0f, "%.3f"))
		{
			*edited = true;
		}
		if (ImGui::SliderFloat("Lacunarity", lacunarity, 1.0f, 8.0f, "%.3f"))
		{
			*edited = true;
		}
		if (ImGui::SliderInt("Octave Count", octaveCount, 1.0f, 10.0f, "%f"))
		{
			*edited = true;
		}

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::SliderFloat("Sea Level", seaLevel, -10.0f, 10.0f, "%.2f");

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::SliderFloat("Light Horizontal Rotation", lightHorizontal, 0.0f, 359.0f, "%.2f");
		ImGui::SliderFloat("Light Vertical Rotation", lightVertical, 1.0f, 90.0f, "%.2f");

		ImGui::End();
	}
}

void Window::ImGui_Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}