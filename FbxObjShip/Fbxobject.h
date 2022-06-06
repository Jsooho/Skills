#pragma once
#include"Model.h"
#include"Texture.h"
#include<fbxsdk.h>
struct Mtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	Texture m_Texture;


	std::vector<Mtrl*> m_SubMtrl;

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
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};
struct Mesh : public Model
{
	int	m_iNumLayer;
	std::vector<Layer> m_LayerList;
	int	m_iMtrlRef;
	TMatrix	m_matWorld;
	std::vector<Mesh*> m_pSubMesh;
	bool Release() override
	{
		Model::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			SAFE_DEL(data);
		}
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
	
	bool  LoadObject(std::string filename);
	void  PreProcess(FbxNode* pNode);
	void  ParseNode(FbxNode* pNode, Mesh* pMesh);
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertMatrix(FbxMatrix& m);
	int GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void LoadMaterial(Mtrl* pMtrl);
public:
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
};

