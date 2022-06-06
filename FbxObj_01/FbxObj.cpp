#include "FbxObj.h"
#include<algorithm>

bool Compare(const pair<float, int>& a, const pair<float, int>& b)
{
    if (a.first == b.first)
        return a.second > b.second;
    return a.first > b.first;
}
TMatrix FbxObj::ConvertMatrix(FbxMatrix& m)
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
TMatrix FbxObj::ConvertAMatrix(FbxAMatrix& m)
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
TMatrix FbxObj::DxConvertMatrix(TMatrix m)
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


bool FbxObj::FbxLoad(string filename)
{
    bool bRet = m_FbxImporter->Initialize(filename.c_str());
    bRet = m_FbxImporter->Import(m_FbxScene);
    if (bRet)
    {
        m_RootNode = m_FbxScene->GetRootNode();
        PreProcess(m_RootNode,nullptr);
    }

    for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
    {
        ParseMesh(m_ObjList[iObj]);
    }

    return true;
}

//노드의 정보얻기
bool FbxObj::PreProcess(FbxNode* pNode, FbxNode* pParentNode)
{
    FbxMesh* pMesh = pNode->GetMesh();
    if (pMesh)
    {
        HFbxObject* pObj = new HFbxObject;
        pObj->m_pfbxNode = pNode;
        pObj->m_pParentNode = pParentNode;
        pObj->m_iIndex = m_ObjList.size();
        FbxVector4  scaleLcl = pNode->LclScaling.Get();
        FbxVector4  rotateLcl = pNode->LclRotation.Get();
        FbxVector4  transLcl = pNode->LclTranslation.Get();
        FbxMatrix matLocal(transLcl, rotateLcl, scaleLcl);
        pObj->m_matWorld = DxConvertMatrix(ConvertMatrix(matLocal));


        m_ObjList.push_back(pObj);
    }

    int iChildCount = pNode->GetChildCount();
    
    for (int iNode = 0; iNode < iChildCount; iNode++)
    {
        FbxNode* ChildNode = pNode->GetChild(iNode);
       
        PreProcess(ChildNode, pNode);
    }

    return true;
}

bool FbxObj::ParseMeshSkinning(FbxMesh* pFbxMesh, HFbxObject* pObject, SkinData* pSkindata)
{
    int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (iNumDeformer == 0)
    {
        return false;
    }
    int iNumVertexCount = pFbxMesh->GetControlPointsCount();
    pSkindata->m_IWVertexList.resize(iNumVertexCount);

    for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
    {
        FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
        FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
        int iNumCluster = pSkin->GetClusterCount();
        // 영향을 미치는 행렬이 iNumCluster
        pObject->m_matBindPoseList.resize(iNumCluster);
        for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
        {
            FbxCluster* pCluster = pSkin->GetCluster(iCluster);
            // 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
            int iNumVertex = pCluster->GetControlPointIndicesCount();
            //영향을 미치는 행렬에 제어점
            FbxAMatrix matXBindPose, matInitPostion;

            pCluster->GetTransformLinkMatrix(matXBindPose);

            pCluster->GetTransformMatrix(matInitPostion);

            FbxAMatrix matBoneBindPos = matInitPostion.Inverse() * matXBindPose;
            TMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
            D3DXMatrixInverse(&matBinePos, NULL, &matBinePos);
            pObject->m_matBindPoseList[iCluster] = matBinePos;

            FbxNode* pLinkNode = pCluster->GetLink();
            pObject->m_fbxNodeList.push_back(pLinkNode);
            //ControlPoint(제어점) 정점리스트
            int* iIndex = pCluster->GetControlPointIndices();
            //가중치리스트
            double* pWeight = pCluster->GetControlPointWeights();
            for (int i = 0; i < iNumVertex; i++)
            {
                pSkindata->m_IWVertexList[iIndex[i]].m_IndexList.push_back(iCluster);
                pSkindata->m_IWVertexList[iIndex[i]].m_WegihtList.push_back(pWeight[i]);
                //iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.				
            }
        }
    }
    return true;
}

// 정점 정보를 가져온다. (PNCT)
void FbxObj::ParseMesh(HFbxObject* pObj)
{
    FbxMesh* pMesh = pObj->m_pfbxNode->GetMesh();

    if (pObj->m_pfbxNode->GetCamera() || pObj->m_pfbxNode->GetLight())
    {
        return;
    }
    int iNumCP = pMesh->GetControlPointsCount();
    SkinData skindata;
    bool bSkinnedMesh = ParseMeshSkinning(pMesh, pObj, &skindata);
    if (bSkinnedMesh)
    {
        _ASSERT(skindata.m_IWVertexList.size() == iNumCP);
        for (int i = 0; i < skindata.m_IWVertexList.size(); i++)
        {
            std::vector<std::pair<float, int>> list;
            for (int j = 0; j <
                skindata.m_IWVertexList[i].m_IndexList.size();
                j++)
            {
                list.push_back(std::make_pair(
                    skindata.m_IWVertexList[i].m_WegihtList[j],
                    skindata.m_IWVertexList[i].m_IndexList[j]));
            }
            std::sort(list.begin(), list.end(), Compare);
            for (int k = 0; k < list.size(); k++)
            {
                skindata.m_IWVertexList[i].m_WegihtList[k] = list[k].first;
                skindata.m_IWVertexList[i].m_IndexList[k] = list[k].second;
            }
        }
    }

    std::vector<FbxLayerElementUV*> VertexUVSet;
    std::vector<FbxLayerElementVertexColor*> VertexColorSet;
    std::vector<FbxLayerElementMaterial*> MaterialSet;
    int iLayerCount = pMesh->GetLayerCount();
    for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
    {
        FbxLayer* pFbxLayer = pMesh->GetLayer(iLayer);
        if (pFbxLayer->GetUVs() != nullptr)
        {
            VertexUVSet.push_back(pFbxLayer->GetUVs());
        }
        if (pFbxLayer->GetVertexColors() != nullptr)
        {
            VertexColorSet.push_back(pFbxLayer->GetVertexColors());
        }
        if (pFbxLayer->GetMaterials() != nullptr)
        {
            MaterialSet.push_back(pFbxLayer->GetMaterials());
        }
    }

    int iNumMtrl = pObj->m_pfbxNode->GetMaterialCount();
    //iNumMtrl = pMesh->GetElementMaterialCount();

    for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
    {
        FbxSurfaceMaterial* pSurfaceMtrl = pObj->m_pfbxNode->GetMaterial(iMtrl);
        if (pSurfaceMtrl)
        {
            string TextureName = ParseMaterial(pSurfaceMtrl);
            std::wstring szTexFileName = L"../../Data/object/fbx/";
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
    //기하행렬(초기 정점 위치를 변환할 때 사용) = 로컬정점을 만들때 사용
    //기하행렬을 역행렬곱하고 전치한 값에 노말값을 곱해서 사용한다
    FbxAMatrix matGeom;
    {
        FbxVector4 rot = pObj->m_pfbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
        FbxVector4 trans = pObj->m_pfbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
        FbxVector4 scale = pObj->m_pfbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
        matGeom.SetT(trans);
        matGeom.SetR(rot);
        matGeom.SetS(scale);
    }
    FbxAMatrix Normalmat = matGeom;
    Normalmat = Normalmat.Inverse();
    Normalmat = Normalmat.Transpose();

    
    int iPolyCount = pMesh->GetPolygonCount();
    int VertexCount = pMesh->GetControlPointsCount();

    FbxVector4* pVertexPosition = pMesh->GetControlPoints();
    
    int iBasePolyIndex = 0;

    for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
    {
        int iPolySize = pMesh->GetPolygonSize(0);
        int ifaceCount = iPolySize - 2;

        int iSubMtrl = 0;
        if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
        {
            iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
        }

        for (int iface = 0; iface < ifaceCount; iface++)
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
                IW_VERTEX IWvertex;

                FbxVector4 v = pVertexPosition[iCornerIndex[iNumindex]];
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

                if (VertexUVSet.size()>0)
                {
                    FbxLayerElementUV* pUVSet = VertexUVSet[0];
                    FbxVector2 uv;
                    ReadTextureCoord(pMesh, pUVSet, iCornerIndex[iNumindex], u[iNumindex], uv);
                    vertex.tex.x = uv.mData[0];
                    vertex.tex.y = 1.0f - uv.mData[1];
                }

                FbxColor color = FbxColor(1, 1, 1, 1);
                if (VertexColorSet.size()>0)
                {
                    color = ReadColor(pMesh, VertexColorSet.size(), VertexColorSet[0], iCornerIndex[iNumindex], iBasePolyIndex + iVertexIndex[iNumindex]);
                }
                vertex.color.x = color.mRed;
                vertex.color.y = color.mGreen;
                vertex.color.z = color.mBlue;
                vertex.color.w = pObj->m_iIndex;

                pObj->m_vertexList.push_back(vertex);
            }
        }
    }
}

string FbxObj::ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl)
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




bool FbxObj::Init()
{
    m_FbxManager = FbxManager::Create();
    m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
    m_FbxScene = FbxScene::Create(m_FbxManager, "");
    return true;
}

bool FbxObj::Release()
{
    m_FbxScene->Destroy();
    m_FbxImporter->Destroy();
    m_FbxManager->Destroy();

    m_FbxScene = nullptr;
    m_FbxImporter = nullptr;
    m_FbxManager = nullptr;
        
    return true;
}



