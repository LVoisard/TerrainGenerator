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
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 viewerPosition;

void main()
{
	vec4 baseColour = vec4(0.0f,0.0f,0.0f,0.0f);

	float angle = degrees(acos(dot(normalize(Normal),normalize(vec3(0.0f,1.0f,0.0f)))));	
	
	if(angle < 0)
	{
		// show red in case of bad normals
		baseColour = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else if(angle >= 60)
	{
		baseColour = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 1 - radians(angle));
	}
	else if(angle >= 45)
	{
		baseColour = mix(texture(texture1, TexCoord), texture(texture0, TexCoord), radians(angle));
	}
	else if (angle >= 20)
	{
		baseColour = mix(texture(texture2, TexCoord), texture(texture1, TexCoord), radians(angle));
	}
	else
	{		
		baseColour = mix(texture(texture3, TexCoord), texture(texture2, TexCoord), radians(angle));		
	}	

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
}