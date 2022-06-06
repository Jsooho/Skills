Texture2D g_txDiffuse : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer cbData : register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
};
cbuffer cbData1 : register(b1)
{
	float4   vLightDir : packoffset(c0);
	float4   vLightPos : packoffset(c1);
};
struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
};
VS_OUT VS(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	output.p = vProj;
	float3 vNormal = mul(vIn.n, (float3x3)g_matWorld);
	output.n = normalize(vNormal);
	float Dot = max(0.5f, dot(output.n, -vLightDir.xyz));
	output.c = vIn.c * float4(Dot,Dot,Dot,1);
	output.t = vIn.t;
	output.r = normalize(vLocal.xyz);
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	float4 color = g_txDiffuse.Sample(g_Sampler, v.t);
	return color;
}