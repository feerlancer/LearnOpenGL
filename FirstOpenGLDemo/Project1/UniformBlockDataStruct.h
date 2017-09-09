#pragma once
#include<glm\glm.hpp>
using namespace glm;
struct DirectionalLight
{
	vec3 direction;
	float padding0;

	vec3 ambient;
	float padding1;

	vec3 diffuse;
	float padding2;

	vec3 specular;
	float padding3;

	glm::mat4 view;

	glm::mat4 projection;
};

struct PointLight
{
	vec3 position;
	float padding0;

	vec3 ambient;
	float padding1;

	vec3 diffuse;
	float padding2;

	vec3 specular;
	float padding3;

	float constant;
	float linear;
	float quadratic;
	float far_plane;

	glm::mat4 view;

	glm::mat4 projection;
};

struct SpotLight
{
	vec3 position;
	float padding0;

	vec3 direction;
	float padding1;

	float cutOffCos;
	float outerCutOffCos;
	float padding2;
	float padding3;

	vec3 ambient;
	float padding4;

	vec3 diffuse;
	float padding5;

	vec3 specular;
	float padding6;

	float constant;
	float linear;
	float quadratic;
	float padding7;
};
struct PBR_DirectionalLight
{
	vec3 position;
	float padding0;
};
struct PBR_PointLight
{
	vec3 position;
	float padding0;

	vec3 color;
	float padding1;
};
struct PBR_SpotLight
{
	vec3 position;
	float padding0;
};
struct UBO_BLOCK_MATRIX //binging = 1, for camera view/ projection
{
	glm::vec3 worldCameraPos;
	float floatPadding0;

	uint32_t Bling;
	uint32_t GammaCorrect;
	float floatPadding1;
	float floatPadding2;
};

struct UBO_BLOCK_1  //binging = 1
{
	glm::vec3 worldCameraPos;
	float floatPadding0;

	uint32_t Bling;
	uint32_t GammaCorrect;
	float floatPadding1;
	float floatPadding2;
};

struct UBO_BLOCK_LIGHT_INFO //binging = 2
{
	DirectionalLight dirlight;
	PointLight pointLights[4];
	SpotLight spotLight;
};

struct UBO_BLOCK_PBR_LIGHT_INFO //binging = 3
{
	PBR_DirectionalLight pbr_dirlight;
	PBR_PointLight pbr_pointLights[4];
	PBR_SpotLight pbr_spotLight;
};
