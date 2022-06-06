#pragma once
#include"Mesh.h"

class Fbxobject 
{
public:
	std::vector<TMatrix>	m_matBindPoseList;
	std::vector<FbxNode*>  m_pFbxNodeList;
	std::vector<Mtrl*>  m_pFbxMaterialList;
	std::vector<Mesh*> m_pMeshList;
	CB_DATA	m_cbData;

public:
	TMatrix			m_matWorld;
	AnimMatrix      m_matAnimMatrix;
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
	Mesh* GetFindInedx(FbxNode* pNode);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
	void SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
public:
	
	bool    LoadObject(std::string filename, std::string shaderName);
	void    PreProcess(FbxNode* pNode);
	void    ParseMesh(FbxNode* pNode, Mesh* pMesh);
	void    ParseNode(FbxNode* pNode, Mesh* pParentMesh);
	TMatrix ParseTransform(FbxNode* pNode, TMatrix& matrixParent);
public:
	void ParseAnimationNode();
	void ParseAnimationStack(FbxString* szData);
	void ParseAnimation();
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, Mesh* pMesh, SkinData* pSkindata);
public:
	TMatrix DxConvertMatrix(TMatrix m);
	TMatrix ConvertMatrix(FbxMatrix& m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void    LoadMaterial(Mtrl* pMtrl);
public:
	FbxVector4  ReadNormal(const FbxMesh* Mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* Mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
};

