#include "SkyBox.h"

bool SkyBox::CreateVertexData()
{
    m_vertexList.resize(24);
    
    //Á¤¸é
    m_vertexList[0] = PNCT_VERTEX(TVector3(-1.0f,1.0f,-1.0f), TVector3(0.0f,0.0f,1.0f), TVector4(0.0f,0.0f,0.0f,1.0f), TVector2(0.0f,0.0f));
    m_vertexList[0] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[1] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[2] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[3] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    // µÞ¸é
    m_vertexList[4] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[5] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[6] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[7] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

    // ¿À¸¥ÂÊ
    m_vertexList[8] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[9] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[10] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[11] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

    // ¿ÞÂÊ
    m_vertexList[12] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[13] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[14] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[15] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

    // À­¸é
    m_vertexList[16] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[17] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[18] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[19] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

    // ¾Æ·§¸é
    m_vertexList[20] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[21] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[22] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
    m_vertexList[23] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));


    return true;
}

bool SkyBox::CreateIndexData()
{
    //Á¤¸é
    m_indexList.push_back(0); m_indexList.push_back(1); m_indexList.push_back(2);
    m_indexList.push_back(0); m_indexList.push_back(2); m_indexList.push_back(3);

    //µÚ
    m_indexList.push_back(4); m_indexList.push_back(5); m_indexList.push_back(6);
    m_indexList.push_back(4); m_indexList.push_back(6); m_indexList.push_back(7);

    //¿À¸¥ÂÊ
    m_indexList.push_back(8); m_indexList.push_back(9); m_indexList.push_back(10);
    m_indexList.push_back(8); m_indexList.push_back(10); m_indexList.push_back(11);

    //¿ÞÂÊ
    m_indexList.push_back(12); m_indexList.push_back(13); m_indexList.push_back(14);
    m_indexList.push_back(12); m_indexList.push_back(14); m_indexList.push_back(15);

    //À§ÂÊ
    m_indexList.push_back(16); m_indexList.push_back(17); m_indexList.push_back(18);
    m_indexList.push_back(16); m_indexList.push_back(18); m_indexList.push_back(19);
    
    //¹Ù´Ú
    m_indexList.push_back(20); m_indexList.push_back(21); m_indexList.push_back(22);
    m_indexList.push_back(20); m_indexList.push_back(22); m_indexList.push_back(23);
       
    return true;
}

bool SkyBox::LoadTexture(std::wstring szTextureName, std::wstring szMaskName)
{
    m_pTexArray[0] = I_Texture.Load(L"../../Data/object/Sky/front.bmp");
    m_pTexArray[1] = I_Texture.Load(L"../../Data/object/Sky/back.bmp");
    m_pTexArray[2] = I_Texture.Load(L"../../Data/object/Sky/right.bmp");
    m_pTexArray[3] = I_Texture.Load(L"../../Data/object/Sky/left.bmp");
    m_pTexArray[4] = I_Texture.Load(L"../../Data/object/Sky/up.bmp");
    m_pTexArray[5] = I_Texture.Load(L"../../Data/object/Sky/down.bmp");

    m_TextureDesc = m_pTexArray[0]->m_TextureDesc;
    return true;
}

bool SkyBox::PostRender(ID3D11DeviceContext* pContext)
{
    for (int iTex = 0; iTex < 6; iTex++)
    {
        m_pContext->PSSetShaderResources(0, 1, &m_pTexArray[iTex]->m_pSRV);
        m_pContext->DrawIndexed(6, 6 * iTex, 0);
    }
    return true;
}

bool SkyBox::Render(ID3D11DeviceContext* pContext)
{
    PreRender(pContext);
    pContext->RSSetState(DxState::g_pRSNoneCullSolid);
    pContext->PSSetSamplers(0, 1, &DxState::g_pPointSS);
    PostRender(pContext);
    return true;
}
