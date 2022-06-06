#pragma once
#include"Core.h"
#include"Fbxobject.h"

class Sample : public Core
{
public:
	std::vector<Fbxobject*> m_pObjectList;
	TVector3	m_vMovePos;
	Fbxobject m_FbxObjectA;
	Fbxobject m_FbxObjectB;
	

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};

