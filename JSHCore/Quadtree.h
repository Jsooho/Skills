#pragma once
#include"Node.h"
#include"Camera.h"
#include"HMap.h"
class QuadTree : public Model
{
public:
	std::vector<LodPatch>   m_LodPatchList;
public:
	UINT m_iMaxDepth;
	UINT m_iNumCell;
	UINT m_iNumPatch;
	int		m_iWidth;
	int		m_iHeight;
	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
public:
	Node* m_pRootNode;
	int	m_iNumCol;
	int	m_iNumRow;
	std::queue<Node*>  m_Queue;
	std::vector<Node*>  m_pLeafList;
	HMap* m_pMap;
	
public:
	bool	UpdateIndexList(Node* pNode);
	bool    CreateIndexBuffer(Node* pNode);
	bool	CreateIndexBuffer(LodPatch& patch, int iCode);
	bool	UpdateVertexList(Node* pNode);
	bool	CreateVertexBuffer(Node* pNode);
	void	SetNeighborNode(Node* pNode);
public:
	bool    ComputeStaticLodIndex(int iMaxCells);
	bool    LoadObject(std::wstring filename);
	template <typename OutputIterator>
	void	Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	void    Build(HMap* pMap);
	bool    Init();
	Node* CreateNode(Node* pParent, float x, float y, float w, float h);
	void	Buildtree(Node*);
	bool    AddObject(TVector3 pos);
	Node* FindNode(Node* pNode, TVector3 pos);
	Node* FindPlayerNode(TVector3 pos);
	Node* CheckBoxtoPoint(TVector3 pos);
	Box SetBox(Node* pNode);
	TVector2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	bool	SubDivide(Node* pNode);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext, DebugCamera* m_pCamera);
	bool    Release();
public:
	QuadTree();
	virtual ~QuadTree();
};

template<typename OutputIterator>
inline void QuadTree::Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first)
{
	size_t start, stop, n = text.length();

	for (start = text.find_first_not_of(delimiters); 0 <= start && start < n;
		start = text.find_first_not_of(delimiters, stop + 1))
	{
		stop = text.find_first_of(delimiters, start);
		if (stop < 0 || stop > n)
		{
			stop = n;
		}
		*first++ = text.substr(start, stop - start);
	}
}