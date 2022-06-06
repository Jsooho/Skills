#pragma once
#include"Object3D.h"

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
		for (auto data : m_SubMtrl)
		{
			data->Release();
			delete data;
			m_Texture.Release();
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
struct Mesh : public Object3D
{
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
	bool Release() override
	{
		Object3D::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			SAFE_DEL(data);
		}
		return true;
	}
	bool CreateIndexData()
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
	bool m_bAnimationPlay = false;
	float m_fElpaseTime = 0.0f;
	int m_iAnimationIndex = 0;
	FbxManager* m_FbxManager;
	FbxImporter* m_FbxImporter;
	FbxScene* m_FbxScene;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
public:
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
	void SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
public:
	
	bool  LoadObject(std::string filename);
	void  PreProcess(FbxNode* pNode);
	void  ParseMesh(FbxNode* pNode, Mesh* pMesh);
	void  ParseNode(FbxNode* pNode, Mesh* pParentMesh);
	TMatrix ParseTransform(FbxNode* pNode, TMatrix& matrixParent);
public:
	void ParseAnimationNode(FbxNode* pNode, Mesh* pMesh);
	void ParseAnimationStack(FbxString* szData);
	void ParseAnimation();
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertMatrix(FbxMatrix& m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	int GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void LoadMaterial(Mtrl* pMtrl);
public:
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
};

