#include "Timer.h"

float g_fSecPerFrame = 0.0f;
float g_fGameTimer = 0.0f;

bool Timer::Init()
{
	QueryPerformanceCounter(&m_Frame); // 이전 틱
	
	return true;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_Current);//현재 틱
	m_fSecPerFrame =static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart)/ static_cast<float>(m_Frequency.QuadPart);//속도측정
	g_fSecPerFrame = m_fSecPerFrame;

	static float fpsTime = 0.0f;
	m_iTmpCounter++;
	fpsTime += m_fSecPerFrame;
	if (fpsTime >= 1.0f)
	{
		m_iFPS = m_iTmpCounter;
		m_iTmpCounter = 0;
		fpsTime -= 1.0f;		
	}
	
	m_fGameTimer += m_fSecPerFrame;
	g_fGameTimer = m_fGameTimer;
	m_Frame = m_Current;
	return true;
}
bool Timer::Render()
{
	_stprintf_s(m_szTimerString,
		L"FPS=%d:Timer=%10.4f",
		m_iFPS,
		m_fGameTimer);
	//OutputDebugString(timerString);
	return true;
}
bool Timer::Release()
{
	return false;
}
Timer::Timer()
{
	m_iTmpCounter = 0;
	m_iFPS = 0;
	m_fSecPerFrame = 0.0f;
	m_fGameTimer = 0.0f;
	QueryPerformanceFrequency(&m_Frequency); //주파수에 따라 1초당 진행되는 틱 수 계산
}
Timer::~Timer()
{

}