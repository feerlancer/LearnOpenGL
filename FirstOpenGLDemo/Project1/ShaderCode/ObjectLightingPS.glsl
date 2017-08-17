#version 430 core
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 pos;
in vec3 normal;
in vec3 lightPos;
in vec2 texCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main()
{
	vec3 diffuseTexColor = vec3(texture(material.diffuse,texCoords));

	vec3 ambient = light.ambient * diffuseTexColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(norm,lightDir),0);
	vec3 diffuse = diffuseTexColor*diff*light.diffuse;

	vec3 cameraDir = normalize(- pos);
	vec3 reflectLightDir = reflect(-lightDir,norm);
	float diffSpecular = max(dot(cameraDir,reflectLightDir),0);
	vec3 specular = pow(diffSpecular,material.shininess) * light.specular * material.specular;

	FragColor = vec4(ambient + diffuse+specular,1);
}