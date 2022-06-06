#pragma once
#include"Std.h"
class Light
{
public:
	TVector3 m_vPos;
	TVector3 m_InitPos;
	TVector3 m_vTarget;
	TVector3 m_vDir;
	TMatrix  m_matWorld;
	TMatrix  m_matView;
	TMatrix  m_matProj;
public:
	void Set(TVector3 vPos, TVector3 vTarget);
	bool Frame();
};


