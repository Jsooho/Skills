#pragma once
#include<Core.h>
#include"HMap.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Shape.h"

class Sample : public Core
{
public:
	HMap		m_Map;
	DebugCamera m_Camera;
	Camera		m_realCamera;
	SkyBox		m_SkyBox;
	BoxShape	m_BoxObj;
	PlaneShape  m_Planeobj;
public:
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

