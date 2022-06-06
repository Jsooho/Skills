#include "FbxObj.h"

bool FbxObj::FbxLoad(string filename)
{
	bool bRet = m_FbxImporter->Initialize(filename.c_str());
	bRet = m_FbxImporter->Import(m_FbxScene);
	if (bRet)
	{
		FbxNode* RootNode = m_FbxScene->GetRootNode();
		PreProcess(RootNode);
	}
	for (int iMtrl = 0; iMtrl < m_MtrlList.size(); iMtrl++)
	{
		Mtrl* pMtrl = m_MtrlList[iMtrl];
		string TextureName = ParseMaterial(pMtrl->pFbxMtrl);
		std::wstring szTexFileName = L"../../Data/object/fbx/";
		szTexFileName += to_mw(TextureName);
		pMtrl->m_szTexFileList.push_back(szTexFileName);
		pMtrl->m_pTextureList.push_back(I_Texture.Load(pMtrl->m_szTexFileList[iMtrl]));

		pMtrl->m_szTexFileName = pMtrl->m_szTexFileList[0];
	}
	
	for (int iObj = 0; iObj < m_FbxNodeList.size(); iObj++)
	{
		FbxNode* pNode = m_FbxNodeList[iObj];
		Mesh* pMesh = new Mesh;
		m_MeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
	}
	return true;
}

void FbxObj::PreProcess(FbxNode* pNode)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);

	if (pFbxMaterial != nullptr)
	{
		Mtrl* pMtrl = new Mtrl;
		pMtrl->m_pFbxNode = pNode;
		pMtrl->pFbxMtrl = pFbxMaterial;
		m_MtrlList.push_back(pMtrl);
	}

	int iNumChildCount = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChildCount; iNode++)
	{
		FbxNode* ChildNode = pNode->GetChild(iNode);
		//노드속성정의
		FbxNodeAttribute::EType type = ChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			m_FbxNodeList.push_back(ChildNode); // Node->mesh리스트
		}
		PreProcess(ChildNode);
	}

}

void FbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv)
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

void FbxObj::ParseNode(FbxNode* pNode, Mesh* pMesh)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->m_iMtrlRef = GetRootMtrl(pFbxMaterial);
	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{
		int iNumLayer = pFbxMesh->GetLayerCount();
		std::vector< FbxLayerElementUV*> VertexUVList;
		
		// 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(0);
			if (pLayer->GetVertexColors() != nullptr) {}
			if (pLayer->GetNormals() != nullptr) {}
			if (pLayer->GetTangents() != nullptr) {}
			if (pLayer->GetUVs() != nullptr)
			{
				VertexUVList.push_back(pLayer->GetUVs());
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				FbxLayerElementMaterial* fbxMaterial = pLayer->GetMaterials();
				//	int iMtrlCount = fbxMaterial->mDirectArray->GetCount();
				//	// 서브메터리얼 구조
				//	if (iMtrlCount > 1)
				//	{
				//		for (int iSub=0;iSub < iMtrlCount; iSub++)
				//		{

				//		}
				//	}
				//	else
				//	{
				//		FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
				//		FbxProperty prop= pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
				//		if (prop.IsValid())
				//		{
				//			int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				//			for (int iTex = 0; iTex < iTexCount; iTex++)
				//			{
				//				FbxTexture* pTex =	prop.GetSrcObject<FbxFileTexture>(iTex);
				//				if (pTex == nullptr) continue;
				//				FbxFileTexture* fileTexture =prop.GetSrcObject<FbxFileTexture>(iTex);
				//				fbxFileTexList.push_back(fileTexture->GetFileName());
				//			}						
				//		}
				//	}
				//}
			}
		}

		int iNumPolygonCount = pFbxMesh->GetPolygonCount();
		int iNumVertexCount = pFbxMesh->GetControlPointsCount();
		FbxVector4* pVertexPos = pFbxMesh->GetControlPoints();

		int iBasePolyIndex = 0;

		for (int iPoly = 0; iPoly < iNumPolygonCount; iPoly++)
		{
			int PolygonSize = pFbxMesh->GetPolygonSize(iPoly);
			int faceCount = PolygonSize - 2;

			for (int iface = 0; iface < faceCount; iface++)
			{
				int iVertexIndex[3] = { 0, iface + 2, iface + 1 };

				int iCornerIndex[3];
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertexIndex[0]);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertexIndex[1]);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertexIndex[2]);
				// 1 2
				// 0 3
				// 0 2 1, 0 3 2
				//ex) 0,1,2 dx -> 0,2,1 max 바꿔줘야함
				//우리는 dx로 저장해야한다.

				for (int iNumindex = 0; iNumindex < 3; iNumindex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 v = pVertexPos[iCornerIndex[iNumindex]];


					vertex.pos.x = v.mData[0];
					vertex.pos.y = v.mData[2];
					vertex.pos.z = v.mData[1];



					int u[3];
					u[0] = pFbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[0]);
					u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[1]);
					u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[2]);

					if (VertexUVList[0] != nullptr)
					{

						FbxVector2 uv;
						ReadTextureCoord(pFbxMesh, VertexUVList[0], iCornerIndex[iNumindex], u[iNumindex], uv);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					pMesh->m_vertexList.push_back(vertex);
				}
				
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

int FbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_MtrlList.size(); iMtrl++)
	{
		if (m_MtrlList[iMtrl]->pFbxMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}



bool FbxObj::Init()
{
	m_FbxManager = FbxManager::Create();
	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
	m_FbxScene = FbxScene::Create(m_FbxManager, "");

	return true;
}

bool Mesh::CreateIndexData()
{
	return true;
}