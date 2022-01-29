// MonsterGuageDlg.cpp: implementation of the CMonsterGuageDlg class.
//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterGuageDlg.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "Monster.h"
#include "GameResourceStruct.h"
#include "CheatMsgParser.h"
#include "ChatManager.h"
#include "./Interface/cWindowManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterGuageDlg::CMonsterGuageDlg()
{
	m_pName = NULL;
	m_pNpcName = NULL;
	m_pLifeText = NULL;
	m_pLifeGuage = NULL;
	m_pShieldText = NULL;
	m_pShieldGuage = NULL;
	m_type = WT_MONSTERGUAGEDLG;
	m_CurMode = -1;
	m_Type = eObjectKind_Monster;
	m_InfoBtn[0] = NULL;
	m_InfoBtn[1] = NULL;
	m_InfoBtn[2] = NULL;
	m_dwCurPlayerID = 0;
}

CMonsterGuageDlg::~CMonsterGuageDlg()
{
	m_pCurMonster = NULL;
	m_dwCurPlayerID = 0;
	for(int i=0; i<eMonGuage_Max; ++i)
		m_MonGuageCtlListArray[i].RemoveAll();
	m_InfoBtn[0] = NULL;
	m_InfoBtn[1] = NULL;
	m_InfoBtn[2] = NULL;
}

void CMonsterGuageDlg::Linking()
{
	m_pName			= (cStatic *)GetWindowForID(CG_MONSTERGUAGENAME);
	m_pLifeText		= (cStatic *)GetWindowForID(CG_MONSTERGUAGELIFETEXT);
	m_pLifeGuage	= (CObjectGuagen *)GetWindowForID(CG_MONSTERGUAGELIFE);
	m_pShieldText	= (cStatic *)GetWindowForID(CG_MONSTERGUAGESHIELDTEXT);
	m_pShieldGuage	= (CObjectGuagen *)GetWindowForID(CG_MONSTERGUAGESHIELD);
	m_pGuildName	= (cStatic *)GetWindowForID(CG_GUILDNAME);
	m_pGuildUnionName	= (cStatic*)GetWindowForID(CG_GUILDUNION_NAME);
	m_pNpcName		= (cStatic *)GetWindowForID(CG_NPCNAME);
	

	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail(m_pLifeGuage);
	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail(m_pShieldGuage);
	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail(m_pLifeText);
	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail((cStatic *)GetWindowForID(CG_MONSTERGUAGELIFEBASE));

	m_MonGuageCtlListArray[eMonGuage_Character].AddTail(m_pGuildName);
	m_MonGuageCtlListArray[eMonGuage_Character].AddTail(m_pGuildUnionName);  
	m_MonGuageCtlListArray[eMonGuage_Character].AddTail(m_pLifeGuage);
	m_MonGuageCtlListArray[eMonGuage_Character].AddTail((cStatic *)GetWindowForID(CG_MONSTERGUAGELIFEBASE));
	m_MonGuageCtlListArray[eMonGuage_Npc].AddTail(m_pNpcName);
	m_InfoBtn[0] = (cButton * )GetWindowForID(CG_VIEWTARGETEQ);
	m_InfoBtn[1] = (cButton * )GetWindowForID(APPLYDEAL);
	m_InfoBtn[2] = (cButton * )GetWindowForID(APPLYPK);
	m_bShowBtn = FALSE;
	m_InfoBtn[0]->SetActive(m_bShowBtn);
	m_InfoBtn[1]->SetActive(m_bShowBtn);
	m_InfoBtn[2]->SetActive(m_bShowBtn);
	
}

void CMonsterGuageDlg::SetMonsterName( char * szMonsterName )
{
	if( m_pName )
	m_pName->SetStaticText(szMonsterName);
	m_pCurMonster = NULL;
	m_pLifeGuage->SetValue(0,0);
	m_pShieldGuage->SetValue(0,0);
}


void CMonsterGuageDlg::SetMonsterNameColor(DWORD dwColor)
{
	if( m_pName != NULL)
	{
		m_pName->SetFGColor(dwColor);
	}	

}

void CMonsterGuageDlg::SetNpcName( char * szNpcName )
{
	if( m_pNpcName )
	m_pNpcName->SetStaticText(szNpcName);
}

void CMonsterGuageDlg::SetMonsterLife( CMonster * pMonster )
{
	if( pMonster->GetSInfo() )
		SetMonsterLife( pMonster->GetLife(), pMonster->GetSInfo()->Life, 0 );


	m_pCurMonster = pMonster;
	m_Type = eObjectKind_Monster;
}

void CMonsterGuageDlg::SetMonsterLife( DWORD curVal, DWORD maxVal, int type )
{

	ASSERT(maxVal>0);

	if(maxVal == 0)
		maxVal = 1;

	char szValue[50];

	if(curVal > maxVal)
		curVal = maxVal;
	sprintf( szValue, "%d / %d", curVal, maxVal );

//#ifdef _CHEATENABLE_		
//	if( m_pLifeText )
//	{	//[怪物血量显示][By:十里坡剑神][QQ:112582793][2017/11/23]
//		//if(CHEATMGR->IsCheatEnable())	
//		m_pLifeText->SetAlign(TXT_CENTER);
//		m_pLifeText->SetStaticText(szValue);
//	}
//#endif
	if( m_pLifeGuage )
	{
		if(type == 0)
			m_pLifeGuage->SetValue((float)curVal/(float)maxVal, 0);
		else
			m_pLifeGuage->SetValue((float)curVal/(float)maxVal, (1500/maxVal)*curVal);
	}
}

void CMonsterGuageDlg::SetMonsterShield( CMonster * pMonster )
{
	if( pMonster->GetSInfo() )
		SetMonsterShield( pMonster->GetShield(), pMonster->GetSInfo()->Shield, 0 );

	m_pCurMonster = pMonster;
}

void CMonsterGuageDlg::SetMonsterShield( DWORD curVal, DWORD maxVal, int type )
{
	ASSERT(maxVal>0);
	if(maxVal == 0)
		maxVal = 1;

	char szValue[50];

	if(curVal > maxVal)
		curVal = maxVal;

	sprintf( szValue, "%d / %d", curVal, maxVal );

//#ifdef _CHEATENABLE_
//	if( m_pShieldText )
//	{//[怪物护体值显示][By:十里坡剑神][QQ:112582793][2017/11/23]
//		//if(CHEATMGR->IsCheatEnable())
//		m_pShieldText->SetAlign(TXT_CENTER);
//		m_pShieldText->SetStaticText(szValue);
//	}
//#endif

	if( m_pShieldGuage )
	{
		if(type == 0)
			m_pShieldGuage->SetValue((float)curVal/(float)maxVal, 0);
		else
			m_pShieldGuage->SetValue((float)curVal/(float)maxVal, (1500/maxVal)*curVal);
	}
}

void CMonsterGuageDlg::SetGuildUnionName(char* GuildName, char* GuildUnionName)
{

	char buf[128];

	if( GuildName[0] )
		sprintf(buf, CHATMGR->GetChatMsg(783), GuildName);
	else
		buf[0] = 0;


	m_pGuildName->SetStaticText(buf);

	if( GuildUnionName[0] )
		sprintf(buf, CHATMGR->GetChatMsg(1137), GuildUnionName);
	else
		buf[0] = 0;

	m_pGuildUnionName->SetStaticText(buf);
}

void CMonsterGuageDlg::ShowMonsterGuageMode(int mode, DWORD CharID/* = 0*/)
{
	m_dwCurPlayerID = CharID;
	if(m_CurMode == mode)
		return;

	
	if(m_CurMode != -1)
	{
		SetActiveMonGuageMode(m_CurMode, FALSE);
	}

	m_CurMode = mode;

	if(eMonGuage_Pet == mode)	return;
	
	SetActiveMonGuageMode(mode, TRUE);
}

void CMonsterGuageDlg::SetActiveMonGuageMode(int mode, BOOL bActive)

{
	PTRLISTSEARCHSTART(m_MonGuageCtlListArray[mode],cWindow*,pWin)

		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}

void CMonsterGuageDlg::Render()
{
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() ) return;
#endif


	if( m_CurMode == eMonGuage_Character )
	{
		/*if( 0 < strlen(m_pGuildUnionName->GetStaticText()) )
			m_pGuildName->SetRelXY(0, 18 );
		else
			m_pGuildName->SetRelXY(0, 26 );*/

		this->SetAbsXY( (LONG)this->GetAbsX(), (LONG)this->GetAbsY() );
		if(!m_bShowBtn)
		{
			m_bShowBtn = TRUE;
			m_InfoBtn[0]->SetActive(m_bShowBtn);
			m_InfoBtn[1]->SetActive(m_bShowBtn);
			m_InfoBtn[2]->SetActive(m_bShowBtn);
		}
	}
	else if(m_bShowBtn)
	{
		m_bShowBtn = FALSE;
		m_InfoBtn[0]->SetActive(m_bShowBtn);
		m_InfoBtn[1]->SetActive(m_bShowBtn);
		m_InfoBtn[2]->SetActive(m_bShowBtn);
	}
	cDialog::Render();
}




