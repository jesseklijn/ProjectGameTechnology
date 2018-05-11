//float4x4 mWorldViewProj;
//float4x4 invertWorld;
//float timeInSeconds;

//struct v2f
//{
//	float4 pos : POSITION0;
//	float4 col : COLOR0;
//	float4 normal : NORMAL0;
//	float2 uv : TEXCOORD0;
//}

//v2f vert() : SV_POSITION
//{
//	v2f o;
//	o.pos = mul(mWorldViewProj, pos);
//	o.pos.x = sin(timeInSeconds * 5) * 2;
//	o.pos.y = sin(timeInSeconds * 5) * 2;
//	return o;
//}


//float4 frag ( float4 col : COLOR)
//{
//return col;
//}