#version 430
layout(location = 0)in vec3 a_Pos;
layout(location = 1)in vec3 a_Normal;

out vec3 worldFragPos;
out vec3 worldNormal;

uniform mat4 model;

layout(binding = 0 ) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
void main()
{
    worldFragPos = vec3(model * vec4(a_Pos,1.f));
    worldNormal = mat3(transpose(inverse(model))) * a_Normal;
    gl_Position = projection*view*model * vec4(a_Pos,1.f);
}