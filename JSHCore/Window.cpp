#include "Window.h"
#include <assert.h>
Window* g_pWindow = nullptr;
HWND  g_hWnd;
RECT  g_rtClient;

LRESULT CALLBACK staticWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 메세지 핸들링
    assert(g_pWindow);
    return g_pWindow->WndProc(hWnd, message, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MsgProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
bool Window::InitWindows(
	HINSTANCE hInstance,
	int nCmdShow,
	const WCHAR* strWindowTitle, int iX, int iY)
{
	m_hInstance = hInstance;

    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = staticWndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = CreateSolidBrush(RGB(133,143,108));
    wcex.lpszClassName = L"KGCA";
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // 앞으로 이런 윈도우 생성할테니 등록해줘(운영체제)
    if (!RegisterClassExW(&wcex))
    {
        return false;
    }
    RECT rc = { 0, 0, iX, iY };
    // 작업영역(  타이틀 바/경계선/메뉴/스크롤 바 등의 영역을 제외한 영역), 윈도우 스타일, 메뉴여부
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// 윈도우 생성 함수
	m_hWnd = CreateWindowEx(
        0,
        L"KGCA",
        strWindowTitle,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        rc.right-rc.left,
        rc.bottom-rc.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (m_hWnd == NULL)
    {
        return false;
    }
   
    
    GetWindowRect(m_hWnd, &m_rtWindow);
    GetClientRect(m_hWnd, &m_rtClient);
    
    g_hWnd = m_hWnd;
    g_rtClient = m_rtClient;
    
    // WM_SHOW
    
    ShowWindow(m_hWnd, nCmdShow);

	return true;
}

bool Window::Run()
{
    GameInit();
    MSG msg;
    while (m_bGameRun)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_bGameRun = GameRun();            
        }
    }
    GameRelease();
	return true;
}
bool Window::GameRun()
{
    return true;
}
bool Window::GameInit()
{
    return true;
}
bool Window::GameRelease() 
{
    return true;
}
Window::Window() : m_bGameRun(true)
{
    g_pWindow = this;
}
   