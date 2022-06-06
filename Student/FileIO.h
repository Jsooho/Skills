#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include"Linkedlist.h"
#include"Student.h"

class FileIO
{
public:
	Linkedlist<Student> m_List;
public:
	bool Save();
	bool Load();
};

