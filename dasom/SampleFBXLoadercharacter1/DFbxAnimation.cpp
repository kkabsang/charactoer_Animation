#include "DFbxObj.h"
#include "TTimer.h"
#include <algorithm>

bool Compare(const pair<int, int>& a, const pair<int, int>& b)
{
	if (a.first == b.first)
		return a.second < b.second;
	return a.first < b.first;
}
DMesh* DFbxObj::GetFindIndex(FbxNode* pNode)
{
	for (int iNode = 0; iNode < m_pMeshList.size(); iNode++)
	{
		DMesh* pMesh = m_pMeshList[iNode];
		if (pMesh->m_pFbxNode == pNode)
		{
			return pMesh;
		}
	}
	return nullptr;
}
bool DFbxObj::ParseMeshSkinning(FbxMesh* pFbxMesh, DMesh* pMesh, DSkinData* pSkindata)
{
	int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iNumDeformer == 0)
	{
		return false;
	}
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();
	pSkindata->m_VertexList.resize(iNumVertexCount);
	for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
	{
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		// 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		int iNumCluster = pSkin->GetClusterCount();
		// 영향을 미치는 행렬이 iNumCluster 있다.
		pMesh->m_matBindPoseList.resize(iNumCluster);
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			FbxAMatrix matXBindPose, matInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);//드레스포즈
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos= matInitPostion.Inverse()*matXBindPose;
			TMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			D3DXMatrixInverse(&matBinePos, NULL, &matBinePos);
			pMesh->m_matBindPoseList[iCluster] = matBinePos;		
			FbxNode* pLinkNode = pCluster->GetLink();
			pMesh->m_pFbxNodeList.push_back(pLinkNode);
			//ControlPoint(제어점) 정점리스트
			int* iIndex = pCluster->GetControlPointIndices();

			// 가중치리스트
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iCluster);
				pSkindata->m_VertexList[iIndex[i]].m_WeightList.push_back(pWeight[i]);
				//iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.	
			}
		}
	}
	return true;
}
void DFbxObj::ParseAnimStack(FbxString* szData)
{
	m_pFbxScene->GetAnimationEvaluator()->Reset();
	// Frame, Tick
	// 1Frame = 160Tick
	// frameSpeed = 1Sec Per 30Frame
	// 1SecTick = 30 * 160 = 4800Tick
	// 0 ~ 100Frame(16000/4800) = 3.3333Sec
	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;

	//max의 시간기준으로 우리가 사용하는 start, end time 기준으로 변경 
	if (pTakeInfo)//프레임, 틱을 시간으로
	{
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();//start가 무조건 0은아님
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

void DFbxObj::ParseAnimation()
{
	FbxArray<FbxString*>AnimStackNameArray;
	m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimStack(AnimStackNameArray.GetAt(iStack));
	}
}

void DFbxObj::ParseAnimationNode()
{
	//애니메이션 데이터 저장
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* currAnimStack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);

	m_pFbxScene->SetCurrentAnimationStack(currAnimStack);
	FbxString Name = currAnimStack->GetNameOnly();
	FbxString TakeName = currAnimStack->GetName();
	FbxTakeInfo* TakeInfo = m_pFbxScene->GetTakeInfo(TakeName);
	FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
	FbxTime Start = LocalTimeSpan.GetStart();
	FbxTime Stop = LocalTimeSpan.GetStop();
	FbxTime Duration = LocalTimeSpan.GetDuration();

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong FrameCount = Duration.GetFrameCount(TimeMode);
	double FrameRate = FbxTime::GetFrameRate(TimeMode);

	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	FbxLongLong a = Start.GetFrameCount(TimeMode);
	FbxLongLong b = Stop.GetFrameCount(TimeMode);

	FbxTime time;
	for (FbxLongLong f = a; f <= b; ++f)
	{
		time.SetFrame(f, TimeMode);
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			DMesh* pMesh = m_pMeshList[iMesh];
			FbxAMatrix matGlobal = pMesh->m_pFbxNode->EvaluateGlobalTransform(time);
			TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
	}
	//애니메이션데이터 저장
	/*FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	float fCurrentTime = m_fStartTime;
	//모든 노드(애니메이션 데이터 저장 끝까지)저장 
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;

		time.SetSecondDouble(fCurrentTime);
		time.SetFrame(0, TimeMode);
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			DMesh* pMesh = m_pMeshList[iMesh];
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			TMatrix matGlobaDx = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDx);
		}
		fCurrentTime += m_fSampleTime;
	}*/
}