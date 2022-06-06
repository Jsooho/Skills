#pragma once
#include"Object3D.h"
#include "fbxsdk.h"

struct Mtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* pFbxMtrl;
	wstring m_szTexFileName;
	vector<wstring>		m_szTexFileList;
	vector<Texture*>	m_pTextureList;
};

struct Mesh : public Object3D
{
public:
	int   m_iMtrlRef;
	
	bool CreateIndexData() override;
};


class FbxObj 
{
public:
	FbxManager*		m_FbxManager;
	FbxImporter*	m_FbxImporter;
	FbxScene*		m_FbxScene;
	vector<Mesh*> m_MeshList;
	vector<FbxNode*> m_FbxNodeList;
	vector<Mtrl*> m_MtrlList;
public:
	bool FbxLoad(string filename);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Mesh* pMesh);
	string ParseMaterial(FbxSurfaceMaterial* pSurfaceMtrl);
	int GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv);
public:
	bool Init();

};

