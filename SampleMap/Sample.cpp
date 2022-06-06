#include "Sample.h"

GAMECORE(sample,1024,768)

LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool Sample::Init()
{

    Texture* maptex = I_Texture.Load(L"../../Data/object/map/020.bmp");
    Shader* pVs = I_Shader.Load(L"MapShader.hlsl");
    Shader* pPs = I_Shader.Load(L"MapShader.hlsl");

    m_Map.Init();
    m_Map.m_Texture = maptex;
    m_Map.m_pVs = pVs;
    m_Map.m_pPs = pPs;

    m_Map.SetContext(m_pImmediateContext);
    m_Map.CreateHeightMap(L"../../Data/object/map/129.jpg");
	m_Map.Create(m_pImmediateContext, L"",L"",L"");
    
    m_SkyBox.Init();
    m_SkyBox.SetPosition(TVector3{ 0.0f,0.0f,0.0f });
    m_SkyBox.Create(m_pImmediateContext, L"../../Data/object/Sky/xxx.bmp", L"", L"SkyShader.hlsl");

    m_Camera.Init();
    m_Camera.m_pSpeed = 5.0f;
    m_Camera.CreateViewMatrix(TVector3(0, 100, 0), TVector3(16, 3, 0));
    m_Camera.CreateProjMatrix(1.0f, 5000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    return true;
}

bool Sample::Frame()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    float fYaw = g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    float fPitch = g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.UpDate(TVector4(fPitch, fYaw, 0.0f, 0.0f));
    m_Camera.Frame();
    m_Map.Frame();
    
    return true;
}

bool Sample::Render()
{
    m_SkyBox.m_matViewSky = m_Camera.m_matView;
    m_SkyBox.m_matViewSky._41 = 0;
    m_SkyBox.m_matViewSky._42 = 0;
    m_SkyBox.m_matViewSky._43 = 0;
    TMatrix Scale, Rotation;
    TMatrix RetScale;
    Rotation.CreateRotationY(g_fGameTimer * 0.00f);
    D3DXMatrixScaling(&RetScale, 3000.0f, 3000.0f, 3000.0f);
    m_SkyBox.m_matWorld = RetScale * Rotation;
    m_SkyBox.SetMatrix(&m_SkyBox.m_matWorld, &m_SkyBox.m_matViewSky, &m_Camera.m_matProjection);

    ApplyDSS(m_pImmediateContext, DxState::g_pDSSDepthDisable);
    m_pImmediateContext->RSSetState(DxState::g_pRSNoneCullSolid);
    ApplySS(m_pImmediateContext, DxState::g_pPointSS, 0);
    m_SkyBox.Render(m_pImmediateContext);

    m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_Map.m_pLightConstantBuffer);
    m_pImmediateContext->UpdateSubresource(m_Map.m_pLightConstantBuffer, 0, NULL, &m_Map.m_CbLight, 0, 0);
    ApplySS(m_pImmediateContext, DxState::g_pWrapSS, 0);
    ApplyDSS(m_pImmediateContext, DxState::g_pLessEqualDSS);
    m_Map.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_Map.Render(m_pImmediateContext);
    
    return true;
}

bool Sample::Release()
{
	m_Map.Release();
    m_Camera.Release();
    m_SkyBox.Release();
   
    return true;
}
