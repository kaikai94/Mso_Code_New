







// EffectManager.cpp: implementation of the CEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectManager.h"
#include "Effect.h"
#include "..\MHFile.h"
#include "..\ObjectManager.h"
#include "..\ObjectStateManager.h"
#include "DamageNumber.h"

#include "DamageEffectUnit.h"
#include "EffectDesc.h"
#include "PartyManager.h"
#include "MHMap.h"

int g_EffectOption = EFFECT_OPTION_ALL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CEffectManager);
CEffectManager::CEffectManager()
{
	m_IllisionEffectMaterialTable.Initialize(32);
	m_EffectProcessTable.Initialize(32);
	m_pDNPool = new CMemoryPoolTempl<CDamageNumber>;
	m_pEffectPool = new CMemoryPoolTempl<CEffect>;
	m_pDNPool->Init(60,40,"CDamageNumber");
	m_pEffectPool->Init(100,50,"CEffect");
	m_bInited = FALSE;
	m_pParam = new EFFECTPARAM;
}

CEffectManager::~CEffectManager()
{
	Release();
	m_pDNPool->Release();
	delete m_pDNPool;
	m_pEffectPool->Release();
	delete m_pEffectPool;
	if (m_pParam)
	{
		delete m_pParam;
		m_pParam = NULL;
	}
}

CEffect* CEffectManager::GetEffect(HEFFPROC heff)
{
	return (CEffect*)m_EffectProcessTable.GetData((DWORD)heff);
}

void* CEffectManager::GetIllusionMaterial(DWORD MaterialNum)
{
	return m_IllisionEffectMaterialTable.GetData(MaterialNum);
}

void CEffectManager::LoadIllusionMaterial()
{
	CMHFile fp;
	void* pMtlHandle =	NULL;

	MATERIAL	mtl;
	DWORD	Color;
	BYTE r,g,b,a;
	DWORD num;
	char strFlag[256];

//	fp.Init("./Resource/IllusionMaterial.bin","rb");
	fp.Init("./Resource/Client/IllusionMaterial.bin","rb");
	if(fp.IsInited() == FALSE)
		return;

	DIRECTORYMGR->SetLoadMode(eLM_Effect);
	while(1)
	{
		if(fp.IsEOF() != FALSE)
			break;
		num = fp.GetWord();
		memset(&mtl, 0, sizeof(mtl));
		
		fp.GetString(mtl.szDiffuseTexmapFileName);
		r = fp.GetByte();
		g = fp.GetByte();
		b = fp.GetByte();
		a = fp.GetByte();
		Color = RGBA_MAKE(r,g,b,a);

		mtl.dwDiffuse	=	Color;
		mtl.dwAmbient	=	Color;

		fp.GetString(strFlag);
		if(strcmp(strFlag,"FILTER") == 0)
			mtl.dwFlag = TRANSP_TYPE_FILTER;
		else if(strcmp(strFlag,"SUBTRACTIVE") == 0)
			mtl.dwFlag = TRANSP_TYPE_SUBTRACTIVE;
		else if(strcmp(strFlag,"ADDITIVE") == 0)
			mtl.dwFlag = TRANSP_TYPE_ADDITIVE;
		else
		{
			ASSERT(0);
		}

		pMtlHandle = g_pExecutive->GetRenderer()->CreateMaterial( &mtl,NULL,NULL, 0);
		
		m_IllisionEffectMaterialTable.Add(pMtlHandle,num);
	}
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
}


void CEffectManager::Init()
{
	m_CurNewEffectID = 0;

	LoadIllusionMaterial();

	CMHFile fp;
	DIRECTORYMGR->SetLoadMode(eLM_EffectScript);
	fp.Init("list_m.befl","rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	m_EffectDescMgr[eEffectForMan].LoadEffectList(eEffectForMan,&fp);
	fp.Release();
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
		
	DIRECTORYMGR->SetLoadMode(eLM_EffectScript);
	
	fp.Init("list_w.befl","rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	m_EffectDescMgr[eEffectForWoman].LoadEffectList(eEffectForWoman,&fp);
	fp.Release();
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	CDamageNumber::LoadImage();

	m_bInited = TRUE;
}
void CEffectManager::Release()
{
	m_bInited = FALSE;
	
	RemoveAllEffect();

	void* pMtl;
	m_IllisionEffectMaterialTable.SetPositionHead();
	while(pMtl = m_IllisionEffectMaterialTable.GetData())
	{
		if(g_pExecutive)
			g_pExecutive->GetRenderer()->DeleteMaterial(pMtl);
	}
	m_IllisionEffectMaterialTable.RemoveAll();

	m_EffectDescMgr[eEffectForMan].Release();
	m_EffectDescMgr[eEffectForWoman].Release();

		
	CDamageNumber* pNumber;
	while(pNumber = (CDamageNumber*)m_DamageNumberList.RemoveTail())
	{
		if(m_pDNPool)
			m_pDNPool->Free(pNumber);
	}

	CDamageNumber::DeleteImage();
}
void CEffectManager::RemoveAllEffect()
{	
	CEffect* pProc;
	m_EffectProcessTable.SetPositionHead();
	while(pProc = (CEffect*)m_EffectProcessTable.GetData())
	{
		EndProcess(pProc);
	}
	m_EffectProcessTable.RemoveAll();
}

HEFFPROC CEffectManager::StartEffectProcess(int EffectDescKind,int EffectNum,EFFECTPARAM* pParam,DWORD Key,DWORD RefCount)

{
	BOOL rt = FALSE;
	pParam->m_StartTime = gCurTime;
	CEffect* pProc = m_pEffectPool->Alloc();
	CObject* pEffectOperator = pParam->m_pOperator;
	rt = m_EffectDescMgr[EffectDescKind].GetEffect(EffectNum,pParam,pProc);

	if(rt == FALSE)
		return NULL;
	m_EffectProcessTable.Add(pProc,(DWORD)Key);
	pProc->SetEffectID(Key);
	pProc->SetRefCount(RefCount);

	pProc->Process(gCurTime);

	return Key;
}

DWORD CEffectManager::GetNewEffectID()
{
	if(++m_CurNewEffectID == 0)
		++m_CurNewEffectID;

	while(GetEffect((HEFFPROC)m_CurNewEffectID))
		++m_CurNewEffectID;

	return m_CurNewEffectID;
}

HEFFPROC CEffectManager::StartEffectProcess(int EffectNum,CObject* pEffectOperator,
											TARGETSET* pEffectTargetSetArray,int nEffectTargetNum,
											DWORD MainTargetID, DWORD dwFlag,VECTOR3* pAddPos,CObject* pSkillOperator)
{
	if(m_bInited == FALSE)
		return NULL;

	if (m_pParam)
	{
		m_pParam->m_StartTime = gCurTime;
		m_pParam->m_dwFlag = dwFlag;
		m_pParam->m_nTargetNum = nEffectTargetNum;
		m_pParam->m_pOperator = pEffectOperator;
		m_pParam->m_pSkillOperator = pSkillOperator;

		for (int n = 0; n < nEffectTargetNum; ++n)
		{
			if (pEffectTargetSetArray[n].pTarget == HERO)
				m_pParam->m_dwFlag |= EFFECT_FLAG_RELATEDTOHERO;
		}
		if (pEffectOperator == HERO || dwFlag & EFFECT_FLAG_HEROATTACK)
			m_pParam->m_dwFlag |= EFFECT_FLAG_RELATEDTOHERO;

		if (nEffectTargetNum)
			memcpy(m_pParam->m_pTargetSet, pEffectTargetSetArray, sizeof(TARGETSET)*nEffectTargetNum);
		if (pAddPos)
			m_pParam->m_AddPos = *pAddPos;

		m_pParam->m_MainTarget.SetMainTarget(MainTargetID);

		BOOL rt = FALSE;
		CEffect* pProc = m_pEffectPool->Alloc();
		CObject* pGenderBasisTarget = dwFlag & EFFECT_FLAG_GENDERBASISTARGET ?
			pEffectTargetSetArray[0].pTarget : pEffectOperator;

		if (pGenderBasisTarget->GetObjectKind() == eObjectKind_Player)
		{
			if ((MAP->IsMapKind(eTitanMap)) && (MAP->GetMapNum() != 101))
			{
				if (((CPlayer*)pGenderBasisTarget)->GetGender() == GENDER_MALE)
					rt = m_EffectDescMgr[eEffectForWoman].GetEffect(EffectNum, m_pParam, pProc, FALSE);
				else
					rt = m_EffectDescMgr[eEffectForMan].GetEffect(EffectNum, m_pParam, pProc);
			}
			else
			{
				if (((CPlayer*)pGenderBasisTarget)->GetGender() == GENDER_MALE)
					rt = m_EffectDescMgr[eEffectForMan].GetEffect(EffectNum, m_pParam, pProc);
				else
					rt = m_EffectDescMgr[eEffectForWoman].GetEffect(EffectNum, m_pParam, pProc, FALSE);
			}
		}

		if (rt == FALSE)
			rt = m_EffectDescMgr[eEffectForMan].GetEffect(EffectNum, m_pParam, pProc);

		if (rt == FALSE)
			return NULL;

		DWORD id = GetNewEffectID();
		m_EffectProcessTable.Add(pProc, id);
		pProc->SetEffectID(id);

		pProc->Process(gCurTime);
		memset(m_pParam, 0, sizeof(EFFECTPARAM));
		return id;
	}
	
}

void CEffectManager::AddDamageNumber(CDamageNumber* pNumber)
{
	m_DamageNumberList.AddHead(pNumber);
}

void CEffectManager::AddDamageNumberDodge(CObject* pTarget)
{
	if(pTarget->GetEngineObject()->IsVisible() == FALSE)
		return;

#ifndef _GMTOOL_
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pTarget)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif

	VECTOR3 pos,velocity;
	pos.x = 0;
	pos.z = 0;
	pos.y = 130.f;	

	VECTOR3 cpos;
	pTarget->GetPosition(&cpos);
	pos = pos + cpos;
	
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
	
	BYTE DamageNumberKind = 0;

	CDamageNumber* pNumber = m_pDNPool->Alloc();

	pNumber->SetDodge(&pos,&velocity);
	AddDamageNumber(pNumber);
}

void CEffectManager::AddDamageNumber(DWORD Damage,CObject* pOperator,CObject* pTarget,WORD DamageKind,BOOL bCritical, BOOL bDecisive)
{
	if(pTarget->GetEngineObject()->IsVisible() == FALSE)
		return;

#ifndef _GMTOOL_
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pTarget)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif

	VECTOR3 pos,velocity;
	pos.x = 0;
	pos.z = 0;
	pos.y = 130.f;	

	if(HERO->InTitan())
	{
		pos.y = 330.f;
	}

	VECTOR3 cpos;
	pTarget->GetPosition(&cpos);
	pos = pos + cpos;
	
	CDamageEffectUnit::GetDamageNumberVelocity(&velocity,pTarget,DamageKind);

	BYTE DamageNumberKind;

	if( pTarget && ( pTarget == HERO || PARTYMGR->IsPartyMember(pTarget->GetID()) ) )
		DamageNumberKind = eDNK_Red;
	else if( pOperator && ( pOperator == HERO || PARTYMGR->IsPartyMember(pOperator->GetID()) ) )
		DamageNumberKind = eDNK_Yellow;
	else
		DamageNumberKind = eDNK_Green;

	CDamageNumber* pNumber = m_pDNPool->Alloc();
	if( !pNumber )	return;
	pNumber->SetDamage(Damage,&pos,&velocity,DamageNumberKind,bCritical,bDecisive);
	AddDamageNumber(pNumber);
}

void CEffectManager::Process(DWORD CurTime)
{
	CEffect* pProc;
	m_EffectProcessTable.SetPositionHead();
	while(pProc = (CEffect*)m_EffectProcessTable.GetData())
	{
		if(pProc->Process(CurTime) == TRUE)
			OnEffectTimeOut((HEFFPROC)pProc->GetEffectID());
	}


	CDamageNumber* pNumber;
	PTRLISTPOS pos = m_DamageNumberList.GetHeadPosition();
	PTRLISTPOS beforepos;
	while(pos)
	{
		beforepos = pos;
		pNumber = (CDamageNumber *)m_DamageNumberList.GetNext(pos);
		if(pNumber->Render() == FALSE)
		{
			m_DamageNumberList.RemoveAt(beforepos);
			m_pDNPool->Free(pNumber);

		}
	}
}

void CEffectManager::Process(DWORD CurTime,HEFFPROC handle)
{
	CEffect* pEffect = GetEffect(handle);

	if(pEffect == NULL)
		return;

	if(pEffect->Process(CurTime) == TRUE)
		OnEffectTimeOut((HEFFPROC)pEffect->GetEffectID());

	return;
}

BOOL CEffectManager::SetDamage(HEFFPROC handle,TARGETSET* TargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget)
{
	ASSERT(handle);

	CEffect* pEffect = GetEffect(handle);
	if(pEffect == NULL)
		return FALSE;

	pEffect->SetDamage(TargetSetArray,MaxTargetSet,pMainTarget);
	return TRUE;
}

DWORD CEffectManager::GetEffectEndTime(HEFFPROC heff)
{
	ASSERT(heff);

	CEffect* pEffect = GetEffect(heff);
	if(pEffect == NULL)
		return FALSE;

	return pEffect->GetEffectEndTime();
}

DWORD CEffectManager::GetOperatorAnimatioEndTime(int EffectNum,int EffectGenderKind,CEngineObject* pEngineObject)
{
	CEffectDesc* pEffectDesc;
	pEffectDesc = m_EffectDescMgr[EffectGenderKind].GetEffectDesc(EffectNum);
	

	if(pEffectDesc == NULL)
		pEffectDesc = m_EffectDescMgr[eEffectForMan].GetEffectDesc(EffectNum);

	if(pEffectDesc == NULL)
		return 0;

	return pEffectDesc->GetOperatorAnimatioEndTime(pEngineObject);
}

void CEffectManager::OnEffectTimeOut(HEFFPROC handle)
{
	ASSERT(handle);
	
	CEffect* pEffect = GetEffect(handle);
	if(pEffect == NULL)
		return;

	CObject* pOperator = pEffect->GetEffectParam()->m_pOperator;
	BOOL bEndFlag = pEffect->IsEndFlag();
	if(pEffect->IsRepeatEffect() && bEndFlag == FALSE)

		ResetProcess(pEffect);
	else
	{
		if (m_pParam)
		{
			m_pParam->Copy(pEffect->GetEffectParam());
			m_pParam->m_DamageRate = 0;
			DWORD Key = (DWORD)pEffect->GetEffectID();
			DWORD NextEffect = pEffect->GetNextEffect();
			DWORD RefCount = pEffect->GetRefCount();
			int EffectKind = pEffect->GetEffectKind();
			EndProcess(pEffect);


			if (NextEffect)
			{
				HEFFPROC proc = StartEffectProcess(EffectKind, NextEffect, m_pParam, Key, RefCount);
				if (bEndFlag)
				{
					SetEffectEndFlag(proc);
				}
			}
			memset(m_pParam, 0, sizeof(EFFECTPARAM));
		}
		
	}
}

BOOL CEffectManager::ForcedEndEffect(HEFFPROC handle)
{
	if(m_bInited == FALSE)
		return TRUE;
	
	ASSERT(handle);
	
	CEffect* pEffect = GetEffect(handle);
	if(pEffect == NULL)
		return TRUE;


	
	
	m_pParam->Copy(pEffect->GetEffectParam());
	DWORD Key = (DWORD)pEffect->GetEffectID();
	DWORD NextEffect = pEffect->GetNextEffect();
	DWORD RefCount = pEffect->GetRefCount();

	int EffectKind = pEffect->GetEffectKind();
	pEffect->DecRefCount();
	if(pEffect->GetRefCount() > 0)
		return FALSE;

	EndProcess(pEffect);
	
	if(NextEffect)
	{
		HEFFPROC proc = StartEffectProcess(EffectKind, NextEffect, m_pParam, Key, RefCount);
		CEffect* pNextEffect = GetEffect(proc);
		if(pNextEffect)
			pNextEffect->SetEndFlag();
	}
	memset(m_pParam, 0, sizeof(EFFECTPARAM));
	
	return TRUE;

}

void CEffectManager::ResetProcess(CEffect* pEffect)
{
	ASSERT(pEffect);

	pEffect->Reset();
}

void CEffectManager::EndProcess(CEffect* pEffect)
{
	ASSERT(pEffect);

	m_EffectProcessTable.Remove(pEffect->GetEffectID());	
	pEffect->Release();

	m_pEffectPool->Free(pEffect);
}

HEFFPROC CEffectManager::StartHeroEffectProcess(int EffectNum)
{
	TARGETSET set;
	set.pTarget = HERO;
	return StartEffectProcess(EffectNum,HERO,&set,1,HEROID);
}

HEFFPROC CEffectManager::StartPlayerEffectProcess(CPlayer* pPlayer, int EffectNum)
{
	TARGETSET set;
	set.pTarget = pPlayer;
	return StartEffectProcess(EffectNum,pPlayer,&set,1,pPlayer->GetID());
}

HEFFPROC CEffectManager::StartEffectProcessWithTargetList(int EffectNum,CObject* pEffectOperator,
		CTargetList* pTList, MAINTARGET* pMainTarget,
		DWORD dwFlag,VECTOR3* pAddPos)
{
	CObject* pEffectMaker = NULL;
	if( GetMainTargetID(pMainTarget) == HERO->GetID() )
	{
		pEffectMaker = (CObject*)HERO;
	}
	else
	{
		pEffectMaker = pEffectOperator;
	}

	if(pTList)
	{
		CTargetListIterator iter(pTList);
		TARGETSET tset[MAX_TARGET_NUM];
		int nTargetNum = 0;
		while(iter.GetNextTarget())
		{
			tset[nTargetNum].pTarget = OBJECTMGR->GetObject(iter.GetTargetID());
			iter.GetTargetData(&tset[nTargetNum].ResultInfo);
			if(tset[nTargetNum].pTarget)
				++nTargetNum;
		}
		iter.Release();
			
		return StartEffectProcess(EffectNum,pEffectOperator,tset,nTargetNum,GetMainTargetID(pMainTarget),dwFlag,NULL,pEffectMaker);
	}
	else

		return StartEffectProcess(EffectNum,pEffectOperator,NULL,0,GetMainTargetID(pMainTarget),dwFlag,NULL,pEffectMaker);		
}

BOOL CEffectManager::SetTargetArray(HEFFPROC handle,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CTargetListIterator iter(pTList);
	TARGETSET tset[MAX_TARGET_NUM];
	int nTargetNum = 0;
	while(iter.GetNextTarget())
	{
		tset[nTargetNum].pTarget = OBJECTMGR->GetObject(iter.GetTargetID());
		iter.GetTargetData(&tset[nTargetNum].ResultInfo);
		if(tset[nTargetNum].pTarget)
			++nTargetNum;
	}
	iter.Release();
	
	return SetDamage(handle,tset,nTargetNum,pMainTarget);
}

void CEffectManager::SetEffectEndFlag(HEFFPROC heff)
{
	ASSERT(heff);
	
	CEffect* pEffect = GetEffect(heff);
	if(pEffect == NULL)
		return;
	
	pEffect->SetEndFlag();
}

void CEffectManager::IncEffectRefCount(HEFFPROC heff)
{
	ASSERT(heff);
	
	CEffect* pEffect = GetEffect(heff);
	if(pEffect == NULL)
		return;
	
	pEffect->IncRefCount();
}

void CEffectManager::SetEndFlagAllEffect()
{
	m_EffectProcessTable.SetPositionHead();
	CEffect* pEff;
	while(pEff = m_EffectProcessTable.GetData())
	{
		pEff->SetEndFlag();
	}
	Process(gCurTime);
}



HEFFPROC CEffectManager::StartSkillMainTainEffect(int EffectNum,CObject* pOperator,DWORD dwFlag,VECTOR3* pAddPos,CObject* pSkillOperator)
{
	TARGETSET set;
	set.pTarget = pOperator;
	return StartEffectProcess(EffectNum,pOperator,&set,1,pOperator->GetID(),dwFlag,pAddPos,pSkillOperator);
}




