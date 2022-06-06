#pragma once
#include"Std.h"
#include"Frustum.h"
class Camera : public Frustum
{
public:
	float m_pSpeed;
	TVector3 m_vCameraPos;
	TVector3 m_vCameraTarget;
	TVector3 m_vLook;
	TVector3 m_vSide;
	TVector3 m_vUp;
	float m_fYaw = 0.0f;
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fRadius = 10.0f;
public:
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProjection;
public:
	bool UpdateVector();
	bool UpDate(TVector4 vDirValue);
public:
	virtual void CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp = TVector3(0,1,0));
	virtual TMatrix CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
public:
	virtual bool Init();
	virtual bool Frame();
	//virtual bool Render();
	virtual bool Release();
public:
	Camera();
	virtual ~Camera();
};

class DebugCamera : public Camera
{
public:
	virtual bool Frame() override;
	virtual TMatrix DebugUpdate(TVector4 vValue);
};



