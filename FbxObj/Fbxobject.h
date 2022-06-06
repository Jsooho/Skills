#pragma once
#include"Object3D.h"
#include<fbxsdk.h>
struct Mtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* pFbxMtrl;
	Texture m_Texture;
	std::vector<Mtrl> m_SubMtrl;
	void Release()
	{
		m_Texture.Release();
		for (auto& data : m_SubMtrl)
		{
			data.Release();
		}
	}
};
struct Mesh : public Object3D
{
	int   m_iMtrlRef;
	virtual bool CreateIndexData()
	{
		return true;
	}
};
class Fbxobject
{
public:
	std::vector<FbxNode*>  m_pFbxNodeList;
	std::vector<Mtrl*>  m_pFbxMaterialList;
	std::vector<Mesh*> m_pMeshList;
	CB_DATA	m_cbData;

public:
	FbxManager* m_FbxManager;
	FbxImporter* m_FbxImporter;
	FbxScene* m_FbxScene;
public:
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
	void SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
public:
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	bool  LoadObject(std::string filename);
	void  PreProcess(FbxNode* pNode);
	void  ParseNode(FbxNode* pNode, Mesh* pMesh);
public:
	int GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void LoadMaterial(Mtrl* pMtrl);
};

