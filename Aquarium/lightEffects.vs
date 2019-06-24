#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.13;
const float gradient = 1.7;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	TexCoords = aTexCoords;

	vec4 positionRelativeToCam = view * vec4(FragPos, 1.0);
    
	gl_Position = projection * view * vec4(FragPos, 1.0);

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance*density),gradient));
	visibility = clamp(visibility,0.0,1.0);
}