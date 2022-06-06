#pragma once
#include "Shape.h"
class Frustum : public BoxShape
{
public:
	Plane m_Plane[6];
	TVector3 m_vFrustum[8];
public:
	bool Init();
	bool PostRender(ID3D11DeviceContext* pContext);
	bool CreateFrustum(TMatrix& matView, TMatrix& matProj);
public:
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
public:
	bool FrustuminPoint(TVector3* point);
	bool FrustuminSphere(Sphere* sphere);
	bool FrustuminAABB(Box* aabb);
	bool FrustuminOBB(Box* obb);
};

