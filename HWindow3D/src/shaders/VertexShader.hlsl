
struct VSOut
{
	float3 color : Color;
	float time : Time;
	float4 pos : SV_Position;
};

VSOut main( float2 pos : Position, float3 color : Color, float time : Time )
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.color = float3(color.x, color.y, color.z);
	vso.time = time;
	return vso;
}