#include "DxState.h"

ID3D11DepthStencilState* DxState::g_pCurrentDSS = nullptr;
ID3D11SamplerState*      DxState::g_pCurrentSS[3] = { nullptr,nullptr,nullptr };
ID3D11RasterizerState*   DxState::g_pCurrentRS = nullptr;

ID3D11DepthStencilState* DxState::g_pLessEqualDSS = nullptr;
ID3D11DepthStencilState* DxState::g_pDSSDepthDisable = nullptr;

ID3D11SamplerState*      DxState::g_pClampSS = nullptr;
ID3D11SamplerState*      DxState::g_pWrapSS = nullptr;
ID3D11SamplerState*		 DxState::g_pPointSS = nullptr;

ID3D11RasterizerState*   DxState::g_pRSSolid = nullptr;
ID3D11RasterizerState*   DxState::g_pRSWireFrame = nullptr;
ID3D11RasterizerState*	 DxState::g_pRSNoneCullSolid = nullptr;
ID3D11RasterizerState*	 DxState::g_pRSNoneCullWireFrame = nullptr;

bool DxState::Init()
{
	CreateDepthStencilState();
	CreateSamplerState();
	CreateRasterizerState();
	return true;
}
HRESULT DxState::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sd.DepthEnable = TRUE;
	sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	sd.StencilEnable = FALSE;
	sd.StencilReadMask = 1;
	sd.StencilWriteMask = 1;
	sd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	sd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	sd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	sd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 디폴트 값
	sd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	sd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	sd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	sd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	hr = g_pd3dDevice->CreateDepthStencilState(&sd, &g_pLessEqualDSS);
	if (FAILED(hr))
	{
		return hr;
	}

	sd.DepthEnable = FALSE;
	if (FAILED(hr = g_pd3dDevice->CreateDepthStencilState(&sd, &g_pDSSDepthDisable)))
	{
		return hr;
	}
	return hr;
}

HRESULT	DxState::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HRESULT hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pClampSS);

	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pWrapSS);
	
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pPointSS);

	return hr;
}

HRESULT    DxState::CreateRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSWireFrame);

	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSNoneCullWireFrame);

	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSSolid);

	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSNoneCullSolid);

	return hr;
}
bool DxState::Release()
{
	SAFE_RELEASE(g_pLessEqualDSS);
	SAFE_RELEASE(g_pClampSS);
	SAFE_RELEASE(g_pWrapSS);
	SAFE_RELEASE(g_pRSSolid);
	SAFE_RELEASE(g_pRSWireFrame);
	SAFE_RELEASE(g_pPointSS);
	return true;
}
