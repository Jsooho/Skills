#pragma once
#include "Model.h"
#include "Texture.h"
#include<fbxsdk.h>
const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};
struct Mtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	Texture			m_Texture;
	std::vector<Mtrl*> m_pSubMtrl;
	Mtrl() {}
	Mtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
struct Layer
{
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};
struct Weight
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WegihtList;
};
struct SkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	std::vector<Weight>   m_VertexList;
};
struct PNCTIW_VERTEX
{
	float   index[4];
	float   weight[4];
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

	AnimMatrix   m_matAnimMatrix;
	ID3D11Buffer* m_pAnimCB = nullptr;
	ID3D11Buffer* m_pIWVertrexBuffer = nullptr;
	// 정점 당 가중치, 인덱스 저장
	std::vector<PNCTIW_VERTEX>       m_WeightList;
	FbxNode* m_pFbxNode = nullptr;
	OBJECTCLASSTYPE     m_ClassType;
	std::wstring		m_szName;
	std::wstring		m_szParentName;
	int					m_iNumLayer = 0;
	std::vector<Layer> m_LayerList;
	int					m_iMtrlRef;
	TMatrix				m_matWorld;
	Mesh* m_pParent = nullptr;
	std::vector<TMatrix> m_AnimationTrack;
	std::vector<Mesh*> m_pSubMesh;
public:
	bool Release() override;
	virtual HRESULT		CreateConstantBuffer()override;
	virtual HRESULT		CreateVertexLayout() override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool		PreRender(ID3D11DeviceContext* pContext)override;
public:
	Mesh();
	virtual ~Mesh();
};

