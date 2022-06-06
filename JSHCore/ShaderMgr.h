#pragma once
#include "BaseMgr.h"

class Shader
{
public:
	int					m_iIndex;
	wstring				m_csName;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSBlob = nullptr;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
public:
	virtual bool CreateVertexShader(wstring filename, string entry = "VS");
	virtual bool CreatePixelShader(wstring filename, string entry = "PS");
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(std::wstring filename);
public:
	Shader();
	virtual ~Shader();
};

class ShaderMgr : public BaseMgr<Shader, ShaderMgr>
{
public:
	friend class Singleton<ShaderMgr>;
public:
	Shader* CreateVertexShader(wstring filename, string entry);
	Shader* CreatePixelShader(wstring filename, string entry);
private:
	ShaderMgr();
public:
	virtual ~ShaderMgr();
};
#define I_Shader ShaderMgr::Get()

