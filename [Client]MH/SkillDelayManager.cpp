// SkillDelayManager.cpp: implementation of the CSkillDelayManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDelayManager.h"
#include "MHFile.h"
#include "../[CC]Skill/DelayGroup.h"
#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSkillDelayManager)


CSkillDelayManager::CSkillDelayManager()
{
	m_htPrimereSkill.Initialize( 100 );
}

CSkillDelayManager::~CSkillDelayManager()
{
	m_htPrimereSkill.SetPositionHead();
	PRIMERESKILL* pPrimereSkill = NULL;

	while( pPrimereSkill = m_htPrimereSkill.GetData() )
	{
		delete pPrimereSkill;
	}

	m_htPrimereSkill.RemoveAll();

}


void CSkillDelayManager::LoadSkillUseInfo()
{

	CMHFile file;

	if( !file.Init( "./Resource/Client/PremierSkill.bin", "rb" ) )
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwGroupNum	= file.GetDword();
		DWORD dwSkillIndex	= file.GetDword();
		DWORD dwDelay		= file.GetDword();
		
		PRIMERESKILL* pPrimereSkill = new PRIMERESKILL;
		pPrimereSkill->dwSkillIndex = dwSkillIndex;

		pPrimereSkill->dwDelay		= dwDelay;
		m_htPrimereSkill.Add( pPrimereSkill, dwSkillIndex );
	}

	file.Release();
}

void CSkillDelayManager::AddSkillDelay( DWORD dwSkillIndex )

{
	PRIMERESKILL* pPrimereSkill = m_htPrimereSkill.GetData( dwSkillIndex );

	if( pPrimereSkill == NULL ) return;

	m_htPrimereSkill.SetPositionHead();
	while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
	{

		if( p->dwSkillIndex != dwSkillIndex )
		{
			HERO->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,(WORD)p->dwSkillIndex,
				pPrimereSkill->dwDelay);
		}
	}
}

void CSkillDelayManager::ContinueSkillDelay( DWORD dwDelayTime, DWORD dwRemainDelay )
{
	
	if( dwRemainDelay > dwDelayTime )
		dwRemainDelay = dwDelayTime;



	m_htPrimereSkill.SetPositionHead();
	while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
	{
		HERO->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill, (WORD)p->dwSkillIndex,
			dwDelayTime, dwDelayTime - dwRemainDelay );
	}

}






