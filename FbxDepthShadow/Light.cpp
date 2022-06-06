#include "Light.h"

void Light::Set(TVector3 vPos, TVector3 vTarget)
{
    m_InitPos = vPos;
    m_vPos = vPos;
    m_vTarget = vTarget;
    m_vDir = m_vTarget - m_vPos;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    TVector3 vUp(0, 1, 0);
    D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
    D3DXMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, 1.0f, 1000.0f);
}

bool Light::Frame()
{
    //m_vPos = m_vPos * m_matWorld;
    D3DXMatrixRotationY(&m_matWorld, XM_PI * g_fGameTimer * 0.1f);
    D3DXVec3TransformCoord(&m_vPos, &m_InitPos, &m_matWorld);
    m_vDir = m_vTarget - m_vPos;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    TVector3 vUp(0, 1, 0);
    D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
    D3DXMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, 1.0f, 1000.0f);
    return true;
}
