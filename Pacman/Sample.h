#pragma once
#include"Hwindow.h"
#include"HCore.h"
#include"HDevice.h"

//������ ����: ����� = ���̺귯������ ���� �ʿ��� �Լ��� ������ ��ӹ޾� ����ϴ� ���
//ex)
// class Xwindow : public Hwindow
// {
//   public:
//     SetWindow(); ����� ��
// �޼��� ���ν����� �Ǿ��� �׷��� ���� �Ȱ��� ��ӹ޾Ƽ� ����� ���!!
//
//===============================����� ����������============================
//class XDevice : public HDevice
//{
//public:
//	//1. ����̽� ����, ����ü�� ����, ���ؽ�Ʈ ����
//	HRESULT CreateDevice(HWND hWnd);
//	//2. ����Ÿ�� �� ����
//	//3. ����Ʈ ����
//	//�� ������ ���� ����� �����
//	//�Ѹ���
//	//���� ���۸�
//	bool Render();
//	//ȹ���� �������̽� �Ҹ�
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

