#pragma once
#include"Linkedlist.h"
class Student
{
public:
	string m_szName;
	int m_iKor;
	int m_iMat;
	int m_iEng;
	int m_iTotal;
	int m_iAvg;
public:
	Student()
	{
		m_szName = "Test";
		m_iKor = 0;
		m_iMat = 0;
		m_iEng = 0;
		m_iTotal = 0;
		m_iAvg = 0;
	}
	Student(string name, int kor, int mat, int eng)
	{
		m_szName = name;
		m_iKor = kor;
		m_iMat = mat;
		m_iEng = eng;
		m_iTotal = kor + mat + eng;
		m_iAvg = m_iTotal / 3;
	}
	
};


