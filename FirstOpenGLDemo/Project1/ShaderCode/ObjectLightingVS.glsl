#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 pos;
out vec3 normal;
out vec3 lightPos;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightWorldPos;
void main()
{
	texCoords = aTexCoords;
	normal = mat3(transpose(inverse(view*model)))*aNormal;
	vec4 v4worldPos = view * model * vec4(aPos,1.0);
	pos = v4worldPos.xyz;
	lightPos = vec3(view*model* vec4(lightWorldPos,1));

	gl_Position = projection* v4worldPos;
}