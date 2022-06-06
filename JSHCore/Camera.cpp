#include "Camera.h"
#include "Input.h"
bool Camera::Init()
{
    Frustum::Init();
    CreateViewMatrix(m_vCameraPos, m_vCameraTarget);
    CreateProjMatrix(0.1f, 3000.0f, (float)g_rtClient.right / (float)g_rtClient.bottom, XM_PI * 0.25f);
    UpdateVector();
    return true;
}
bool Camera::UpdateVector()
{
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

    CreateFrustum(m_matView, m_matProjection);
    return true;
}

bool Camera::UpDate(TVector4 vDirValue)
{
    m_fYaw += vDirValue.y;
    m_fPitch += vDirValue.x;
    m_fRoll += vDirValue.z;
    
    TQuaternion q;
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);

    m_vCameraPos += m_vLook * vDirValue.w;
    m_fRadius += vDirValue.w;

    TMatrix matRotation;
    D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DXMatrixInverse(&m_matView, NULL, &matRotation);

    return UpdateVector();

}
void Camera::CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    m_vUp = vUp;
    D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vCameraTarget, &vUp);

    UpdateVector();

    /*m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;*/
   
}

TMatrix Camera::CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect)
{
    D3DXMatrixPerspectiveFovLH(&m_matProjection, fFov, fAspect, fNear, fFar);
    return m_matProjection;
}
TMatrix DebugCamera::DebugUpdate(TVector4 vValue)
{
    m_fYaw += vValue.y;
    m_fPitch += vValue.x;
    m_fRoll += vValue.z;
    m_fRadius += vValue.w;
    TQuaternion q;
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);
    TMatrix matRotation;
    D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DXMatrixInverse(&m_matView, NULL, &matRotation);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

    return matRotation;

}

bool Camera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('R') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_pSpeed * g_fSecPerFrame;
    }

    CreateViewMatrix(m_vCameraPos, m_vCameraTarget);

    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

    return true;
}

//bool Camera::Render()
//{
//    return true;
//}

bool Camera::Release()
{
    return true;
}

Camera::Camera()
{
    m_pSpeed = 0.005f;
    m_vCameraPos = { 0.0f, 0.0f, 0.0f };
    m_vCameraTarget = { 0.0f, 0.0f, 100.0f };
    m_vUp = { 0.0f,1.0f,0.0f };
}

Camera::~Camera()
{
}

bool DebugCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
       m_vCameraPos = m_vCameraPos + m_vLook * m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
       m_vCameraPos = m_vCameraPos + m_vSide * m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey(VK_SPACE) >= KEY_HOLD)
    {
        m_pSpeed += g_fSecPerFrame * 200.0f;
    }
    else
    {
        m_pSpeed -= g_fSecPerFrame * 100.0f;
        if (m_pSpeed <= 100.0f)m_pSpeed = 100.0f;
    }
    TVector3 vLook;
    vLook.x = m_vLook.x;
    vLook.y = m_vLook.y;
    vLook.z = m_vLook.z;
    TVector3 vTarget;
    vTarget.x = m_vCameraPos.x;
    vTarget.y = m_vCameraPos.y;
    vTarget.z = m_vCameraPos.z;

    TMatrix matRotation, matY, matX;
    if (g_Input.GetKey(VK_RIGHT) >= KEY_PUSH)
    {
        matRotation = TMatrix::CreateRotationY(g_fSecPerFrame);
    }
    if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
    {
        matRotation = TMatrix::CreateRotationY(-g_fSecPerFrame);
    }
    
    return true;
}