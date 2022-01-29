



// MonsterGuageDlg.h: interface for the CMonsterGuageDlg class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
#define AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;
class cPushupButton;
class CObjectGuagen;
class CMonster;

enum MonsterGuageMode
{
	eMonGuage_Monster,
	eMonGuage_Character,
	eMonGuage_Npc,
	eMonGuage_Pet,
	eMonGuage_Max
};

class CMonsterGuageDlg : public cDialog  
{
public:
	CMonsterGuageDlg();
	virtual ~CMonsterGuageDlg();

	void	SetMonsterName( char * szMonsterName );
	void	SetMonsterLife( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterLife( CMonster * pMonster );
	void	SetMonsterShield( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterShield( CMonster * pMonster );
	void	SetGuildUnionName(char* GuildName, char* GuildUnionName);
	void	SetActiveMonGuageMode(int mode, BOOL bActive);
	void	SetNpcName( char * szNpcName );
	void	Linking();
	void	ShowMonsterGuageMode(int mode,DWORD CharID = 0);
	DWORD	CurPlayerID(){ return m_dwCurPlayerID; }
	virtual void Render();

	CMonster*	GetCurMonster() { return m_pCurMonster; }
	
	void	SetObjectType( DWORD Type )		{	m_Type = Type;	}
	DWORD	GetObjectType()					{	return m_Type;	}

	void SetMonsterNameColor(DWORD dwColor); 

protected:


	cStatic * m_pName;
	cStatic * m_pLifeText;
	CObjectGuagen * m_pLifeGuage;
	cStatic * m_pShieldText;
	CObjectGuagen * m_pShieldGuage;
	cStatic * m_pGuildName;
	cStatic * m_pGuildUnionName;

	cStatic * m_pNpcName;
	
	cPtrList m_MonGuageCtlListArray[eMonGuage_Max];
	int m_CurMode;

	CMonster * m_pCurMonster;				
	DWORD	m_Type;	
	cButton * m_InfoBtn[3];
	BOOL	m_bShowBtn;
	DWORD	m_dwCurPlayerID;
};

#endif // !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)


