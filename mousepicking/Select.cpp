#include "Select.h"

void Select::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	Update();
}

void Select::Update()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	// Compute the vector of the pick ray in screen space
	TVector3 v;
	// ��� 1
	/*v.x =  ( ( ( 2.0f * ptCursor.x ) / TBASIS::g_iClientWidth  ) - 1 );
	v.y = -( ( ( 2.0f * ptCursor.y ) / TBASIS::g_iClientHeight ) - 1 );
	v.z =  1.0f;
	D3DXVec3TransformCoord( &v, &v, &m );*/
	// ��� 2


	v.x = (((2.0f * ptCursor.x) / g_iClientWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / g_iClientHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	// ���尡 ��ȯ�Ǿ��ٸ� ���ؽ����۵� ���ؾ� �Ѵ�. �׷��� ��ŷ���ؽ��� ��������� �������� ����
	// ���ؽ�����(��Į��ǥ)���� ���� ����ϱ� ������ ������ ��ǥ�� �ٸ��� �ȴ�. 
	// �ᱹ ���ؽ������� ������ ��ķ� ���ؼ� ������ ����ϰų�( ����ĸ� �ι����Ѵ�. )
	// ������ ��ȯ�ؾ߸� �ȴ�.( ����ĵ� �ι��� �ؾ��Ѵ�.)
	// ���⼭�� ������ ��ȯ�Ͽ� ������ ��ǥ��� ó���ϰ� �ִ�.
	// Ư���� ��찡 �ƴ϶�� ������ĸ� ������ķ� ����ض�.
	TMatrix mWorldView = m_matWorld * m_matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	// ��� 1
	m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);

	// ��� 2		
	//m_Ray.vDirection.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	//m_Ray.vDirection.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	//m_Ray.vDirection.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	//D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
	//m_Ray.vOrigin.x = m._41;
	//m_Ray.vOrigin.y = m._42;
	//m_Ray.vOrigin.z = m._43;
}

bool Select::AABBtoRay(Box* a, RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = 0;
	float t_max = 999999.0f;
	for (int i = 0; i < 3; i++)
	{
		// �࿡ ������ ���
		if (abs(pRay->vDirection[i]) < 0.0001f)
		{
			if (pRay->vOrigin[i] < a->vMin[i] || pRay->vOrigin[i] > a->vMax[i])
				return false;
		}
		else
		{
			float denom = 1.0f / pRay->vDirection[i];
			float t1 = (a->vMin[i] - pRay->vOrigin[i]) * denom;
			float t2 = (a->vMax[i] - pRay->vOrigin[i]) * denom;
			if (t1 > t2)
			{
				swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
	}
	m_vIntersection = pRay->vOrigin + pRay->vDirection * t_min;
	return true;
}