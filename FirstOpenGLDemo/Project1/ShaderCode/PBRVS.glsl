#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normal;

out VS_OUT
{
	vec3 worldFragPosition;
	vec2 texCoords;
	vec3 worldNormal;
}vs_out;

layout(binding = 0) uniform MATRIX
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position =projection * view*  model * vec4(a_Position,1);
	vs_out.worldFragPosition = vec3(model *vec4(a_Position,1));
	vs_out.texCoords = a_TexCoords;
	vs_out.worldNormal = mat3(model)* a_Normal;
}