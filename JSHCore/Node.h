#pragma once
#include"Std.h"
#include"Model.h"
#include"Shape.h"
struct HRect
{
	float  x, y, w, h;
};
struct SRect
{
	TVector2  p0;
	TVector2  p1;
	TVector2  wh;
};
typedef std::vector<DWORD> DWORD_VECTOR;
struct LodPatch
{
	UINT lodLevel;
	DWORD_VECTOR		indexList[16];
	ID3D11Buffer* indexBufferList[16];
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (indexBufferList[iBuffer])
			{
				indexBufferList[iBuffer]->Release();
				indexBufferList[iBuffer] = nullptr;
			}

		}
	}
	LodPatch() {}
	~LodPatch()
	{
	}
};
class Node
{
public:
	UINT m_iLodLevel;
public:
	static int g_iNewCounter;
	int	m_iIndex;
	SRect m_Rect;
	std::vector<TVector2>  m_ObjectList;
	std::vector<TVector3>  m_ObjectList3;
	std::vector<DWORD>  m_CornerList; // 인덱스 정점 코너
	std::vector<DWORD>  m_IndexList; // 노드가 들고있는 인덱스 정점
	ID3D11Buffer* m_pIndexBuffer;
	std::vector<PNCT_VERTEX>  m_pVertexList;
	ID3D11Buffer* m_pVertexBuffer;
	TVector3 m_vCenter;
	Node* m_pChild[4];
	Node* m_NeighborList[4];
	int	m_iDepth;
	bool m_bLeaf;
	Node* m_pParent;
	Box m_box;
public:
	bool AddObject(float fX, float fY);
	bool AddObject(TVector2 pos);
	bool AddObject(TVector3 pos);
	bool IsRect(TVector2 pos);
	void SetRect(float x, float y, float w, float h);
	
	
	
public:
	Node() {};
	Node(UINT x, UINT y, UINT w, UINT h);
	~Node();
};
