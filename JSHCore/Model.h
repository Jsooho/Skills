#pragma once
#include"Std.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"
struct P_VECTOR
{
	TVector3 pos;
	P_VECTOR(TVector3 p)
	{
		pos = p;
	}
};
struct PS_VECTOR
{
	TVector3 pos;
	TVector4 Color;
	PS_VECTOR(TVector3 p, TVector4 c)
	{
		pos = p;
		Color = c;
	}
	PS_VECTOR()
	{
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;
		Color.w = 1.0f;
	}

};
struct PCT_VERTEX
{
	TVector3 pos;
	TVector4 color;
	TVector2 tex;
	PCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PNCT_VERTEX
{
	TVector3 pos;
	TVector3 normal;
	TVector4 color;
	TVector2 tex;
	PNCT_VERTEX(TVector3 p, TVector3 n, TVector4 c, TVector2 t)
	{
		pos = p;
		normal = n;
		color = c;
		tex = t;
	}
	PNCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct CB_DATA // vs,ps
{
	TMatrix matrixWorld;
	TMatrix matrixView;
	TMatrix matrixProjection;
	TMatrix matNormal;
	TVector4 vColor;
	
	TVector4 vValue;
};
struct LightData
{
	TMatrix	   matWorldInverse;
	TVector4   vLightDir;
};
class Model
{
public:
	wstring						m_csName;
public:
	Texture*					m_Texture = nullptr;
	Texture*					m_MaskTexture = nullptr;
	Shader*						m_pVs = nullptr;
	Shader*						m_pPs = nullptr;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	std::vector<PNCT_VERTEX> m_vertexList;
	ID3D11Buffer* m_VertexBuffer;

	std::vector<DWORD> m_indexList;
	ID3D11Buffer* m_IndexBuffer;

	CB_DATA m_CbData;
	LightData m_CbLight;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pLightConstantBuffer = nullptr;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11DeviceContext* m_pContext = nullptr;

	ID3D11BlendState* m_AlphaBlend;
	ID3D11BlendState* m_AlphaBlendDisable;

	UINT m_iVertexSize=0;
	UINT m_iNumIndex=0;

	ID3DBlob* m_pVSBlob = nullptr;
	Shader* m_pMainPs = nullptr;

	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProjection;

public:
	virtual void SetContext(ID3D11DeviceContext* pContext);
	virtual bool LoadTexture(std::wstring szTextureName, std::wstring szMaskName);
	virtual void SetMatrix(TMatrix* pMatrixWorld, TMatrix* pMatrixView, TMatrix* pProjection);
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool CreateConstantData();
	virtual bool Create(ID3D11DeviceContext* pContext, wstring Colorname, wstring maskname, wstring filename);
	virtual bool  LoadObject(std::wstring filename);

	virtual bool CreateConstantBuffer();
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateVertexLayout();

	virtual bool CreateVertexShader(wstring filename);
	virtual bool CreatePixelShader(wstring filename);
public:
	
	
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	static ID3DBlob* LoadShaderBlob(std::wstring vs, std::string function, std::string version);

public:
	Model();
	virtual ~Model();
};

