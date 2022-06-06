#pragma once
#include"Object3D.h"
struct face
{
	UINT v0, v1, v2;
	TVector3 vNormal;
};
struct HMapInfo
{
	int m_iNumCol;
	int m_iNumRow;
	int m_iNumCellCol;
	int m_iNumCellRow;
	int m_iNumFace;
	int m_iNumVertex;
	float m_fCellDistence;
};

class HMap : public Object3D
{
public:
	HMapInfo m_MapInfo;
	vector<float> m_fHeightList;
	vector<face> m_FaceList;
public:
	bool CreateMap(HMapInfo& info);
	bool CreateHeightMap(std::wstring heightmap);
	
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
public:
	bool Frame() override;
	
};

