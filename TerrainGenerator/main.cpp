#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

#include <noise/noise.h>
#include "Vendor/noiseutils.h"

#include <vector>
#include <math.h>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Skybox.h"

using namespace std;
using namespace noise;

// Window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;

int gridX = 300, gridZ = 300;
double gridScale = 0.05;


Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader*> shaderList;
Light mainLight;
Material terrainMaterial;
Material waterMaterial;

Texture mountainTexture;
Texture rockyGrassTexture;
Texture grassTexture;
Texture dirtTexture;
Texture snowTexture;
Texture waterTexture;

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat timeLastFrame = 0.0f;

int indicesCount = 0;

module::RidgedMulti* terrainModule = new module::RidgedMulti();

static float terrainScale = 1.0f;
static float elevation = 1.000f;
static float frequency = 0.250f;
static float lacunarity = 2.5f;
static int octaveCount = 4;
static float persistence = 1.0f;

static float seaLevel = 0.0f;

static float lightHorizontal = 0.0f;
static float lightVertical = 90.0f;

void LoadShaders()
{
	const char* terrainVertexShaderSource = "Shaders/terrainShader.vert";

	const char* terrainFragmentShaderSource = "Shaders/terrainShader.frag";

	const char* waterVertexShaderSource = "Shaders/waterShader.vert";

	const char* waterFragmentShaderSource = "Shaders/waterShader.frag";


	Shader* terrainShader = new Shader();
	terrainShader->CreateFromFile(terrainVertexShaderSource, terrainFragmentShaderSource);
	shaderList.push_back(terrainShader);

	Shader* waterShader = new Shader();
	waterShader->CreateFromFile(waterVertexShaderSource, waterFragmentShaderSource);
	shaderList.push_back(waterShader);
}

void InitialiseTerrainModule()
{
	module::RidgedMulti* mountainTerrain = new module::RidgedMulti;
	mountainTerrain->SetFrequency(0.25);
	mountainTerrain->SetLacunarity(3);
}

void UpdateTerrainModule()
{
	terrainModule->SetFrequency(frequency);
	terrainModule->SetLacunarity(lacunarity);
	terrainModule->SetOctaveCount(octaveCount);
	//terrainModule->Set(persistence);
}

void ChangeSeed()
{
	terrainModule->SetSeed(time(0));
}

void EditGrid(Mesh* mesh)
{
	for (int z = 0; z < gridX + 1; z++)
	{
		for (int x = 0; x < gridZ + 1; x++)
		{
			vec3* pos = &mesh->vertices[z * (gridX + 1) + x].pos;
			double val = terrainModule->GetValue(pos->x, 1, pos->z);
			pos->y = elevation * val;
		}
	}
	// calculate normals
	mesh->CalculateNormals();
	mesh->UpdateVertices();
}

void DrawSeaLevel()
{
	Vertex verts[] = {
		Vertex(vec3(-gridX / 2 * gridScale, -1.5, -gridZ / 2 * gridScale), vec3(0,1,0), vec2(0, 0)), // 0
		Vertex(vec3(gridX / 2 * gridScale, -1.5, -gridZ / 2 * gridScale), vec3(0,1,0), vec2(0, 1)), // 1
		Vertex(vec3(-gridX / 2 * gridScale, -1.5, gridZ / 2 * gridScale), vec3(0,1,0), vec2(1, 1)), // 2
		Vertex(vec3(gridX / 2 * gridScale, -1.5, gridZ / 2 * gridScale), vec3(0,1,0), vec2(1, 0)), // 3
	};

	unsigned int indi[] = {
		0,1,2,
		2,3,1
	};

	Mesh* mesh = new Mesh(verts, 4, indi, 6);
	meshList.push_back(mesh);
}

int main()
{
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	InitialiseTerrainModule();
	UpdateTerrainModule();
	meshList.push_back(new Mesh(gridX, gridZ, gridScale, terrainModule, elevation));
	DrawSeaLevel();
	LoadShaders();

	terrainMaterial = Material(1.0f, 8.0f);

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.25f, 0.01f);

	mountainTexture = Texture("Textures/rock_06_diff_4k.jpg");
	mountainTexture.Load();
	rockyGrassTexture = Texture("Textures/aerial_rocks_02_diff_4k.jpg");
	rockyGrassTexture.Load();
	grassTexture = Texture("Textures/aerial_rocks_04_diff_4k.jpg");
	grassTexture.Load();
	dirtTexture = Texture("Textures/aerial_grass_rock_diff_4k.jpg");
	dirtTexture.Load();
	snowTexture = Texture("Textures/snow_02_diff_4k.jpg");
	snowTexture.Load();

	waterTexture = Texture("Textures/water.jpg");
	waterTexture.Load();

	mainLight = Light(1.0f,1.0f,1.0f,1.0f,
		1.0f, -1.0f, 0.0f, 1.0f);

	vector<string> skyboxFacesLocation;

	skyboxFacesLocation.push_back("Textures/Skybox/px.png");
	skyboxFacesLocation.push_back("Textures/Skybox/nx.png");
	skyboxFacesLocation.push_back("Textures/Skybox/py.png");
	skyboxFacesLocation.push_back("Textures/Skybox/ny.png");
	skyboxFacesLocation.push_back("Textures/Skybox/pz.png");
	skyboxFacesLocation.push_back("Textures/Skybox/nz.png");

	skybox = Skybox(skyboxFacesLocation);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformViewerPosition = 0,
		uniformAmbientColour = 0, uniformAmbientIntensity = 0, uniformDiffuseIntensity = 0, uniformDirection = 0,
		uniformSpecularIntensity = 0, uniformPhongCoefficient = 0,
		uniformTexture0 = 0, uniformTexture1 = 0, uniformTexture2 = 0, uniformTexture3 = 0, uniformTexture4 = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 1000.0f);
	
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool registerMovement = true;

	bool heightMapEdited = false;
	mainLight.Rotate(radians(lightHorizontal), radians(lightVertical));

	// Loop until window closed
	while (!mainWindow.GetWindowShouldClose())
	{
		if (mainWindow.GetKeys()[GLFW_KEY_R])
		{
			ChangeSeed();
			heightMapEdited = true;
		}

		if (mainWindow.GetKeys()[GLFW_KEY_L])
		{
			std::cout << "Light Direction is: " << mainLight.direction.x << " " << mainLight.direction.y << " " << mainLight.direction.z << endl;
		}

		if (heightMapEdited) {
			UpdateTerrainModule();
			EditGrid(meshList[0]);
			heightMapEdited = false;
			mainWindow.takeInput = true;
		}

		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - timeLastFrame;
		timeLastFrame = currentTime;

		// Get + Handle user input events
		glfwPollEvents();
		mainWindow.ImGui_NextFrame();

		if (mainWindow.GetInputState() == State::Explore && mainWindow.takeInput)
		{
			camera.KeyControl(mainWindow.GetKeys(), deltaTime);
			camera.MouseControl(mainWindow.GetPosChangeX(), mainWindow.GetPosChangeY());
		}
		else if (mainWindow.GetInputState() == State::Trackball && mainWindow.takeInput)
		{
			camera.TrackballControl(mainWindow.GetKeys(), deltaTime, mainWindow.GetPosChangeX(), mainWindow.GetPosChangeY(), mainWindow.GetScrollChange());
		}

		// Clear window
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainLight.Rotate(radians(90 - lightVertical), radians(lightHorizontal));

		mat4 view = camera.CalculateViewMatrix();

		skybox.DrawSkybox(view, projection);
		shaderList[0]->Use();

		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformAmbientColour = shaderList[0]->GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
		uniformDiffuseIntensity = shaderList[0]->GetDiffuseIntensityLocation();
		uniformDirection = shaderList[0]->GetDirectionLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformPhongCoefficient = shaderList[0]->GetPhongCoefficientLocation();
		uniformViewerPosition = shaderList[0]->GetViewerPositionLocation();
		uniformTexture0 = shaderList[0]->GetTexture0Location();
		uniformTexture1 = shaderList[0]->GetTexture1Location();
		uniformTexture2 = shaderList[0]->GetTexture2Location();
		uniformTexture3 = shaderList[0]->GetTexture3Location();
		uniformTexture4 = shaderList[0]->GetTexture4Location();

		mainLight.Illuminate(uniformAmbientIntensity, uniformAmbientColour,
			uniformDiffuseIntensity, uniformDirection);

		
		
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(uniformViewerPosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		terrainMaterial.SetMaterialValues(uniformSpecularIntensity, uniformPhongCoefficient);
		glUniform1i(uniformTexture0, 0);
		mountainTexture.Use(GL_TEXTURE0);
		glUniform1i(uniformTexture1, 1);
		rockyGrassTexture.Use(GL_TEXTURE1);
		glUniform1i(uniformTexture2, 2);
		grassTexture.Use(GL_TEXTURE2);
		glUniform1i(uniformTexture3, 3);
		dirtTexture.Use(GL_TEXTURE3);
		glUniform1i(uniformTexture4, 4);
		snowTexture.Use(GL_TEXTURE4);
		meshList[0]->Draw();

		glUseProgram(0);

		shaderList[1]->Use();
		uniformModel = shaderList[1]->GetModelLocation();
		uniformProjection = shaderList[1]->GetProjectionLocation();
		uniformView = shaderList[1]->GetViewLocation();
		uniformAmbientColour = shaderList[1]->GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[1]->GetAmbientIntensityLocation();
		uniformDiffuseIntensity = shaderList[1]->GetDiffuseIntensityLocation();
		uniformDirection = shaderList[1]->GetDirectionLocation();
		uniformSpecularIntensity = shaderList[1]->GetSpecularIntensityLocation();
		uniformPhongCoefficient = shaderList[1]->GetPhongCoefficientLocation();
		uniformViewerPosition = shaderList[1]->GetViewerPositionLocation();

		mainLight.Illuminate(uniformAmbientIntensity, uniformAmbientColour,
			uniformDiffuseIntensity, uniformDirection);

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, -seaLevel, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformViewerPosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		waterTexture.Use();
		waterMaterial = Material(1.0f, 256);
		waterMaterial.SetMaterialValues(uniformSpecularIntensity, uniformPhongCoefficient);
		meshList[1]->Draw();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(0);

		if (mainWindow.GetKeys()[GLFW_KEY_Z])
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (mainWindow.GetKeys()[GLFW_KEY_X])
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mainWindow.PerlinEditWindow(&heightMapEdited, &terrainScale, &elevation, &frequency, &lacunarity, &octaveCount, &persistence, &seaLevel, &lightHorizontal, &lightVertical);
		// mainWindow.ImGui_CreateWindow();
		mainWindow.ImGui_Render();
		mainWindow.SwapBuffers();
	}

	mainWindow.ImGui_Shutdown();

	glfwTerminate();
	return 0;
}