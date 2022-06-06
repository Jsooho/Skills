#pragma once
#include"Core.h"
#include"Fbxobject.h"
#include"Texture.h"
class Sample : public Core
{
public:
	Fbxobject m_FbxObject;
	Texture m_Texture;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

