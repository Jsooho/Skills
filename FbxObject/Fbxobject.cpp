#include "Fbxobject.h"
void Fbxobject::ParseNode(FbxNode* pNode)
{
	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	if (pFbxMesh != nullptr)
	{
		FbxLayerElementUV* pUVList = nullptr;
		// �������� ����Ʈ
		FbxLayer* pLayer = pFbxMesh->GetLayer(0);
		if (pLayer->GetVertexColors() != nullptr) {}
		if (pLayer->GetNormals() != nullptr) {}
		if (pLayer->GetTangents() != nullptr) {}
		if (pLayer->GetUVs() != nullptr) {
			pUVList = pLayer->GetUVs();
		}

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// ��������Ʈ �ּ�
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			// �ﰢ��, �簢��
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0;
				iTriangle < m_iNumTriangle;
				iTriangle++)
			{
				// ��ġ �ε���
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				// UV �ε���
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

					if (pUVList != nullptr)
					{
						FbxVector2 uv = ReadTextureCoord(pFbxMesh, 1, pUVList, iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					m_vertexList.push_back(vertex);
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

	// �������� ����� 4�� ����, ������ ������ 6�� ������ �����Ѵ�.
	// �׷��� �ؽ�ó ��ǥ�� ���� ���̾� ���� ������ �Ǵ� �������� ���еȴ�.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // �迭���� ���� ��´�.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // �迭�� �ش��ϴ� �ε����� ���Ͽ� ��´�.
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
	case FbxLayerElementUV::eByPolygonVertex: // ���� �� 1���� ���� ��ǥ�� �ִ�.
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
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		//���Ӽ�����
		FbxNodeAttribute::EType type = pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			ParseNode(pChildNode);
		}
		PreProcess(pChildNode);

	}
}
bool Fbxobject::LoadObject(std::string filename)
{
	m_FbxManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(m_FbxManager, IOSROOT);
	m_FbxManager->SetIOSettings(ios);
	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
	m_FbxScene = FbxScene::Create(m_FbxManager, "");

	INT iFileFormet = -1;
	bool bRet = m_FbxImporter->Initialize(filename.c_str(), iFileFormet, m_FbxManager->GetIOSettings());
	bRet = m_FbxImporter->Import(m_FbxScene);
	FbxNode* m_RootNode = m_FbxScene->GetRootNode();

	PreProcess(m_RootNode);
	Create(L"FbxShader.hlsl", L"FbxShader.hlsl");
	return bRet;
}

bool Fbxobject::Create(std::wstring vsFile, std::wstring psFile)
{
	if (CreateVertexData())
	{
		CreateConstantBuffer();
		CreateVertexBuffer();
		//CreateIndexBuffer();
		LoadShader(vsFile, psFile);
		CreateVertexLayout();
		return true;
	}
	return false;
}

bool Fbxobject::postRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
	pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->Draw(m_vertexList.size(), 0);
	return true;
}