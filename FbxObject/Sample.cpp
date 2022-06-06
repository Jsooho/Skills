#include "Sample.h"
GAMECORE(FbxObject,1024,768)

bool Sample::Init()
{
	m_FbxObject.LoadObject("../../Data/box.FBX");
	
	m_Texture.LoadTexture(L"../../data/aaa.JPG");
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	m_FbxObject.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_Texture.m_pSampler);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Texture.m_pTextureSRV);
	m_FbxObject.Render(m_pImmediateContext);
	return true;
}

bool Sample::Release()
{
	m_FbxObject.Release();
	m_Texture.Release();
	return true;
}