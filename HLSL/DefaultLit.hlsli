// 頂点シェーダー　→　ピクセルシェーダーに渡す値

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

cbuffer CbScene : register(b0)
{
	row_major float4x4	viewProjection;
	float4				lightDirection;
	float4		        lightColor;
	float4		        cameraPosition;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};
