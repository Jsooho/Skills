#pragma once
#include "Model.h"
class Object3D : public Model
{
public:
	TVector3			m_vPos;
	TVector3			m_vDirection;
	TVector4			m_vColor;
	TMatrix				m_matWorld;
	TVector3			m_vSide; 	
	TVector3			m_vUp; 
	TVector3			m_vLook; 
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
	virtual void    FadeIn();
	virtual void    FadeOut();
public:
	virtual void		AddPosition(TVector3 vPos);
	virtual void		SetPosition(TVector3 vPos);
public:
	virtual bool		CreateVertexData() override;
	virtual bool		CreateIndexData() override;
	virtual bool		Frame() override;
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
	virtual void  UpdateData() {}
	
public:
	Object3D();
	virtual ~Object3D();
};

