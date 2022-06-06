#pragma once
#include"Model.h"
#include"Texture.h"
#include<fbxsdk.h>


const enum OBJECTCLASSTYPE 
{
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};
struct Mtrl
{
	FbxNode*            m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	Texture             m_Texture;
	std::vector<Mtrl*>  m_SubMtrl;
	Mtrl() {}
	Mtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}

	void Release()
	{
		m_Texture.Release();
		for (auto data : m_SubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
struct Layer
{
	FbxLayerElementUV*          pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal*      pNormal;
	FbxLayerElementMaterial*    pMaterial;
};
struct Weight //뼈대
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WegihtList;
};
struct SkinData //스킨
{
	std::vector<FbxNode*>  m_MatrixList;
	std::vector<Weight>   m_VertexList;
};
struct PNCTIW_VERTEX 
{
	int   iIndex[4];
	float weight[4];
};
struct AnimMatrix
{
	TMatrix   matAnimation[255];
};
class Mesh : public Model
{
public:
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<Mesh*>     m_pMeshLinkList;
	std::vector<TMatrix>	m_matBindPoseList;

	std::vector<int>  m_iBoneList;
	AnimMatrix   m_matAnimMatrix;
	ID3D11Buffer* m_pAnimCB;
	ID3D11Buffer* m_pIWVertrexBuffer;
	std::vector<PNCTIW_VERTEX> m_weightList; // 정점 각자 가중치,인덱스를 저장하는 변수
	FbxNode* m_pFbxNode = nullptr;
	OBJECTCLASSTYPE     m_ClassType;
	std::wstring		m_szName;
	std::wstring		m_szParentName;
	int	m_iNumLayer;
	std::vector<Layer> m_LayerList;
	int	m_iMtrlRef;
	Mesh* m_pParent;
	TMatrix	m_matWorld;
	std::vector<TMatrix> m_AnimationTrack;
	std::vector<Mesh*> m_pSubMesh;
		
public:
	virtual HRESULT		CreateConstantBuffer()override;
	virtual HRESULT		CreateVertexLayout() override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool        PreRender(ID3D11DeviceContext* pContext) override;
	virtual bool        Release()override;
public:
	Mesh();
	virtual ~Mesh();
	
};

