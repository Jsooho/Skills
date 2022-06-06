#pragma once
#include "Object3D.h"
#include <fbxsdk.h>



struct Weight
{
	vector<int>     m_IndexList;
	vector<float>   m_WegihtList;
};
struct IW_VERTEX
{
	float iIndex[4];
	float fWeight[4];
};

struct AnimMatrix
{
	TMatrix matAnimation[255];
};

struct SkinData
{
	std::vector<FbxNode*>		m_MatrixList;
	std::vector<Weight>		m_IWVertexList;
};

class HFbxObject : public Object3D
{
public:
	FbxNode*	m_pfbxNode = nullptr;
	FbxNode*	m_pParentNode = nullptr;
	HFbxObject* m_pParentObj = nullptr;
	wstring		m_szTexFileName;
	vector<FbxNode*> m_fbxNodeList;
	vector<TMatrix>	m_matBindPoseList;
	int			m_iIndex;
public:
	AnimMatrix		m_matAnimMatrix;
	vector<TMatrix> m_AnimationTrack;
	vector<IW_VERTEX> m_WeightList;
	ID3D11Buffer* m_pAnimCB = nullptr;
	ID3D11Buffer* m_pIWVertrexBuffer = nullptr;
public:
	vector<wstring>		m_szTexFileList;
	vector<Texture*>	m_pTextureList;
	using SubVertex = vector<PNCT_VERTEX>;
	vector<SubVertex>      m_pSubVertexList;
	vector<ID3D11Buffer*>  m_pVBList;

public:
	bool CreateIndexData() override;
	bool CreateVertexData() override;
	bool CreateConstantBuffer() override;
	bool CreateVertexBuffer() override;
	bool CreateVertexLayout() override;
};

class FbxObj
{
public:
	FbxManager*		m_FbxManager;
	FbxImporter*	m_FbxImporter;
	FbxScene*		m_FbxScene;
	FbxNode*		m_RootNode;
public:
	vector<HFbxObject*> m_ObjList;

public:
	bool FbxLoad(string filename);
	bool PreProcess(FbxNode* pNode, FbxNode* pParentNode);
	void ParseMesh(HFbxObject* pObj);
	string ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl);
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	TMatrix ConvertMatrix(FbxMatrix& m);
public:
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, HFbxObject* pObject , SkinData* pSkindata);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList);

public:
	bool Init();
	bool Release();
};

