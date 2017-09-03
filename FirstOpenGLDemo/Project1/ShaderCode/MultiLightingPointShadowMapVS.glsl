#version 430 core
struct PointLight
{
	mat4 view;
	mat4 projection;
};
//in
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

//out
out VS_OUT
{
	vec3 worldFragPos;
	vec3 worldNormal;
	vec2 texCoords;
}vs_out;

//uniform
uniform mat4 model;

layout(binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
	vs_out.texCoords = aTexCoords;
	vs_out.worldNormal = mat3(transpose(inverse(model)))*aNormal;
	vec4 v4WorldPos = model * vec4(aPos,1.0);
	vs_out.worldFragPos = vec3(v4WorldPos);
	gl_Position = projection*view* v4WorldPos;
}