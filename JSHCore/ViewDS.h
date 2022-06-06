#pragma once
#include"Std.h"
class ViewDS
{
public:
	//FbxDepth Ã³¸®
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DepthStencilState* m_pDsvState;
public:
	HRESULT CreateDepthStencilView(UINT Width, UINT Height);
	ID3D11Texture2D* CreateTexture(UINT Width, UINT Height);
	bool Release();
};

