#version 430
layout(location = 0) in vec3 a_Pos;
out vec3 texCoords;
uniform mat4 view, projection;
void main()
{
    gl_Position = projection *view * vec4(a_Pos,1.f);
    gl_Position = gl_Position.xyww;
    texCoords = a_Pos;
}