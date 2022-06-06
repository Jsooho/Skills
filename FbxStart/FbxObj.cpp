#include "FbxObj.h"
// 정점 노말을 읽는 함수 

bool Compare(const pair<float, int>& a, const pair<float, int>& b)
{
    if (a.first == b.first)
        return a.second > b.second;
    return a.first > b.first;
}
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
FbxVector4 FbxLoader::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
    if (mesh->GetElementNormalCount() < 1) {}

    const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
    // 노말 획득 
    FbxVector4 result;
    // 노말 벡터를 저장할 벡터 
    switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
    {
        // 제어점 마다 1개의 매핑 좌표가 있다.
    case FbxGeometryElement::eByControlPoint:
    {
        // control point mapping 
        switch (vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
            result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
        } break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
            // 인덱스를 얻어온다. 
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
            result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
        }break;
        }break;
    }break;
    // 정점 마다 1개의 매핑 좌표가 있다.
    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
            result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
            // 인덱스를 얻어온다. 
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
            result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
        }break;
        }
    }break;
    }
    return result;
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

    for (int iObj = 0; iObj < m_fbxObjList.size(); iObj++)
    {
        ParseMesh(m_fbxObjList[iObj]);
    }
    return true;
}

void FbxLoader::PreProcess(FbxNode* pNode, FbxNode* pParentNode)
{
    // camera, light, mesh, shape, animation
    FbxMesh* pMesh = pNode->GetMesh();
    if (pMesh)
    {
        HFbxObject* fbxObj = new HFbxObject;
        fbxObj->m_pfbxNode = pNode;
        fbxObj->m_pParentNode = pParentNode;

        FbxVector4  scaleLcl = pNode->LclScaling.Get();
        FbxVector4  rotateLcl = pNode->LclRotation.Get();
        FbxVector4  transLcl = pNode->LclTranslation.Get();
        FbxMatrix matLocal(transLcl, rotateLcl, scaleLcl);
        fbxObj->m_matWorld = DxConvertMatrix(ConvertMatrix(matLocal));
        m_fbxObjList.push_back(fbxObj);
    }

    int iNumChildNode = pNode->GetChildCount();
    
    for (int iNode = 0; iNode < iNumChildNode; iNode++)
    {
        FbxNode* ChildNode = pNode->GetChild(iNode);
        PreProcess(ChildNode, pNode);
    }
}

void FbxLoader::ParseMesh(HFbxObject* pObj)
{
    FbxMesh* pMesh = pObj->m_pfbxNode->GetMesh();

    if (pMesh)
    {

        int iNumLayerCount = pMesh->GetLayerCount();
        pObj->m_LayerList.resize(iNumLayerCount);
        // 정점성분 레이어 리스트
        for (int iLayer = 0; iLayer < iNumLayerCount; iLayer++)
        {
            FbxLayer* pLayer = pMesh->GetLayer(iLayer);

            if (pLayer->GetVertexColors() != nullptr)
            {
                pObj->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
            }
            if (pLayer->GetNormals() != nullptr)
            {
                pObj->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
            }
            if (pLayer->GetUVs() != nullptr)
            {
                pObj->m_LayerList[iLayer].pUV = pLayer->GetUVs();

            }
            if (pLayer->GetMaterials() != nullptr)
            {
                pObj->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
            }
        }

        int iNumMtrl = pObj->m_pfbxNode->GetMaterialCount();

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

        FbxLayerElementMaterial*    fbxSubMaterial =        pObj->m_LayerList[0].pMaterial;
        FbxLayerElementUV*          VertexUVList =          pObj->m_LayerList[0].pUV;
        FbxLayerElementVertexColor* VertexColorList =       pObj->m_LayerList[0].pColor;
        FbxLayerElementNormal*      VertexNormalList =      pObj->m_LayerList[0].pNormal;

        int iNumPolygonCount = pMesh->GetPolygonCount();
        int iNumVertexCount = pMesh->GetControlPointsCount();
        FbxVector4* pVertexPos = pMesh->GetControlPoints();

        int iBasePolyIndex = 0;

        for (int iPoly = 0; iPoly < iNumPolygonCount; iPoly++)
        {
            int PolygonSize = pMesh->GetPolygonSize(iPoly);
            int faceCount = PolygonSize - 2;

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

                    if (VertexUVList != nullptr)
                    {
                        
                        FbxVector2 uv;  
                        ReadTextureCoord(pMesh, VertexUVList, iCornerIndex[iNumindex], u[iNumindex], uv);
                        vertex.tex.x = uv.mData[0];
                        vertex.tex.y = 1.0f - uv.mData[1];
                    }
                    /*FbxColor fbxcolor = FbxColor(1,1,0,1);
                    if (VertexColorList != nullptr )
                    {
                        fbxcolor = ReadColor(pMesh, ColorSet.size(), ColorSet[0], iCornerIndex[iNumindex], iBasePolyIndex + iVertexIndex[iNumindex]);
                    }
                    vertex.color.x = fbxcolor.mRed;
                    vertex.color.y = fbxcolor.mGreen;
                    vertex.color.z = fbxcolor.mBlue;
                    vertex.color.w = 1;*/



                    pObj->m_vertexList.push_back(vertex);
                }

            }
        }
    }
}
void FbxLoader::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
    int vertexIndex, int uvIndex, FbxVector2& uv)
{
    FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
    if (pFbxLayerElementUV == nullptr) {
        return;
    }

    switch (pFbxLayerElementUV->GetMappingMode())
    {
    case FbxLayerElementUV::eByControlPoint:
    {
        switch (pFbxLayerElementUV->GetReferenceMode())
        {
        case FbxLayerElementUV::eDirect:
        {
            FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
            uv.mData[0] = fbxUv.mData[0];
            uv.mData[1] = fbxUv.mData[1];
            break;
        }
        case FbxLayerElementUV::eIndexToDirect:
        {
            int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
            FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
            uv.mData[0] = fbxUv.mData[0];
            uv.mData[1] = fbxUv.mData[1];
            break;
        }
        }
        break;
    }
    case FbxLayerElementUV::eByPolygonVertex:
    {
        switch (pFbxLayerElementUV->GetReferenceMode())
        {
            // Always enters this part for the example model
        case FbxLayerElementUV::eDirect:
        case FbxLayerElementUV::eIndexToDirect:
        {
            uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
            uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
            break;
        }
        }
        break;
    }
    }
}

FbxColor FbxLoader::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex)
{
    FbxColor Value(1, 1, 1, 1);
    if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
    {
        // Crack apart the FBX dereferencing system for Color coordinates		
        switch (pVertexColorSet->GetMappingMode())
        {
        case FbxLayerElement::eByControlPoint:
            switch (pVertexColorSet->GetReferenceMode())
            {
            case FbxLayerElement::eDirect:
            {
                Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
            }break;
            case FbxLayerElement::eIndexToDirect:
            {
                int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
                Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
            }break;
            }
        case FbxLayerElement::eByPolygonVertex:
            switch (pVertexColorSet->GetReferenceMode())
            {
            case FbxLayerElement::eDirect:
            {
                int iColorIndex = dwVertexIndex;
                Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
            }break;
            case FbxLayerElement::eIndexToDirect:
            {
                int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
                Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
            }break;
            }
            break;
        }
    }
    return Value;
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

bool FbxLoader::Render(ID3D11DeviceContext* pContext)
{
    return true;
}

bool FbxLoader::Release()
{
    m_pFbxScene->Destroy();
    m_pFbxImporter->Destroy();
    m_pFbxManager->Destroy();
    return true;
}

bool HFbxObject::CreateIndexData()
{
    return true;
}
