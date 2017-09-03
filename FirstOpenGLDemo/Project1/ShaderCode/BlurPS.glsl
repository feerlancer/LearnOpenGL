#version 430
uniform sampler2D texture1;

in vec2 texCoords;

out vec4 FragColor;

const float offset = 1.f/300.f;

const int sampleNum = 9;
vec2 offsets[] =
{
	vec2(-offset,offset),
	vec2(0,offset),
	vec2(offset,offset),
	vec2(-offset,0),
	vec2(0,0),
	vec2(offset,0),
	vec2(-offset,-offset),
	vec2(0,-offset),
	vec2(offset,-offset),
};
// float kernel[] =
// {
// 	-1, -1, -1,
// 	-1,  10, -1,
// 	-1, -1, -1
// };
// float kernel[] =
// {
// 	0,0,0,
// 	0,1,0,
// 	0,0,0
// };
float kernel[] = 
{
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
};

void main()
{
	FragColor=vec4(0);
	for(int i = 0;i<sampleNum;++i)
	{
		FragColor+=kernel[i]* texture(texture1,texCoords+offsets[i]);
	}
}