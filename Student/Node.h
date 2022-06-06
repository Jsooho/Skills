#pragma once
#include<iostream>

using namespace std;

template <class J>
class LNode
{
public:
	J*         m_iData;
	LNode*     m_Next;
};