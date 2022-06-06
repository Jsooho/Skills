#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)

bool Sample::Init()
{
    m_FbxObj.Init();
    m_FbxObj.FbxLoad("../../Data/Object/fbx/MultiCameras.FBX");

	Shader* pVShader = I_Shader.CreateVertexShader(L"FbxShader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(L"FbxShader.hlsl", "PS");
	for (int iObj = 0; iObj < m_FbxObj.m_ObjList.size(); iObj++)
	{
		m_FbxObj.m_ObjList[iObj]->Init();
		m_FbxObj.m_ObjList[iObj]->m_Texture = I_Texture.Load(m_FbxObj.m_ObjList[iObj]->m_szTexFileName);
		m_FbxObj.m_ObjList[iObj]->m_pVs = pVShader;
		m_FbxObj.m_ObjList[iObj]->m_pPs = pPShader;
		//m_FbxObj.m_ObjList[iObj]->SetPosition(TVector3(0.0f, 1.0f, 0.0f));
		if (!m_FbxObj.m_ObjList[iObj]->Create(m_pImmediateContext, L"", L"", L""))
		{
			return false;
		}
	}

    return true;
}

bool Sample::Frame()
{
    return true;
}

bool Sample::Render()
{
	for (int iObj = 0; iObj < m_FbxObj.m_ObjList.size(); iObj++)
	{
		m_FbxObj.m_ObjList[iObj]->SetMatrix(&m_FbxObj.m_ObjList[iObj]->m_matWorld,
			&m_Camera.m_matView,
			&m_Camera.m_matProjection);
		m_FbxObj.m_ObjList[iObj]->Render(m_pImmediateContext);
	}
    return true;
}

bool Sample::Release()
{
	m_FbxObj.Release();
    return true;
}
