Texture2D g_txDiffuse : register(t0);
Texture2D g_txCube : register(t1);
SamplerState g_Sampler : register(s0);

cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float4 g_LightColor : packoffset(c12);
	float  g_fTimer : packoffset(c13.z);
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
	float2 r : TEXCOORD1;
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
	//float3 vNormal = mul(vIn.n, (float3x3)g_matWorld);
	//output.n = normalize(vNormal);
	//float3 fDot = max(0, dot(output.n, -g_LightColor.xyz));
	output.c = vIn.c;
	output.t = vIn.t;
	//output.r = normalize(vLocal.xyz);
	return output;
}
VS_OUT VSfrustum(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	output.p = vProj;
	output.c = vIn.c;
	output.t = normalize(vLocal.xyz);
	return output;
}

float4 PS(VS_OUT v) : SV_TARGET
{
	return g_txDiffuse.Sample(g_Sampler, v.t);//g_txDiffuse.Sample(g_Sampler,v.t);
}

float4 PSfrustum(VS_OUT v) : SV_TARGET
{
	float4 vColor = v.c;
	vColor = (1.0f, 0.5f, 0.0f, 1.0f);
	vColor.a = 0.5f;
	return vColor;
}