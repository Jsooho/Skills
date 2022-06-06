#include "Shape.h"
bool PlaneShape::CreateVertexData()
{
    m_vertexList.resize(4);

    m_vertexList[0].pos = TVector3(-1.0f, 1.0f, 0.0f);
    m_vertexList[0].normal = TVector3(0.0f, 1.0f, 0.0f);
    m_vertexList[0].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[0].tex = TVector2(0.0f, 0.0f);

    m_vertexList[1].pos = TVector3(1.0f, 1.0f, 0.0f);
    m_vertexList[1].normal = TVector3(0.0f, 1.0f, 0.0f);
    m_vertexList[1].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[1].tex = TVector2(1.0f, 0.0f);


    m_vertexList[2].pos = TVector3(-1.0f, -1.0f, 0.0f);
    m_vertexList[2].normal = TVector3(0.0f, 1.0f, 0.0f);
    m_vertexList[2].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[2].tex = TVector2(0.0f, 1.0f);


    m_vertexList[3].pos = TVector3(1.0f, -1.0f, 0.0f);
    m_vertexList[3].normal = TVector3(0.0f, 1.0f, 0.0f);
    m_vertexList[3].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexList[3].tex = TVector2(1.0f, 1.0f);
    return true;
}
bool PlaneShape::CreateIndexData()
{
    m_indexList.push_back(0);  m_indexList.push_back(1);  m_indexList.push_back(2);
    m_indexList.push_back(2);  m_indexList.push_back(1);  m_indexList.push_back(3);
    return true;
}

bool PlaneShape::Render(ID3D11DeviceContext* pContext)
{
    PreRender(pContext);
    PostRender(pContext);
    pContext->IASetPrimitiveTopology(m_PlanePrimitive);
    return true;
}

bool BoxShape::CreateVertexData()
{
    m_vertexList.resize(24);
    //뒷면(n z+) 456 657
    m_vertexList[0] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[1] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[2] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[3] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));

    //정면(n z-) 012 213
    m_vertexList[4] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[5] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[6] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[7] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
    
  
    //오른쪽(n x+) 8910 10911
    m_vertexList[8] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[9] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[10] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[11] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));

    //왼쪽(n x-) 12 13 14 14 13 15
    m_vertexList[12] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[13] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[14] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[15] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));

    //윗면(n y+) 16 17 18 18 17 20
    m_vertexList[16] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[17] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[18] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[19] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));

    //아랫면(n y-)
    m_vertexList[20] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[21] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[22] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[23] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
    
  

    return true;
}



bool BoxShape::CreateIndexData()
{
    m_indexList.push_back(0);  m_indexList.push_back(1);  m_indexList.push_back(2);
    m_indexList.push_back(2);  m_indexList.push_back(1);  m_indexList.push_back(3);

    m_indexList.push_back(4);  m_indexList.push_back(5);  m_indexList.push_back(6);
    m_indexList.push_back(6);  m_indexList.push_back(5);  m_indexList.push_back(7);

    m_indexList.push_back(8);  m_indexList.push_back(9);  m_indexList.push_back(10);
    m_indexList.push_back(10);  m_indexList.push_back(9);  m_indexList.push_back(11);

    m_indexList.push_back(12);  m_indexList.push_back(13);  m_indexList.push_back(14);
    m_indexList.push_back(14);  m_indexList.push_back(13);  m_indexList.push_back(15);

    m_indexList.push_back(16);  m_indexList.push_back(17);  m_indexList.push_back(18);
    m_indexList.push_back(18);  m_indexList.push_back(17);  m_indexList.push_back(19);

    m_indexList.push_back(20);  m_indexList.push_back(21);  m_indexList.push_back(22);
    m_indexList.push_back(22);  m_indexList.push_back(21);  m_indexList.push_back(23);

    return true;
}
bool BoxShape::Render(ID3D11DeviceContext* pContext)
{
    PreRender(pContext);
    PostRender(pContext);
    pContext->IASetPrimitiveTopology(m_BoxPrimitive);
    return true;
}
bool BoxShape::PostRender(ID3D11DeviceContext* pContext)
{
    
    if (m_indexList.size() <= 0)
    {
        pContext->Draw(m_vertexList.size(), 0);
    }
    else
    {
        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(m_indexList.size(), 0, 0);
    }

    return true;
}


bool LineShape::CreateVertexBuffer()
{
    m_iVertexSize = sizeof(PS_VECTOR);

    HRESULT hr = S_OK;
    if (m_LineVertexList.size() <= 0) return false;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_iVertexSize * m_LineVertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_LineVertexList.at(0);


    if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer)))
    {
        return false;
    }

    return true;
}

bool LineShape::CreateVertexData()
{
    
    m_LineVertexList.resize(2);

    m_LineVertexList[0].pos = TVector3(0.0f, 0.0f, 0.0f);
    m_LineVertexList[0].Color = TVector4(1.0f, 1.0f, 0.0f, 1.0f);
    m_LineVertexList[1].pos = TVector3(-1.0f, 0.0f, 0.0f);
    m_LineVertexList[1].Color = TVector4(1.0f, 1.0f, 0.0f, 1.0f);


    /*m_LineVertexList[2].pos = TVector3(0.0f, 0.0f, 0.0f);
    m_LineVertexList[2].Color = TVector4(0.0f, 1.0f, 1.0f, 1.0f);
    m_LineVertexList[3].pos = TVector3(0.0f, 1.0f, 0.0f);
    m_LineVertexList[3].Color = TVector4(0.0f, 1.0f, 1.0f, 1.0f);


    m_LineVertexList[4].pos = TVector3(0.0f, 0.0f, 0.0f);
    m_LineVertexList[4].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_LineVertexList[5].pos = TVector3(0.0f, 0.0f, -1.0f);
    m_LineVertexList[5].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);*/
    return true;
}

bool LineShape::CreateIndexBuffer()
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




bool LineShape::CreateIndexData()
{
    m_indexList.push_back(0); m_indexList.push_back(1);
    /*m_indexList.push_back(2); m_indexList.push_back(3);
    m_indexList.push_back(4); m_indexList.push_back(5);*/
    return true;
}

bool LineShape::CreateVertexLayout()
{
    HRESULT hr = S_OK;

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
       
    };

    int Size = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
    hr = g_pd3dDevice->CreateInputLayout(vertexDesc, Size, m_pVs->m_pVSBlob->GetBufferPointer(), m_pVs->m_pVSBlob->GetBufferSize(), &m_pVertexLayout);
    if (FAILED(hr)) return false;
    return true;
}

bool LineShape::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_LineVertexList.size() <= 0) return true;
    pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_CbData, 0, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
  
    //pContext->PSSetShaderResources(1, 1, &m_MaskTexture->m_pSRV);
    pContext->VSSetShader(m_pVs->m_pVertexShader, NULL, 0);
    pContext->PSSetShader(m_pPs->m_pPixelShader, NULL, 0);
    pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(PS_VECTOR);
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    return true;
}

bool LineShape::Render(ID3D11DeviceContext* pContext)
{
    //USAGE_DYNAMIC(GPU 읽기전용, CPU 쓰기전용) CPU에 의해 프레임마다 1회이상 갱신되는 경우에는 동적 리소스를 선택하는 것이 좋다
    /*HRESULT hr;
   
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    if (SUCCEEDED(hr = pContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
    {
        CopyMemory(MappedResource.pData,(void*)&m_LineVertexList, m_LineVertexList.size());
        pContext->Unmap(m_VertexBuffer, 0);
    }*/
    PreRender(pContext);
    
    PostRender(pContext);
    
 
    return true;
}

bool LineShape::DrawLine(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor)
{
    PS_VECTOR vertices[2];
    vertices[0].pos = vStart;
    vertices[0].Color = vColor;
    vertices[1].pos = vEnd;
    vertices[1].Color = vColor;
    pContext->UpdateSubresource(m_VertexBuffer, 0, NULL, &m_LineVertexList, 0, 0);
    return Render(pContext);
    
}

LineShape::LineShape()
{
    m_Primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
  
}
LineShape::~LineShape()
{

}

bool LineShape::PostRender(ID3D11DeviceContext* pContext)
{
    if (m_indexList.size() <= 0)
    {
        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->Draw(m_LineVertexList.size(), 0);
    }
    else
    {
        pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->DrawIndexed(m_indexList.size(), 0, 0);
    }
    return true;
}
