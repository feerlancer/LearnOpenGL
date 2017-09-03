#version 430 core
uniform sampler2DMS texture1;

in vec2 texCoords;

out vec4 FragColor;

const float offset = 1.f/300.f;

const int sampleNum = 4;
// vec2 offsets[] =
// {
// 	vec2(-offset,offset),
// 	vec2(0,offset),
// 	vec2(offset,offset),
// 	vec2(-offset,0),
// 	vec2(0,0),
// 	vec2(offset,0),
// 	vec2(-offset,-offset),
// 	vec2(0,-offset),
// 	vec2(offset,-offset),
// };
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
// float kernel[] = 
// {
//     1.0 / 16, 2.0 / 16, 1.0 / 16,
//     2.0 / 16, 4.0 / 16, 2.0 / 16,
//     1.0 / 16, 2.0 / 16, 1.0 / 16  
// };

void main()
{
	FragColor=vec4(0);
	for(int i = 0;i<sampleNum;++i)
	{
		FragColor+=(0.25* texelFetch(texture1, ivec2(texCoords.x * 800,texCoords.y * 600), i));
	}
}