#pragma once
#include "BaseMgr.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
class Texture
{
public:
	int							 m_iIndex;
	wstring						 m_csName;
	ID3D11ShaderResourceView*	 m_pSRV;
	ID3D11Texture2D*			 m_pTexture;
	D3D11_TEXTURE2D_DESC		 m_TextureDesc;
public:
	bool Load(wstring filename);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Texture();
	virtual ~Texture();
};

class TextureMgr : public BaseMgr<Texture, TextureMgr>
{
public:
	friend class Singleton<TextureMgr>;
private:
	TextureMgr();
public:
	virtual ~TextureMgr();
};
#define I_Texture TextureMgr::Get()