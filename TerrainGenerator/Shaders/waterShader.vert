#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	
	Normal = mat3(transpose(inverse(model))) * normal;
	
	FragPos = (model * vec4(pos, 1.0)).xyz;
	
	TexCoord = texture;
}