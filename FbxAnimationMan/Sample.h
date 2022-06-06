#pragma once
#include "Core.h"
#include "Fbxobject.h"
class Sample : public Core
{
	ViewRT          m_ViewRt;
	TVector3	    m_vMovePos;
	Fbxobject		m_FbxObjA;
	Fbxobject		m_FbxObjB;

	TPlane          m_Plane;
	TVector4        m_Vector4;
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
