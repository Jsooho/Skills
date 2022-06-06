#pragma once
#include"Object3D.h"
#include"DxState.h"
#include <fbxsdk.h>

struct AnimMatrix
{
	TMatrix   matAnimation[255];
};

class HFbxObject : public Object3D
{
public:
	AnimMatrix      m_matAnimMatrix;
	vector<TMatrix> m_AnimationTrack;
	TMatrix m_MatrixWolrd;
	FbxNode* m_pfbxNode = nullptr;
	FbxNode* m_pParentNode = nullptr;
	HFbxObject* m_pParentObj = nullptr;
	wstring m_szTexFileName;

	vector<wstring>		m_szTexFileList;
	vector<Texture*>	m_pTextureList;
	using SubVertex = vector<PNCT_VERTEX>;
	vector<SubVertex>      m_pSubVertexList;
	vector<ID3D11Buffer*>  m_pVBList;
	vector<TMatrix>	m_matBindPoseList;
	int m_iIndex;
public:
	virtual bool PreRender(ID3D11DeviceContext* pContext) override;
	virtual bool CreateVertexBuffer()override;
	virtual bool CreateVertexData()override;
	virtual bool CreateIndexData() override;
	virtual bool PostRender(ID3D11DeviceContext* pContext)override;
	virtual bool Release()override;
};
class FbxLoader
{
public:
	FbxManager*  m_pFbxManager; //sdk 관리 
	FbxImporter* m_pFbxImporter;
	FbxScene*	 m_pFbxScene;
public:
	FbxNode*	 m_RootNode;
	Texture*	 m_Texture;

	vector<HFbxObject*> m_fbxObjList;
	vector<HFbxObject*> m_NodeList;
public:
	
	bool m_bAnimationPlay = false;
	float m_fElpaseTime = 0.0f;
	int m_iAnimationIndex = 0;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
	
public:
	virtual bool FbxLoad(string filename);
	virtual void PreProcess(FbxNode* pNode, HFbxObject* pParentobj);
	virtual void ParseMesh(HFbxObject* pObj);
	virtual string ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl);
	
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertMatrix(FbxMatrix& m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
public:
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList);
public:
	void ParseAnimationNode();
	void ParseAnimationStack(FbxString* szData);
	void ParseAnimation();
	
public:
	 bool Init() ; // 초기화작업
	 bool Frame();
	 bool Release();
};

