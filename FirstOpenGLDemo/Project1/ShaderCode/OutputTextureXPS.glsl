#version 430
uniform sampler2D texture1;

in vec2 texCoords;
out vec4 FragColor;
void main()
{
	FragColor = vec4(texture(texture1, texCoords).rrr,1);
}