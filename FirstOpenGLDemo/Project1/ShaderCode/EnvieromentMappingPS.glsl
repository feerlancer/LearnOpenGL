#version 430
in vec3 worldFragPos;
in vec3 worldNormal;

uniform vec3 worldCameraPos;
uniform samplerCube cubeTexture;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(worldNormal);
    vec3 cameraToFragDir = normalize(worldFragPos - worldCameraPos);
    vec3 reflectDir = reflect(cameraToFragDir,norm);
    vec3 refractDir = refract(cameraToFragDir,norm,1.00 / 1.52);

    vec4 reflectColor = texture(cubeTexture,reflectDir);
    vec4 refractColor = texture(cubeTexture,refractDir);

    FragColor = mix(reflectColor,refractColor,0.2);
}