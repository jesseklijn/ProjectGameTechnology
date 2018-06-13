
cbuffer MatrixBuffer
{
matrix world;
matrix view;
matrix proj;
};


struct VS_INPUT
{
	float4 pos : SV_POSITION;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT o;
	o.pos = mul(input.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);
	o.col = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return o;
}


//struct VS_INPUT 
////{
//	float4 Pos : SV_POSITION;
//   float4 Col : COLOR0;
//};

//struct VS_OUTPUT 
//{
//   float4 PosOut : SV_POSITION;
//   float4 ColOut : COLOR0;
//};


//VS_OUTPUT main( VS_INPUT Input )
//{
//   VS_OUTPUT Output;

//   Output.PosOut = Input.Pos;
//   Output.ColOut = float4(1.0f, 0.0f, 0.0f, 1.0f);
//   return( Output );
   
//}
