#pragma once
#include<Core.h>
#include"FbxObj.h"
class Sample : public Core
{
public:
	FbxLoader	m_fbxLoader;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

