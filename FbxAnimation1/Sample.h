#pragma once
#include"Core.h"
#include"Fbxobject.h"
#include"Texture.h"
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

