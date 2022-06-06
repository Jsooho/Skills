#pragma once
#include"HVector.h"
struct Matrix3
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class HMatrix : public Matrix3
{
public:
	void Identity()
	{
		
			_11 = 1; _12 = 0; _13 = 0; _14 = 0;
			_21 = 0; _22 = 1; _23 = 0; _24 = 0;
			_31 = 0; _32 = 0; _33 = 1; _34 = 0;
			_41 = 0; _42 = 0; _43 = 0; _44 = 1;
	}
		
			
		
	HMatrix()
	{

		Identity();

	}
	static HMatrix RotationZ(float fRadian)
	{
		HMatrix matRet;
		float fCos = cosf(fRadian);
		float fSin = sinf(fRadian);
		matRet._11 = fCos; matRet._12 = fSin;
		matRet._21 = -fSin; matRet._22 = fCos;
		return matRet;
	}
	HMatrix Transpose()
	{
		HMatrix Matrix;
		Matrix._11 = _11; Matrix._12 = _21; Matrix._13 = _31; Matrix._14 = _41;
		Matrix._21 = _12; Matrix._22 = _22; Matrix._23 = _32; Matrix._24 = _42;
		Matrix._31 = _13; Matrix._32 = _23; Matrix._33 = _33; Matrix._34 = _43;
		Matrix._41 = _14; Matrix._42 = _24; Matrix._43 = _34; Matrix._44 = _44;
		return Matrix;
	}
	static HMatrix ViewLookAt(HVector3& vPos,
		HVector3& vTarget,
		HVector3& vUp)//up은 임시벡터, x벡터를 만들기위한 것
	{
		HMatrix matRet;
		HVector3 vZ = (vTarget - vPos).Normal();//방향벡터
		HVector3 vX = (vUp ^ vZ).Normal();
		HVector3 vY = (vZ ^ vX).Normal();
		matRet._11 = vX.x; matRet._21 = vX.y; matRet._31 = vX.z;
		matRet._12 = vY.x; matRet._22 = vY.y; matRet._32 = vY.z;
		matRet._13 = vZ.x; matRet._23 = vZ.y; matRet._33 = vZ.z;
		matRet._41 = -(vPos | vX);
		matRet._42 = -(vPos | vY);
		matRet._43 = -(vPos | vZ);
		return matRet;
	}
	static HMatrix PerspectiveFovLH(
		float fNearPlane, float fFarPlane,
		float fovy, float Aspect) // w / h =1.33
	{
		float    h, w, Q;
		h = 1 / tan(fovy * 0.5f);  // 1/tans(x) = cot(x)
		w = h / Aspect;
		Q = fFarPlane / (fFarPlane - fNearPlane);
		HMatrix ret;
		ZeroMemory(&ret, sizeof(HMatrix));
		ret._11 = w;
		ret._22 = h;
		ret._33 = Q;
		ret._43 = -Q * fNearPlane;
		ret._34 = 1;
		return ret;
	}
};

