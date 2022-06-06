#pragma once
#include"Model.h"
#include<fbxsdk.h>

class Fbxobject : public Model
{
public:
	FbxManager* m_FbxManager;
	FbxImporter* m_FbxImporter;
	FbxScene* m_FbxScene;
public:
	bool  LoadObject(std::string filename);
public:
	bool Create(std::wstring vsFile, std::wstring psFile) override;
	bool postRender(ID3D11DeviceContext* pContext, UINT iNumValue) override;
	
public:
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	void  PreProcess(FbxNode* pNode);
	void  ParseNode(FbxNode* pNode);
};

