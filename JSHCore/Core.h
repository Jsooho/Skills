#pragma once
#include "Window.h"
#include "Device.h"
#include "Timer.h"
#include "Input.h"
#include "Sound.h"
#include "DxWrite.h"
#include "Camera.h"
#include "DxState.h"
#include "Std.h"





class Core : public Window
{
public:
	Timer m_Timer;
	Sound m_Sound;
	DxWrite	m_Write;
	DebugCamera m_Camera;
	bool m_bDebugText = false;
	bool m_bWireFrame = false;
private:
	bool GameInit()	override;
	bool GameRun()	override;
	bool GameFrame();
	bool GameRender();
	bool GameRelease()override;
public:

	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void FrameCamera();
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
public:
};

