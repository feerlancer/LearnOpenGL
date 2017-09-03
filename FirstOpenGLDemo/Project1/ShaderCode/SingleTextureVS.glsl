#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
layout(binding = 0 ) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 texCoords;
void main()
{
	gl_Position = projection*view*model * vec4(aPos,1.0f);
	texCoords = aTexCoords;
};