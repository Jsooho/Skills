Texture2D		g_txColor : register(t0);
Texture2D		g_txMask : register(t1);
Texture2D		g_txColorArray[6] : register(t2);
SamplerState	g_SamplePoint : register(s0);
SamplerState	g_SampleLinear : register(s1);

cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
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
};
VS_OUT VS(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	output.p = vProj.xyzz;
	output.n = vIn.n;
	output.c = vIn.c;
	output.t = vIn.t;
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	float4 color = g_txColor.Sample(g_SamplePoint, v.t);
	
	return color;
}