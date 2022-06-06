Texture2D g_txDiffuse : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer cbData : register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
};

cbuffer cbLight : register(b1)
{
	matrix g_matWorldInverse : packoffset(c0);
	float4 g_LightDir :packoffset(c4);
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
	output.p = vProj;
	output.n = vIn.n;
	output.c = vIn.c;
	output.t = vIn.t;
	
	
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	float4 color = float4(0.0f,0.0f,0.0f,1.0f);
	return g_txDiffuse.Sample(g_Sampler, v.t);
	
}

float4 PS_Color(VS_OUT v) : SV_TARGET
{
	
	return v.c;

}

float4 PSShadow(VS_OUT v) : SV_TARGET
{
	return float4(1.0f,1.0f,1.0f,1.0f);
}