float4 main(float3 color : Color, float time : Time) : SV_TARGET
{
	return float4(color.x, color.y + time, color.z, 1.0f);
}