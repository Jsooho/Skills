#pragma once
#include<Core.h>
#include"HMap.h"
#include "Camera.h"
#include "SkyBox.h"

class Sample : public Core
{
public:
	HMap		m_Map;
	DebugCamera m_Camera;
	SkyBox		m_SkyBox;
public:
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

