#include "Sample.h"
GAMECORE(FbxObject,1024,768)

bool Sample::Init()
{
	
	//m_FbxObjectB.LoadObject("../../Data/object/box.FBX");
	//m_FbxObjectB.LoadObject("../../Data/object/multiCamera.fbx");
	//m_FbxObjectA.LoadObject("../../Data/object/BoxAnim.fbx", "../../Data/Shader/CharacterShader.hlsl");
	m_FbxObjectB.LoadObject("../../Data/object/man.fbx", "../../Data/Shader/CharacterShader.hlsl");
	//m_FbxObject.LoadObject("../../Data/object/ship.fbx");
	//m_FbxObjectA.LoadObject("../../Data/object/Turret.fbx", "../../Data/Shader/CharacterShader.hlsl");

	
	

	return true;
}

bool Sample::Frame()
{
	if (g_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		m_FbxObjectA.m_bAnimationPlay = !m_FbxObjectA.m_bAnimationPlay;
	}
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxObjectB.m_bAnimationPlay = !m_FbxObjectB.m_bAnimationPlay;
	}
	//m_FbxObjectA.Frame();
	m_FbxObjectB.Frame();

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
		D3DXMatrixRotationY(&m_FbxObjectB.m_matWorld, g_fGameTimer);
	}
	m_FbxObjectB.m_matWorld._41 = m_vMovePos.x;
	m_FbxObjectB.m_matWorld._42 = m_vMovePos.y;
	m_FbxObjectB.m_matWorld._43 = m_vMovePos.z;
	return true;

}
bool Sample::Render()
{
	//m_FbxObjectA.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
	//m_FbxObjectA.Render(m_pImmediateContext);
	m_FbxObjectB.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObjectB.Render(m_pImmediateContext);
	return true;
}

bool Sample::Release()
{
	
	//m_FbxObjectA.Release();
	m_FbxObjectB.Release();
	return true;
}

Sample::Sample()
{
	
}
Sample::~Sample()
{

}