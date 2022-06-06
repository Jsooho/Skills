#include "QuadTree.h"

// 0           2            4
// 5           7             
// 10          12           14
// 15           17 
// 20          22           24 		
bool QuadTree::UpdateIndexList(Node* pNode)
{
	//
	int iNumCols = m_pMap->m_MapInfo.m_iNumCol;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	//pNode->m_IndexList.reserve(iNumColCell * iNumRowCell * 2 * 3);
	m_IndexList.resize(iNumColCell * iNumRowCell * 2 * 3);

	int iIndex = 0;
	for (int iRow = iStartRow; iRow < iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol < iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * (iNumColCell + 1) + iCol;
			int iNextRow = (iRow + 1) * (iNumColCell + 1) + iCol;
			m_IndexList[iIndex + 0] = iCurrentIndex;
			m_IndexList[iIndex + 1] = iCurrentIndex + 1;
			m_IndexList[iIndex + 2] = iNextRow;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow + 1;
			iIndex += 6;
		}
	}
	if (m_IndexList.size() > 0) return true;
	return false;
}
bool QuadTree::CreateIndexBuffer(Node* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return false;
	return true;
}

bool QuadTree::UpdateVertexList(Node* pNode)
{
	int numCols = m_pMap->m_MapInfo.m_iNumCol;
	int startRow = pNode->m_CornerList[0] / numCols;
	int endRow = pNode->m_CornerList[2] / numCols;
	int startCol = pNode->m_CornerList[0] % numCols;
	int endCol = pNode->m_CornerList[1] % numCols;

	int numColCell = endCol - startCol;
	int numRowCell = endRow - startRow;
	pNode->m_pVertexList.resize((endCol - startCol + 1) * (endRow - startRow + 1));

	int index = 0;
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			int currentIndex = row * numCols + col;
			pNode->m_pVertexList[index++] = m_pMap->m_vertexList[currentIndex];
		}
	}
	if (pNode->m_pVertexList.size() > 0) return true;
	return false;
}

bool QuadTree::CreateVertexBuffer(Node* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth =m_iVertexSize * pNode->m_pVertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &pNode->m_pVertexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &pNode->m_pVertexBuffer);
	if (FAILED(hr)) return false;
	return true;
}

void QuadTree::SetNeighborNode(Node* pNode)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		Node* pNode = m_pLeafList[iNode];
		TVector3 vLT = m_pMap->m_vertexList[pNode->m_CornerList[0]].pos;
		TVector3 vRT = m_pMap->m_vertexList[pNode->m_CornerList[1]].pos;
		TVector3 vlB = m_pMap->m_vertexList[pNode->m_CornerList[2]].pos;
		TVector3 vRB = m_pMap->m_vertexList[pNode->m_CornerList[3]].pos;
		TVector3 vCenter = (vLT + vRT + vlB + vRB);
		vCenter /= 4.0f;
		// RIGHT
		TVector3 vPoint;
		vPoint.x = pNode->m_box.vMax.x + pNode->m_box.vSize.x; //vCenter.x + (vRT.x - vLT.x);
		vPoint.y = 0.0f;//vCenter.z;
		vPoint.z = vCenter.z;
		for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
		{
			
			if (m_pLeafList[iNode] == pNode) continue;
			if (m_pLeafList[iNode] = CheckBoxtoPoint(vPoint))
			{
				pNode->m_NeighborList[0] = m_pLeafList[iNode];
				break;
			}
		}
		// LEFT
		vPoint.x = pNode->m_box.vMin.x - pNode->m_box.vSize.x; // vCenter.x - (vRT.x - vLT.x);
		vPoint.y = 0.0f;
		vPoint.z = pNode->m_box.vCenter.z;
		
		for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
		{
			if (m_pLeafList[iNode] == pNode) continue;
			if (m_pLeafList[iNode] = CheckBoxtoPoint(vPoint))
			{
				pNode->m_NeighborList[1] = m_pLeafList[iNode];
				break;
			}
		}
		// BOTTOM
		vPoint.x = vCenter.x;
		vPoint.y = 0.0f;
		vPoint.z = pNode->m_box.vMin.z - pNode->m_box.vSize.z;//vCenter.z - (vLT.z - vRB.z);
		for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
		{
			if (m_pLeafList[iNode] == pNode) continue;
			if (m_pLeafList[iNode] = CheckBoxtoPoint(vPoint))
			{
				pNode->m_NeighborList[2] = m_pLeafList[iNode];
				break;
			}
		}
		//TOP
		vPoint.x = vCenter.x;
		vPoint.y = 0.0f;
		vPoint.z = pNode->m_box.vMax.z + pNode->m_box.vSize.z;//vCenter.z + (vLT.z - vRB.z);
		for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
		{
			if (m_pLeafList[iNode] == pNode) continue;
			if (m_pLeafList[iNode] = CheckBoxtoPoint(vPoint))
			{
				pNode->m_NeighborList[3] = m_pLeafList[iNode];
				break;
			}
		}
	}
}

bool QuadTree::ComputeStaticLodIndex(int iMaxCells)
{
	m_LodPatchList.reserve(m_iNumPatch);
	return true;
}

bool QuadTree::LoadObject(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		return false;
	}

	TCHAR data[256] = { 0, };

	TCHAR buffer[256] = { 0, };
	int iVersion = 0;
	_fgetts(buffer, 256, fp);
	TCHAR tmp[256] = { 0, };
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

	int iNumPatch = 0;
	_fgetts(buffer, 256, fp);
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iNumPatch);

	int index = 0;

	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		LodPatch lod;
		lod.lodLevel = iLod;
		for (int iCode = 0; iCode < 16; iCode++)
		{
			std::vector<std::wstring>	ListTokens;
			_fgetts(buffer, 256, fp);
			_stscanf_s(buffer, _T("%d %s"), &index, data, _countof(data));

			std::wstring sentence = data;
			Tokenize(sentence, L",", std::back_inserter(ListTokens));
			int iMaxCnt = (int)ListTokens.size();
			lod.indexList[iCode].resize(iMaxCnt);
			for (int i = 0; i < iMaxCnt; i++)
			{
				lod.indexList[iCode][i] = (DWORD)(_tstoi(ListTokens[i].c_str()));
			}
		}
		m_LodPatchList.push_back(lod);

		_fgetts(buffer, 256, fp);
	}
	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		for (int iCode = 0; iCode < 16; iCode++)
		{
			CreateIndexBuffer(m_LodPatchList[iLod], iCode);
		}
	}
	fclose(fp);
	return true;
}
bool QuadTree::CreateIndexBuffer(LodPatch& patch, int iCode)
{

	patch.indexBufferList[iCode] = nullptr;
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * patch.indexList[iCode].size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &patch.indexList[iCode].at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &patch.indexBufferList[iCode]);
	if (FAILED(hr)) return false;
	return true;
}
bool	QuadTree::Render(ID3D11DeviceContext* pContext, DebugCamera* m_pCamera)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iLodLevel = 0;
		float fDistance = (m_pLeafList[iNode]->m_vCenter - m_pCamera->m_vCameraPos).Length();
		if (fDistance < 30.0f)
		{
			m_pLeafList[iNode]->m_iLodLevel = 2;
		}
		else if (fDistance < 60.0f)
		{
			m_pLeafList[iNode]->m_iLodLevel = 1;
		}
		else
			m_pLeafList[iNode]->m_iLodLevel = 0;
	}
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iRenderCode = 0;
		// 동서남북
		if (m_pLeafList[iNode]->m_NeighborList[0] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[0]->m_iLodLevel)
		{
			iRenderCode += 2;
		}
		if (m_pLeafList[iNode]->m_NeighborList[1] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[1]->m_iLodLevel)
		{
			iRenderCode += 8;
		}
		if (m_pLeafList[iNode]->m_NeighborList[2] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[2]->m_iLodLevel)
		{
			iRenderCode += 4;
		}
		if (m_pLeafList[iNode]->m_NeighborList[3] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[3]->m_iLodLevel)
		{
			iRenderCode += 1;
		}

		UINT iNumIndex = 0;
		ID3D11Buffer* pRenderBuffer = nullptr;
		UINT iLodLevel = m_pLeafList[iNode]->m_iLodLevel;
		if (m_pLeafList[iNode]->m_iLodLevel == 0)
		{
			iNumIndex = m_LodPatchList[iLodLevel].indexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].indexBufferList[iRenderCode];
		}
		else if (m_pLeafList[iNode]->m_iLodLevel == 1)
		{
			iNumIndex = m_LodPatchList[iLodLevel].indexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].indexBufferList[iRenderCode];
		}
		else
		{
			iNumIndex = m_IndexList.size();
			pRenderBuffer = m_pIndexBuffer;
		}
		

		m_pMap->PreRender(pContext);
		UINT pStrides = m_iVertexSize;
		UINT pOffsets = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pLeafList[iNode]->m_pVertexBuffer,
			&pStrides, &pOffsets);
		pContext->IASetIndexBuffer(pRenderBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pMap->PostRender(pContext);
	}
	return true;
}
bool	QuadTree::Frame()
{
	return true;
}
void    QuadTree::Build(HMap* pMap)
{
	
	m_pMap = pMap;
	m_iNumCol = pMap->m_MapInfo.m_iNumCol; //wid
	m_iNumRow = pMap->m_MapInfo.m_iNumRow; 
	m_iWidth = pMap->m_MapInfo.m_iNumCol;
	m_iHeight = pMap->m_MapInfo.m_iNumRow;
	m_pRootNode = CreateNode(nullptr, 0, m_iNumCol - 1, (m_iNumRow - 1) * m_iNumCol, m_iNumRow * m_iNumCol - 1);
	Buildtree(m_pRootNode);
	SetNeighborNode(m_pRootNode);
	m_iNumCell = (m_iNumCol - 1) / pow(2.0f, m_iMaxDepth);
	m_iNumPatch = (log(m_iNumCell) / log(2.0f));
	if (m_iNumPatch > 0)
	{
		m_LodPatchList.resize(2);
	}
	//공유 인덱스 버퍼
	if (UpdateIndexList(m_pLeafList[0]))
	{
		CreateIndexBuffer(m_pLeafList[0]);
	}
}
bool    QuadTree::Init()
{
	return true;
}
bool  QuadTree::SubDivide(Node* pNode)
{
	if ((pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 4)
	{
		return true;
	}
	return false;
}
void QuadTree::Buildtree(Node* pNode)
{
	if (SubDivide(pNode))
	{
		
		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerList[0],mt,ml,center);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] = CreateNode(pNode, mt,pNode->m_CornerList[1],center,mr);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, ml,center,pNode->m_CornerList[2],mb);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, center,mr,mb,pNode->m_CornerList[3]);
		Buildtree(pNode->m_pChild[3]);
	}
	else
	{
		pNode->m_bLeaf = true;
		m_iMaxDepth = pNode->m_iDepth;
		TVector3 vLT = m_pMap->m_vertexList[pNode->m_CornerList[0]].pos;
		TVector3 vRT = m_pMap->m_vertexList[pNode->m_CornerList[1]].pos;
		TVector3 vLB = m_pMap->m_vertexList[pNode->m_CornerList[2]].pos;
		TVector3 vRB = m_pMap->m_vertexList[pNode->m_CornerList[3]].pos;

		pNode->SetRect(vLT.x, vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);

		// 공유 인덱스버퍼용(정점버퍼 리프노드 당)
		if (UpdateVertexList(pNode))
		{
			CreateVertexBuffer(pNode);
		}
		m_pLeafList.push_back(pNode);
	}
	
}
Node* QuadTree::FindNode(Node* pNode, TVector3 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr)
			{
				if (pNode->m_pChild[iNode] = CheckBoxtoPoint(pos))
				{
					m_Queue.push(pNode->m_pChild[iNode]);
					break;
				}
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    QuadTree::AddObject(TVector3 pos)
{
	Node* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
Node* QuadTree::FindPlayerNode(TVector3 pos)
{
	Node* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}

Node* QuadTree::CheckBoxtoPoint(TVector3 pos)
{
	for (auto node : m_pLeafList)
	{
		if (node->m_box.vMin.x <= pos.x && node->m_box.vMax.x >= pos.x &&
			node->m_box.vMin.z <= pos.z && node->m_box.vMax.z >= pos.z)
		{
			return node;
		}
	}
	return nullptr;
}

Box QuadTree::SetBox(Node* pNode)
{
	TVector3 v0, v4;
	v0 = m_pMap->m_vertexList[pNode->m_CornerList[0]].pos; // 0
	v4 = m_pMap->m_vertexList[pNode->m_CornerList[3]].pos; // 24
	pNode->m_box.vMin.x = v0.x;
	pNode->m_box.vMin.z = v4.z;
	pNode->m_box.vMax.x = v4.x;
	pNode->m_box.vMax.z = v0.z;
	TVector2 vHeight = GetHeightFromNode(
		pNode->m_CornerList[0],
		pNode->m_CornerList[1],
		pNode->m_CornerList[2],
		pNode->m_CornerList[3]);
	pNode->m_box.vMin.y = vHeight.y;
	pNode->m_box.vMax.y = vHeight.x;
	pNode->m_box.vAxis[0] = TVector3(1, 0, 0);
	pNode->m_box.vAxis[1] = TVector3(0, 1, 0);
	pNode->m_box.vAxis[2] = TVector3(0, 0, 1);
	pNode->m_box.vSize.x = (pNode->m_box.vMax.x - pNode->m_box.vMin.x) / 2.0f;
	pNode->m_box.vSize.y = (pNode->m_box.vMax.y - pNode->m_box.vMin.y) / 2.0f;
	pNode->m_box.vSize.z = (pNode->m_box.vMax.z - pNode->m_box.vMin.z) / 2.0f;
	pNode->m_box.vCenter = (pNode->m_box.vMax + pNode->m_box.vMin);
	pNode->m_box.vCenter /= 2.0f;

	return pNode->m_box;
}
TVector2 QuadTree::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{


	DWORD dwStartRow = dwTL / m_iWidth;
	DWORD dwEndRow = dwBL / m_iWidth;

	DWORD dwStartCol = dwTL % m_iWidth;
	DWORD dwEndCol = dwTR % m_iWidth;

	TVector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;

	// 0,  4, 
	// 20 ,24

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_pMap->m_vertexList[dwRow * m_iWidth + dwCol].pos.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_vertexList[dwRow * m_iWidth + dwCol].pos.y;
			}
			if (m_pMap->m_vertexList[dwRow * m_iWidth + dwCol].pos.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_vertexList[dwRow * m_iWidth + dwCol].pos.y;
			}
		}
	}

	return vHeight;
}

bool QuadTree::Release()
{
	for (int iPatch = 0; iPatch < m_LodPatchList.size(); iPatch++)
	{
		m_LodPatchList[iPatch];
	}
	if (m_pIndexBuffer)m_pIndexBuffer->Release();
	delete m_pRootNode;
	m_pRootNode = nullptr;
	return true;
}
Node* QuadTree::CreateNode(Node* pParent, float x, float y, float w, float h)
{
	Node* pNode = new Node(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	pNode->m_iIndex = Node::g_iNewCounter;
	Node::g_iNewCounter++;
	SetBox(pNode);

	return pNode;
}
QuadTree::QuadTree()
{
	m_pRootNode = nullptr;
}

QuadTree::~QuadTree()
{
}
