// DelayGroup.cpp: implementation of the CDelayGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CDelayGroup::CDelay>* CDelayGroup::m_pDelayPool = NULL;
DWORD CDelayGroup::m_PoolRefCount = 0;

DWORD CDelayGroup::MakeKey(WORD Kind,WORD Idx)
{
	return (Kind << 16) | Idx;
}

CDelayGroup::CDelayGroup()
{
	if(m_pDelayPool == NULL)
	{
		m_pDelayPool = new CMemoryPoolTempl<CDelay>;
#ifdef _MAPSERVER_
		m_pDelayPool->Init(2048,512,"CDelayGroup");
#else
		m_pDelayPool->Init(24,24,"CDelayGroup");
#endif
	}

	++m_PoolRefCount;

	m_DelayedTable.Initialize(24);

	m_pTitanPotionDelay = NULL;
	for (int i = 0; i < 20;i++)
		m_pYoungYakPotionDelay[i] = NULL;

}

CDelayGroup::~CDelayGroup()
{
	Release();

	if(m_PoolRefCount == 0)
	{
		static BOOL bMsgBox = TRUE;
		if(bMsgBox)
			MessageBox(0,"DelayGroup 捞 捞惑窍促!!",0,0);

		return;
	}

	--m_PoolRefCount;
	if(m_PoolRefCount == 0)
	{
		m_pDelayPool->Release();
		delete m_pDelayPool;
		m_pDelayPool = NULL;
	}
}

void CDelayGroup::Init()
{
	m_pTitanPotionDelay = NULL;
	for (int i = 0; i < 20; i++)
		m_pYoungYakPotionDelay[i] = NULL;
	Release();
}

void CDelayGroup::Release()
{
	m_DelayedTable.SetPositionHead();
	while(CDelay* pDelay = m_DelayedTable.GetData())
	{
		m_pDelayPool->Free(pDelay);
	}
	m_DelayedTable.RemoveAll();
}

float CDelayGroup::CheckDelay(WORD Kind,WORD Idx, DWORD* pRemainTime)
{
	DWORD key = MakeKey(Kind,Idx);
	CDelay* pDelay = m_DelayedTable.GetData(key);
	if (pDelay == NULL)
	{
		if (pRemainTime)
			*pRemainTime = 0;
		return 0;
	}

	DWORD Elapsed = gCurTime - pDelay->StartTime;
#ifdef _MAPSERVER_
	float rate = 1 - ( (Elapsed+500)/(float)pDelay->DelayTime);
#else
	float rate = 1 - (Elapsed/(float)pDelay->DelayTime);
#endif
	
	if( pRemainTime )
	{
		if( pDelay->DelayTime > Elapsed )
			*pRemainTime = pDelay->DelayTime - Elapsed;
		else
			*pRemainTime = 0;
	}

	if(rate <= 0)
	{
		if( pRemainTime )
			*pRemainTime = 0;
		m_pDelayPool->Free(pDelay);
		m_DelayedTable.Remove(key);
		return 0;
	}

	return rate;
}
void CDelayGroup::AddDelay(WORD Kind,WORD Idx,DWORD DelayTime,DWORD ElapsedTime )
{
	DWORD key = MakeKey(Kind,Idx);
	CDelay* pDelay = m_DelayedTable.GetData(key);

	if(pDelay)
	{
		pDelay->StartTime = gCurTime;
		pDelay->DelayTime = DelayTime;
		return;
	}

	pDelay = m_pDelayPool->Alloc();
	pDelay->Kind = Kind;
	pDelay->Idx = Idx;
	
	DWORD dwStartTime = gCurTime-ElapsedTime;
	if( gCurTime < ElapsedTime )
	{
		dwStartTime = gCurTime;

		if( DelayTime > ElapsedTime )
			DelayTime -= ElapsedTime;
		else
			DelayTime = 0;
	}
		
	pDelay->StartTime = dwStartTime;
	pDelay->DelayTime = DelayTime;

	m_DelayedTable.Add(pDelay,key);
}

float CDelayGroup::CheckTitanPotionDelay( DWORD* pRemainTime )
{
	if( m_pTitanPotionDelay == NULL )
		return 0;

	DWORD Elapsed = gCurTime - m_pTitanPotionDelay->StartTime;
	float rate = 1 - (Elapsed/(float)m_pTitanPotionDelay->DelayTime);

	if( pRemainTime )
	{
		if( m_pTitanPotionDelay->DelayTime > Elapsed )
			*pRemainTime = m_pTitanPotionDelay->DelayTime - Elapsed;
		else
			*pRemainTime = 0;
	}

	if(rate <= 0)
	{
		if( pRemainTime )
			*pRemainTime = 0;
		m_pDelayPool->Free(m_pTitanPotionDelay);
		m_pTitanPotionDelay = NULL;
		return 0;
	}

	return rate;
}
void CDelayGroup::AddTitanPotionDelay( DWORD DelayTime, DWORD ElapsedTime )
{
	if( m_pTitanPotionDelay )
	{
		m_pTitanPotionDelay->StartTime = gCurTime;
		m_pTitanPotionDelay->DelayTime = DelayTime;
		return;
	}

	m_pTitanPotionDelay = m_pDelayPool->Alloc();

	DWORD dwStartTime = gCurTime-ElapsedTime;
	if( gCurTime < ElapsedTime )
	{
		dwStartTime = gCurTime;

		if( DelayTime > ElapsedTime )
			DelayTime -= ElapsedTime;
		else
			DelayTime = 0;
	}

	m_pTitanPotionDelay->StartTime = dwStartTime;
	m_pTitanPotionDelay->DelayTime = DelayTime;
}
//增加吃药品延迟时间 by:十里坡剑神 QQ:112582793

float CDelayGroup::CheckYoungYakPotionDelay(WORD TYPE, DWORD* pRemainTime )
{
	if( m_pYoungYakPotionDelay[TYPE] == NULL )
		return 0;

	DWORD Elapsed = gCurTime - m_pYoungYakPotionDelay[TYPE]->StartTime;
	float rate = 1 - (Elapsed/(float)m_pYoungYakPotionDelay[TYPE]->DelayTime);

	if( pRemainTime )
	{
		if( m_pYoungYakPotionDelay[TYPE]->DelayTime > Elapsed )
			*pRemainTime = m_pYoungYakPotionDelay[TYPE]->DelayTime - Elapsed;
		else
			*pRemainTime = 0;
	}

	if(rate <= 0)
	{
		if( pRemainTime )
			*pRemainTime = 0;
		m_pDelayPool->Free(m_pYoungYakPotionDelay[TYPE]);
		m_pYoungYakPotionDelay[TYPE] = NULL;
		return 0;
	}

	return rate;
}
void CDelayGroup::AddYoungYakPotionDelay( DWORD DelayTime,WORD Type, DWORD ElapsedTime )
{
	if( m_pYoungYakPotionDelay[Type] )
	{
		m_pYoungYakPotionDelay[Type]->StartTime = gCurTime;
		m_pYoungYakPotionDelay[Type]->DelayTime = DelayTime;
		return;
	}

	m_pYoungYakPotionDelay[Type] = m_pDelayPool->Alloc();

	DWORD dwStartTime = gCurTime-ElapsedTime;
	if( gCurTime < ElapsedTime )
	{
		dwStartTime = gCurTime;

		if( DelayTime > ElapsedTime )
			DelayTime -= ElapsedTime;
		else
			DelayTime = 0;
	}

	m_pYoungYakPotionDelay[Type]->StartTime = dwStartTime;
	m_pYoungYakPotionDelay[Type]->DelayTime = DelayTime;
}
