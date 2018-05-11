float4x4 world;
float4x4 view;
float4x4 projection;

float4 ambientColor = float4(1,1,1,1);
float ambientIntensity = 0,1;

struct vertInput
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
}

struct v2f
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
}

float4 v2f(vertInput input)
{
	v2f o;
	o.pos = mul(world, input.pos);
	return o;
}