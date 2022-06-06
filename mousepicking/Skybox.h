#pragma once
#include "Object3D.h"
#include "DxState.h"
class Skybox : public Object3D
{
public:
	TMatrix		m_matViewSky;
	Texture*    m_pTexArray[6];
public:
	 bool CreateVertexData()override;
	 bool CreateIndexData()override;
	 bool LoadTexture(std::wstring szTextureName, std::wstring szMaskName) override;
public:
	bool Render(ID3D11DeviceContext* pContext) override;
	bool PostRender(ID3D11DeviceContext* pContext) override;
};

