#include"Fbxobject.h"
void Fbxobject::ParseAnimationStack(FbxString* szData)
{
	//1Frame = 160Tick //fbx파일이 Tick으로 계산되어있음
	//FrameSpeed = 1초당 30Frame 기준
	//1초당Tick = 30 * 160Tick = 4800Tick
	// 0~100Frame(16000 = 100Frame / 4800 = 30Frame) = 3.3333초
	FbxTakeInfo* pTakeInfo = m_FbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, m_FbxScene->GetGlobalSettings().GetTimeMode());
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
		m_FbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
void Fbxobject::ParseAnimation()
{
	FbxArray<FbxString*> AnimationStackNameArray;
	m_FbxScene->FillAnimStackNameArray(AnimationStackNameArray);
	int iAnimationStackCount = AnimationStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimationStackCount; iStack++)
	{
		ParseAnimationStack(AnimationStackNameArray.GetAt(iStack));
	}
}
void Fbxobject::ParseAnimationNode(FbxNode* pNode, Mesh* pMesh)
{
	//Animation Data 저장
	FbxAnimEvaluator* pAnim = m_FbxScene->GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pNode, time);
		TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
		pMesh->m_AnimationTrack.push_back(matGlobaDX);
		fCurrentTime += m_fSampleTime;
	}
}