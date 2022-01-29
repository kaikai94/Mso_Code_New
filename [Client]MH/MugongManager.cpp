// MugongManager.cpp: implementation of the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongManager.h"
#include "GameIn.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"

#include "ItemManager.h"
#include "chatmanager.h"
#include "GameDef.h"
#include "QuickManager.h"
#include "SkillManager_Client.h"
#include "JinBub.h"
#include "SimBub.h"
#include "ObjectManager.h"
#include "CheatMsgParser.h"
#include "GameEventManager.h"
#include "QuickDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMugongManager)
CMugongManager::CMugongManager()
{
	m_pMugongDlg = NULL;
	m_MugongHash.Initialize(100);
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_MUGONG_STARTINDEX);		
}

CMugongManager::~CMugongManager()
{
	Release();

	m_IconIndexCreator.Release();
}

void CMugongManager::Release()
{
	CMugongBase * pMugong = NULL;
	m_MugongHash.SetPositionHead();
	while(pMugong = m_MugongHash.GetData())
	{
		DeleteMugongIcon(pMugong);
	}
	m_MugongHash.RemoveAll();
}

void CMugongManager::Init()
{
}

void CMugongManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_MUGONG_TOTALINFO_LOCAL:
		{
			MUGONG_TOTALINFO * msg = (MUGONG_TOTALINFO *)pMsg;
			for(int i = 0 ; i < SLOT_MUGONGTOTAL_NUM ; i++)
			{
				m_pMugongDlg->AddMugong(&msg->mugong[i]);
				
				if(msg->mugong[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->mugong[i].QuickPosition,msg->mugong[i].Position, msg->mugong[i].wIconIdx);
				}
			}

			for(int i = 0 ; i < SLOT_MUGONGTOTAL_NUM ; i++)
			{
				char line[64] = { 0, };
				CMugongBase* pMugong = GetMugong(msg->mugong[i].wIconIdx);

				if( pMugong )
					SetToolTipIcon(pMugong);
			}
		}
		break;
	case MP_MUGONG_MOVE_ACK:
		{
			MSG_MUGONG_MOVE_ACK * msg = (MSG_MUGONG_MOVE_ACK *)pMsg;
			m_pMugongDlg->MoveMugong(msg->ToPos, msg->FromPos);

			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_MUGONG_MOVE_NACK:
		{
			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_MUGONG_REM_ACK:
		{
			MSG_MUGONG_REM_ACK * pmsg = (MSG_MUGONG_REM_ACK *)pMsg;

			CMugongBase * mugongOut;
			m_pMugongDlg->DeleteMugong(pmsg->TargetPos, &mugongOut);
			
			DeleteMugongIcon(mugongOut);

			GAMEIN->GetMugongDialog()->SetDisable( FALSE );

			ITEMMGR->RefreshAllItem();
		}
		break;
	case MP_MUGONG_REM_NACK:
		{
			ASSERT(0);
			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_MUGONG_ADD_ACK:
		{
			MSG_MUGONG_ADD_ACK * msg = (MSG_MUGONG_ADD_ACK *)pMsg;
			m_pMugongDlg->AddMugong(&msg->MugongBase);
			
			CMugongBase * icon = GetMugong(msg->MugongBase.wIconIdx);
			if( !icon ) return;
			CSkillInfo* pCommSkill = icon->m_pSkillInfo;
			if(!pCommSkill)	return;
			if(SKILLMGR->IsDeadlyMugong(pCommSkill->GetSkillIndex()))
			{
				if(HERO->InTitan() == TRUE)
                    EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_ChangeMugong);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_Mugong_ChangeMugong);

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(791), pCommSkill->GetSkillName() );
				QUICKMGR->SetQuickItemReal(msg->MugongBase.QuickPosition,msg->MugongBase.Position,msg->MugongBase.wIconIdx);
			}
			else if(msg->MugongBase.Sung == 0) 
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(189), pCommSkill->GetSkillName() );
				GAMEIN->GetMugongDialog()->SetDisable(FALSE);				
			}
			else if(SKILLMGR->CheckSkillKind(pCommSkill->GetSkillKind()) == TRUE) 
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 184 ),pCommSkill->GetSkillName() );
			}
			else 
			{
				if( SKILLMGR->GetSkillChangeInfo( msg->MugongBase.wIconIdx ) )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 677 ),pCommSkill->GetSkillName() );
			}
			GAMEEVENTMGR->AddEvent(eGameEvent_Mugong0);

			ITEMMGR->RefreshAllItem();
		}
		break;
	case MP_MUGONG_ADD_NACK:
		{
			GAMEIN->GetMugongDialog()->SetDisable(FALSE);
		}
		break;
	case MP_MUGONG_SUNG_NOTIFY:
		{
			MSG_WORD3 * pmsg = (MSG_WORD3 * )pMsg;
			m_pMugongDlg->SetMugongSung(pmsg->wData1, (BYTE)pmsg->wData2);

			CMugongBase* mugong = (CMugongBase*)m_pMugongDlg->GetMugongAbs(pmsg->wData1);
			if( !mugong ) return;

			CSkillInfo* pInfo = mugong->m_pSkillInfo;


			if( pInfo )
			{
				if(pmsg->wData3 == eLog_MugongLevelup)
				{
					if( pmsg->wData2 < 12 )
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(182), pInfo->GetSkillName(), pmsg->wData2 );
					else
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(183), pInfo->GetSkillName() );
					
					if( pmsg->wData2 == 1 )
						GAMEEVENTMGR->AddEvent( eGameEvent_Mugong1 );
				}
				else if(pmsg->wData3 == eLog_MugongLevelDownByBadFame)
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 677 ),pInfo->GetSkillName() );
				else if(pmsg->wData3 == eLog_MugongLevelDownByWanted)
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 687 ),pInfo->GetSkillName() );
			}
			RefreshMugong();
			ITEMMGR->RefreshAllItem();
		}
		break;
	case MP_MUGONG_EXPPOINT_NOTIFY:
		{
			MSG_MUGONGEXP* pmsg = (MSG_MUGONGEXP*)pMsg;
			m_pMugongDlg->SetMugongExpPoint((POSTYPE)pmsg->absPos,pmsg->ExpPoint);
			GAMEIN->GetQuickDialog()->RefreshIcon(m_pMugongDlg->GetMugongAbs((POSTYPE)pmsg->absPos)->GetQuickPosition());
		}
		break;
	case MP_MUGONG_SUNG_LEVELUP:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )			return;

			if(pPlayer->InTitan() == TRUE)
                EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,pPlayer,NULL,0,0);
			else
				EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pPlayer,NULL,0,0);
		}
		break;
	case MP_MUGONG_OPTION_ACK:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pmsg->wData3);
			if(!pOption) return;

			CMugongBase* pMugong = GetMugongByMugongIdx(pmsg->wData1);
			if(!pMugong) return;

			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( pmsg->wData4, &pOutItem);
			

			pMugong->GetMugongBase()->OptionIndex = pOption->Index;

			SetToolTipIcon(pMugong);
			GAMEIN->GetQuickDialog()->RefreshIcon(pMugong->GetQuickPosition());

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1312 ) );
			
		}
		break;
	case MP_MUGONG_OPTION_NACK:
		{
		}
		break;
	case MP_MUGONG_OPTION_CLEAR_ACK:
		{

			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CMugongBase* pMugong = GetMugongByMugongIdx(pmsg->wData1);
			if(!pMugong) return;

			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( pmsg->wData4, &pOutItem);
			
			pMugong->GetMugongBase()->OptionIndex = 0;

			SetToolTipIcon(pMugong);
			GAMEIN->GetQuickDialog()->RefreshIcon(pMugong->GetQuickPosition());

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1326 ) );
		}
		break;
	case MP_MUGONG_OPTION_CLEAR_NACK:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wData4 );
			if( !pitembase )		return;

			CItem* pitem = ITEMMGR->GetItem( pitembase->dwDBIdx );
			if( pitem && pitem->GetItemKind() & eSHOP_ITEM )
				pitem->SetLock( FALSE );
		}
		break;
	}
}
CMugongBase * CMugongManager::NewMugongIcon(MUGONGBASE * mugongBase)
{
	CMugongBase * icon = NULL;
	if(!(icon = GetMugong(mugongBase->wIconIdx)))
	{
		cImage skillLowImage;
		cImage skillHighImage;
		GetIconImage(mugongBase->wIconIdx, &skillLowImage, &skillHighImage);
		CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
		if(pSkillInfo == NULL)
		{
			icon = new CSimBub;
		}
		else if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
		{
			icon = new CJinBub;
		}
		else
		{
			icon = new CMugongBase;
		}

		icon->Init(0,0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, &skillLowImage, &skillHighImage, m_IconIndexCreator.GenerateIndex());
		icon->SetData(mugongBase->wIconIdx);
		icon->SetMugongBase(mugongBase);
		icon->m_pSkillInfo = pSkillInfo;
		m_MugongHash.Add(icon, mugongBase->wIconIdx);
		WINDOWMGR->AddWindow(icon);
	}

	MUGONGMGR->RefreshMugong();
	return icon;
}
CMugongBase * CMugongManager::GetMugong(DWORD dwIdx)
{
	CMugongBase * pMugong;
	pMugong = m_MugongHash.GetData(dwIdx);
	return pMugong;
}
CMugongBase * CMugongManager::GetMugongByMugongIdx(WORD MugongIdx)
{
	CMugongBase * pMugong;
	m_MugongHash.SetPositionHead();
	while(pMugong = m_MugongHash.GetData())
	{
		if(pMugong->GetItemIdx() == MugongIdx)
			return pMugong;
	}
	return NULL;
}

void CMugongManager::SetToolTipIcon(CMugongBase *pMugong)
{
	CSkillInfo* pCommSkill = pMugong->m_pSkillInfo;
	if(!pCommSkill)	return;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	pMugong->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	char line[64];
	
	wsprintf(line, "[%s]", pCommSkill->GetSkillName() );

	if(pMugong->GetOption() == eSkillOption_None)
		pMugong->AddToolTipLine( line, TTTC_MUGONGNAME );
	else
		pMugong->AddToolTipLine( line, TTTC_TRANSMUGONGNAME );
	pMugong->AddToolTipLine( "" );

	wsprintf(line, CHATMGR->GetChatMsg(460), pCommSkill->GetSkillInfo()->RestrictLevel );

	if( HERO->GetCharacterTotalInfo()->Level < pCommSkill->GetSkillInfo()->RestrictLevel )
		pMugong->AddToolTipLine( line, TTTC_LIMIT );
	else
		pMugong->AddToolTipLine( line, TTTC_FREELIMIT );
	
	switch( pCommSkill->GetSkillKind())
	{
	case SKILLKIND_OUTERMUGONG: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(461), TTTC_MUGONGKIND ); break;
	case SKILLKIND_INNERMUGONG: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(462), TTTC_MUGONGKIND ); break;
	case SKILLKIND_SIMBUB:		pMugong->AddToolTipLine( CHATMGR->GetChatMsg(463), TTTC_MUGONGKIND ); break;
	case SKILLKIND_JINBUB:		pMugong->AddToolTipLine( CHATMGR->GetChatMsg(464), TTTC_MUGONGKIND ); break;
	case SKILLKIND_TITAN:		pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1654), TTTC_MUGONGKIND ); break;	// magi82 - Titan(070911) Ÿ��ź ����������Ʈ
	}

	WORD EquipType = 0;
	if(  pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN)
        EquipType = HERO->GetTitanWeaponEquipType();
	else
		EquipType = HERO->GetWeaponEquipType();

	switch(pCommSkill->GetWeaponType())
	{
	case WP_GUM:	
		{
			if( EquipType == WP_GUM )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(465), TTTC_FREELIMIT );
			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(465), TTTC_LIMIT );
		}
		break;
	case WP_GWUN:	
		{
			if( EquipType == WP_GWUN )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(466), TTTC_FREELIMIT );
			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(466), TTTC_LIMIT );
		}
		break;
	case WP_DO:	
		{
			if( EquipType == WP_DO )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(467), TTTC_FREELIMIT );
			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(467), TTTC_LIMIT );
		}
		break;
	case WP_CHANG:	
		{
			if( EquipType == WP_CHANG )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(468), TTTC_FREELIMIT );
			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(468), TTTC_LIMIT );
		}
		break;
	case WP_GUNG:	
		{
			if( EquipType == WP_GUNG )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(469), TTTC_FREELIMIT );

			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(469), TTTC_LIMIT );
		}
		break;
	case WP_AMGI:	
		{
			if( EquipType == WP_AMGI )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(470), TTTC_FREELIMIT );
			else
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(470), TTTC_LIMIT );
		}
		break;
	}

	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN )
	{
		if( HERO->InTitan() == TRUE )
            pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_FREELIMIT );
		else
			pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_LIMIT );
	}
	else
	{

		if( HERO->InTitan() == TRUE )

			pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1656), TTTC_LIMIT );
		else
			pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1656), TTTC_FREELIMIT );
	}

	if( pCommSkill->GetSkillKind() == SKILLKIND_TITAN )
	{
		if( pCommSkill->GetWeaponType() != WP_WITHOUT )
		{
			if( HERO->GetWeaponEquipType() != HERO->GetTitanWeaponEquipType() )
				pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1660), TTTC_LIMIT );
		}
	}

	int NeedExp = pCommSkill->GetNeedExp( pMugong->GetSung() );

	if( NeedExp != 0)
	{
		if( pMugong->GetSung() < 12 )
		{
			float percent = pMugong->GetExpPoint() * 100.0f / NeedExp;
			if( pMugong->GetExpPoint() != NeedExp-1 )
			if( percent > 99.99f )
			{
					percent = 99.99f;
			}

			sprintf( line, CHATMGR->GetChatMsg(471), pMugong->GetSung(), percent );
		}
		else
		{
			if(SKILLMGR->IsDeadlyMugong(pMugong->GetItemIdx()) == FALSE)
				sprintf( line, CHATMGR->GetChatMsg(405), pMugong->GetSung() );
		}
		pMugong->AddToolTipLine( line, TTTC_MUGONGEXP );
		
#ifdef _CHEATENABLE_		
		if(CHEATMGR->IsCheatEnable())
		{
			wsprintf( line, "%d / %d", pMugong->GetExpPoint(), NeedExp );
			pMugong->AddToolTipLine( line, TTTC_MUGONGEXP );
		}
#endif
	}
	
	if( pMugong->GetSung() > 0 )
	{
		wsprintf( line, CHATMGR->GetChatMsg(472), pCommSkill->GetNeedNaeRyuk(pMugong->GetSung()) );
		pMugong->AddToolTipLine( line, TTTC_MUGONGNEEDMANA );
	}
	
	switch( pCommSkill->GetAttrib() )
	{
	case ATTR_FIRE: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(473), TTTC_MUGONGATTRKIND ); break;
	case ATTR_WATER: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(474), TTTC_MUGONGATTRKIND ); break;
	case ATTR_TREE: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(475), TTTC_MUGONGATTRKIND ); break;
	case ATTR_IRON: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(476), TTTC_MUGONGATTRKIND ); break;
	case ATTR_EARTH: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(477), TTTC_MUGONGATTRKIND ); break;
	case ATTR_NONE: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(478), TTTC_MUGONGATTRKIND ); break;
	case ATTR_ALLATTR: pMugong->AddToolTipLine( CHATMGR->GetChatMsg(479), TTTC_MUGONGATTRKIND ); break;
	}

	if(pMugong->GetLastSecond())
	{
		char ToolTipText[32];
		wsprintf(ToolTipText, CHATMGR->GetChatMsg(582), pMugong->GetLastSecond() );
		pMugong->AddToolTipLine( ToolTipText, TTTC_QUESTITEM );
	}

	if(pCommSkill->GetSpecialState())
	{
		pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1359), TTTC_MUGONGNEEDMANA );
		
		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(pCommSkill->GetSpecialState());
		if(pInfo)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1360), pInfo->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_MUGONGNEEDMANA );
		}
	}
				
	SKILLOPTION* pOption = SKILLMGR->GetSkillOption(pMugong->GetOption());

	if(pOption)
	{
		pMugong->AddToolTipLine( "" );

		if(pOption->Range > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Range < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->ReduceNaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->ReduceNaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->PhyAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->BaseAtk)
		{
			char ToolTipText[32];			
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->AttAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->Life > 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );

		}
		if(pOption->Life < 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->NaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->NaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		
		if(pOption->Shield > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Shield < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->PhyDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->AttDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(-pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}

		if(pOption->Duration > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Duration < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), -pOption->Duration);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
	}

	ITEMMGR->AddItemDescriptionToolTip( pMugong, pCommSkill->GetSkillInfo()->SkillTooltipIdx );
}

void CMugongManager::GetIconImage(WORD ItemIdx, cImage * pLowImage , cImage * pHighImage)
{
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(ItemIdx);
	if(pSkillInfo == NULL)
	{
		return;
	}

	if( pSkillInfo->GetLowImage() != -1 )
	SCRIPTMGR->GetImage( pSkillInfo->GetLowImage(), pLowImage, PFT_MUGONGPATH );
	if( pSkillInfo->GetHighImage() != -1 )
	SCRIPTMGR->GetImage( pSkillInfo->GetHighImage(), pHighImage, PFT_MUGONGPATH );
}
void CMugongManager::DeleteMugongIcon(CMugongBase * mugong)
{	
	if(mugong->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(mugong->GetQuickPosition());
	}

	m_MugongHash.Remove(mugong->GetMugongBase()->wIconIdx);
	m_IconIndexCreator.ReleaseIndex(mugong->GetID());
	WINDOWMGR->AddListDestroyWindow( mugong );
}



void CMugongManager::RefreshMugong()
{
	CMugongBase * pMugong = NULL;
	m_MugongHash.SetPositionHead();
	eMUGONG_CANNOT_REASON reason;
	while(pMugong = m_MugongHash.GetData())
	{
		reason = CanUse(pMugong);
		ySWITCH(reason)
			yCASE(eMCUR_Usable)
				pMugong->SetImageRGB( ICONCLR_USABLE );
				pMugong->SetAlpha( ICONALPHA_USABLE );

				pMugong->SetFontColor( ICONTEXT_USABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANUSE );
			yCASE(eMCUR_SungZero)

				pMugong->SetImageRGB( ICONCLR_DISABLE_BYZEROSUNG );
				pMugong->SetAlpha( ICONALPHA_DISABLE_BYZEROSUNG );
				pMugong->SetFontColor( ICONTEXT_DISABLE_BYZEROSUNG );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Weapon)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Limit)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Disable)
				pMugong->SetActive(FALSE);
			yCASE(eMCUR_Titan)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
		yENDSWITCH
		
		SetToolTipIcon( pMugong );
	}
}
CMugongManager::eMUGONG_CANNOT_REASON CMugongManager::CanUse(CMugongBase * pMugong)
{
	if( pMugong == NULL ) return eMCUR_Disable;
	
	if( HERO && pMugong->m_pSkillInfo )
	if( pMugong->m_pSkillInfo->GetSkillInfo() )
	if( pMugong->m_pSkillInfo->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )
		return eMCUR_Limit;
		
	if(pMugong->GetSung() == 0)
		return eMCUR_SungZero;

	if( pMugong->m_pSkillInfo == NULL )
		return eMCUR_Disable;

	if( HERO->InTitan() == FALSE && pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN )
		return eMCUR_Titan;
	if( HERO->InTitan() == TRUE && (pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG || pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_INNERMUGONG) )
		return eMCUR_Titan;

	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG &&
		pMugong->m_pSkillInfo->GetWeaponType() != HERO->GetWeaponEquipType() )
		return eMCUR_Weapon;

	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN)
	{
		WORD wSkillWeaponType = pMugong->m_pSkillInfo->GetWeaponType();


		if( wSkillWeaponType != WP_WITHOUT )
		{
			if( wSkillWeaponType != HERO->GetTitanWeaponEquipType() )
				return eMCUR_Titan;

			if( HERO->GetWeaponEquipType() != HERO->GetTitanWeaponEquipType() )
				return eMCUR_Titan;
		}
	}

	if( HERO->GetStage() & eStage_Hwa )
	{
		if( SKILLMGR->IsGeukMaSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) )
			return eMCUR_Limit;
	}
	else if( HERO->GetStage() & eStage_Geuk )
	{
		if( SKILLMGR->IsHwaKungSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) )
			return eMCUR_Limit;
	}

	return eMCUR_Usable;
}


BOOL CMugongManager::CheckMugongTree(WORD MugongIdx)
{
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(MugongIdx);
	if(pInfo == NULL)
		return FALSE;
	
	if(pInfo->GetBeforeSkill())
	{
		CMugongBase* pMugong = GetMugongByMugongIdx(pInfo->GetBeforeSkill());
		
		if(pMugong == NULL)
			return FALSE;
		
		if(pMugong->GetSung() < 1)
			return FALSE;
	}
	return TRUE;
}

int CMugongManager::GetMugongNum()
{
	int count = 0;
	m_MugongHash.SetPositionHead();
	CMugongBase* pMugongBase;
	while(pMugongBase = m_MugongHash.GetData())
	{
		if(pMugongBase->GetSung() > 0)
			++count;
	}
	return count;
}

BOOL CMugongManager::SearchJobSkill()
{
	m_MugongHash.SetPositionHead();
	CMugongBase* pMugongBase = NULL;
	while(pMugongBase = m_MugongHash.GetData())
	{		
		CSkillInfo* pSkillInfo = pMugongBase->m_pSkillInfo;
		if(pSkillInfo == NULL)
		{
			return FALSE;
		}

		WORD wSkillKind = pSkillInfo->GetSkillKind();
		if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
		{
			return FALSE;
		}
	}

	return TRUE;
}
