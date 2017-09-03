#version 430 core
layout(location = 0) in vec3 a_Pos;

struct DirectionalLight
{
	mat4 view;
	mat4 projection;
};

layout(binding = 2) uniform UNIFORM_BLOCK_LIGHT_INFO
{
	DirectionalLight dirlight;
};

uniform mat4 model;

void main()
{
	gl_Position = dirlight.projection*dirlight.view*model*vec4(a_Pos,1);
}