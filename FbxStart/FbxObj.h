#pragma once
#include"Object3D.h"
#include <fbxsdk.h>

struct HLayer
{
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};

class HFbxObject : public Object3D
{
public:
	FbxNode* m_pfbxNode = nullptr;
	FbxNode* m_pParentNode = nullptr;
	HFbxObject* m_pParentObj = nullptr;
	wstring m_szTexFileName;

public:
	vector<HLayer> m_LayerList;
	vector<wstring>		m_szTexFileList;
	vector<Texture*>	m_pTextureList;
	virtual bool CreateIndexData() override;
	
};
class FbxLoader
{
public:
	FbxManager*  m_pFbxManager; //sdk 관리 
	FbxImporter* m_pFbxImporter;
	FbxScene*	 m_pFbxScene;
	FbxNode*	 m_RootNode;
	Texture*	 m_Texture;
	vector<HFbxObject*> m_fbxObjList;


	
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	TMatrix ConvertMatrix(FbxMatrix& m);
	virtual bool FbxLoad(string filename);
	virtual void PreProcess(FbxNode* pNode, FbxNode* pParentNode);
	virtual void ParseMesh(HFbxObject* pObj);
	virtual string ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
public:
	 bool Init() ; // 초기화작업
	 bool Render(ID3D11DeviceContext* pContext);
	 bool Release();
};

