#include "Sample.h"
GAMECORE(FbxObject,1024,768)

bool Sample::Init()
{
	
	//m_FbxObjectB.LoadObject("../../Data/object/box.FBX");
	m_FbxObjectB.LoadObject("../../Data/object/multiCamera.fbx");
	//m_FbxObjectA.LoadObject("../../Data/object/BoxAnim.fbx");
	//m_FbxObject.LoadObject("../../Data/object/ship.fbx");
	//m_FbxObjectA.LoadObject("../../Data/object/Turret.fbx");
	//m_FbxObjectB.LoadObject("../../Data/object/SM_Tree_Var01.FBX");
	

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

