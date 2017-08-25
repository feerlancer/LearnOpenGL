#version 430 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D outTexture;

float n = 0.1f;
float f = 100.f;
float LinearizeDepth(float depth)
{
	//[0,1]->[-1,1] NDC
	depth = depth*2-1;
	return 2*f*n/(depth*(f-n)-(f+n));
}

void main()
{
	FragColor = texture(outTexture, texCoord);
	//FragColor=vec4(vec3(LinearizeDepth(gl_FragCoord.z)/f),1);
};