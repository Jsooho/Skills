#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)

bool Sample::Init()
{
    m_fbxLoader.Init();
    //m_fbxLoader.FbxLoad("../../Data/object/BoxAnim.FBX");
	m_fbxLoader.FbxLoad("../../Data/object/fbx/Ship.FBX");
	
	Shader* pVShader = I_Shader.CreateVertexShader(L"FbxShader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(L"FbxShader.hlsl", "PS");
	for (int iObj = 0; iObj < m_fbxLoader.m_fbxObjList.size(); iObj++)
	{
		m_fbxLoader.m_fbxObjList[iObj]->Init();
		m_fbxLoader.m_fbxObjList[iObj]->m_Texture = I_Texture.Load(m_fbxLoader.m_fbxObjList[iObj]->m_szTexFileName);
		m_fbxLoader.m_fbxObjList[iObj]->m_pVs = pVShader;
		m_fbxLoader.m_fbxObjList[iObj]->m_pPs = pPShader;
		//m_fbxLoader.m_fbxObjList[iObj]->SetPosition(TVector3(0, 0, 0));
		if (!m_fbxLoader.m_fbxObjList[iObj]->Create(m_pImmediateContext,L"",L"",L""))
		{
			return false;
		}
	}
    
    return true;
}

bool Sample::Frame()
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_fbxLoader.m_bAnimationPlay = !m_fbxLoader.m_bAnimationPlay;
	}
	m_fbxLoader.Frame();


    return true;
}

bool Sample::Render()
{
	for (int iObj = 0; iObj < m_fbxLoader.m_fbxObjList.size(); iObj++)
	{
		TVector3 vLight(10.0f, 10.0f, 10.0f);
		vLight.Normalize();
		vLight = vLight * -1.0f;
		m_fbxLoader.m_fbxObjList[iObj]->m_CbLight.vLightDir.x = vLight.x;
		m_fbxLoader.m_fbxObjList[iObj]->m_CbLight.vLightDir.y = vLight.y;
		m_fbxLoader.m_fbxObjList[iObj]->m_CbLight.vLightDir.z = vLight.z;
		m_fbxLoader.m_fbxObjList[iObj]->m_CbLight.vLightDir.w = 1.0f;

		m_fbxLoader.m_fbxObjList[iObj]->SetMatrix(&m_fbxLoader.m_fbxObjList[iObj]->m_matWorld,
			&m_Camera.m_matView,
			&m_Camera.m_matProjection);
		m_fbxLoader.m_fbxObjList[iObj]->Render(m_pImmediateContext);
	}
    return true;
}

bool Sample::Release()
{
    m_fbxLoader.Release();
    return true;
}
