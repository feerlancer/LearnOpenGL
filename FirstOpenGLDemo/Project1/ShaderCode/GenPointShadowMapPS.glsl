#version 430 core

uniform vec3 pointlightWorldPos;
uniform float pointlightViewFarPlane;
in vec3 fragWorldPos;
void main()
{
    float distance = length(fragWorldPos - pointlightWorldPos);
    gl_FragDepth = distance/pointlightViewFarPlane; // map to [0,1]
}