#pragma once
#include"Mesh.h"

class Fbxobject
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
public:
	TMatrix			m_matWorld;
	AnimMatrix     m_matAnimMatrix;
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime = 0.0f;
	int		m_iAnimIndex = 0;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<Mtrl*>		m_pFbxMaterialList;
	std::vector<Mesh*>		m_pMeshList;
	CB_DATA					m_cbData;
	/// <summary>
	/// 가상함수 리스트
	/// </summary>
public:
	Mesh* GetFindInedx(FbxNode* pNode);
	bool		LoadObject(std::string filename,
		std::string shaderName);
	bool		Frame();
	bool		Release();
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
public:
	void	SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(Mtrl* pMtrl);
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode, Mesh* pMesh);
	void	ParseMesh(FbxNode* pNode, Mesh* pMesh);
	TMatrix   ParseTransform(FbxNode* pNode, TMatrix& matParent);
public:
	void	ParseAnimationNode();
	void	ParseAnimation();
	void	ParseAnimStack(FbxString* szData);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, Mesh* pMesh, SkinData* pSkindata);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
		DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
public:
	void SetPixelShader(ID3D11PixelShader* ps);
};


