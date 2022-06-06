#pragma once
#include<Core.h>
#include "FbxObj.h"
class Sample : public Core
{
public:
	FbxObj m_FbxObj;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

