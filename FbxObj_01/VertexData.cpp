#include "FbxObj.h"

bool HFbxObject::CreateIndexData()
{
    return true;
}

bool HFbxObject::CreateConstantBuffer()
{
	
	Object3D::CreateConstantBuffer();

	HRESULT hr = S_OK;

	if (m_vertexList.size() <= 0) return hr;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(AnimMatrix);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_matAnimMatrix;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pAnimCB);
	if (FAILED(hr)) return false;

    return true;
}

bool HFbxObject::CreateVertexData()
{
	if (m_pSubVertexList.size() > 0)
	{
		m_pVBList.resize(m_pSubVertexList.size());
		m_WeightList.resize(m_pSubVertexList.size());
	}
	return true;
}

bool HFbxObject::CreateVertexBuffer()
{

	Object3D::CreateVertexBuffer();

	if (m_pSubVertexList.size() <= 0) return false;
	HRESULT hr;
	for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
	{
		if (m_pSubVertexList[iMtrl].size() <= 0) continue;

		//gpu메모리에 버퍼 할당(원하는 할당 크기)
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSubVertexList[iMtrl].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubVertexList[iMtrl].at(0);

		if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVBList[iMtrl])))
		{
			return false;
		}
	}
	
	
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(IW_VERTEX) * m_WeightList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_WeightList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIWVertrexBuffer);
	if (FAILED(hr)) return false;
	
    return true;
}

bool HFbxObject::CreateVertexLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout), m_pVs->m_pVSBlob->GetBufferPointer(), m_pVs->m_pVSBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr)) return false;

	

    return true;
}