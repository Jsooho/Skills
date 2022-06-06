#include "Sample.h"
GAMECORE(FbxObject,1024,768)

bool Sample::Init()
{
	//m_FbxObject.LoadObject("../../Data/object/box.FBX");
	//m_FbxObject.LoadObject("../../Data/object/multiCamera.fbx");
	m_FbxObject.LoadObject("../../Data/object/ship.fbx");
	
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	m_FbxObject.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_FbxObject.Render(m_pImmediateContext);
	return true;
}

bool Sample::Release()
{
	m_FbxObject.Release();
	m_Texture.Release();
	return true;
}

