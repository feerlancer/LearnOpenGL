#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightWorldPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	vec3 normal = mat3(transpose(inverse(view*model)))*aNormal;
	vec4 v4worldPos = view * model * vec4(aPos,1.0);
	vec3 pos = v4worldPos.xyz;
	vec3 lightPos = vec3(view*model* vec4(lightWorldPos,1));

	gl_Position = projection* v4worldPos;

	float ambientStrength = 0.1;
	vec3 ambient = lightColor * ambientStrength;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(norm,lightDir),0);
	vec3 diffuse = diff*lightColor;

	float specularStrength = 1;
	vec3 cameraDir = normalize(- pos);
	vec3 reflectLightDir = reflect(-lightDir,norm);
	float diffSpecular = max(dot(cameraDir,reflectLightDir),0);
	vec3 specular = pow(diffSpecular,32) * lightColor * specularStrength;

	color = vec4((ambient + diffuse+specular)* objectColor,1);
}