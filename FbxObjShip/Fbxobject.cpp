#include "Fbxobject.h"
TMatrix Fbxobject::DxConvertMatrix(TMatrix m)
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
TMatrix Fbxobject::ConvertMatrix(FbxMatrix& m)
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
void Fbxobject::LoadMaterial(Mtrl* pMtrl)
{
	if (pMtrl == nullptr) return;
	if (pMtrl->m_SubMtrl.size() > 0)
	{
		for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
		{
			Mtrl* pSubMtrl = pMtrl->m_SubMtrl[iSub];
			FbxSurfaceMaterial* pFbxMaterial = pSubMtrl->m_pFbxSurfaceMtrl;
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
						if (_stricmp(Ext, ".tga") == 0)
						{
							szFileName += ".dds";
						}
						else
						{
							szFileName += Ext;
						}
					}

					pSubMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
					pSubMtrl->m_Texture.m_szFileName += L"object/";
					pSubMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
					pSubMtrl->m_Texture.LoadTexture(pSubMtrl->m_Texture.m_szFileName);
				}
			}
		}
	}
	else
	{
		//FbxSurfaceMaterial* pFbxSurfaceMtrl = pMtrl->m_pFbxSurfaceMtrl;
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
					if (_stricmp(Ext, ".tga") == 0)
					{
						szFileName += ".dds";
					}
					else
					{
						szFileName += Ext;
					}
				}

				pMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
				pMtrl->m_Texture.m_szFileName += L"object/";
				pMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
				pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
			}
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
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 서브메터리얼 존재  
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				Mtrl* pSubMtrl =
					m_pFbxMaterialList[pMesh->m_iMtrlRef]->m_SubMtrl[iSub];
				pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				pMesh->m_pSubMesh[iSub]->SetMatrix(&pMesh->m_matWorld, &m_cbData.matrixView, &m_cbData.matrixProjection);
				pMesh->m_pSubMesh[iSub]->Render(pContext);
			}
		}
		else
		{
			if (pMesh->m_iMtrlRef >= 0)
			{
				pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
			}
			if (pMtrl != nullptr)
			{
				pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
			}
			pMesh->SetMatrix(&pMesh->m_matWorld, &m_cbData.matrixView, &m_cbData.matrixProjection);
			pMesh->Render(pContext);
		}
	}
	return true;
}
int Fbxobject::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->m_pFbxSurfaceMtrl == pFbxMaterial)
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
		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);
		//std::vector< FbxLayerElementUV*> VertexUVList;
		//std::vector< FbxLayerElementVertexColor*> VertexColorList;
		//std::vector< FbxLayerElementNormal*> VertexNormalList;
		//FbxLayerElementMaterial* fbxSubMaterial=nullptr;
		// 
		// todo : 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
			if (pLayer->GetVertexColors() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
			}
			if (pLayer->GetNormals() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
			}
			if (pLayer->GetUVs() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
			}
		}
		// TODO : 월드행렬
		FbxAMatrix matGeom;
		{
			FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matGeom.SetT(trans);
			matGeom.SetR(rot);
			matGeom.SetS(scale);
		}
		FbxMatrix matA = matGeom;
		pMesh->m_matWorld = DxConvertMatrix(ConvertMatrix(matA));
		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		int iBasePolyIndex = 0;

		int iNumFbxMaterial = pNode->GetMaterialCount();
		if (iNumFbxMaterial > 1)
		{
			pMesh->m_pSubMesh.resize(iNumFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				pMesh->m_pSubMesh[iSub] = new Mesh;
			}
		}

		FbxLayerElementMaterial* fbxSubMaterial =
			pMesh->m_LayerList[0].pMaterial;
		FbxLayerElementUV* VertexUVList =
			pMesh->m_LayerList[0].pUV;
		FbxLayerElementVertexColor* VertexColorList =
			pMesh->m_LayerList[0].pColor;
		FbxLayerElementNormal* VertexNormalList =
			pMesh->m_LayerList[0].pNormal;

		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			int iSubMtrlIndex = 0;
			if (fbxSubMaterial != nullptr)
			{
				switch (fbxSubMaterial->GetMappingMode())
				{
				case FbxLayerElement::eByPolygon:
				{
					switch (fbxSubMaterial->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						iSubMtrlIndex = iPoly;
					}break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
					{
						iSubMtrlIndex = fbxSubMaterial->GetIndexArray().GetAt(iPoly);
					}break;
					}
				}break;
				default:
				{
					iSubMtrlIndex = 0;
				}break;
				}
			}
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0;
				iTriangle < m_iNumTriangle;
				iTriangle++)
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

				for (int iIndex = 0;
					iIndex < 3;
					iIndex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					//FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];
					if (VertexUVList != nullptr)
					{
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePolyIndex + 0;
						iColorIndex[1] = iBasePolyIndex + iTriangle + 2;
						iColorIndex[2] = iBasePolyIndex + iTriangle + 1;

						FbxColor color = ReadColor(
							pFbxMesh, 1, VertexColorList,
							iCornerIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;

					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePolyIndex + 0;
						iNormalIndex[1] = iBasePolyIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePolyIndex + iTriangle + 1;
						FbxVector4 normal = ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iCornerIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					if (iNumFbxMaterial > 1)
					{ 
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_vertexList.push_back(vertex);
					}
					else
					{
						pMesh->m_vertexList.push_back(vertex);
					}
				}
			}
			iBasePolyIndex += iPolySize;
		}
	}
}


void Fbxobject::PreProcess(FbxNode* pNode)
{
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (iNumFbxMaterial > 1)
	{
		Mtrl* pMtrl = new Mtrl(pNode, pFbxMaterial);
		for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
		{
			FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
			_ASSERT(pFbxSubMaterial != nullptr);
			Mtrl* pSubMtrl = new Mtrl(pNode, pFbxSubMaterial);
			pMtrl->m_SubMtrl.push_back(pSubMtrl);
		}
		m_pFbxMaterialList.push_back(pMtrl);
	}
	else
	{
		if (pFbxMaterial != nullptr)
		{
			Mtrl* pMtrl = new Mtrl(pNode, pFbxMaterial);
			m_pFbxMaterialList.push_back(pMtrl);
		}
	}

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		FbxNodeAttribute::EType type =
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			m_pFbxNodeList.push_back(pChildNode); //mesh
		}
		PreProcess(pChildNode);
	}
}
bool Fbxobject::LoadObject(std::string filename)
{
	m_FbxManager = FbxManager::Create();
	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
	m_FbxScene = FbxScene::Create(m_FbxManager, "");
	bool bRet = m_FbxImporter->Initialize(filename.c_str());
	bRet = m_FbxImporter->Import(m_FbxScene);

	FbxNode* m_pRootNode = m_FbxScene->GetRootNode();
	PreProcess(m_pRootNode);
	// todo : 중복처리 미작업
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		Mtrl* pMtrl = m_pFbxMaterialList[iMtrl];
		LoadMaterial(pMtrl);
	}
	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		FbxNode* pNode = m_pFbxNodeList[iNode];
		Mesh* pMesh = new Mesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
	}
	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		Mesh* pMesh = m_pMeshList[iMesh];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				Mesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				// todo : 쉐이더 등등 중복처리 미작업
				pSubMesh->Create(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl");
			}
		}
		else
		{
			pMesh->Create(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl");
		}
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