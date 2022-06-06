#include"Sample.h"

bool		Sample::Init()
{
	m_matTex._11 = 0.5f; m_matTex._22 = -0.5f;
	m_matTex._41 = 0.5f; m_matTex._42 = 0.5f;

	m_Light.Set(TVector3(100, 150, 100), TVector3(0, 0, 0));

	m_MiniMap.Create(L"../../Data/shader/PlaneVS.txt", L"../../Data/shader/PlanePS.txt", L"");
	m_MapObj.Create(L"../../Data/shader/ProjectionShadow.txt", L"../../Data/shader/ProjectionShadow.txt", L"../../Data/object/Chrome1.bmp");

	TMatrix matWorld, matScale;
	D3DXMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DXMatrixScaling(&matScale, 200.0f, 200.0f, 200.0f);
	m_MapObj.m_matWorld = matScale * matWorld;
	m_ViewRt.Create(4096, 4096);

	ID3DBlob* PSBlob = nullptr;
	PSBlob = Model::LoadShaderBlob(L"CharacterShader.hlsl", "PSShadow", "ps_5_0");
	if (PSBlob != nullptr)

	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &m_pPSShadow);
		if (FAILED(hr))
		{
			return false;
		}
		PSBlob->Release();
	}

	m_FbxObjB.LoadObject("../../Data/object/Man.fbx", "CharacterShader.hlsl");

	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool		Sample::Frame()
{

	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxObjB.m_bAnimPlay = !m_FbxObjB.m_bAnimPlay;
	}
	
	m_FbxObjB.Frame();
	m_Light.Frame();
	m_CbShadow.g_matShadow1 = m_Light.m_matView * m_Light.m_matProj * m_matTex;

	return true;
}
bool		Sample::Render()
{
	//ApplyRS(m_pImmediateContext, DxState::g_pRSSolid);
	//ApplyRS(m_pImmediateContext, DxState::g_pRSWireFrame);

	if (m_ViewRt.Begin(m_pImmediateContext))
	{
		m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
		m_FbxObjB.SetPixelShader(m_pPSShadow);
		m_FbxObjB.Render(m_pImmediateContext);
		m_ViewRt.End(m_pImmediateContext);
	}

	ApplySS(m_pImmediateContext, DxState::g_pClampSS, 1);
	m_MapObj.m_CbData.matNormal = m_CbShadow.g_matShadow1;
	m_MapObj.SetMatrix(&m_MapObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_ViewRt.m_pTextureSRV);
	m_MapObj.Render(m_pImmediateContext);

	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_ViewRt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext, m_MiniMap.m_iNumIndex);

	m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjB.SetPixelShader(nullptr);
	m_FbxObjB.Render(m_pImmediateContext);


	if (g_Input.GetKey(VK_F5) == KEY_PUSH)
	{
		m_ViewRt.Save(m_pImmediateContext, L"frame.jpg");
	}
	return true;
}
bool		Sample::Release()
{
	m_MapObj.Release();
	m_MiniMap.Release();
	m_ViewRt.Release();
	m_FbxObjB.Release();
	SAFE_RELEASE(m_pPSShadow);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}

GAMECORE(ss, 1024, 768);