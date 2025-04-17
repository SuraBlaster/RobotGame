#include "DefaultLit.hlsli"

VS_OUT main(
	float4 position     : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 boneWeights : WEIGHTS,
	uint4  boneIndices : BONES)
{
	float4 p = { 0, 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]]));
		n += mul(boneTransforms[boneIndices[i]], float4(normal, 0)).xyz;
	}

	n = normalize(n);

	VS_OUT vout;
	position = p;

	vout.position = mul(p, viewProjection);
	vout.world_position = p;
	vout.normal = n;
	vout.texcoord = texcoord;
	
	float3 t = float3(0, 0, 0);

	for (int i = 0; i < 4; i++)
	{
		t += boneWeights[i] * mul(float4(tangent, 0), boneTransforms[boneIndices[i]]).xyz;
	}

	vout.tangent = t;
	vout.binormal = cross(vout.normal, vout.tangent);
	return vout;
}