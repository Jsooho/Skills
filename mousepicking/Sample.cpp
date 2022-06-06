#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)


bool Sample::Init()
{
    //바운딩박스
    m_Box[0].vMax = TVector3(-10.0f, 5.0f, 5.0f);
    m_Box[0].vMin = TVector3(-20.0f, -5.0f, -5.0f);
    m_Box[0].vCenter = (m_Box[0].vMax + m_Box[0].vMin);
    m_Box[0].vCenter.x /= 2.0f;
    m_Box[0].vCenter.y /= 2.0f;
    m_Box[0].vCenter.z /= 2.0f;

    m_Box[1].vMax = TVector3(5.0f, 5.0f, 5.0f);
    m_Box[1].vMin = TVector3(-5.0f, -5.0f, -5.0f);
    m_Box[1].vCenter = (m_Box[1].vMax + m_Box[1].vMin);
    m_Box[1].vCenter.x /= 2.0f;
    m_Box[1].vCenter.y /= 2.0f;
    m_Box[1].vCenter.z /= 2.0f;

    m_Box[2].vMax = TVector3(20.0f, 5.0f, 5.0f);
    m_Box[2].vMin = TVector3(10.0f, -5.0f, -5.0f);
    m_Box[2].vCenter = (m_Box[2].vMax + m_Box[2].vMin);
    m_Box[2].vCenter.x /= 2.0f;
    m_Box[2].vCenter.y /= 2.0f;
    m_Box[2].vCenter.z /= 2.0f;

    D3DXMatrixIdentity(&m_matWorld);
    for (int iBox = 0; iBox < 3; iBox++)
    {
        
        if (FAILED(m_BoxShape[iBox].Create(m_pImmediateContext, L"../../Data/object/checker_with_numbers.bmp",L"", L"Box.hlsl")))
        {
            MessageBox(0, _T("m_BoxShape 실패"), _T("Fatal error"), MB_OK);
            return false;
        }
    }
    /*TMatrix s, r;
    Shader* vs = I_Shader.CreateVertexShader(L"Box.hlsl","VS");
    Shader* ps = I_Shader.CreatePixelShader(L"Box.hlsl", "PS");
    D3DXMatrixScaling(&s, 10.0f, 10.0f, 10.0f);
    m_Box.m_matWorld = s;
    m_Box.SetPosition(TVector3(0.0f, 20.0f, 0.0f));
    m_Box.m_pVs = vs;
    m_Box.m_pPs = ps;
    m_Box.Create(m_pImmediateContext, L"../../Data/object/metal.bmp", L"", L"");*/

    TMatrix s, r;
    D3DXMatrixScaling(&s, 300.0f, 300.0f, 0.0f);
    D3DXMatrixRotationX(&r, TBASIS_PI /2);
    m_Plane.m_matWorld = s * r;
    m_Plane.SetPosition(TVector3(0.0f, -20.0f, 0.0f));
    m_Plane.Create(m_pImmediateContext, L"../../Data/object/wal156S.bmp", L"", L"Box.hlsl");
    
    
    m_Camera.CreateViewMatrix(TVector3(0.0f, 0.0f, -50.0f), TVector3(0.0f, 0.0f, 0.0f));
    m_Camera.CreateProjMatrix(1.0f, 3000.0f, TBASIS_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);
    m_Camera.m_pSpeed = 5.0f;
   
    return true;
}

bool Sample::Frame()
{
    m_Select.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
    if (g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
    {
        for (int iNum = 0; iNum < 3; iNum++)
        {
            float fT;
            //aabb박스 Ray 교차 충돌 체크
            if (m_Select.AABBtoRay(&m_Box[iNum]))
            {
                TCHAR szBuffer[256];
                _stprintf_s(szBuffer, _T(" %d번 박스와 직선의 충돌, 교점=(%10.4f, %10.4f, %10.4f)"),
                    iNum, m_Select.m_vIntersection.x, m_Select.m_vIntersection.y, m_Select.m_vIntersection.z);
                MessageBox(0, szBuffer, _T("충돌"), MB_OK);
            }

        }
    }
    return true;
}

bool Sample::Render()
{

    m_Plane.SetMatrix(&m_Plane.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_Plane.Render(m_pImmediateContext);

    /*m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProjection);
    m_Box.Render(m_pImmediateContext);*/
    TMatrix matWorld, matScale;
    for (int iObj = 0; iObj < 3; iObj++)
    {
        D3DXMatrixIdentity(&matWorld);
        D3DXMatrixIdentity(&matScale);
        TVector3 vCenter = (m_Box[iObj].vMax + m_Box[iObj].vMin);
        vCenter.x /= 2.0f;
        vCenter.y /= 2.0f;
        vCenter.z /= 2.0f;
        D3DXMatrixScaling(&matScale, m_Box[iObj].vMax.x - vCenter.x,
            m_Box[iObj].vMax.y - vCenter.y,
            m_Box[iObj].vMax.z - vCenter.z);
        matScale._41 = m_Box[iObj].vCenter.x;
        matScale._42 = m_Box[iObj].vCenter.y;
        matScale._43 = m_Box[iObj].vCenter.z;
        m_BoxShape[iObj].SetMatrix(&matScale, &m_Camera.m_matView, &m_Camera.m_matProjection);
        m_BoxShape[iObj].Render(m_pImmediateContext);
    }
    return true;

    return true;
}

bool Sample::Release()
{

    return true;
}


