#pragma once
#include "Std.h"
#include "Shape.h"
struct RAY
{
	TVector3		vOrigin;
	TVector3		vDirection;
};

class Select
{
public:
	TVector3	m_vIntersection;
	RAY			m_Ray;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
	TVector3	m_vCenter;
public:
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	void Update();
	bool AABBtoRay(Box* a, RAY* pRay = NULL);
};

