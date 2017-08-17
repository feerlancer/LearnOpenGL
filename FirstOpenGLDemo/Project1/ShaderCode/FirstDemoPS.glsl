#version 430 core
in vec2 texCoord;
out vec4 FragColor;
uniform float mixValue;
uniform sampler2D outTexture;
uniform sampler2D outTexture1;
void main()
{
	FragColor = mix(texture(outTexture, texCoord), texture(outTexture1, texCoord),mixValue);
};