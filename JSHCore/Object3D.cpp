#include "Object3D.h"
bool Object3D::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	return true;
}
void Object3D::AddPosition(TVector3 vPos)
{
	// 현재위치
	m_vPos += vPos;
	//  10,10f    ~     w(50, 10)
	//
	//   h
	CreateVertexData();
	CreateIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(
			m_VertexBuffer, 0, NULL, &m_vertexList.at(0), 0, 0);
	}
}
/// <summary>
/// m_rtDraw, m_rtCollision 갱신된다.
/// </summary>
/// <param name="vPos"></param>
void Object3D::SetPosition(TVector3 vPos)
{
	m_vPos = vPos;
	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;
}
bool Object3D::CreateVertexData()
{
	return true;
}
bool Object3D::CreateIndexData()
{
	m_indexList.clear();
	m_indexList.push_back(0); m_indexList.push_back(1); m_indexList.push_back(2);
	m_indexList.push_back(2); m_indexList.push_back(1); m_indexList.push_back(3);
	return true;
}
void Object3D::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}
void    Object3D::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
bool	Object3D::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_CbLight.vLightDir = m_vColor;
	m_CbData.vValue = TVector4(
		g_fGameTimer,
		0,
		0,
		1.0f);
	return true;
}
Object3D::Object3D()
{
	m_fAlpha = 1.0f;
	m_vColor = TVector4(1, 1, 1, 1);
	m_vSide.x = 1;
	m_vSide.y = 0;
	m_vSide.z = 0;
	m_vUp.x = 0;
	m_vUp.y = 1;
	m_vUp.z = 0;
	m_vLook.x = 0;
	m_vLook.y = 0;
	m_vLook.z = 1;
}
Object3D::~Object3D()
{

}