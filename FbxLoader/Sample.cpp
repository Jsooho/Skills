#include "Sample.h"

//GAMECORE(Sample)
GAMECORE(sample,1024,768)

bool Sample::Init()
{
    m_FbxObj.Init();
    m_FbxObj.FbxLoad("../../Data/object/fbx/box.fbx");

    

    for (int iObj = 0; iObj < m_FbxObj.m_FbxNodeList.size(); iObj++)
    {
        m_FbxObj.m_MeshList[iObj]->Init();
        m_FbxObj.m_MeshList[iObj]->m_Texture = I_Texture.Load(m_FbxObj.m_MtrlList[iObj]->m_szTexFileName);
        m_FbxObj.m_MeshList[iObj]->SetPosition(TVector3(0.0f, 1.0f, 0.0f));
        m_FbxObj.m_MeshList[iObj]->Create(m_pImmediateContext,L"",L"",L"FbxShader.hlsl");
    }
   
   
    return true;
}

bool Sample::Frame()
{
    return true;
}

bool Sample::Render()
{
    for (int iObj = 0; iObj < m_FbxObj.m_FbxNodeList.size(); iObj++)
    {
        m_FbxObj.m_MeshList[iObj]->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProjection);
        m_FbxObj.m_MeshList[iObj]->Render(m_pImmediateContext);
    }
    return true;
}

bool Sample::Release()
{
    return true;
}
