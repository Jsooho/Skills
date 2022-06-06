#pragma once
#include<Core.h>
#include"Shape.h"
#include"Select.h"


class Sample : public Core
{
public:
	Box			m_Box[3];
	BoxShape	m_BoxShape[3];
	TMatrix		m_matWorld;
	PlaneShape	m_Plane;
	Select		m_Select;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

