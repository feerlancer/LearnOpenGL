#version 430
const float Pi = 3.1415926;

struct PBR_DirectionalLight
{
	vec3 position;
	float padding0;
};
struct PBR_PointLight
{
	vec3 position;
	float padding0;

	vec3 color;
	float padding1;
};
struct PBR_SpotLight
{
	vec3 position;
	float padding0;
};

layout(binding = 1) uniform UBO_BLOCK_MATRIX
{
	vec3 worldCameraPos;
	float floatPadding0;

	int Bling;
	int GammaCorrect;
	float floatPadding1;
	float floatPadding2;
};

layout(binding = 3) uniform UBO_BLOCK_PBR_LIGHT_INFO //binging = 3
{
	PBR_DirectionalLight pbr_dirlight;
	PBR_PointLight pbr_pointLights[4];
	PBR_SpotLight pbr_spotLight;
};

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

in VS_OUT
{
	vec3 worldFragPosition;
	vec2 texCoords;
	vec3 worldNormal;
}ps_in;
out vec4 FragColor;

//TODO
// float NormalDistributionFunction(float a_roughness,vec3 a_normal,vec3 a_HalfVector)
// {
// 	float a2 = a_roughness * a_roughness;
// 	float dot_n_h = dot(normalize(a_normal), normalize(a_HalfVector));
// 	dot_n_h = max(0,dot_n_h);
// 	float dot_n_h_2 = dot_n_h * dot_n_h;
// 	return a2/(Pi * pow(dot_n_h_2 *(a2-1) + 1,2));
// }

float NormalDistributionFunction(float roughness,vec3 N, vec3 H)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = Pi * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(vec3 a_F0, vec3 a_normal, vec3 a_viewDir)
{
	return a_F0 + (1-a_F0) * pow(1-max(0,dot(normalize(a_normal), normalize(a_viewDir))),5);
}

//TODO
#define NR_POINT_LIGHTS 4

void main()
{
	vec3 F0 = mix(vec3(0.04),albedo,metallic);
	vec3 fragToCameraDir = worldCameraPos - ps_in.worldFragPosition;
	vec3 V = normalize(fragToCameraDir);
	vec3 N = normalize(ps_in.worldNormal);
	float dotNV = max(0,dot(N,V));
	vec3 tempColor = vec3(0);
	for(int i =0;i< NR_POINT_LIGHTS;++i)
	{
		vec3 fragToLightDir = pbr_pointLights[i].position-  ps_in.worldFragPosition;
		vec3 L = normalize(fragToLightDir);
		vec3 H = normalize(L + V);
		float dotNL = max(0,dot(N,L));
		float dotNH = max(0,dot(N,H));

		float distance = length(fragToLightDir);
		vec3 Li = pbr_pointLights[i].color/(distance * distance);
		//corrtance BRDF
		//BRDF = (diffuseItem + specularItem)
		//diffuseItem = Kd * diffuse /Pi;
		//diffuse = mix(0.04,albedo,metallic)
		//specularItem = Ks * DFG / (4*dot(n,v)*dot(n,l));
		//TODO
		 //vec3 F = fresnelSchlick(F0,N,L);
		vec3 F = fresnelSchlick(F0,H,V);

		vec3 Ks = F; // ratio of reflect energy
		vec3 Kd = 1-Ks; 
		Kd*=(1-metallic); // ratio of refract energy
		vec3 diffuseItem =Kd * albedo / Pi; 

		float D = NormalDistributionFunction(roughness,N,H);
		float G = GeometrySmith( N,V, L, roughness);

		//vec3 specularItem = D* F* G /(4*dotNV*dotNL);
		vec3 specularItem = (D* F* G)/(4*max(0.000001,dotNV)*max(0.000001,dotNL));

		tempColor += ((diffuseItem + specularItem) * Li *dotNL);
	}
	vec3 ambient = vec3(0.03) * albedo;
	//TODO
	tempColor+=ambient;
	//HDR->LDR
	tempColor = tempColor /(tempColor + 1);

	//gammaCorrection
	FragColor = vec4(pow(tempColor,vec3(1/2.2)),1);
}