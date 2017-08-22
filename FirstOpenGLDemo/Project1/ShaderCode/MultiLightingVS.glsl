#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 worldFragPos;
out vec3 worldNormal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoords = aTexCoords;
	worldNormal = mat3(transpose(inverse(model)))*aNormal;
	vec4 v4WorldPos = model * vec4(aPos,1.0);
	worldFragPos = vec3(v4WorldPos);
	gl_Position = projection*view* v4WorldPos;
}