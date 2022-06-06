#pragma once
#include"Core.h"
#include"Fbxobject.h"
class Sample : public Core
{
public:
	Fbxobject m_FbxObjectA;
	Fbxobject m_FbxObjectB;
	

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

