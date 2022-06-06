#pragma once
#include<Core.h>
#include"Shape.h"
#include"HMap.h"
#include"Skybox.h"
class Sample : public Core
{
public:
	HMap		m_Map;
	BoxShape	m_Box;
	Skybox		m_Skybox;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

