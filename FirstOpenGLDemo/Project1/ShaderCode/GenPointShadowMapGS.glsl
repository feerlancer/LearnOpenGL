#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowVPMatrices[6];

out vec3 fragWorldPos;
void main()
{
	for(int face =0;face<6;++face)
	{
		gl_Layer = face;
		for(int i =0;i<3;++i)
		{
			fragWorldPos = vec3(gl_in[i].gl_Position);
			gl_Position = shadowVPMatrices[face] * vec4(fragWorldPos,1);
			EmitVertex();
		}
		EndPrimitive();
	}
}