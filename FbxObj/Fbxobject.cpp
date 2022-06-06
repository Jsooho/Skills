#include "Fbxobject.h"
void Fbxobject::LoadMaterial(Mtrl* pMtrl)
{
	//FbxSurfaceMaterial* pFbxSurfaceMtrl = pMtrl->pFbxMtrl;
	FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
	FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
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
			}
			pMtrl->m_Texture.m_csName = TBASIS::g_szDataPath;
			pMtrl->m_Texture.m_csName += L"object/";
			pMtrl->m_Texture.m_csName += TBASIS::mtw(szFileName);
			pMtrl->m_Texture.Load(pMtrl->m_Texture.m_csName);
		}
	}
}
void Fbxobject::SetMatrix(TMatrix* pMatWorld,TMatrix* pMatView, TMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_cbData.matrixWorld = *pMatWorld;
	}
	if (pMatView != nullptr)
	{
		m_cbData.matrixView = *pMatView;
	}
	if (pMatProj != nullptr)
	{
		m_cbData.matrixProjection = *pMatProj;
	}
}
bool Fbxobject::Render(ID3D11DeviceContext* pContext)
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		Mesh* pMesh = m_pMeshList[iObj];
		Mtrl* pMtrl = nullptr;
		if (pMesh->m_iMtrlRef >= 0)
		{
			pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
		}
		pMesh->SetMatrix(nullptr, &m_cbData.matrixView, &m_cbData.matrixProjection);
		if (pMtrl != nullptr)
		{
			pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pSRV);
		}
		pMesh->Render(pContext);
	}
	return true;
}
int Fbxobject::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->pFbxMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}
void Fbxobject::ParseNode(FbxNode* pNode, Mesh* pMesh)
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

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0; iTriangle < m_iNumTriangle; iTriangle++)
			{
				// 위치 인덱스
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				// UV 인덱스
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

				for (int iIndex = 0; iIndex < 3; iIndex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];

					if (VertexUVList.size())
					{
						// todo : uvlist
						FbxLayerElementUV* pUVElement =
							VertexUVList[0];
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, pUVElement,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					pMesh->m_vertexList.push_back(vertex);
				}
			}
		}
	}
}
FbxVector2 Fbxobject::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 uv(0, 0);
	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
	{
		return uv;
	}
	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
	FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

	// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점을 위미한다.
	// 그래서 텍스처 좌표와 같은 레이어 들은 제어점 또는 정점으로 구분된다.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // 제어점 당 1개의 텍스처 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // 배열에서 직접 얻는다.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // 배열에 해당하는 인덱스를 통하여 얻는다.
		{
			int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex: // 정점 당 1개의 매핑 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
	return uv;
}

void Fbxobject::PreProcess(FbxNode* pNode)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0); 

	if (pFbxMaterial != nullptr)
	{
		Mtrl* pMtrl = new Mtrl;
		pMtrl->pFbxMtrl = pFbxMaterial;
		pMtrl->m_pFbxNode = pNode;
		m_pFbxMaterialList.push_back(pMtrl); // Material리스트
	}

	int iNumChild = pNode->GetChildCount(); 
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		//노드속성정의
		FbxNodeAttribute::EType type = pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			m_pFbxNodeList.push_back(pChildNode); // Node->mesh리스트
		}
		PreProcess(pChildNode);

	}
}
bool Fbxobject::LoadObject(std::string filename)
{
	m_FbxManager = FbxManager::Create(); //메모리관리
	FbxIOSettings* ios = FbxIOSettings::Create(m_FbxManager, IOSROOT);
	m_FbxManager->SetIOSettings(ios);
	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
	m_FbxScene = FbxScene::Create(m_FbxManager, "");

	INT iFileFormet = -1;
	bool bRet = m_FbxImporter->Initialize(filename.c_str(), iFileFormet, m_FbxManager->GetIOSettings());
	bRet = m_FbxImporter->Import(m_FbxScene); // 파일읽고 scene에 저장해라
	FbxNode* m_RootNode = m_FbxScene->GetRootNode();

	PreProcess(m_RootNode);
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		Mtrl* pMtrl = m_pFbxMaterialList[iMtrl];
		LoadMaterial(pMtrl);
	}

	for (int iObj = 0; iObj < m_pFbxNodeList.size(); iObj++)
	{
		FbxNode* pNode = m_pFbxNodeList[iObj];
		Mesh* pMesh = new Mesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
		pMesh->Create(pMesh->m_pContext,L"",L"", L"FbxShader.hlsl");
	}
	
	return bRet;
}

bool Fbxobject::Release()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
	}
	for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
	{
		m_pFbxMaterialList[iObj]->Release();
		delete m_pFbxMaterialList[iObj];
	}
	return true;
}