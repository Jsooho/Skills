#include "FbxObj.h"

void    FbxLoader::ParseAnimationStack(FbxString* szData)
{
	m_pFbxScene->GetAnimationEvaluator()->Reset();
	// Frame, Tick
	// 1Frame = 160Tick
	// frameSpeed = 1Sec Per 30Frame
	// 1SecTick = 30 * 160 = 4800Tick
	// 0 ~ 100Frame(16000/4800) = 3.3333Sec
	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;

	if (pTakeInfo)
	{
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
void	FbxLoader::ParseAnimation()
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimationStack(AnimStackNameArray.GetAt(iStack));
	}
}

void	FbxLoader::ParseAnimationNode()
{
	// 에니메이션 데이터 저장
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();

	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		for (int iMesh = 0; iMesh < m_fbxObjList.size(); iMesh++)
		{
			HFbxObject* pobj = m_fbxObjList[iMesh];
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pobj->m_pfbxNode, time);
			TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pobj->m_AnimationTrack.push_back(matGlobaDX);
		}
		fCurrentTime += m_fSampleTime;
	}
}

