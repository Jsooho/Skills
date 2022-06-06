#pragma once
#include "Core.h"
#include "Fbxobject.h"
#include "Shape.h"
#include "Light.h"
class Minimap : public PlaneShape
{
public:
	bool LoadTexture()
	{
		m_Texture.LoadTexture(L"../../Date/object/Chrome1.bmp");
	}
	bool CreateVertexData()
	{
		m_vertexList.resize(4);

		m_vertexList[0].pos = TVector3(-1.0f, 1.0f, 0.0f);
		m_vertexList[0].normal = TVector3(0.0f, 0.0f, -1.0f);
		m_vertexList[0].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexList[0].tex = TVector2(0.0f, 0.0f);

		m_vertexList[1].pos = TVector3(-0.5f, 1.0f, 0.0f);
		m_vertexList[1].normal = TVector3(0.0f, 0.0f, -1.0f);
		m_vertexList[1].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexList[1].tex = TVector2(1.0f, 0.0f);


		m_vertexList[2].pos = TVector3(-1.0f, 0.5f, 0.0f);
		m_vertexList[2].normal = TVector3(0.0f, 0.0f, -1.0f);
		m_vertexList[2].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexList[2].tex = TVector2(0.0f, 1.0f);


		m_vertexList[3].pos = TVector3(-0.5f, 0.5f, 0.0f);
		m_vertexList[3].normal = TVector3(0.0f, 0.0f, -1.0f);
		m_vertexList[3].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexList[3].tex = TVector2(1.0f, 1.0f);
		return true;
	}
};
struct cbDataShadow
{
	TMatrix g_matShadow1;
};
class Sample : public Core
{
	cbDataShadow    m_CbShadow;
	Light           m_Light;
	Minimap			m_MiniMap;
	PlaneShape      m_MapObj;
	ViewRT          m_ViewRt;
	Fbxobject		m_FbxObjB;
	TMatrix         m_matTex;
	TMatrix         m_MatShadow;
	ID3D11PixelShader* m_pPSShadow = nullptr;

public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
	
public:
	Sample();
	virtual ~Sample();
};
