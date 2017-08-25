#version 430 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D texture1;
void main()
{
	FragColor = vec4(texture(texture1,texCoords));
};