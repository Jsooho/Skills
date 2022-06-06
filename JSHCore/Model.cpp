#include "Model.h"


bool Model::Create(ID3D11DeviceContext* pContext, wstring Colorname, wstring maskname, wstring filename)
{
    
    SetContext(pContext);

    if (!Colorname.empty() &&!LoadTexture(Colorname, maskname))
    {
        return false;
    }
    if (!CreateVertexData())
    {
        return false;
    }
    if (!CreateIndexData())
    {
        return false;
    }
    if (!CreateConstantData())
    {
        return false;
    }
    if (!CreateVertexBuffer())
    {
        return false;
    }
    if (!CreateIndexBuffer())
    {
        return false;
    }
    if (!CreateConstantBuffer())
    {
        return false;
    }
    if (!filename.empty() && !CreateVertexShader(filename))
    {
        return false;
    }
    if (!filename.empty() && !CreatePixelShader(filename))
    {
        return false;
    }
    if (!CreateVertexLayout())
    {
        return false;
    }
    HRESULT hr;
    D3D11_BLEND_DESC  blend;
    ZeroMemory(&blend, sizeof(D3D11_BLEND_DESC));
    /*bd.AlphaToCoverageEnable;
    bd.IndependentBlendEnable;*/
    blend.RenderTarget[0].BlendEnable = TRUE;
    blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //// A 연산 저장
    blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = g_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlend);

    blend.RenderTarget[0].BlendEnable = FALSE;
    hr = g_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlendDisable);

    return true;
}
void Model::SetContext(ID3D11DeviceContext* pContext)
{
    m_pContext = pContext;
}
bool Model::LoadTexture(std::wstring szTextureName, std::wstring szMaskName)
{
    m_Texture = I_Texture.Load(szTextureName);

    if (!szMaskName.empty())
    {
        m_MaskTexture = I_Texture.Load(szMaskName);
    }

    m_TextureDesc = m_Texture->m_TextureDesc;
    return true;
}
void Model::SetMatrix(TMatrix* pMatrixWorld, TMatrix* pMatrixView, TMatrix* pProjection)
{
    if (pMatrixWorld != nullptr)
    {
        m_CbData.matrixWorld = pMatrixWorld->Transpose();
    }
    if (pMatrixView != nullptr)
    {
        m_CbData.matrixView = pMatrixView->Transpose();
    }
    if (pProjection != nullptr)
    {
        m_CbData.matrixProjection = pProjection->Transpose();
    }
    m_CbData.matNormal = m_CbData.matNormal.Transpose();
   /* m_CbData.matrixWorld = m_matWorld.Transpose();
    m_CbData.matrixView = m_matView.Transpose();
    m_CbData.matrixProjection = m_matProjection.Transpose();*/
}

bool  Model::LoadObject(std::wstring filename) // 파일 입출력
{
    FILE* fp = nullptr;
    _tfopen_s(&fp, filename.c_str(), _T("rt"));
    if (fp == NULL)
    {
        return false;
    }
    TCHAR buffer[256] = { 0, };
    int iVersion = 0;
    _fgetts(buffer, 256, fp);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, fp);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for (int iLine = 0; iLine < iNumVertex; iLine++)
    {
        PNCT_VERTEX v;
        _fgetts(buffer, 256, fp);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.color.x, &v.color.y, &v.color.z, &v.color.w);
        m_vertexList.push_back(v);
    }
    fclose(fp);
    return true;
}

bool Model::CreateIndexBuffer()
{
    if (m_indexList.size() <= 0)
    {
        return true;
    }
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_indexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_indexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_IndexBuffer);
    if (FAILED(hr))return false;

    m_iNumIndex = m_indexList.size();
    return true;
}
bool Model::CreateConstantBuffer() //constantbuffer: 정점,픽셀쉐이더에서 사용될 상수를 모아놓은 버퍼
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_CbData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
    if (FAILED(hr)) return false;

  
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_CbLight;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pLightConstantBuffer);
    if (FAILED(hr)) return false;

    return true;
}
bool Model::CreateVertexBuffer()
{

    HRESULT hr = S_OK;
    if (m_vertexList.size() <= 0) return false;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_iVertexSize * m_vertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_vertexList.at(0);
   

    if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer)))
    {
        return false;
    }

    return true;
}

bool Model::CreateVertexLayout()
{
    HRESULT hr = S_OK;

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE",0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    int Size = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
    hr = g_pd3dDevice->CreateInputLayout(vertexDesc, Size, m_pVs->m_pVSBlob->GetBufferPointer(), m_pVs->m_pVSBlob->GetBufferSize(), &m_pVertexLayout);
    if (FAILED(hr)) return false;
    return true;

}
bool Model::CreateVertexShader(wstring filename)
{
    m_pVs = I_Shader.CreateVertexShader(filename, "VS");
    return true;
}
bool Model::CreatePixelShader(wstring filename)
{
    m_pPs = I_Shader.CreatePixelShader(filename, "PS");
    return true;
}
ID3D10Blob* Model::LoadShaderBlob(std::wstring vs, std::string function, std::string version)
{
    HRESULT hr = S_OK;
    ID3DBlob* ret = nullptr;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        vs.c_str(),
        nullptr,
        nullptr,
        function.c_str(),
        version.c_str(),
        0,
        0,
        &ret,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return ret;
    }
    return ret;
}


bool Model::CreateVertexData()
{
    if (m_vertexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool Model::CreateIndexData()
{
    if (m_indexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool Model::CreateConstantData()
{
    ZeroMemory(&m_CbData, sizeof(CB_DATA));
    m_CbData.matrixWorld = TMatrix();
    m_CbData.matrixView = TMatrix();
    m_CbData.matrixProjection = TMatrix();

    /*m_CbData.vValue.x = 0.0f;
    m_CbData.vValue.y = 1.0f;
    m_CbData.vValue.z = 0.0f;
    m_CbData.vValue.w = 0.0f;

    ZeroMemory(&m_CbLight, sizeof(LightData));
    m_CbLight.vLightDir.x = 1.0f;
    m_CbLight.vLightDir.y = 1.0f;
    m_CbLight.vLightDir.z = 1.0f;
    m_CbLight.vLightDir.w = 1.0f;*/

    return true;
}
bool Model::Init()
{
    return true;
}
bool Model::Frame()
{
    m_CbData.vValue.z = g_fGameTimer;
    return true;
}

bool Model::Render(ID3D11DeviceContext* pContext)
{
    if (PreRender(pContext) == false)
    {
        return false;
    }
    if (PostRender(pContext) == false)
    {
        return false;
    }
    return true;
}
bool Model::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_vertexList.size() <= 0) return true;
    pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_CbData, 0, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    if (m_Texture != nullptr)
    {
        pContext->PSSetShaderResources(0, 1, &m_Texture->m_pSRV);
    }
    
    //pContext->PSSetShaderResources(1, 1, &m_MaskTexture->m_pSRV);
    pContext->VSSetShader(m_pVs->m_pVertexShader, NULL, 0);
    pContext->PSSetShader(m_pPs->m_pPixelShader, NULL, 0);
    pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(PNCT_VERTEX);
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    return true;
}
bool Model::PostRender(ID3D11DeviceContext* pContext)
{

    if (m_indexList.size() > 0)
    {
        pContext->DrawIndexed(m_indexList.size(), 0, 0);
    }
    else
    {
        pContext->Draw(m_vertexList.size(), 0);
    }
    return true;
}
bool Model::Release()
{
    SAFE_RELEASE(m_VertexBuffer);
    SAFE_RELEASE(m_pVertexLayout);
    SAFE_RELEASE(m_IndexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);
    SAFE_RELEASE(m_pVs);
    SAFE_RELEASE(m_pPs);
    return true;
}
Model::Model()
{
    SAFE_ZERO(m_VertexBuffer);
    SAFE_ZERO(m_IndexBuffer);
    SAFE_ZERO(m_pVertexLayout);
    SAFE_ZERO(m_pConstantBuffer);
    SAFE_ZERO(m_pVs);
    SAFE_ZERO(m_pPs);
    m_iVertexSize = sizeof(PNCT_VERTEX);
    m_iNumIndex = 0;

}
Model::~Model()
{

}