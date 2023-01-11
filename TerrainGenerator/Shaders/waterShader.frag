#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour; 
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float phongCoefficient;
};

uniform sampler2D texture0;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 viewerPosition;

void main()
{
	vec4 baseColour = texture(texture0, TexCoord);
	
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), - normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	vec4 specularColour = vec4(0,0,0,0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 viewingRay = normalize(viewerPosition - FragPos);
		vec3 reflectedvertex = reflect(-normalize(directionalLight.direction), normalize(Normal));
		
		float specularFactor = dot(viewingRay, reflectedvertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.phongCoefficient);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	colour = baseColour * (ambientColour + diffuseColour + specularColour);
	colour.a = 0.5f;
}