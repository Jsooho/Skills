#pragma once
#include<Core.h>
#include"Shape.h"
#include"HMap.h"
#include"Light.h"

class Minimap : public Object3D
{
public:
	bool CreateVertexData() override;
};

struct cbDataShadow
{
	TMatrix g_matShadow1;
};

class Sample : public Core
{
public:
	Minimap		m_Minimap;
	Light		m_Light;
	BoxShape	m_Box;
	BoxShape	m_BoxA;
	HMap		m_map;
	ViewRT		m_VRt;
	TMatrix     m_matShadow;
	cbDataShadow m_cbShadow;
	TMatrix			m_matTexture;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

