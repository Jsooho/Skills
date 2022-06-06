#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)

bool Sample::Init()
{

    Texture* MapTexture = I_Texture.Load(L"../../Data/object/map/020.bmp");
    m_Map.m_Texture = MapTexture;
    /*m_Map.SetContext(m_pImmediateContext);
    m_Map.CreateHeightMap(L"../../Data/object/map/heightmap.bmp");
    */
    HMapInfo info{ 8 + 1,8 + 1,0,0,0,0,1.0f };
    m_Map.CreateMap(info);
    m_Map.SetPosition(TVector3(0.0f, 1.0f, 0.0f));
    m_Map.Create(m_pImmediateContext, L"", L"", L"Map.hlsl");

    Texture* BoxTexture = I_Texture.Load(L"../../Data/object/metal.bmp");
    m_Box.m_Texture = BoxTexture;
    D3DXMatrixScaling(&m_Box.m_matWorld, 50.0f, 50.0f, 50.0f);
    m_Box.SetPosition(TVector3(0.0f, 300.0f, 0.0f));
    m_Box.Create(m_pImmediateContext, L"", L"", L"Box.hlsl");

    m_Skybox.Init();
    m_Skybox.Create(m_pImmediateContext, L"../../Data/object/Sky/xxx.bmp", L"", L"SkyShader.hlsl");

    m_Camera.Init();
    m_Camera.m_pSpeed = 5.0f;
    m_Camera.CreateViewMatrix(TVector3(0, 500.0f, -5.0f), TVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 7000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    return true;
}

bool Sample::Frame()
{

    TVector3 vLight(500.0f, 500.0f, 400.0f);
    vLight.Normalize();
    vLight = vLight * -1.0f;
    m_Box.m_CbLight.vLightDir.x = vLight.x;
    m_Box.m_CbLight.vLightDir.y = vLight.y;
    m_Box.m_CbLight.vLightDir.z = vLight.z;

    m_Map.m_CbLight.vLightDir.x = vLight.x;
    m_Map.m_CbLight.vLightDir.y = vLight.y;
    m_Map.m_CbLight.vLightDir.z = vLight.z;

    return true;
}

bool Sample::Render()
{
    m_Skybox.m_matViewSky = m_Camera.m_matView;
    m_Skybox.m_matViewSky._41 = 0;
    m_Skybox.m_matViewSky._42 = 0;
    m_Skybox.m_matViewSky._43 = 0;
    TMatrix Scale, Rotation;
    TMatrix RetScale;
    Rotation.CreateRotationY(g_fGameTimer * 0.00f);
    D3DXMatrixScaling(&RetScale, 3000.0f, 3000.0f, 3000.0f);
    m_Skybox.m_matWorld = RetScale * Rotation;
    m_Skybox.SetMatrix(&m_Skybox.m_matWorld, &m_Skybox.m_matViewSky, &m_Camera.m_matProjection);
    m_Skybox.Render(m_pImmediateContext);

    if (!m_bWireFrame)
    {
        ApplyRS(m_pImmediateContext, DxState::g_pRSWireFrame);
    }
    else
    {
        ApplyRS(m_pImmediateContext, DxState::g_pRSSolid);
    }

    m_Map.SetMatrix(&m_Map.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_Map.m_pLightConstantBuffer);
    m_pImmediateContext->UpdateSubresource(m_Map.m_pLightConstantBuffer, 0, NULL, &m_Map.m_CbLight, 0, 0);
    m_pImmediateContext->PSSetSamplers(0, 1, &DxState::g_pWrapSS);
    m_pImmediateContext->OMSetDepthStencilState(DxState::g_pLessEqualDSS, 0x01);
    m_Map.Render(m_pImmediateContext);

    m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_Box.m_pLightConstantBuffer);
    m_pImmediateContext->UpdateSubresource(m_Box.m_pLightConstantBuffer, 0, NULL, &m_Box.m_CbLight, 0, 0);
    m_pImmediateContext->PSSetSamplers(0, 1, &DxState::g_pWrapSS);
    m_pImmediateContext->OMSetDepthStencilState(DxState::g_pLessEqualDSS, 0x01);
    m_Box.Render(m_pImmediateContext);

    
    return true;
}

bool Sample::Release()
{
    return true;
}
