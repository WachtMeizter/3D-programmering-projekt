cbuffer EXAMPLE_BUFFER : register(b0)
{
	float4x4 view;
	float4x4 world;
	float4x4 proj;
};

struct VS_IN
{
	float3 VPos : POSITION;
	float3 Color : COLOR;
};

struct VS_OUT
{
	float4 VPos : SV_POSITION;
	float3 Color : COLOR;
};



VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	float4x4 transformation = mul(world, mul(view, proj));
	/*input.Position.x += sin(value1);
	input.Position.y -= cos(value1);*/

	output.VPos = float4(input.VPos, 1);
	output.Color = input.Color;
	//output.Color = float3(0.5f, 0.5f, 0.5f);

	return output;
}