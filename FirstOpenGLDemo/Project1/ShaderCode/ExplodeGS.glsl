#version 430 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;


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

uniform float deltaTime;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a,b));
}

vec4 explode(vec4 a_Pos,vec3 a_Normal)
{
    float magnitude = 4;
    float t_deltaTime = (sin(deltaTime)+1)/2 * magnitude;
    return a_Pos + vec4(t_deltaTime * a_Normal,0);
}

void main()
{
    vec3 normal = GetNormal();
    for(int i = 0;i<3;++i)
    {
        gs_out.worldFragPos = gs_in[i].worldFragPos;
        gs_out.worldNormal = gs_in[i].worldNormal;
        gs_out.texCoords = gs_in[i].texCoords;
        //gl_Position = gl_in[i].gl_Position;
        //normal = normalize((projection * view * vec4(gs_in[i].worldNormal,0)).xyz);
        gl_Position = explode(gl_in[i].gl_Position,normal);
        EmitVertex();
    }
    EndPrimitive();

}

