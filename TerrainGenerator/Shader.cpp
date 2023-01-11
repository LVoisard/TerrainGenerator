#include "Shader.h"
Shader::Shader()
{
	ID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	ID = glCreateProgram();

	if (!ID)
	{
		printf("Error creating the shader");
		return;
	}

	AddShader(ID, vertexCode, GL_VERTEX_SHADER);
	AddShader(ID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &result);	
	if (!result)
	{
		glGetProgramInfoLog(ID, sizeof(eLog), NULL, eLog);
		printf("Error linking the program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(ID);
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(ID, "projection");
	uniformModel = glGetUniformLocation(ID, "model");
	uniformView = glGetUniformLocation(ID, "view");
	uniformAmbientColour = glGetUniformLocation(ID, "directionalLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(ID, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(ID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(ID, "directionalLight.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(ID, "material.specularIntensity");
	uniformPhongCoefficient = glGetUniformLocation(ID, "material.phongCoefficient");
	uniformViewerPosition = glGetUniformLocation(ID, "viewerPosition");

	uniformTexture0 = glGetUniformLocation(ID, "texture0");
	uniformTexture1 = glGetUniformLocation(ID, "texture1");
	uniformTexture2 = glGetUniformLocation(ID, "texture2");
	uniformTexture3 = glGetUniformLocation(ID, "texture3");
	uniformTexture4 = glGetUniformLocation(ID, "texture4");
}

void Shader::AddShader(GLuint programID, const char* shaderCode, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shaderID, 1, code, codeLength);
	glCompileShader(shaderID);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(programID, shaderID);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexShaderCode = ReadFile(vertexPath);
	std::string fragmentShaderCode = ReadFile(fragmentPath);

	CompileShader(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

std::string Shader::ReadFile(const char* filePath)
{
	std::string code;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read the file %s, file does not exist", filePath);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		code.append(line + "\n");
	}

	fileStream.close();
	return code;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformAmbientColour;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetPhongCoefficientLocation()
{
	return uniformPhongCoefficient;
}

GLuint Shader::GetViewerPositionLocation()
{
	return uniformViewerPosition;
}

GLuint Shader::GetTexture0Location()
{
	return uniformTexture0;
}

GLuint Shader::GetTexture1Location()
{
	return uniformTexture1;
}

GLuint Shader::GetTexture2Location()
{
	return uniformTexture2;
}

GLuint Shader::GetTexture3Location()
{
	return uniformTexture3;
}

GLuint Shader::GetTexture4Location()
{
	return uniformTexture4;
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::Clear()
{
	if (ID != 0)
		glDeleteProgram(0);
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

Shader::~Shader()
{
	Clear();
}
