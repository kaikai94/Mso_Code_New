#include "stdafx.h"
#include "Battle_VimuStreet.h"
#include "BattleTeam_VimuStreet.h"
#include "Object.h"
#include "ObjectStateManager.h"
#include "WantedManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "PKManager.h"
#include "GuildManager.h"
#include "GuildUnion.h"

#ifdef _MAPSERVER_
#include "ObjectStateManager.h"
#include "UserTable.h"
#include "Player.h"
#include "PackedData.h"
#include "SkillManager_Server.h"
#include "MapDBMsgParser.h"
#else
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "MHMap.h"
#include "ChatManager.h"
#endif

//#ifdef _JAPAN_LOCAL_
//#include "Monster.h"
//#endif


CBattle_VimuStreet::CBattle_VimuStreet()
{
	m_BattleFlag = BATTLE_FLAG_NONE;
#ifndef _MAPSERVER_
	m_pCurShowImage	= NULL;
#endif
}

CBattle_VimuStreet::~CBattle_VimuStreet()
{

}

void CBattle_VimuStreet::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::Initialize(pCreateInfo,&m_Team[0],&m_Team[1]);
}


//////////////////////////////////////////////////////////////////////////
// { start virtual func π›µÂΩ√ ø¿πˆ∂Û¿Ãµ˘ «ÿæﬂ«‘




// Battle ¡§∫∏ ∞¸∑√
#ifdef _MAPSERVER_
void CBattle_VimuStreet::GetBattleInfo(char* pInfo,WORD* size)
{
	BATTLE_INFO_VIMUSTREET* info = (BATTLE_INFO_VIMUSTREET*)pInfo;

	memcpy(info,&m_VimuStreetInfo,sizeof(BATTLE_INFO_VIMUSTREET));
	
	ToEachTeam(pTeam)
		CBattleTeam_VimuStreet* pTeamVM	= (CBattleTeam_VimuStreet*)pTeam;
		info->Character[TeamNumber] = pTeamVM->GetCharacterID();
	EndToEachTeam
	
	*size = sizeof(BATTLE_INFO_VIMUSTREET);
}
/*
void CBattle_VimuStreet::GetBattleInfo(BATTLE_INFO_BASE*& pInfo,int& size)
{
	pInfo = &m_VimuStreetInfo;
	size = sizeof(BATTLE_INFO_BASE);
}
*/
#endif



BOOL CBattle_VimuStreet::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTarget->GetObjectKind() & eObjectKind_Monster )
		{
/*#ifdef _JAPAN_LOCAL_
 			WORD wMonsterKind = ((CMonster*)pTarget)->GetMonsterKind();
			if( wMonsterKind == 166 || wMonsterKind == 167 ||
 				wMonsterKind == 168 || wMonsterKind == 169 )
 				return FALSE;	
#endif	*/		
			return TRUE;
		}
		else if( pTarget->GetObjectKind() == eObjectKind_Player )
		{
			if( pOperator->GetBattleID() == pTarget->GetBattleID() &&
				pOperator->GetBattleTeam() != pTarget->GetBattleTeam() &&
				GetBattleState() == eBATTLE_STATE_FIGHT )
				return TRUE;	

			

#ifdef _MAPSERVER_
			DWORD dwChannel = ((CPlayer*)pOperator)->GetChannelID();
			if( ((CPlayer*)pOperator)->GetPartyIdx() != 0 )
			{
				CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pOperator)->GetPartyIdx() );
				if( pParty )
				if( pParty->IsPartyMember( pTarget->GetID() ) )
				{
					return FALSE;	//∆ƒ∆ºø¯¿∫ ¿˚¿Ã æ∆¥œ¥Ÿ.
				}
			}
			if( PKMGR->IsPKAllow(dwChannel) || ((CPlayer*)pOperator)->GetShopItemManager()->GetUsingItemInfo( eIncantation_Tracking_Jin ) )
			if( WANTEDMGR->IsOwner((CPlayer*)pOperator, ((CPlayer*)pTarget)->GetWantedIdx()) == TRUE )
				return TRUE;		//√¥ªÏ ¥ÎªÛ¿⁄¥¬ ¿˚¿Ã¥Ÿ.
#else
			if( pOperator == HERO )
			{
				if( PARTYMGR->IsPartyMember( pTarget->GetID() ) )
				{
//					if( OBJECTMGR->GetSelectedObjectID() == pTarget->GetID() )
//					{ //π¸¿ß∞¯∞›¿∏∑Œ ∆ƒ∆ºø¯¿ª ∞¯∞›«“∂ß¥¬ ∏ﬁºº¡ˆ æ¯∞‘ «œ±‚ ¿ß«ÿ
//						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
//					}
					return FALSE;	//∆ƒ∆ºø¯¿∫ ¿˚¿Ã æ∆¥œ¥Ÿ.
				}
				if( PKMGR->IsPKAllow() )
				if( WANTEDMGR->IsChrIDInList( pTarget->GetID() ) )
					return TRUE;	//√¥ªÏ ¥ÎªÛ¿⁄¥¬ ¿˚¿Ã¥Ÿ.
			}
#endif

#ifdef _MAPSERVER_
			if( PKMGR->IsPKAllow(dwChannel) )
#else
			if( PKMGR->IsPKAllow() )
#endif
			{
				if( ((CPlayer*)pOperator)->IsPKMode() )				
				{
					if(pOperator == pTarget )
						return FALSE;
					else
						return TRUE;	//¿⁄Ω≈¿Ã PK∏µÂ¿Ã∏È ¿⁄Ω≈¿ª ª´ ≥™∏”¡ˆ¥¬ ¿˚¿Ã¥Ÿ.
				}
				if( ((CPlayer*)pTarget)->IsPKMode() )
					return TRUE;	//ªÛ¥Î∞° PK∏µÂ¿Ã∏È ¿˚¿Ã¥Ÿ.
			}

			return FALSE;
		}
		else if( pOperator->GetObjectKind() & eObjectKind_Monster )
		{	// Operator ∞° ∏ÛΩ∫≈Õ¿œ∞ÊøÏ	//ªÁΩ« ø©±‚ ∞…∏± ¿œ¿Ã æ¯¥Ÿ. ∏ÛΩ∫≈Õ¥¬ ∫Òπ´∏¶ ∏¯«œπ«∑Œ
			if( pTarget->GetObjectKind() & eObjectKind_Monster )	// ∏ÛΩ∫≈Õ¥¬ ¿˚¿Ã æ∆¥‘
				return FALSE;
		}
		else if(pTarget->GetObjectKind() == eObjectKind_SkillObject)
		{
			return TRUE;
		}
		else	//≥™∏”¡ˆ¥¬ ¿˚¿Ã æ∆¥œ¥Ÿ.
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CBattle_VimuStreet::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if(pOperator->GetBattleID() != pTarget->GetBattleID())
		return FALSE;
	
	if(pOperator->GetBattleTeam() != pTarget->GetBattleTeam())
		return FALSE;
	
	return TRUE;
}

// event func
void CBattle_VimuStreet::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::OnCreate(pCreateInfo,pTeam1,pTeam2);

	memcpy(&m_VimuStreetInfo,pCreateInfo,sizeof(BATTLE_INFO_VIMUSTREET));

	m_Team[0].SetCharacterID(m_VimuStreetInfo.Character[VIMUTEAM_BLUE]);	
	m_Team[1].SetCharacterID(m_VimuStreetInfo.Character[VIMUTEAM_RED]);

#ifndef _MAPSERVER_	
	m_ImageNumber.Init( 32, 0 );
	m_ImageNumber.SetFillZero( FALSE );
	m_ImageNumber.SetLimitCipher( 2 );
	m_ImageNumber.SetPosition( 512 - 16, 80 );
	m_ImageNumber.SetScale( 3.0f, 3.0f );

	m_dwFadeOutStartTime = 0;
	m_bFadeOut	 = FALSE;

//	SCRIPTMGR->GetImage( 59, &m_ImageReady, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 60, &m_ImageStart, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 61, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 76, &m_ImageDraw, PFT_HARDPATH );
	
//	m_pCurShowImage = &m_ImageReady;

	m_vTitlePos.x = (1024 - 512) / 2;	//128 width
	m_vTitlePos.y = 32;
	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	
	CTileManager* pTileManager = MAP->GetTileManager();

	//∫Òπ´¿Â¿ª ∏∏µÈ¿⁄...
	VECTOR3 vPos	= m_VimuStreetInfo.vStgPos;
	VECTOR3 vStgPos;

	float fx, fz = vPos.z;
//	RECT rc = { -100.0f, -50.0f, -50.0f, 0.0f };
	float fAdd = -50.0f;

	for( fx = vPos.x - 1000.0f ; fx <= vPos.x + 1000.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1000.0f;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1000.0f + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1000.0f ; fz <= vPos.z + 1000.0f + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - 1000.0f;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1000.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

//---¿”Ω√	
	fz = vPos.z;
	for( fx = vPos.x - 1050.0f ; fx <= vPos.x + 1050.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1050.0f;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1050.0f + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1050.0f ; fz <= vPos.z + 1050.0f + fAdd ; fz += 50.0f )
	{
		vStgPos.x = fx - 1050.0f;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1050.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
//------------------

	//∞≈∏Ææ»ø° ¿÷≥™? æ¯¿∏∏È? confirm
	
	CPlayer* pPlayer1 = (CPlayer*)OBJECTMGR->GetObject( m_VimuStreetInfo.Character[0] );
	CPlayer* pPlayer2 = (CPlayer*)OBJECTMGR->GetObject( m_VimuStreetInfo.Character[1] );
	if( pPlayer1 )
	if( pPlayer1->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR->EndObjectState(pPlayer1, eObjectState_BattleReady );
	if( pPlayer2 )
	if( pPlayer2->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR->EndObjectState(pPlayer2, eObjectState_BattleReady );

#else

//	m_bDieByOp = FALSE;
	m_bDieByOp[0] = FALSE;
	m_bDieByOp[1] = FALSE;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[1]);
	if( pPlayer1 )
	if( pPlayer1->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_BattleReady);

	if( pPlayer2 )
	if( pPlayer2->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);

#endif


//∞¢¡æ √≥∏Æ....
//-¿Ã∆Â∆Æ∏¶ √≥∏Æ«—¥Ÿ.
//-ƒ¡∆Æ∑— ¥©∏£¡ˆ æ ∞Ìµµ ªÛ¥Î∏¶ ∞¯∞›∞°¥…«œµµ∑œ «—¥Ÿ.
//-¥Ÿ∏• ƒ≥∏Ø≈Õ¿« PK∏¶ ∏∑¥¬¥Ÿ.

}

void CBattle_VimuStreet::OnFightStart()
{
	CBattle::OnFightStart();

#ifndef _MAPSERVER_
	m_pCurShowImage = &m_ImageStart;
	m_ImageNumber.SetFillZero( TRUE );
	m_ImageNumber.SetFadeOut( 0 );	//fadeout off
	m_ImageNumber.SetScale( 1.0f, 1.0f );
	m_ImageNumber.SetPosition( 512 + 32, 130 );
#endif
	
}

void CBattle_VimuStreet::OnDestroy()
{

#ifndef _MAPSERVER_	

	CTileManager* pTileManager = MAP->GetTileManager();
	//∫Òπ´¿Â¿ª «ÿ√º
	VECTOR3 vPos	= m_VimuStreetInfo.vStgPos;
	VECTOR3 vStgPos;

	float fx, fz = vPos.z;
	float fAdd = -50.0f;
	
	for( fx = vPos.x - 1000.0f ; fx <= vPos.x + 1000.0f + fAdd ; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1000.0f;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1000.0f + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
	fx = vPos.x;
	for( fz = vPos.z - 1000.0f ; fz <= vPos.z + 1000.0f + fAdd ; fz += 50.0f )
	{
		vStgPos.x = fx - 1000.0f;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1000.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

//---¿”Ω√	
	fz = vPos.z;
	for( fx = vPos.x - 1050.0f ; fx <= vPos.x + 1050.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1050.0f;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1050.00f + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1050.0f ; fz <= vPos.z + 1050.0f + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - 1050.0f;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1050.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
//------------------

#else

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[1]);
	if( pPlayer1 )
	{
		pPlayer1->SetBattleID(pPlayer1->GetGridID());
		pPlayer1->SetVimuing( FALSE );

		/////////////////////////////////////////////////////////////////////////////////
		// 06. 06. µøΩ√ø° ¡◊¿ªΩ√ 2π¯ Player¥¬ ∫Òπ´ ¡◊¿Ω √≥∏Æ æ»µ«¥¬ πÆ¡¶ «ÿ∞· - ¿Ãøµ¡ÿ
		// m_bDieByOp -> m_bDieByOp[0]
		if( pPlayer1->GetState() == eObjectState_Die && m_bDieByOp[0] )
		{
			pPlayer1->ReviveAfterVimu();
			m_bDieByOp[0] = FALSE;
		}
		if(m_VimuStreetInfo.bIsGoldBattle)
		{//[—∫◊¢±»Œ‰—∫ΩÕÀªπ][BY: Æ¿Ô∆¬Ω£…Ò][QQ:112582793][2019-4-27][19:57:08]
			pPlayer1->SetGoldMoney(m_VimuStreetInfo.dwGoldMoney/2,eGetGoldMoney);
			ItemShopUseLog(eLog_VimuBattleGetForNoWiner,pPlayer1->GetID(),pPlayer1->GetObjectName(),0,"±»Œ‰—∫Ω",0,
				0,0,0,
				pPlayer1->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney / 2, pPlayer1->GetGoldMoney() + m_VimuStreetInfo.dwGoldMoney / 2,
				0,0,0);
			//LogGoldMoney(eLog_VimuBattleGetForNoWiner, 65533, 0, pPlayer1->GetID(), pPlayer1->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney / 2, 0,0, 0);
		}
	}
	if( pPlayer2 )
	{
		pPlayer2->SetBattleID(pPlayer2->GetGridID());
		pPlayer2->SetVimuing( FALSE );
		
		/////////////////////////////////////////////////////////////////////////////////
		// 06. 06. µøΩ√ø° ¡◊¿ªΩ√ 2π¯ Player¥¬ ∫Òπ´ ¡◊¿Ω √≥∏Æ æ»µ«¥¬ πÆ¡¶ «ÿ∞· - ¿Ãøµ¡ÿ
		// m_bDieByOp -> m_bDieByOp[1]
		if( pPlayer2->GetState() == eObjectState_Die && m_bDieByOp[1] )
		{
			pPlayer2->ReviveAfterVimu();
			m_bDieByOp[1] = FALSE;
		}
		if(m_VimuStreetInfo.bIsGoldBattle)
		{//[—∫◊¢±»Œ‰—∫ΩÕÀªπ][BY: Æ¿Ô∆¬Ω£…Ò][QQ:112582793][2019-4-27][19:57:08]
			pPlayer2->SetGoldMoney(m_VimuStreetInfo.dwGoldMoney/2,eGetGoldMoney);
			ItemShopUseLog(eLog_VimuBattleGetForNoWiner, pPlayer2->GetID(), pPlayer2->GetObjectName(), 0, "±»Œ‰—∫Ω", 0,
				0, 0, 0,
				pPlayer2->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney / 2, pPlayer2->GetGoldMoney() + m_VimuStreetInfo.dwGoldMoney / 2,
				0, 0, 0);
			//LogGoldMoney(eLog_VimuBattleGetForNoWiner, 65533, 0, pPlayer2->GetID(), pPlayer2->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney / 2, 0,0,0);
		}
	}

#endif

	CBattle::OnDestroy();
	
}

void CBattle_VimuStreet::OnTeamMemberAdd( int Team, DWORD MemberID, char* Name )
{
#ifdef _MAPSERVER_
	CObject* pObject = g_pUserTable->FindUser(MemberID);
	ASSERT(pObject);

	MSG_BATTLE_VIMU_CREATESTAGE msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_VIMU_CREATESTAGE;
	msg.dwObjectID = pObject->GetID();
	msg.dwBattleID = pObject->GetBattleID();
	msg.vPosStage = m_VimuStreetInfo.vStgPos;
	pObject->SendMsg( &msg, sizeof(msg) );

	
#endif
}

BOOL CBattle_VimuStreet::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
	ASSERT(Team < eBattleTeam_Max);
	
#ifdef _MAPSERVER_
	CObject* pVictimMember = g_pUserTable->FindUser(VictimMemberID);
	CObject* pKiller = g_pUserTable->FindUser(KillerID);
	if(pVictimMember == NULL || pKiller == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	
	ASSERT(pVictimMember->GetBattleID() == GetBattleID());

	if( pVictimMember->GetObjectKind() != eObjectKind_Player )
	{
		return FALSE;
	}
	
	((CBattleTeam_VimuStreet*)m_TeamArray[Team])->AddTeamDieNum();

	if(pKiller->GetBattleID() != GetBattleID())
	{
		if( pKiller->GetObjectKind() == eObjectKind_SkillObject )
		{
			if( ((CSkillObject*)pKiller)->GetOperator()->GetBattleID() != GetBattleID() )
				return FALSE;

			ASSERT(0);
		}
		else
		{
			return FALSE;
		}
	}

	if(m_VimuStreetInfo.Character[0] == VictimMemberID)
		m_bDieByOp[0] = TRUE; 
	else
		m_bDieByOp[1] = TRUE;

	((CPlayer*)pVictimMember)->m_bNeedRevive = FALSE;		
	
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	MSG_DWORD3 msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_DIE_NOTIFY);
	msg.dwData1 = Team;
	msg.dwData2 = VictimMemberID;
	msg.dwData3 = KillerID;
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam

	
#endif
	return FALSE;
	
}

void CBattle_VimuStreet::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}

void CBattle_VimuStreet::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}

BOOL CBattle_VimuStreet::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{	
#ifdef _MAPSERVER_
/*
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( MemberID );

		if( pPlayer )
		if( pPlayer->GetState() == eObjectState_Die )
			pPlayer->ReviveAfterVimu();	
*/
#endif

	return TRUE;
}


//serverø°º≠∏∏ ªÁøÎ
void CBattle_VimuStreet::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
//			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
//			if( ElapTime > BATTLE_VIMUSTREET_READYTIME)
//			{
//				ToEachTeam(pTeam)
//					if(pTeam->GetAliveTeamMemberNum() == 0)
//						break;
//				EndToEachTeam
				
//				StartBattle();
//			}
			
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_VIMUSTREET_READYTIME )
			{
				for( int n = 0; n < eBattleTeam_Max; ++n )
				{
					if( m_TeamArray[n]->GetAliveTeamMemberNum() == 0 )
					{
						Draw();
						return;
					}
				}

				StartBattle();
			}

		yCASE(eBATTLE_STATE_FIGHT)
			Judge();

		yCASE(eBATTLE_STATE_RESULT)
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_VIMUSTREET_RESULTTIME)
			{
				SetDestroyFlag();
				
			}
	
	yENDSWITCH
}

// Ω¬∆– ∆«¡§

BOOL CBattle_VimuStreet::JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam)
{
	CBattleTeam_VimuStreet* pTheTeam = (CBattleTeam_VimuStreet*)m_TeamArray[TheTeam];
	CBattleTeam_VimuStreet* pOtherTeam = (CBattleTeam_VimuStreet*)m_TeamArray[OtherTeam];

//	if(pOtherTeam->GetAliveTeamMemberNum() == 0)
	if( pTheTeam->GetTeamDieNum() == 0 )				//KKK
	if( pOtherTeam->GetTeamDieNum() != 0 )
	{
		Victory( TheTeam, OtherTeam );
		return TRUE;
	}

	return FALSE;
}

 
BOOL CBattle_VimuStreet::Judge()
{

	if(JudgeOneTeamWinsOtherTeam(eBattleTeam1,eBattleTeam2) == TRUE)
		return TRUE;

	if(JudgeOneTeamWinsOtherTeam(eBattleTeam2,eBattleTeam1) == TRUE)
		return TRUE;

	if( ((CBattleTeam_VimuStreet*)m_TeamArray[eBattleTeam1])->GetTeamDieNum()
		&& ((CBattleTeam_VimuStreet*)m_TeamArray[eBattleTeam2])->GetTeamDieNum() )
		Draw();


	DWORD ElapsedTime = gCurTime - m_BattleInfo.BattleTime;
	if(ElapsedTime > BATTLE_VIMUSTREET_FIGHTTIME)
		Draw();

	return FALSE;
}


void CBattle_VimuStreet::Victory(int WinnerTeamNum,int LoserTeamNum)
{
	CBattle::Victory(WinnerTeamNum,LoserTeamNum);
	
#ifndef _MAPSERVER_
	if( HERO->GetBattleTeam() == WinnerTeamNum )
	{
		m_pCurShowImage = &m_ImageWin;
	}
	else if( HERO->GetBattleTeam() == LoserTeamNum )
	{
		m_pCurShowImage = &m_ImageLose;
	}


	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	// 
	m_vTitlePos.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - 512 * m_vTitleScale.x ) / 2;	//128 = width
//	m_vTitlePos.y = ( 768 - 128 * m_vTitleScale.y ) / 2;		//42 = height
	m_vTitlePos.y = 100;

	//AutoAttack∏ÿ√„
	HERO->DisableAutoAttack();					//∞¯∞›¡ﬂ ¿Ã¥¯∞≈ √Îº“µ«≥™?
	HERO->SetNextAction(NULL);			//Ω∫≈≥ æ≤¥¯∞Õµµ √Îº“µ«≥™?

#else
//	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[1]);
	CPlayer* pWinner = NULL;
	CPlayer* pLoser = NULL;

	if( WinnerTeamNum == 0 )
	{
		pWinner = pPlayer1;
		pLoser = pPlayer2;
	}
	else if( WinnerTeamNum == 1 )
	{
		pWinner = pPlayer2;
		pLoser = pPlayer1;
	}

	if( pPlayer1 == NULL || pPlayer2 == NULL || pWinner == NULL )
	{
		if(m_VimuStreetInfo.bIsGoldBattle&&pWinner!=NULL)
		{//[—∫◊¢±»Œ‰][BY: Æ¿Ô∆¬Ω£…Ò][QQ:112582793][2019-4-27][16:31:26]
			pWinner->SetGoldMoney(m_VimuStreetInfo.dwGoldMoney,eGetGoldMoney);
			ItemShopUseLog(eLog_VimuBattleGet,pWinner->GetID(),pWinner->GetObjectName(),0,"±»Œ‰ªÒ §",0,
				0,0,0,
				pWinner->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney, pWinner->GetGoldMoney() + m_VimuStreetInfo.dwGoldMoney,
				0,0,0);
			//LogGoldMoney(eLog_VimuBattleGet, 65533, 0, pWinner->GetID(), pWinner->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney, 0,0,0);
			MSG_DWORD2 resultMsg;
			resultMsg.Category		= MP_SIGNAL;
			resultMsg.Protocol		= MP_SIGNAL_GOLDVIMU_RESULT;
			resultMsg.dwObjectID	= pWinner->GetID();
			resultMsg.dwData1		= m_VimuStreetInfo.dwGoldMoney/2;		//winner
			resultMsg.dwData2		= 0;		//loser
			PACKEDDATA_OBJ->QuickSend( pWinner, &resultMsg, sizeof(resultMsg) );
			m_VimuStreetInfo.bIsGoldBattle = FALSE;
		}
		return;
	}

	if(m_VimuStreetInfo.bIsGoldBattle)
	{//[—∫◊¢±»Œ‰][BY: Æ¿Ô∆¬Ω£…Ò][QQ:112582793][2019-4-27][16:31:26]
		pWinner->SetGoldMoney(m_VimuStreetInfo.dwGoldMoney,eGetGoldMoney);
		ItemShopUseLog(eLog_VimuBattleGet, pWinner->GetID(), pWinner->GetObjectName(), 0, "±»Œ‰ªÒ §", 0,
			0, 0, 0,
			pWinner->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney, pWinner->GetGoldMoney() + m_VimuStreetInfo.dwGoldMoney,
			0, 0, 0);
	//	LogGoldMoney(eLog_VimuBattleGet, pLoser->GetID(), pLoser->GetGoldMoney() - m_VimuStreetInfo.dwGoldMoney, pWinner->GetID(), pWinner->GetGoldMoney(), m_VimuStreetInfo.dwGoldMoney, 0,0,0);
		MSG_DWORD2 resultMsg;
		resultMsg.Category		= MP_SIGNAL;
		resultMsg.Protocol		= MP_SIGNAL_GOLDVIMU_RESULT;
		resultMsg.dwObjectID	= pWinner->GetID();
		resultMsg.dwData1		= m_VimuStreetInfo.dwGoldMoney/2;		//winner
		resultMsg.dwData2		= pLoser->GetID();		//loser
		PACKEDDATA_OBJ->QuickSend( pWinner, &resultMsg, sizeof(resultMsg) );
		m_VimuStreetInfo.bIsGoldBattle = FALSE;
	}
	else
	{
		MSG_DWORD2 resultMsg;
		resultMsg.Category		= MP_SIGNAL;
		resultMsg.Protocol		= MP_SIGNAL_VIMU_RESULT;
		resultMsg.dwObjectID	= pWinner->GetID();
		resultMsg.dwData1		= pWinner->GetID();		//winner
		resultMsg.dwData2		= pLoser->GetID();		//loser
		PACKEDDATA_OBJ->QuickSend( pWinner, &resultMsg, sizeof(resultMsg) );
	}


/*
	TESTMSG resultMsg;
	resultMsg.Category		= MP_SIGNAL;
	resultMsg.Protocol		= MP_SIGNAL_VIMU_RESULT;
	resultMsg.dwObjectID	= pWinner->GetID();
	//strcpy( resultMsg.Msg, buf );
	SafeStrCpy( resultMsg.Msg, buf, MAX_CHAT_LENGTH+1);
	//PACKEDDATA_OBJ->QuickSend( pWinner, (MSGBASE*)&resultMsg, sizeof(TESTMSG) );
	PACKEDDATA_OBJ->QuickSend( pWinner, (MSGBASE*)&resultMsg, resultMsg.GetMsgLength() );	//CHATMSG 040324
*/	

#endif
}

void CBattle_VimuStreet::Draw()
{
	CBattle::Draw();	

#ifndef _MAPSERVER_
	m_pCurShowImage = &m_ImageDraw;

	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	// 
	m_vTitlePos.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - 512 * m_vTitleScale.x ) / 2;	//128 = width
//	m_vTitlePos.y = ( 768 - 128 * m_vTitleScale.y ) / 2;		//42 = height
	m_vTitlePos.y = 100;
	
	//AutoAttack∏ÿ√„
	HERO->DisableAutoAttack();					//∞¯∞›¡ﬂ ¿Ã¥¯∞≈ √Îº“µ«≥™?
	HERO->SetNextAction(NULL);			//Ω∫≈≥ æ≤¥¯∞Õµµ √Îº“µ«≥™?
#else
//	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_VimuStreetInfo.Character[1]);

	if( pPlayer1 == NULL || pPlayer2 == NULL  )
		return;

	char buf[128];
	sprintf( buf, "%s¥‘∞˙ %s¥‘¿« ∫Òπ´∞° π´Ω¬∫Œ∑Œ ≥°≥µΩ¿¥œ¥Ÿ.",
			 pPlayer1->GetObjectName(),
			 pPlayer2->GetObjectName() );

	TESTMSG resultMsg;
	resultMsg.Category		= MP_SIGNAL;
	resultMsg.Protocol		= MP_SIGNAL_BATTLE;
	resultMsg.dwObjectID	= pPlayer1->GetID();
	//strcpy( resultMsg.Msg, buf );	
	SafeStrCpy( resultMsg.Msg, buf, MAX_CHAT_LENGTH+1 );

	//PACKEDDATA_OBJ->QuickSend( pPlayer1, (MSGBASE*)&resultMsg, sizeof(TESTMSG) );
	PACKEDDATA_OBJ->QuickSend( pPlayer1, (MSGBASE*)&resultMsg, resultMsg.GetMsgLength() );	//CHATMSG 040324
	
#endif
}



#ifdef _MHCLIENT_

#include "interface/cFont.h"
//#include "interface/cImage.h"
#include "ImageNumber.h"

void CBattle_VimuStreet::Render()
{


//Title
//	RECT rect = { 30, 100, 500, 150 };
//	char temp[256];
//	sprintf( temp, "VimuStreet Battle" );
//	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffffffff );

//¡¶«—Ω√∞£...

//	SetRect( &rect, 30, 120, 500, 150 );
//	RECT rect = { 30, 100, 500, 150 };
	DWORD ElapsedTime = gCurTime-m_BattleInfo.BattleTime;


	switch(m_BattleInfo.BattleState )
	{
	case eBATTLE_STATE_READY:
		{
			//»≠∏È¡ﬂ∞£ø°
			DWORD RemainTime = BATTLE_VIMUSTREET_READYTIME > ElapsedTime ? BATTLE_VIMUSTREET_READYTIME - ElapsedTime : 0;
			//sprintf( temp, "Battle Start in %d sec", RemainTime / 1000 );
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			if( m_ImageNumber.IsNumberChanged() )
				m_ImageNumber.SetFadeOut( 500 );

			m_ImageNumber.Render();
			
//			if( m_pCurShowImage )
//				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );
		}
		break;

	case eBATTLE_STATE_FIGHT:
		{
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );

			//»≠∏È¡ﬂ∞£ø° ≥™ø‘¥Ÿ∞° ¿ß∑Œ?
			DWORD RemainTime = BATTLE_VIMUSTREET_FIGHTTIME > ElapsedTime ? BATTLE_VIMUSTREET_FIGHTTIME - ElapsedTime : 0;
			//sprintf( temp, "Battle End in %d sec", RemainTime / 1000 );
			m_ImageNumber.SetNumber( RemainTime / 1000 );

//			m_ImageNumber.SetPosition( 512 + 32, 130 ); //130
//			m_ImageNumber.SetPosition( 512 + 32, 30 ); //130
			m_ImageNumber.Render();			

		}
		break;

	case eBATTLE_STATE_RESULT:
		{
			DWORD dwAlpha = 255;
			if( !m_bFadeOut )
			{
				if( ElapsedTime > 5500 )
				{
					m_dwFadeOutStartTime = gCurTime;
					m_bFadeOut = TRUE;
				}
			}
			else
			{
				DWORD dwElapsed = gCurTime - m_dwFadeOutStartTime;
				DWORD dwMinusAlpha = 255 * dwElapsed / 1000;
				if( dwMinusAlpha < 255 )
				{
					dwAlpha = 255 - dwMinusAlpha;
				}
				else
				{
					dwAlpha = 0;
				}
			}

			if(m_WinnerTeam == eBattleTeam_Max)
			{				
				if( m_pCurShowImage )
					m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0x00ffffff | (dwAlpha << 24) );
			}
			else
			{
				if(ElapsedTime > 2500)
				{
					if( m_pCurShowImage )
						m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0x00ffffff | (dwAlpha << 24) );
				}
			}
		}
		break;
	}

	//CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);	

}

#endif
// } end virtual func
//////////////////////////////////////////////////////////////////////////
