#pragma once
#include <iostream>
using namespace std;
struct LNode
{
	int m_iValue;
	LNode* m_pNext;
};


class LLinkedlisk
{
public:
	LNode* m_pHead;
	LNode* m_pTail;
public:
	void Addl(LNode* pData);
	void Show();
	int Del();
	
public:
	LLinkedlisk();
	~LLinkedlisk();

};

