#version 430 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.4,0.9,0.2,1);
	gl_FragDepth = 0;
};