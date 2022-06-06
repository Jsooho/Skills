#pragma once
#include"Node.h"


template <class J>
class Linkedlist
{
public:
	LNode<J>* m_Head;
	LNode<J>* m_Tail;

public:
	int iCount;
public:
	void  Init();
	void  Add(J*);
	void  Delpop();
	void  Show();
	void  DelAll();
	int   check();
	
	Linkedlist();
	~Linkedlist();
};
template <class J>
Linkedlist<J>::Linkedlist()
{

}
template <class J>
Linkedlist<J>::~Linkedlist()
{

	delete m_Tail;
	m_Tail = nullptr;
	delete m_Head;
	m_Head = nullptr;
	
}
template <class J>
void Linkedlist<J>::Init()
{
	m_Head = new LNode<J>;
	//m_Head->m_iData = 100;
	m_Head->m_Next = nullptr;
	m_Head->m_iData = 0;
	m_Tail = new LNode<J>;
	//m_Tail->iData = 700;
	m_Tail->m_Next = nullptr;
	m_Head->m_Next = m_Tail;
	m_Tail->m_iData = 0;
	iCount = 0;

}
template <class J>
void Linkedlist<J>::Add(J* val)
{
	if (m_Head->m_Next != nullptr)
	{
		LNode<J>* New = new LNode<J>;
		LNode<J>* temp = m_Head->m_Next; // Tail ����!
		m_Head->m_Next = New;
		New->m_Next = temp;
		New->m_iData = val;
	}
}
template <class J>
void Linkedlist<J>::Delpop()
{
	if (m_Head->m_Next != m_Tail)
	{
		if (m_Head->m_Next != nullptr)
		{
			LNode<J>*  del = m_Head->m_Next;
			m_Head->m_Next = del->m_Next;
			delete del;
		}
	}
	else
	{
		cout << "������ ������ ����!" << endl;
	}
}
template <class J>
void Linkedlist<J>::Show()
{
	
	for (LNode<J>* pNode = m_Head->m_Next; pNode != m_Tail; pNode = pNode->m_Next)
	{
		
		cout << "�̸�:" << pNode->m_iData->m_szName << " ";
		cout << "����:" << pNode->m_iData->m_iKor << " " ;
		cout << "����:" << pNode->m_iData->m_iMat << " " ;
		cout << "����:" << pNode->m_iData->m_iEng << " " ;
		cout << "�հ�:" << pNode->m_iData->m_iTotal << " ";
		cout << "���:" << pNode->m_iData->m_iAvg << " " << endl;
	}
	if (m_Head->m_Next == m_Tail)
	{
		cout << "������ ����!" << endl;
	}
}

template <class J>
int Linkedlist<J>::check()
{
	int count = 0;
	for (LNode<J>* pNode = m_Head->m_Next; pNode != m_Tail; pNode = pNode->m_Next)
	{
		count++;
	}
	return count;
}

template<class J>
void Linkedlist<J>::DelAll()
{
	
		while (m_Head->m_Next != m_Tail)
		{
			LNode<J>* del = m_Head->m_Next;
			m_Head->m_Next = del->m_Next;
			delete del;
		}
	
}