#include "billboard.hlsli"

VS_OUT main(float4 position:POSITION,float4 color:COLOR, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	VS_OUT vout;
	vout.position = mul(position, world_view_projection);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-light_direction);

	//vout.color = color;
	vout.color = material_color;
	vout.texcoord = texcoord;
	
	return vout;
}
