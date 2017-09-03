#version 430 core

//#define USE_GS
struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
    vec3 direction;
    float cutOffCos;
	float outerCutOffCos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

// in vec3 worldFragPos;
// in vec3 worldNormal;
// in vec2 texCoords;


in VS_OUT
{
	vec3 worldFragPos;
	vec3 worldNormal;
	vec2 texCoords;
	vec3 lightNDCSpaceFragPos;
}ps_in;

out vec4 FragColor;

layout(binding = 14) uniform sampler2D shadowMap;
uniform Material material;

#define NR_POINT_LIGHTS 4
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

//uniform vec3 worldCameraPos;
layout(binding = 1) uniform UNIFORM_BLOCK_1
{
	vec3 worldCameraPos;
	float floatPadding0;

	bool Bling;
	bool GammaCorrect;
	float floatPadding1;
	float floatPadding2;
};

const vec2 offset = 1.f/textureSize(shadowMap,0);

const int sampleNum = 9;
vec2 ShadowMapSampleOffsets[] =
{
	vec2(-offset.x,offset.y),
	vec2(0,offset.y),
	vec2(offset.x,offset.y),
	vec2(-offset.x,0),
	vec2(0,0),
	vec2(offset.x,0),
	vec2(-offset.x,-offset.y),
	vec2(0,-offset),
	vec2(offset.x,-offset.y),
};
float kernel[] =
{
	-1, -1, -1,
	-1,  10, -1,
	-1, -1, -1
};

vec3 caculateDirectionalLight(DirectionalLight a_DirLight, vec3 a_Normal,vec3 a_FragToCameraDir);
vec3 caculatePointLight(PointLight a_PointLight, vec3 a_Normal, vec3 a_FragPos,vec3 a_FragToCameraDir);
vec3 caculateSpotLight(SpotLight a_SpotLight,vec3 a_Normal,vec3 a_FragPos,vec3 a_FragToCameraDir);
vec3 caculateShadow();
void main()
{
	vec3 FragColorV3 = vec3(0);
	vec3 fragToCameraDir = worldCameraPos - ps_in.worldFragPos;
	FragColorV3 = caculateDirectionalLight(directionalLight,ps_in.worldNormal,fragToCameraDir);
	for(int i=0;i<NR_POINT_LIGHTS;++i)
	{
		FragColorV3+=caculatePointLight(pointLights[i],ps_in.worldNormal,ps_in.worldFragPos,fragToCameraDir);
	}
	FragColorV3+=caculateSpotLight(spotLight,ps_in.worldNormal,ps_in.worldFragPos,fragToCameraDir);

	if(GammaCorrect)
	{
		FragColorV3 = pow(FragColorV3,vec3(1f/2.2f));
	}
	FragColor = vec4(FragColorV3,1);
}

float isInShadow()
{
	//[-1,1]->[0,1];
	vec3 projPos = ps_in.lightNDCSpaceFragPos;
	projPos = (projPos+1)/2;
	if(projPos.z >1)return 0.0;

	float bias = max(0.001,0.003*(1-dot(normalize(ps_in.worldNormal),normalize(-directionalLight.direction))));
	float depthInshadowMap;
#if 1
	float countIsInShadow = 0;
	for(int i =0;i<sampleNum;++i)
	{
		depthInshadowMap = texture(shadowMap,projPos.xy + ShadowMapSampleOffsets[i]).r;
		depthInshadowMap+=bias;
		countIsInShadow +=((depthInshadowMap<projPos.z)?1:0);
	}
	return countIsInShadow/sampleNum;
#else	
	depthInshadowMap = texture(shadowMap,projPos.xy).r;
	depthInshadowMap+=bias;
	return (depthInshadowMap<projPos.z)?1:0;
#endif
}

float caculateSpecular(vec3 a_FragToLightDir,vec3 normal,vec3 a_FragToCameraDir)
{
	float result;
	vec3 norm = normalize(normal);
	vec3 normFragToCameraDir= normalize(a_FragToCameraDir);
	vec3 normFragToLightDir= normalize(a_FragToLightDir);
	if(!Bling)//phong
	{
		vec3 reflectDir = normalize(reflect(-normFragToLightDir,norm));
		float diff = max(0,dot(reflectDir,normFragToCameraDir));
		result = pow(diff,material.shininess);
	}
	else//bling-phone
	{
		vec3 halfwayVector = normalize(normFragToLightDir + normFragToCameraDir);
		float diff = max(dot(halfwayVector,norm),0);
		result = pow(diff, 4* material.shininess);
	}
	return result;
}
vec3 caculateDirectionalLight(DirectionalLight a_DirLight, vec3 a_Normal,vec3 a_FragToCameraDir)
{
	vec3 diffuseTextureColor = vec3(texture(material.texture_diffuse0, ps_in.texCoords));
	vec3 specularTextureColor = vec3(texture(material.texture_specular0, ps_in.texCoords));
	vec3 norm = normalize(a_Normal);
	//ambient
	vec3 ambient = a_DirLight.ambient* diffuseTextureColor;
	//diffuse
	float diffuseDiff = max(0,dot(normalize(-a_DirLight.direction),norm));
	vec3 diffuse = a_DirLight.diffuse*diffuseDiff * diffuseTextureColor;
	//specular
	float diff = caculateSpecular(-a_DirLight.direction,a_Normal,a_FragToCameraDir);
	vec3 specular = a_DirLight.specular*diff*specularTextureColor;

	return ambient + (1-isInShadow())*(diffuse + specular);
	//return ambient + diffuse + specular;
}

vec3 caculatePointLight(PointLight a_PointLight, vec3 a_Normal, vec3 a_FragPos,vec3 a_FragToCameraDir)
{
	vec3 diffuseTextureColor = vec3(texture(material.texture_diffuse0, ps_in.texCoords));
	vec3 specularTextureColor = vec3(texture(material.texture_specular0, ps_in.texCoords));
	vec3 norm = normalize(a_Normal);
	vec3 fragToLightDir = normalize(a_PointLight.position - a_FragPos);
	//ambient
	vec3 ambient = a_PointLight.ambient * diffuseTextureColor;
	//diffuse
	float diffuseDiff = max(0,dot(fragToLightDir,norm));
	vec3 diffuse = a_PointLight.diffuse*diffuseDiff*diffuseTextureColor;
	//specular
	float specularDiff = caculateSpecular(fragToLightDir,a_Normal,a_FragToCameraDir);
	vec3 specular = specularDiff* a_PointLight.specular * specularTextureColor;
	//attenuation
	float distanceLightToFrag = length(a_PointLight.position - a_FragPos);
	float attenuation = 1/(a_PointLight.constant +a_PointLight.linear * distanceLightToFrag + a_PointLight.quadratic *distanceLightToFrag*distanceLightToFrag);
	
	return clamp(attenuation*(ambient + diffuse + specular),0,1);
}

vec3 caculateSpotLight(SpotLight a_SpotLight,vec3 a_Normal,vec3 a_FragPos,vec3 a_FragToCameraDir) 
{
	vec3 textureDiffse = vec3(texture(material.texture_diffuse0,ps_in.texCoords));
	vec3 textureSpecular = vec3(texture(material.texture_specular0,ps_in.texCoords));
	vec3 fragToLightDir = normalize(a_SpotLight.position-a_FragPos);
	vec3 norm = normalize(a_Normal);

	//cone attenuation
	float lightCos = dot(fragToLightDir,normalize(-a_SpotLight.direction));
	float coneAttenuation = (lightCos - a_SpotLight.outerCutOffCos)/(a_SpotLight.cutOffCos - a_SpotLight.outerCutOffCos);
	coneAttenuation = clamp(coneAttenuation,0,1);
	//radius attenuation
	float distance = length(a_FragPos - a_SpotLight.position);
	float radiusAttenuation = 1/(a_SpotLight.constant + a_SpotLight.linear * distance + a_SpotLight.quadratic * distance * distance);
	//ambient
	vec3 ambient = a_SpotLight.ambient * textureDiffse * radiusAttenuation;
	//diffuse
	float diffuseDiff = max(0,dot(fragToLightDir,norm));
	vec3 diffuse = diffuseDiff * a_SpotLight.diffuse * textureDiffse * coneAttenuation * radiusAttenuation;
	//specular
	float specularDiff = caculateSpecular(fragToLightDir,a_Normal,a_FragToCameraDir);
	vec3 specular = specularDiff * a_SpotLight.specular * textureSpecular * coneAttenuation * radiusAttenuation;

	return clamp(ambient + diffuse + specular,0,1);
}