#pragma once
#include "Std.h"
// 키보드, 마우스 ->윈도우API 사용
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class Input : public Singleton<Input>
{
	friend class Singleton<Input>;
private:
	DWORD m_dwKeyState[256];
public:
	POINT m_ptBeforePos;
	POINT m_ptPos;
	POINT m_pDragDown;
	POINT m_pDrag;
	bool m_bDrag;
	bool m_bMove;
	int m_iWheel;
	BYTE		m_MouseState[3];
public:
	DWORD   GetKey(DWORD dwKey);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	void OnMove(int iX, int iY);
	void OnBegin(int iX, int iY);
	void OnEnd();
private:
	Input();
public:
	~Input();
};

#define g_Input Input::Get()