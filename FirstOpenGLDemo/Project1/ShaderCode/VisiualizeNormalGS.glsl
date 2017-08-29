#version 430 core
layout(triangles) in;
//layout(triangle_strip,max_vertices = 3) out;
layout(line_strip,max_vertices = 6) out;

in VS_OUT
{
	vec3 worldFragPos;
	vec3 worldNormal;
	vec2 texCoords;
}gs_in[];

out VS_OUT
{
	vec3 worldFragPos;
	vec3 worldNormal;
	vec2 texCoords;
}gs_out;

layout(binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    float magnitude = 0.4;
    vec3 normal;
    for(int i = 0;i<3;++i)
    {
        gs_out.worldFragPos = gs_in[i].worldFragPos;
        gs_out.worldNormal = gs_in[i].worldNormal;
        gs_out.texCoords = gs_in[i].texCoords;

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        normal = normalize((projection * view * vec4(gs_in[i].worldNormal,0)).xyz);
        gl_Position = gl_Position + magnitude * normal;
        EmitVertex();
        EndPrimitive();
    }
}

