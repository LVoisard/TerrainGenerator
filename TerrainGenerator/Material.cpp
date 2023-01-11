#include "Material.h"

Material::Material()
{
	specularIntensity = 0.0f;
	phongCoefficient = 0.0f;
}

Material::Material(float si, float sc)
{
	specularIntensity = si;
	phongCoefficient = sc;
}

void Material::SetMaterialValues(GLuint specularIntensityLocation, GLuint phongCoefficientLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(phongCoefficientLocation, phongCoefficient);
}
