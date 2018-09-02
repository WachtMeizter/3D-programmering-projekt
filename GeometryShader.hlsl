cbuffer EXAMPLE_BUFFER : register(b0)
{
	float4x4 view;
	float4x4 world;
	float4x4 proj;
};

struct GS_IN
{
	float4 VPos : SV_POSITION;
	float3 Color : COLOR;
};

struct GS_OUT
{
	float4 VPos : SV_POSITION;
	float3 Color : COLOR;
	float4 Normal : NORMAL;
	float3 PointOnSurface : POSITION;
};

[maxvertexcount(3)]
void GS_main(
	triangle GS_IN input[3], 
	inout TriangleStream< GS_OUT > PS_STREAM
)
{
	GS_OUT element;
	float4 normal = normalize(float4(cross(input[1].VPos - input[0].VPos, input[2].VPos - input[0].VPos), 0.0f));
	
	float4x4 transformation = mul(world, mul(view, proj));
	for (uint i = 0; i < 3; i++)
	{
		element.VPos = mul(input[i].VPos, transformation);
		element.PointOnSurface = mul(input[i].VPos, world);
		element.Normal = mul(normal, transformation);
		element.Color = input[i].Color;
		if (element.Normal.z <= 0.2f)
		PS_STREAM.Append(element);
	}


}