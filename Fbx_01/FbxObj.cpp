#include "FbxObj.h"

TMatrix FbxLoader::ConvertMatrix(FbxMatrix& m)
{
    TMatrix mat;
    float* pMatArray = reinterpret_cast<float*>(&mat);
    double* pSrcArray = reinterpret_cast<double*>(&m);
    for (int i = 0; i < 16; i++)
    {
        pMatArray[i] = pSrcArray[i];
    }
    return mat;
}
TMatrix FbxLoader::ConvertAMatrix(FbxAMatrix& m)
{
    TMatrix mat;
    float* pMatArray = reinterpret_cast<float*>(&mat);
    double* pSrcArray = reinterpret_cast<double*>(&m);
    for (int i = 0; i < 16; i++)
    {
        pMatArray[i] = pSrcArray[i];
    }
    return mat;
}
TMatrix FbxLoader::DxConvertMatrix(TMatrix m)
{
    TMatrix mat;
    mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
    mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
    mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
    mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
    mat._14 = mat._24 = mat._34 = 0.0f;
    mat._44 = 1.0f;
    return mat;
}


bool FbxLoader::FbxLoad(string filename)
{
    bool bRet = m_pFbxImporter->Initialize(filename.c_str());
    bRet = m_pFbxImporter->Import(m_pFbxScene);

    if (bRet)
    {
        m_RootNode = m_pFbxScene->GetRootNode();
        PreProcess(m_RootNode, nullptr);
    }
    ParseAnimation();
    ParseAnimationNode();
    for (int iObj = 0; iObj < m_fbxObjList.size(); iObj++)
    {
        ParseMesh(m_fbxObjList[iObj]);
    }
    
    return true;
}

void FbxLoader::PreProcess(FbxNode* pNode, HFbxObject* pParentobj)
{
    // camera, light, mesh, shape, animation
    HFbxObject* fbx = nullptr;
    if (pNode != nullptr)
    {
        fbx = new HFbxObject;
        fbx->m_pParentNode = pNode->GetParent();
        fbx->m_pfbxNode = pNode;
        fbx->m_csName = to_mw(pNode->GetName());
        fbx->m_pParentObj = pParentobj;
        fbx->m_iIndex = m_NodeList.size();


        FbxVector4  scaleLcl = pNode->LclScaling.Get();
        FbxVector4  rotateLcl = pNode->LclRotation.Get();
        FbxVector4  transLcl = pNode->LclTranslation.Get();
        FbxMatrix matLocal(transLcl, rotateLcl, scaleLcl);
        fbx->m_matWorld = DxConvertMatrix(ConvertMatrix(matLocal));

        m_NodeList.push_back(fbx);
    }
    FbxMesh* pMesh = pNode->GetMesh();
    if (pMesh)
    {
        m_fbxObjList.push_back(fbx);
    }

    int iNumChildNode = pNode->GetChildCount();
    for (int iNode = 0; iNode < iNumChildNode; iNode++)
    {
        FbxNode* ChildNode = pNode->GetChild(iNode);
        PreProcess(ChildNode, fbx);
    }
}

void FbxLoader::ParseMesh(HFbxObject* pObj)
{
    FbxMesh* pMesh = pObj->m_pfbxNode->GetMesh();

    if (pMesh)
    {
        
        FbxAMatrix matGeom;
        FbxVector4 rot = pObj->m_pfbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
        FbxVector4 trans = pObj->m_pfbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
        FbxVector4 scale = pObj->m_pfbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
        matGeom.SetT(trans);
        matGeom.SetR(rot);
        matGeom.SetS(scale);
       
        FbxAMatrix Normalmat = matGeom;
        Normalmat = Normalmat.Inverse();
        Normalmat = Normalmat.Transpose();
        
        

        vector<FbxLayerElementUV*> VertexUVSet;
        vector<FbxLayerElementMaterial*> MaterialSet;
        vector<FbxLayerElementVertexColor*> ColorSet;

        int iNumLayerCount = pMesh->GetLayerCount();

        for (int iLayer = 0; iLayer < iNumLayerCount; iLayer++)
        {
            FbxLayer* pLayer = pMesh->GetLayer(iLayer);
            if (pLayer->GetUVs() != nullptr)
            {
                VertexUVSet.push_back(pLayer->GetUVs());
            }
            if (pLayer->GetVertexColors() != nullptr)
            {
                ColorSet.push_back(pLayer->GetVertexColors());
            }
            if (pLayer->GetMaterials() != nullptr)
            {
                MaterialSet.push_back(pLayer->GetMaterials());
            }
        }

        //1개의 오브젝트가 여러장의 텍스처를 사용한다면
        //각각의 텍스처이름을 얻고 저장한다.
        //어떤 페이스(폴리곤)가 어떤 텍스처를 쓰는지 얻어내야한다.
        //같은 텍스처를 사용하는 폴리곤들 끼리 저장
        //메터리얼이 여러장이면  배열로 저장후 사용한다.
        //레이어 갯수가 1개 이상이면 위치빼고는 성분이 전부달라질수있다.
        int iNumMtrl = pObj->m_pfbxNode->GetMaterialCount();
        //iNumMtrl = pMesh->GetElementMaterialCount();

        for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
        {
            FbxSurfaceMaterial* pSurfaceMtrl = pObj->m_pfbxNode->GetMaterial(iMtrl);
            if (pSurfaceMtrl)
            {
                string TextureName = ParseMaterial(pSurfaceMtrl);
                std::wstring szTexFileName = L"../../Data/object/";
                szTexFileName += to_mw(TextureName);
                pObj->m_szTexFileList.push_back(szTexFileName);
                pObj->m_pTextureList.push_back(I_Texture.Load(pObj->m_szTexFileList[iMtrl]));
            }
        }
        if (pObj->m_szTexFileList.size() > 0)
        {
            pObj->m_szTexFileName = pObj->m_szTexFileList[0];
        }
        if (iNumMtrl > 0)
        {
            pObj->m_pSubVertexList.resize(iNumMtrl);
        }
        else
        {
            pObj->m_pSubVertexList.resize(1);
        }

        int iNumPolygonCount = pMesh->GetPolygonCount();
        int iNumVertexCount = pMesh->GetControlPointsCount();
        FbxVector4* pVertexPos = pMesh->GetControlPoints();

        int iBasePolyIndex = 0;

        for (int iPoly = 0; iPoly < iNumPolygonCount; iPoly++)
        {
            int PolygonSize = pMesh->GetPolygonSize(iPoly);
            int faceCount = PolygonSize - 2;

            int iSubMtrl = 0;
            if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
            {
                iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
            }

            for (int iface = 0; iface < faceCount; iface++)
            {
                int iVertexIndex[3] = { 0, iface + 2, iface + 1 };

                int iCornerIndex[3];
                iCornerIndex[0] = pMesh->GetPolygonVertex(iPoly, iVertexIndex[0]);
                iCornerIndex[1] = pMesh->GetPolygonVertex(iPoly, iVertexIndex[1]);
                iCornerIndex[2] = pMesh->GetPolygonVertex(iPoly, iVertexIndex[2]);
                // 1 2
                // 0 3
                // 0 2 1, 0 3 2
                //ex) 0,1,2 dx -> 0,2,1 max 바꿔줘야함
                //우리는 dx로 저장해야한다.

                for (int iNumindex = 0; iNumindex < 3; iNumindex++)
                {
                    PNCT_VERTEX vertex;
                    FbxVector4 v = pVertexPos[iCornerIndex[iNumindex]];
                    v = matGeom.MultT(v);

                    vertex.pos.x = v.mData[0];
                    vertex.pos.y = v.mData[2];
                    vertex.pos.z = v.mData[1];

                    FbxVector4 Normal = ReadNormal(pMesh, iCornerIndex[iNumindex], iBasePolyIndex + iVertexIndex[iNumindex]);
                    Normal = Normalmat.MultT(Normal);
                    vertex.normal.x = Normal.mData[0];
                    vertex.normal.y = Normal.mData[2];
                    vertex.normal.z = Normal.mData[1];

                    int u[3];
                    u[0] = pMesh->GetTextureUVIndex(iPoly, iVertexIndex[0]);
                    u[1] = pMesh->GetTextureUVIndex(iPoly, iVertexIndex[1]);
                    u[2] = pMesh->GetTextureUVIndex(iPoly, iVertexIndex[2]);

                    if (VertexUVSet.size() > 0)
                    {
                        FbxLayerElementUV* pUVElement = VertexUVSet[0];
                        FbxVector2 uv;  
                        ReadTextureCoord(pMesh, pUVElement, iCornerIndex[iNumindex], u[iNumindex], uv);
                        vertex.tex.x = uv.mData[0];
                        vertex.tex.y = 1.0f - uv.mData[1];
                    }
                    FbxColor fbxcolor = FbxColor(1,1,0,1);
                    if (ColorSet.size() > 0)
                    {
                        fbxcolor = ReadColor(pMesh, ColorSet.size(), ColorSet[0], iCornerIndex[iNumindex], iBasePolyIndex + iVertexIndex[iNumindex]);
                    }
                    vertex.color.x = fbxcolor.mRed;
                    vertex.color.y = fbxcolor.mGreen;
                    vertex.color.z = fbxcolor.mBlue;
                    vertex.color.w = 1;



                    pObj->m_pSubVertexList[iSubMtrl].push_back(vertex);
                }
               
            }
            iBasePolyIndex += PolygonSize;
        }
    }
}

string FbxLoader::ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl)
{
   
    FbxProperty prop = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (prop.IsValid())
    {
        int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
        for (int iTex = 0; iTex < iTexCount; iTex++)
        {
            FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
            if (pTex == nullptr) continue;
            FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

            std::string szFileName;
            char Drive[MAX_PATH] = { 0, };
            char Dir[MAX_PATH] = { 0, };
            char FName[MAX_PATH] = { 0, };
            char Ext[MAX_PATH] = { 0, };
            if (fileTexture->GetFileName())
            {
                _splitpath_s(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
                Ext[4] = 0;
                szFileName = FName;
                szFileName += Ext;

                string texName = FName;
                string ext = Ext;
                if (ext == ".tga" || ext == ".TGA")
                {
                    ext.clear();
                    ext = ".dds";
                }
                texName += ext;
                return texName;
            }
        }
    }
    return("");
}
  
bool FbxLoader::Init()
{
    m_pFbxManager  = FbxManager::Create();
    m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
    return true;
}

bool FbxLoader::Frame()
{
    if (m_bAnimationPlay)
    {
        m_fElpaseTime += g_fSecPerFrame * 1.0f;
        m_iAnimationIndex = m_fElpaseTime * 30.0f;
        if (m_fEndTime < m_fElpaseTime)
        {
            m_iAnimationIndex = 0;
            m_fElpaseTime = 0;
            m_bAnimationPlay = false;
        }
    }
    
    return true;
}

bool FbxLoader::Release()
{
    m_pFbxScene->Destroy();
    m_pFbxImporter->Destroy();
    m_pFbxManager->Destroy();
    return true;
}

bool HFbxObject::PreRender(ID3D11DeviceContext* pContext)
{
    return true;
}
bool HFbxObject::CreateVertexBuffer()
{
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
    return true;
}

bool HFbxObject::CreateVertexData()
{
    if (m_pSubVertexList.size() > 0)
    {
        m_pVBList.resize(m_pSubVertexList.size());
    }
    return true;
}

bool HFbxObject::CreateIndexData()
{
    return true;
}

bool HFbxObject::PostRender(ID3D11DeviceContext* pContext)
{
    for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
    {
        pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_CbData, 0, 0);
        pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

        pContext->UpdateSubresource(m_pLightConstantBuffer, 0, NULL, &m_CbLight, 0, 0);
        pContext->VSSetConstantBuffers(1, 1, &m_pLightConstantBuffer);

        pContext->VSSetShader(m_pVs->m_pVertexShader, NULL, 0);
        pContext->PSSetShader(m_pPs->m_pPixelShader, NULL, 0);
        pContext->IASetInputLayout(m_pVertexLayout);
        UINT Strides = sizeof(PNCT_VERTEX);
        UINT Offsets = 0;

       if (m_pTextureList.size() > 0 && m_pTextureList[iMtrl] != nullptr)
       {
           m_pContext->PSSetShaderResources(0, 1, &m_pTextureList[iMtrl]->m_pSRV);
       }
       m_pContext->IASetVertexBuffers(0, 1, &m_pVBList[iMtrl], &Strides, &Offsets);
        
        if (m_indexList.size() <= 0)
        {
            m_pContext->Draw(m_pSubVertexList[iMtrl].size(), 0);
        }
        else
        {
            m_pContext->DrawIndexed(m_indexList.size(), 0, 0);
        }
    }
    return true;
}

bool HFbxObject::Release()
{
    Object3D::Release();
    for (int ivb = 0; ivb < m_pVBList.size(); ivb++)
    {
        if (m_pVBList[ivb] != nullptr)
        {
            m_pVBList[ivb]->Release();
        }
    }
    return true;
}
