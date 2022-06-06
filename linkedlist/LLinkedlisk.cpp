#include "LLinkedlisk.h"

LLinkedlisk::LLinkedlisk()
{
	m_pHead = new LNode; // 동적할당
	m_pHead->m_iValue = 5; // 확인위해 값을 넣고 초기화
	m_pHead->m_pNext=nullptr; 


	m_pTail = new LNode;
	m_pTail->m_iValue = 9;
	m_pTail->m_pNext = nullptr;

	m_pHead->m_pNext = m_pTail; //헤드 다음이 가르키고 있는게 꼬리로 연결해줌
}
LLinkedlisk::~LLinkedlisk() //소멸자
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
void LLinkedlisk::Addl(LNode* pData) //삽입
{
	LNode* pTemp = m_pHead->m_pNext; 
	m_pHead->m_pNext = pData;
	pData->m_pNext = pTemp;
}

void LLinkedlisk::Show()//출력
{

	for (LNode* pNode = m_pHead->m_pNext; pNode != m_pTail; pNode = pNode->m_pNext)
	{
		cout << pNode->m_iValue << endl;
	}


}
int LLinkedlisk::Del()//삭제
{
	LNode* pDelNode = m_pHead->m_pNext;
	int ir = pDelNode->m_iValue;
	m_pHead->m_pNext = pDelNode->m_pNext;
	delete pDelNode;
	return ir;

}