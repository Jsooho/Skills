#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)

bool Sample::Init()
{
    //조명  set
    m_Light.Set(TVector3(100.0f, 100.0f, 100.0f), TVector3(0.0f, 0.0f, 0.0f));
    
    //맵 생성
    HMapInfo info{ 512 + 1,512 + 1,0,0,0,0,1.0f };
    m_map.SetPosition(TVector3(0.0f, 0.0f, 0.0f));
    m_map.CreateMap(info);
    m_map.Create(m_pImmediateContext, L"../../Data/object/map/020.bmp", L"", L"Shadow.hlsl");
    
    m_Minimap.Create(m_pImmediateContext, L"../../Data/object/Chrome1.bmp", L"", L"Box.hlsl");
    
    //랜더타겟 생성
    m_VRt.Create(4096, 4096);
 
    // 박스 오브젝트 생성
    Shader* BoxShader = I_Shader.Load(L"Box.hlsl");
    Shader* BoxpsShader = I_Shader.Load(L"Box.hlsl");
    Texture* BoxTexture = I_Texture.Load(L"../../Data/object/metal.bmp");
    m_Box.m_Texture = BoxTexture;
    m_Box.m_pVs = BoxShader;
    m_Box.m_pPs = BoxpsShader;
    
    //텍스처 행렬 = x,y를 0~1 값으로 만들어야한다.
    m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
                         , 0.0f, -0.5f, 0.0f, 0.0f
                         , 0.0f, 0.0f, 1.0f, 0.0f
                         , 0.5f, 0.5f, 0.0f, 1.0f);

    
    m_Box.Create(m_pImmediateContext, L"", L"", L"Box.hlsl");

    m_BoxA.m_Texture = BoxTexture;
    m_BoxA.m_pVs = BoxShader;
    m_BoxA.m_pPs = BoxpsShader;

    

    m_BoxA.Create(m_pImmediateContext, L"", L"", L"Box.hlsl");
   

    
    //카메라 세팅
    m_Camera.Init();
    m_Camera.m_pSpeed = 5.0f;
    m_Camera.CreateViewMatrix(TVector3(5, 10.0f, 5.0f), TVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 10000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);


    return true;
}

bool Sample::Frame()
{

    m_Light.Frame();
    m_cbShadow.g_matShadow1 = m_Light.m_matView * m_Light.m_matProj * m_matTexture;

    return true;
}

bool Sample::Render()
{

    if (m_VRt.Begin(m_pImmediateContext))
    {
        m_Box.SetMatrix(&m_Box.m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
        Shader* PSBox = I_Shader.CreatePixelShader(L"Box.hlsl", "PSShadow");
        m_Box.m_pPs = PSBox;
        m_Box.Render(m_pImmediateContext);

        m_BoxA.SetMatrix(&m_BoxA.m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
        PSBox = I_Shader.CreatePixelShader(L"Box.hlsl", "PSShadow");
        m_BoxA.m_pPs = PSBox;
        m_BoxA.Render(m_pImmediateContext);
        m_VRt.End(m_pImmediateContext);
    }

    

    ApplySS(m_pImmediateContext, DxState::g_pClampSS, 1);
    m_map.m_CbData.matNormal = m_cbShadow.g_matShadow1;
    m_map.SetMatrix(&m_map.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_VRt.m_pTextureSRV);
    m_map.Render(m_pImmediateContext);

    m_Minimap.SetMatrix(nullptr, nullptr, nullptr);
    m_Minimap.PreRender(m_pImmediateContext);
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_VRt.m_pTextureSRV);
    m_Minimap.PostRender(m_pImmediateContext);

    TMatrix S;
    TMatrix T;
    D3DXMatrixScaling(&S, 5.0f, 5.0f, 5.0f);
    D3DXMatrixTranslation(&T, 0, 5, 0);

    m_Box.m_matWorld =  S*T;

    Shader* BoxpsShader = I_Shader.Load(L"Box.hlsl");
    m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_Box.m_pPs = BoxpsShader;
    m_Box.Render(m_pImmediateContext);

    D3DXMatrixTranslation(&T, 30, 5, 0);
    m_BoxA.m_matWorld = S * T;

    BoxpsShader = I_Shader.Load(L"Box.hlsl");
    m_BoxA.SetMatrix(&m_BoxA.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_BoxA.m_pPs = BoxpsShader;
    m_BoxA.Render(m_pImmediateContext);


    

    return true;
}

bool Sample::Release()
{
    return true;
}

bool Minimap::CreateVertexData()
{
    m_vertexList.resize(4);
    
    m_vertexList[0].pos = TVector3(-1.0f, 1.0f, 0.0f);
    m_vertexList[0].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_vertexList[0].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[0].tex = TVector2(0.0f, 0.0f);

    m_vertexList[1].pos = TVector3(-0.5f, 1.0f, 0.0f);
    m_vertexList[1].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_vertexList[1].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[1].tex = TVector2(1.0f, 0.0f);


    m_vertexList[2].pos = TVector3(-1.0f, 0.5f, 0.0f);
    m_vertexList[2].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_vertexList[2].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[2].tex = TVector2(0.0f, 1.0f);


    m_vertexList[3].pos = TVector3(-0.5f, 0.5f, 0.0f);
    m_vertexList[3].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_vertexList[3].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[3].tex = TVector2(1.0f, 1.0f);
    return true;

}