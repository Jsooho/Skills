#include "TextureMgr.h"

bool Texture::Load(wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		g_pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			g_pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pSRV);
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TextureDesc);
	return true;
}

bool Texture::Init()
{
	return true;
}

bool Texture::Frame()
{
	return true;
}

bool Texture::Render()
{
	return true;
}

bool Texture::Release()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	
	
	return true;
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
}
