#version 430 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 worldPos;
    vec3 worldDirection;
    float cutOffCos;
	float outCutOffCos;

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
uniform Light light;
uniform vec3 worldCameraPos;

void main()
{
	vec3 diffuseTexColor = vec3(texture(material.diffuse,texCoords));

	vec3 ambient = light.ambient * diffuseTexColor;

	vec3 norm = normalize(worldNormal);
	vec3 lightDir = normalize(light.worldPos - worldFragPos);
	float diff = max(dot(norm,lightDir),0);
	vec3 diffuse = diffuseTexColor*diff*light.diffuse;

	vec3 cameraDir = normalize(worldCameraPos- worldFragPos);
	vec3 reflectLightDir = reflect(-lightDir,norm);
	float diffSpecular = max(dot(cameraDir,reflectLightDir),0);
	vec3 specular = pow(diffSpecular,material.shininess) * light.specular * texture(material.specular,texCoords);

	float distance = length(light.worldPos-worldFragPos);
	float attenuation = 1/(light.constant+light.linear*distance+light.quadratic*distance*distance);

	float inter = (dot(lightDir,normalize(-light.worldDirection)) - light.outCutOffCos)/(light.cutOffCos - light.outCutOffCos);
	float spotLightIntensity = clamp(inter,0,1);

	FragColor = vec4(attenuation*(ambient + spotLightIntensity * (diffuse + specular)),1);
}