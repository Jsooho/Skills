#pragma once
#include"Hwindow.h"
#include"HCore.h"
#include"HDevice.h"

//디자인 패턴: 어댑터 = 라이브러리에서 내가 필요한 함수가 없을때 상속받아 사용하는 방법
//ex)
// class Xwindow : public Hwindow
// {
//   public:
//     SetWindow(); 만들면 됨
// 메세지 프로시저가 또없다 그러면 위와 똑같이 상속받아서 만들어 사용!!
//
//===============================어댑터 디자인패턴============================
//class XDevice : public HDevice
//{
//public:
//	//1. 디바이스 생성, 스왑체인 생성, 컨텍스트 생성
//	HRESULT CreateDevice(HWND hWnd);
//	//2. 랜더타겟 뷰 생성
//	//3. 뷰포트 세팅
//	//매 프레임 마다 백버퍼 지우기
//	//뿌리기
//	//더블 버퍼링
//	bool Render();
//	//획득한 인터페이스 소멸
//	bool Release();
//};
//============================================================================

class Sample : public HCore
{
public:
	virtual bool Init()   override;
	virtual bool Frame()  override;
	virtual bool Render() override;
	virtual bool Release()override;
};

