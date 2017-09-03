#version 430 core

struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
	float shininess;
};

in VS_OUT
{
	vec3 worldFragPos;
	vec3 worldNormal;
	vec2 texCoords;
}ps_in;

out vec4 FragColor;

uniform Material material;

void main()
{
	FragColor = texture(material.texture_diffuse0, ps_in.texCoords);
}

