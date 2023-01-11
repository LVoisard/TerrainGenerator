#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexPath, const char* fragmentPath);
	std::string ReadFile(const char* filePath);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetPhongCoefficientLocation();
	GLuint GetViewerPositionLocation();
	GLuint GetTexture0Location();
	GLuint GetTexture1Location();
	GLuint GetTexture2Location();
	GLuint GetTexture3Location();
	GLuint GetTexture4Location();

	
	void Use();
	void Clear();

	~Shader();
private:
	GLuint ID, uniformProjection, uniformModel, uniformView, uniformViewerPosition,
		uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection,
		uniformSpecularIntensity, uniformPhongCoefficient,
		uniformTexture0, uniformTexture1, uniformTexture2, uniformTexture3, uniformTexture4;

	void CompileShader(const char* vertexPath, const char* fragmentPath);
	void AddShader(GLuint programID, const char* shaderCode, GLenum shaderType);
};

