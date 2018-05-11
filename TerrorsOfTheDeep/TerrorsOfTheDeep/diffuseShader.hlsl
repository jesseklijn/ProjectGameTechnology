float4x4 mWorldViewProj;
float4x4 world;
float3 camPos;
float3 lightPos;
float4 lightCol;
float4 ambientLightCol;
float4 lightDistance;
Texture diffuseTexture;

sampler texSampler = sampler_state
{
Texture = diffuseTexture;
};

struct appdata
{
float4 vertex : POSITION;
float3 normal : NORMAL;
float2 uv : TEXCOORD0;
};

struct v2f
{
	float4 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float4 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

v2f vert(appdata i)
{
	v2f o;
	o.normal = normalize(mul(i.normal, mWorldViewProj));
	o.pos = mul(mWorldViewProj, i.vertex);
	o.uv = i.uv;
	o.normal = normalize(o.normal);
	return o;
}


float4 frag (v2f i): COLOR
{
	float3 lightDir = normalize(i.worldPos - lightPos);
	float diffuseLighting = saturate(dot(i.normal, -lightDir));
	
	diffuseLighting *= (lightDistance / dot(lightPos - i.worldPos, lightPos -i.worldPos));

	float3 h = normalize(normalize(camPos - i.worldPos) - lightDir);
	float4 tex = tex2D(texSampler, i.uv);

	return float4(saturate(ambientLightCol + (tex.xyz * lightCol * lightCol * diffuseLighting * 0.6)), uv.w);
}