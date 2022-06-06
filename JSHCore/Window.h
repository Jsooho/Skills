#pragma once
#include <windows.h>
#include "Device.h"
class Window : public Device
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE m_hInstance;
	HWND m_hWnd;
	bool m_bGameRun;
public:
	bool InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int iX, int iY);
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool Run();
	virtual bool GameInit();
	virtual bool GameRun();
	virtual bool GameRelease();
public:
	Window();
};

