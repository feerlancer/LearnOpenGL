#version 430 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
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

in vec3 worldFragPos;
in vec3 worldNormal;
in vec2 texCoords;

out vec4 FragColor;

uniform Material material;

#define NR_POINT_LIGHTS 4
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 worldCameraPos;


vec3 caculateDirectionalLight(DirectionalLight a_DirLight, vec3 a_Normal,vec3 a_FragToCameraDir);
vec3 caculatePointLight(PointLight a_PointLight, vec3 a_Normal, vec3 a_FragPos,vec3 a_FragToCameraDir);
vec3 caculateSpotLight(SpotLight a_SpotLight,vec3 a_Normal,vec3 a_FragPos,vec3 a_FragToCameraDir);

void main()
{
	vec3 FragColorV3 = vec3(0);
	vec3 fragToCameraDir = worldCameraPos - worldFragPos;
	FragColorV3 = caculateDirectionalLight(directionalLight,worldNormal,fragToCameraDir);
	for(int i=0;i<NR_POINT_LIGHTS;++i)
	for(int i=0;i<1;++i)
	{
		FragColorV3+=caculatePointLight(pointLights[i],worldNormal,worldFragPos,fragToCameraDir);
	}
	FragColorV3+=caculateSpotLight(spotLight,worldNormal,worldFragPos,fragToCameraDir);

	FragColor = vec4(FragColorV3,1);
}

vec3 caculateDirectionalLight(DirectionalLight a_DirLight, vec3 a_Normal,vec3 a_FragToCameraDir)
{
	vec3 diffuseTextureColor = vec3(texture(material.diffuse, texCoords));
	vec3 specularTextureColor = vec3(texture(material.specular, texCoords));
	vec3 norm = normalize(a_Normal);
	//ambient
	vec3 ambient = a_DirLight.ambient* diffuseTextureColor;
	//diffuse
	float diffuseDiff = max(0,dot(normalize(-a_DirLight.direction),norm));
	vec3 diffuse = a_DirLight.diffuse*diffuseDiff * diffuseTextureColor;
	//specular
	vec3 reflectDir = normalize(reflect(a_DirLight.direction,norm));
	float diff = max(0,dot(reflectDir,normalize(a_FragToCameraDir)));
	diff = pow(diff,material.shininess);
	vec3 specular = a_DirLight.specular*diff*specularTextureColor;

	return ambient + diffuse + specular;
}

vec3 caculatePointLight(PointLight a_PointLight, vec3 a_Normal, vec3 a_FragPos,vec3 a_FragToCameraDir)
{
	vec3 diffuseTextureColor = vec3(texture(material.diffuse, texCoords));
	vec3 specularTextureColor = vec3(texture(material.specular, texCoords));
	vec3 norm = normalize(a_Normal);
	vec3 fragToLightDir = normalize(a_PointLight.position - a_FragPos);
	//ambient
	vec3 ambient = a_PointLight.ambient * diffuseTextureColor;
	//diffuse
	float diffuseDiff = max(0,dot(fragToLightDir,norm));
	vec3 diffuse = a_PointLight.diffuse*diffuseDiff*diffuseTextureColor;
	//specular
	vec3 reflectDir = normalize(reflect(-fragToLightDir,norm));
	float specularDiff = max(0,dot(reflectDir,normalize(a_FragToCameraDir)));
	specularDiff = pow(specularDiff,material.shininess);
	vec3 specular = specularDiff* a_PointLight.specular * specularTextureColor;
	//attenuation
	float distanceLightToFrag = length(a_PointLight.position - a_FragPos);
	float attenuation = 1/(a_PointLight.constant +a_PointLight.linear * distanceLightToFrag + a_PointLight.quadratic *distanceLightToFrag*distanceLightToFrag);
	
	return attenuation*(ambient + diffuse + specular);
}

vec3 caculateSpotLight(SpotLight a_SpotLight,vec3 a_Normal,vec3 a_FragPos,vec3 a_FragToCameraDir) 
{
	vec3 textureDiffse = vec3(texture(material.diffuse,texCoords));
	vec3 textureSpecular = vec3(texture(material.specular,texCoords));
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
	vec3 reflectDir = normalize(reflect(-fragToLightDir,norm));
	float specularDiff = max(0,dot(reflectDir,normalize(a_FragToCameraDir)));
	specularDiff = pow(specularDiff,material.shininess);
	vec3 specular = specularDiff * a_SpotLight.specular * textureSpecular * coneAttenuation * radiusAttenuation;

	return ambient + diffuse + specular;
}