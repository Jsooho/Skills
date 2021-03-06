#include"Sample.h"

bool		Sample::Init()
{
	//m_ViewRt.CreateRenderTargetView(512, 512);

	//m_Plane.x = 0.0f; // 지면
	//m_Plane.y = 1.0f;
	//m_Plane.z = 0.0f;
	//m_Plane.w = 0.0f;


	//m_Vector4.x = 300.0f; // 조명
	//m_Vector4.y = 300.0f;
	//m_Vector4.z = 300.0f;
	//m_Vector4.w = 0.0f;

	//D3DXMatrixShadow(&m_MatShadow, &m_Vector4, &m_Plane); // 쉐도우 행렬 생성 함수

	//ID3DBlob* PSBlob = nullptr;
	

	//m_FbxObjA.LoadObject("../../Data/object/Ship.fbx", "CharacterShader.hlsl");

	m_FbxObjB.LoadObject("../../Data/object/Man.fbx", "CharacterShader.hlsl");

	/*PSBlob = Model::LoadShaderBlob(L"CharacterShader.hlsl", "PSShadow", "ps_5_0");
	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &m_pPSShadow);
		if (FAILED(hr))
		{
			return hr;
		}
		PSBlob->Release();
	}*/

	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		m_FbxObjA.m_bAnimPlay = !m_FbxObjA.m_bAnimPlay;
	}
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxObjB.m_bAnimPlay = !m_FbxObjB.m_bAnimPlay;
	}
	//m_FbxObjA.Frame();
	m_FbxObjB.Frame();


	if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
	{
		m_vMovePos.z += g_fSecPerFrame * 100.0f;
	}
	
	if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
	{
		m_vMovePos.z -= g_fSecPerFrame * 100.0f;
	}
	if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
	{
		D3DXMatrixRotationY(&m_FbxObjB.m_matWorld, g_fGameTimer);
	}
	m_FbxObjB.m_matWorld._41 = m_vMovePos.x;
	m_FbxObjB.m_matWorld._42 = m_vMovePos.y;
	m_FbxObjB.m_matWorld._43 = m_vMovePos.z;
	
	
	return true;
}
bool		Sample::Render()
{
	/*m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjA.SetPixelShader(nullptr);
	m_FbxObjA.Render(m_pImmediateContext);
	
	m_FbxObjA.SetMatrix(&m_MatShadow, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjA.SetPixelShader(m_pPSShadow);
	m_FbxObjA.Render(m_pImmediateContext);*/
	

	m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjB.SetPixelShader(nullptr);
	m_FbxObjB.Render(m_pImmediateContext);
	
	/*m_FbxObjB.SetMatrix(&m_MatShadow, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjB.SetPixelShader(m_pPSShadow);
	m_FbxObjB.Render(m_pImmediateContext);*/
	return true;
}
bool		Sample::Release()
{
	m_ViewRt.Release();
	//m_FbxObjA.Release();
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