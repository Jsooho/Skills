#include "HMap.h"

bool HMap::Frame()
{
    TVector3 vLight(30.0f, 20.0f, 50.0f);
    vLight.Normalize();
    vLight = vLight * -1.0f;
    m_CbLight.vLightDir.x = vLight.x;
    m_CbLight.vLightDir.y = vLight.y;
    m_CbLight.vLightDir.z = vLight.z;
    m_CbLight.vLightDir.w = 1.0f;
    return true;
}

bool HMap::CreateMap(HMapInfo& info)
{
    m_MapInfo = info;
    m_MapInfo.m_iNumCellCol = m_MapInfo.m_iNumCol - 1;
    m_MapInfo.m_iNumCellRow = m_MapInfo.m_iNumRow - 1;
    //전체 정점 = 가로정점 * 세로정점 
    m_MapInfo.m_iNumVertex =  m_MapInfo.m_iNumCol * m_MapInfo.m_iNumRow;
    m_MapInfo.m_iNumFace = m_MapInfo.m_iNumCellCol * m_MapInfo.m_iNumCellRow * 2;
    return true;
}

bool HMap::CreateHeightMap(std::wstring szheightmap)
{
    HRESULT hr = S_OK;
    ID3D11ShaderResourceView* pSRV = nullptr;
    ID3D11Resource* pTexture;
    size_t maxsize = 0;
    if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice, szheightmap.c_str(), maxsize,
        D3D11_USAGE_STAGING,
        0,
        D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
        0,
        WIC_LOADER_DEFAULT,
        &pTexture,
        nullptr)))
    {
        return false;
    }

    ID3D11Texture2D* pTexture2D = NULL;
    if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC desc;
    pTexture2D->GetDesc(&desc);

    m_fHeightList.resize(desc.Height * desc.Width);

    if (pTexture2D)
    {
        D3D11_MAPPED_SUBRESOURCE MappedFaceDesc;

        if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDesc)))
        {
            UCHAR* pTexels = (UCHAR*)MappedFaceDesc.pData;
            PNCT_VERTEX v;
            for (UINT row = 0; row < desc.Height; row++)
            {
                UINT rowStart = row * MappedFaceDesc.RowPitch;
                for (UINT col = 0; col < desc.Width; col++)
                {
                    UINT colStart = col * 4;
                    m_fHeightList[row * desc.Width + col] = (float)pTexels[rowStart + colStart + 0];// uRed
                }
            }
            m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
        }
    }
    m_MapInfo.m_iNumRow = desc.Height;
    m_MapInfo.m_iNumCol = desc.Width;

    if (pTexture2D) pTexture2D->Release();
    HMapInfo info{ m_MapInfo.m_iNumCol, m_MapInfo.m_iNumRow ,0 ,0 ,0, 0, 10.0f };
    CreateMap(info);
    return true;
}



bool HMap::CreateVertexData()
{
    //Row = 열
    //Col = 행

    m_vertexList.resize(m_MapInfo.m_iNumVertex);
    //필드에 중앙을 기준
    float fHalfCol = (m_MapInfo.m_iNumCol - 1) / 2.0f;
    float fHalfRow = (m_MapInfo.m_iNumRow - 1) / 2.0f;
    //
    float fOffSetU = 1.0f / (m_MapInfo.m_iNumCol - 1);
    float fOffSetV = 1.0f / (m_MapInfo.m_iNumRow - 1);

    for (int iRow = 0; iRow < m_MapInfo.m_iNumRow; iRow++)
    {
        for (int iCol = 0; iCol < m_MapInfo.m_iNumCol; iCol++)
        {
            int index = iRow * m_MapInfo.m_iNumRow + iCol;
            m_vertexList[index].pos.x = (iCol - fHalfCol) * m_MapInfo.m_fCellDistence;
            m_vertexList[index].pos.y = m_fHeightList[index];
            m_vertexList[index].pos.z = -((iRow - fHalfRow) * m_MapInfo.m_fCellDistence);
            m_vertexList[index].normal = TVector3(0, 1, 0);
            m_vertexList[index].color = TVector4(randstep(0.0f, 1.0f),
                randstep(0.0f, 1.0f),
                randstep(0.0f, 1.0f), 1.0f);
            m_vertexList[index].tex = TVector2(fOffSetU * iCol, fOffSetV * iRow);
        }
    }

    return true;
}

bool HMap::CreateIndexData()
{
    m_indexList.resize(m_MapInfo.m_iNumFace * 3);
    int iIndex = 0;
    TVector3 vLight(100, 100, 0);
    vLight.Normalize();
    vLight = vLight * 1.0f;
   
    for (int iRow = 0; iRow < m_MapInfo.m_iNumCellRow; iRow++)
    {
        for (int iCol = 0; iCol < m_MapInfo.m_iNumCellCol; iCol++)
        {
            int iNextRow = iRow + 1;
            int iNextCol = iCol + 1;
            face f;
            m_indexList[iIndex + 0] = iRow * m_MapInfo.m_iNumCol + iCol;
            m_indexList[iIndex + 1] = iRow * m_MapInfo.m_iNumCol + iNextCol;
            m_indexList[iIndex + 2] = iNextRow * m_MapInfo.m_iNumCol + iCol;

            f.v0 = m_indexList[iIndex + 0];
            f.v1 = m_indexList[iIndex + 1];
            f.v2 = m_indexList[iIndex + 2];
            
            TVector3 vE0 = (m_vertexList[f.v1].pos - m_vertexList[f.v0].pos);
            vE0.Normalize();
            TVector3 vE1 = (m_vertexList[f.v2].pos - m_vertexList[f.v0].pos);
            vE1.Normalize();
            D3DXVec3Cross(&f.vNormal, &vE0, &vE1);
            f.vNormal.Normalize();

            m_vertexList[f.v0].normal += f.vNormal;
            m_vertexList[f.v1].normal += f.vNormal;
            m_vertexList[f.v2].normal += f.vNormal;
            
            float fLightDot = D3DXVec3Dot(&vLight, &f.vNormal);
            float fDot = max(0.0f, fLightDot);

            m_vertexList[f.v0].color = TVector4(fDot, fDot, fDot, 1);
            m_vertexList[f.v1].color = TVector4(fDot, fDot, fDot, 1);
            m_vertexList[f.v2].color = TVector4(fDot, fDot, fDot, 1);

            m_FaceList.push_back(f);

            m_indexList[iIndex + 3] = m_indexList[iIndex + 2];
            m_indexList[iIndex + 4] = m_indexList[iIndex + 1];
            m_indexList[iIndex + 5] = iNextRow * m_MapInfo.m_iNumCol + iNextCol;

            f.v0 = m_indexList[iIndex + 3];
            f.v1  = m_indexList[iIndex + 4];
            f.v2  = m_indexList[iIndex + 5];

            vE0 = (m_vertexList[f.v1].pos - m_vertexList[f.v0].pos);
            vE0.Normalize();
            vE1 = (m_vertexList[f.v2].pos - m_vertexList[f.v0].pos);
            vE1.Normalize();
            D3DXVec3Cross(&f.vNormal, &vE0, &vE1);
            f.vNormal.Normalize();

            m_vertexList[f.v0].normal += f.vNormal;
            m_vertexList[f.v1].normal += f.vNormal;
            m_vertexList[f.v2].normal += f.vNormal;

            fLightDot = D3DXVec3Dot(&vLight, &f.vNormal);
            fDot = max(0.0f, fLightDot);

            m_vertexList[f.v0].color = TVector4(fDot, fDot, fDot, 1);
            m_vertexList[f.v1].color = TVector4(fDot, fDot, fDot, 1);
            m_vertexList[f.v2].color = TVector4(fDot, fDot, fDot, 1);

            m_FaceList.push_back(f);

            iIndex += 6;
        }
    }
    for (int iRow = 0; iRow < m_MapInfo.m_iNumRow; iRow++)
    {
        for (int iCol = 0; iCol < m_MapInfo.m_iNumCol; iCol++)
        {
            m_vertexList[iRow * m_MapInfo.m_iNumCol + iCol].normal.Normalize();
            float fLV= D3DXVec3Dot(&vLight, &m_vertexList[iRow * m_MapInfo.m_iNumCol + iCol].normal);
            float fDot = max(0.0f, fLV);
            m_vertexList[iRow * m_MapInfo.m_iNumCol + iCol].color = TVector4(fDot, fDot, fDot, 1);
        }
    }
    return true;
}
