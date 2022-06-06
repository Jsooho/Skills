#include "ShaderMgr.h"
bool Shader::CreateVertexShader(wstring filename, string entry)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;

	hr = D3DCompileFromFile(filename.c_str(), NULL, NULL, entry.c_str(), "vs_5_0", 0, 0, &m_pVSBlob, &m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = g_pd3dDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Shader::CreatePixelShader(wstring filename, string entry)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;

	hr = D3DCompileFromFile(filename.c_str(), NULL, NULL, entry.c_str(), "ps_5_0", 0, 0, &m_pPSBlob, &m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = g_pd3dDevice->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Shader::Init()
{
	return true;
}

bool Shader::Frame()
{
	return true;
}

bool Shader::Render()
{
	return true;
}

bool Shader::Release()
{
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

	return true;
}

bool Shader::Load(wstring filename)
{
	if (!CreateVertexShader(filename))
	{
		return false;
	}
	if (!CreatePixelShader(filename))
	{
		return false;
	}
	return true;
}

Shader::Shader()
{
}

Shader::~Shader()
{
}

Shader* ShaderMgr::CreateVertexShader(wstring filename, string entry)
{
	wstring name = Splitpath(filename, to_mw(entry));
	Shader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new Shader;
	if (!pData->CreateVertexShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_Datalist.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}

Shader* ShaderMgr::CreatePixelShader(wstring filename, string entry)
{
	wstring name = Splitpath(filename, to_mw(entry));
	Shader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new Shader;
	if (!pData->CreatePixelShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_Datalist.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
ShaderMgr::ShaderMgr()
{
	m_iIndex = 0;
}
ShaderMgr::~ShaderMgr()
{
}
