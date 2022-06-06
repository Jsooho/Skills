#include "FbxObj.h"
int FbxLoader::GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList)
{
    int iSubMtrl = 0;
    if (pMaterialSetList != nullptr)
    {
        switch (pMaterialSetList->GetMappingMode())
        {
        case FbxLayerElement::eByPolygon:
        {
            switch (pMaterialSetList->GetReferenceMode())
            {
            case FbxLayerElement::eIndex:
            {
                iSubMtrl = iPoly;
            }break;
            case FbxLayerElement::eIndexToDirect:
            {
                iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
            }break;
            }
        }
        default:
        {
            break;
        }
        }
    }
    return iSubMtrl;
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