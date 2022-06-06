#include "LLinkedlisk.h"

LLinkedlisk::LLinkedlisk()
{
	m_pHead = new LNode; // �����Ҵ�
	m_pHead->m_iValue = 5; // Ȯ������ ���� �ְ� �ʱ�ȭ
	m_pHead->m_pNext=nullptr; 


	m_pTail = new LNode;
	m_pTail->m_iValue = 9;
	m_pTail->m_pNext = nullptr;

	m_pHead->m_pNext = m_pTail; //��� ������ ����Ű�� �ִ°� ������ ��������
}
LLinkedlisk::~LLinkedlisk() //�Ҹ���
{
	for (LNode* pNode = m_pHead->m_pNext; pNode != m_pTail;)
	{
		LNode* pNext = pNode->m_pNext;
		delete pNode;
		pNode = pNext;
	}
	
	delete m_pHead;
	delete m_pTail;
	

}
void LLinkedlisk::Addl(LNode* pData) //����
{
	LNode* pTemp = m_pHead->m_pNext; 
	m_pHead->m_pNext = pData;
	pData->m_pNext = pTemp;
}

void LLinkedlisk::Show()//���
{

	for (LNode* pNode = m_pHead->m_pNext; pNode != m_pTail; pNode = pNode->m_pNext)
	{
		cout << pNode->m_iValue << endl;
	}


}
int LLinkedlisk::Del()//����
{
	LNode* pDelNode = m_pHead->m_pNext;
	int ir = pDelNode->m_iValue;
	m_pHead->m_pNext = pDelNode->m_pNext;
	delete pDelNode;
	return ir;

}