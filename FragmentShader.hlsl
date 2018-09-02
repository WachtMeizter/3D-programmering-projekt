struct PS_OUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
	float3 PointOnSurface : POSITION;
};

struct GS_OUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
	float3 PointOnSurface : POSITION;
};

float4 PS_main(GS_OUT input) : SV_Target
{
	float4 ambient = { 2.5f, 0.0f, -2.0f, 1.0f };	
	float3 grey = { 0.2f, 0.2f, 0.2f };
	input.Color = grey;
	float4 direction = ambient - float4(input.PointOnSurface, 1);
	float diffuse = max(dot(direction, input.Normal), 0);
	float3 finalcolor = saturate(grey * diffuse);
	finalcolor = input.Color * diffuse;
	return float4(finalcolor, 1.0f);
}