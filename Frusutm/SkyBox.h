#pragma once
#include"Object3D.h"
#include"DxState.h"
class SkyBox : public Object3D
{
public:
	TMatrix		m_matViewSky;
	Texture*	m_pTexArray[6];
public:
	 virtual bool CreateVertexData();
	 virtual bool CreateIndexData();
	 virtual bool LoadTexture(std::wstring szTextureName, std::wstring szMaskName);
public:
	bool PostRender(ID3D11DeviceContext* pContext) override;
	bool Render(ID3D11DeviceContext* pContext)override;
};

