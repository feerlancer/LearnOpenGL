#version 430
in vec3 texCoords;
uniform samplerCube cubeTexture;
out vec4 FragColor;
void main()
{
    FragColor = texture(cubeTexture,texCoords);
}