#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(float si, float sc);

	void SetMaterialValues(GLuint specularIntensityLocation, GLuint phongCoefficientLocation);

private:
	float specularIntensity;
	float phongCoefficient;
};

