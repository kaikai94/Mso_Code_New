#include "stdafx.h"
#include "PKManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "CharStateDialog.h"
#include "ChatManager.h"
#include "./Interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "VimuManager.h"
#include "PKLootingDialog.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "QuickManager.h"
#include "GameEventManager.h"
#include "MugongManager.h"
#include "ObjectStateManager.h"
#include "SkillManager_Client.h"
#include "PartyWar.h"
#include "AIManager.h"
#include "cDivideBox.h"
#include "cUnionWarManager.h"//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-29][14:27:29]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int gChannelNum;
extern bool IsPeaceChannel[];
GLOBALTON(CPKManager)

CPKManager::CPKManager()
{
	//m_dwPKContinueTime	= PKMODETIME;
	//m_htLootingDlg.Initialize( 3 );
}

CPKManager::~CPKManager()
{
}


void CPKManager::Init()
{
	m_IndexGenerator.Init(1000, IG_PKLOOTINGDLG_START);	
	m_bSyncing			= FALSE;
	m_bLooted			= FALSE;
	m_bLooting			= FALSE;
	m_bPKAllow			= FALSE;
}

void CPKManager::Release()
{
	//m_htLootingDlg.RemoveAll();
	std::map<DWORD,CPKLootingDialog*>::iterator iter;
	iter = m_htLootingDlg.begin();
	while(1)
	{
		if(iter==m_htLootingDlg.end())
			break;
		else
		{
			delete iter->second;
			iter->second= NULL;
		}
		iter++;
	}
	m_htLootingDlg.clear();
	m_IndexGenerator.Release();
}

BOOL CPKManager::HEROPKModeOn()
{
	if( !IsPKAllow() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 145 ) );
		return FALSE;
	}		
	if( PARTYWAR->IsPartyWar() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
		return FALSE;
	}
	if( m_bSyncing )			return FALSE;
	if( !HERO )					return FALSE;
	if( HERO->IsPKMode() )		return FALSE;
	if( WINDOWMGR->GetWindowForID( MBI_PKMODE ) ) return FALSE;
	//[增加安全线配置][By:十里坡剑神][QQ:112582793][2018/3/13]
	if(IsPeaceChannel[gChannelNum])
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2262 ) );
		return FALSE;
	}
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PKMODE, MBT_YESNO, CHATMGR->GetChatMsg( 149 ) );		
	if( !pMsgBox ) return FALSE;

	GAMEEVENTMGR->AddEvent( eGameEvent_PKInterface );

	return TRUE;
}

void CPKManager::SendMsgPKOn()
{

	if( HERO->GetState() == eObjectState_Die )
	{

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 557 ) );
		return;
	}

	if( VIMUMGR->IsVimuing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
		return;
	}

	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}
	////////////////////////////////////////////////////////////////////////////////

	MSGBASE msg;
	msg.Category	= MP_PK;
	msg.Protocol	= MP_PK_PKON_SYN;
	msg.dwObjectID	= HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	m_bSyncing		= TRUE;
}

BOOL CPKManager::HEROPKModeOff()
{
	if( m_bSyncing )		return FALSE;
	if( !HERO )				return FALSE;
	if( !HERO->IsPKMode() )	return FALSE;

	/*if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime )
	{*/
		MSGBASE msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_PKOFF_SYN;
		msg.dwObjectID	= HERO->GetID();
		NETWORK->Send( &msg, sizeof(msg) );
		m_bSyncing		= TRUE;
		return TRUE;
	/*}
	else
	{
		int nRemain = (int)( m_dwPKContinueTime - (gCurTime - m_dwPKModeStartTime) ) / 1000;
		if( nRemain < 0 ) nRemain = 0;				
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(146), nRemain + 1);
	}
	return FALSE;*/
}

void CPKManager::SetPlayerPKMode( CPlayer* pPlayer, BOOL bPKMode )
{
	pPlayer->SetPKMode( bPKMode );
}

BOOL CPKManager::ToggleHeroPKMode()
{
	if( HERO )
	if( HERO->IsPKMode() )
	{
		return HEROPKModeOff();
	}
	else
	{
		return HEROPKModeOn();
	}

	return FALSE;

}
#include "UnionWarInfo.h"
void CPKManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_PK_PKON_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			if( HERO )
			if( pmsg->dwData == HEROID )
			{
				m_bSyncing			= FALSE;
				m_dwPKModeStartTime	= gCurTime;
				//m_dwPKContinueTime	= HERO->GetBadFame()/200 + PKMODETIME;
				//if( m_dwPKContinueTime > 3600000 )
				//	m_dwPKContinueTime = 3600000;

				HERO->DisableAutoAttack();	
				HERO->SetNextAction(NULL);	
				if( HERO->GetState() == eObjectState_Immortal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Immortal );

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(147) );
			}
			if (cUNIONWARMGR->IsUnionWarStart())
				m_bPKAllow = TRUE;

			pPlayer->SetPKMode( TRUE );

			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);

			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);	
			}
		}
		break;

	case MP_PK_PKON_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			m_bSyncing		= FALSE;

			switch( pmsg->bData )
			{
			case ePKCODE_VIMUING:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
				break;
			case ePKCODE_STATECONFLICT:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 557 ) );
				break;
			case ePKCODE_NOTALLAW:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 145 ) );
				break;
			}
		}
		break;

	case MP_PK_PKOFF_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			pPlayer->SetPKMode( FALSE );
			
			if( HERO )
			if( pmsg->dwData == HERO->GetID() )
			{
				m_bSyncing		= FALSE;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(148) );
			}
			else
			{
				HERO->ClearTarget( pPlayer );				
			}

			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			pPlayer->RemoveObjectEffect( PK_EFFECT_ID );


		}
		break;

	case MP_PK_DISAPPEAR:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			pPlayer->SetPKMode( FALSE );
			
			if( HERO )
			if( pmsg->dwData == HERO->GetID() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(628) );
			}
			else
			{
				HERO->ClearTarget( pPlayer );
			}

			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			pPlayer->RemoveObjectEffect( PK_EFFECT_ID );
		}
		break;

	case MP_PK_PKOFF_NACK:
		{

			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			if (cUNIONWARMGR->IsUnionWarStart())
			{//[盟战地图盟战开始后不能关闭杀气操作][BY:十里坡剑神][QQ:112582793][2019-10-29][14:28:50]
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2427));
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(146), pmsg->dwData+1);
			m_bSyncing		= FALSE;
		}
		break;

	case MP_PK_LOOTING_START:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//此处拦截掠夺窗口弹出	by:十里坡剑神	QQ:112582793
			if(sCPlayerAI->sAiConfig.AutoSwitch[eAutoCloseWin])
			{
				MSG_DWORD	msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_LOOTING_ENDLOOTING;

				msg.dwObjectID	= HEROID;
				msg.dwData		= pmsg->dwData;
				NETWORK->Send( &msg, sizeof(msg) );
				break;
			}
			CreateLootingDlg( pmsg->dwData );
			SetPKLooting( TRUE );
			GAMEEVENTMGR->AddEvent( eGameEvent_KillPK );
		}
		break;

	case MP_PK_LOOTING_BEINGLOOTED:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pAttacker = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( pAttacker )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 562 ), pAttacker->GetObjectName() );
			SetPKLooted( TRUE );
			GAMEEVENTMGR->AddEvent( eGameEvent_DieFromPK );
		}
		break;
	case MP_PK_LOOTING_ITEMLOOTED_UPDATEF_DIE:
		{
			MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );
			if(pItemInfo)
			{
				GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->wAbsPosition,-50);
				QUICKMGR->RefreshQickItem();
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 564 ), 
					ITEMMGR->GetItemInfo(pItemInfo->wIconIdx)->ItemName );
			}
		}
		break;
	case MP_PK_LOOTING_ITEMLOOTING:
		{
			MSG_LOOTINGITEM* pmsg = (MSG_LOOTINGITEM*)pMsg;
			
			CItem* pItem = ITEMMGR->MakeNewItem(&pmsg->ItemInfo,"MP_PK_LOOTING_ITEMLOOTING");
			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, (MSG_LINKITEMOPTIONS*)pmsg);

			GAMEIN->GetInventoryDialog()->AddItem(pItem);

			QUICKMGR->RefreshQickItem();

			//CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwMoney );
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwMoney);

			if(iter!=m_htLootingDlg.end())
			{
				iter->second->ChangeIconImage(pmsg->wAbsPosition, eLIK_ITEM, pmsg->ItemInfo.wIconIdx );
				iter->second->AddLootingItemNum();
				iter->second->SetMsgSync( FALSE );
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 563 ), 
				ITEMMGR->GetItemInfo(pmsg->ItemInfo.wIconIdx)->ItemName );
			ITEMMGR->ItemDropEffect( pmsg->ItemInfo.wIconIdx );
		}
		break;

	case MP_PK_LOOTING_ITEMLOOTED_UPDATE:
		{//[抢夺叠加物品][By:十里坡剑神][QQ:112582793][2018/5/1 19:39]
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem *pItem = NULL;
			WORD wItemIndex = 0;
			for(int i = 0;i<pmsg->ItemNum;++i)
			{
				wItemIndex = pmsg->GetItem(i)->wIconIdx;
				if(pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
				}
				else
				{
					pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_PK_LOOTING_ITEMLOOTED_UPDATE");
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
					
				}
			}
			QUICKMGR->RefreshQickItem();
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwLootID);

			if(iter!=m_htLootingDlg.end())
			{
				iter->second->ChangeIconImage(pmsg->BuyType, eLIK_ITEM, wItemIndex );
				iter->second->AddLootingItemNum();
				iter->second->SetMsgSync( FALSE );
			}
			else
			{
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 563 ), 
				ITEMMGR->GetItemInfo(wItemIndex)->ItemName );

			ITEMMGR->ItemDropEffect( wItemIndex );
			
		}
		break;
	case MP_PK_LOOTING_ITEMLOOTED:
		{
			MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );
			if(pItemInfo)
			{
				ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition );
				QUICKMGR->RefreshQickItem();

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 564 ), 
					ITEMMGR->GetItemInfo(pItemInfo->wIconIdx)->ItemName );
			}
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		}
		break;
	case MP_PK_LOOTING_MOENYLOOTING:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwData1);
			if(iter!=m_htLootingDlg.end())
			{
				iter->second->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_MONEY );
				iter->second->AddLootingItemNum();
				iter->second->SetMsgSync( FALSE );
			}

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 565 ), AddComma( pmsg->dwData3 ) );

			ITEMMGR->MoneyDropEffect();
		}
		break;

	case MP_PK_LOOTING_MOENYLOOTED:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 566 ), AddComma( pmsg->dwData3 ) );
		}
		break;

	case MP_PK_LOOTING_EXPLOOTING:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwData1);
			if(iter!=m_htLootingDlg.end())
			{
				iter->second->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_EXP );
				iter->second->AddLootingItemNum();
				iter->second->SetMsgSync( FALSE );
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 567 ), pmsg->dwData3 );

			if( HERO->InTitan() == TRUE )
                EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_PK_LootExp);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_PK_LootExp);
		}

		break;

	case MP_PK_LOOTING_EXPLOOTED:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 568 ), pmsg->dwData3 );
		}
		break;

	case MP_PK_LOOTING_NOLOOTING:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			//CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			//if( pDlg )
			//{
			//	pDlg->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_NONE );
			//	pDlg->SetMsgSync( FALSE );
			//}
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwData1);
			if(iter!=m_htLootingDlg.end())
			{
				iter->second->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_NONE );
				iter->second->SetMsgSync( FALSE );
			}
		}
		break;
		
	case MP_PK_LOOTING_NOINVENSPACE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 354 ) );
			//CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData );
			//if( pDlg )
			//	pDlg->SetMsgSync( FALSE );
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwData );
			if(iter!=m_htLootingDlg.end())
				iter->second->SetMsgSync( FALSE );
		}
		break;

	case MP_PK_LOOTING_ENDLOOTING:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData == HEROID )
			{
				SetPKLooted( FALSE );
			}
			else
			{
				std::map<DWORD,CPKLootingDialog*>::iterator iter;
				iter = m_htLootingDlg.find(pmsg->dwData );
				if(iter!=m_htLootingDlg.end())
				{
					iter->second->SetLootingEnd( TRUE );
					CloseLootingDialog( iter->second, FALSE );
				}
				//CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData );
				//if( pDlg )
				//{
				//	pDlg->SetLootingEnd( TRUE );
				//	CloseLootingDialog( pDlg, FALSE );
				//}
			}
		}
		break;

	case MP_PK_DESTROY_ITEM:
		{
			MSG_ITEM_DESTROY* pmsg = (MSG_ITEM_DESTROY*)pMsg;


			WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );
			
			if( pItemInfo == NULL ) return;
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItemInfo->wIconIdx );
	
			if( pInfo )
			{	
				ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition );
				QUICKMGR->RefreshQickItem();
				if(pmsg->cbReason==3)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 180 ), pInfo->ItemName );
				}
				else if( pmsg->cbReason == 0 )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 569 ), pInfo->ItemName );
	
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 688 ), pInfo->ItemName );
				}
			}
		}
		break;
	case MP_PK_LOOTING_ERROR:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			//CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			//if( pDlg )
			//	pDlg->SetMsgSync( FALSE );
			std::map<DWORD,CPKLootingDialog*>::iterator iter;
			iter = m_htLootingDlg.find(pmsg->dwData1 );
			if(iter!=m_htLootingDlg.end())
				iter->second->SetMsgSync( FALSE );
			switch( pmsg->dwData2 )
			{
			case eLOOTINGERROR_OVER_DISTANCE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 561 ) );
				break;
			default:
				break;
			}

		}
		break;
	case MP_PK_PKALLOW_CHANGED:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			PKMGR->SetPKAllow( (BOOL)pmsg->bData );

			if( PKMGR->IsPKAllow() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(627) );
			}
			else
			{
				OBJECTMGR->AllPlayerPKOff();
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(626) );
			}

		}
		break;
	}
}


CPKLootingDialog* CPKManager::CreateLootingDlg( DWORD dwDiePlayerIdx )
{
	CPKLootingDialog* pLootDlg = (CPKLootingDialog*)WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/Looting.bin", "rb" );

	pLootDlg->Linking();

	LONG x = ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - 251 ) / 2;
	LONG y = ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - 275 ) / 2;

	PositioningDlg( x, y );

	pLootDlg->InitPKLootDlg( AllocWindowId(), x, y, dwDiePlayerIdx );
	WINDOWMGR->AddWindow( pLootDlg );
	
	pLootDlg->SetActive(TRUE);


	//CPKLootingDialog* pLootTempDlg = m_htLootingDlg.GetData( dwDiePlayerIdx );
	//if( pLootTempDlg )
	//	CloseLootingDialog( pLootTempDlg, FALSE );
	std::map<DWORD,CPKLootingDialog*>::iterator iter;
	iter = m_htLootingDlg.find(dwDiePlayerIdx );
	if(iter!=m_htLootingDlg.end())
		CloseLootingDialog( iter->second, FALSE );
	//m_htLootingDlg.Add( pLootDlg, dwDiePlayerIdx );
	m_htLootingDlg[dwDiePlayerIdx] = pLootDlg;
	return pLootDlg;
}

void CPKManager::PositioningDlg( LONG& rX, LONG& rY )
{
	int nCount = 0;
	//m_htLootingDlg.SetPositionHead();
	//CPKLootingDialog* pLootDlg = NULL;
	//while( pLootDlg = m_htLootingDlg.GetData() )
	//{
	//	if( (LONG)pLootDlg->GetAbsX() == rX )
	//	if( (LONG)pLootDlg->GetAbsY() == rY )
	//	{
	//		rX += 20;
	//		rY += 20;
	//		if( ++nCount >= 10 )
	//		{
	//			rX -= 200;
	//			rY -= 200;
	//			nCount = 0;
	//		}
	//	}
	//}
	std::map<DWORD,CPKLootingDialog*>::iterator iter;
	iter = m_htLootingDlg.begin();
	for(;iter!=m_htLootingDlg.end();iter++)
	{
		if( (LONG)iter->second->GetAbsX() == rX )
			if( (LONG)iter->second->GetAbsY() == rY )
			{
				rX += 20;
				rY += 20;
				if( ++nCount >= 10 )
				{
					rX -= 200;
					rY -= 200;
					nCount = 0;
				}
			}
	}
}



void CPKManager::CloseLootingDialog( CPKLootingDialog* pDlg, BOOL bMsgToServer )
{
	pDlg->SetDisable( FALSE );	
	pDlg->SetActiveRecursive( FALSE );
	pDlg->ReleaseAllIcon();
	pDlg->SetDisable( TRUE );
	
	FreeWindowId( pDlg->GetID() );
	WINDOWMGR->AddListDestroyWindow( pDlg );

	if( m_htLootingDlg.size() == 0 )
	{
		SetPKLooting( FALSE );
	}

	if( bMsgToServer )
	{
		MSG_DWORD	msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_LOOTING_ENDLOOTING;

		msg.dwObjectID	= HEROID;
		msg.dwData		= pDlg->GetDiePlayerIdx();

		NETWORK->Send( &msg, sizeof(msg) );
	}
	std::map<DWORD,CPKLootingDialog*>::iterator iter;
	iter = m_htLootingDlg.find(pDlg->GetDiePlayerIdx());
	if(iter!=m_htLootingDlg.end())
	{
		iter->second->SetMsgSync( FALSE );
		m_htLootingDlg.erase(iter);
	}
}

int CPKManager::GetLootingChance( DWORD dwBadFame )
{
//#ifdef _HK_LOCAL_
//#ifndef _TW_LOCAL_
//	if( dwBadFame < 1000 )
//		return 3;
//	else if( dwBadFame < 2000 )
//		return 4;
//	else if( dwBadFame < 5000 )
//		return 5;
//
//
//	else if( dwBadFame < 10000 )
//		return 6;
//	else if( dwBadFame < 20000 )
//		return 7;
//	else if( dwBadFame < 50000 )
//		return 8;
//	else if( dwBadFame < 100000 )
//		return 9;
//	else
//		return 10;
//#else
//
//	if( dwBadFame < 100000 )
//		return 3;
//	else if( dwBadFame < 500000 )
//		return 4;
//	else if( dwBadFame < 1000000 )
//		return 5;
//	else if( dwBadFame < 5000000 )
//		return 6;
//	else if( dwBadFame < 10000000 )
//		return 7;
//	else if( dwBadFame < 50000000 )
//		return 8;
//	else if( dwBadFame < 100000000 )
//		return 9;
//	else
//		return 10;
//#endif
//#else

	if( dwBadFame < 100000 )
		return 3;
	else if( dwBadFame < 500000 )
		return 4;
	else if( dwBadFame < 1000000 )
		return 5;
	else if( dwBadFame < 5000000 )
		return 6;
	else if( dwBadFame < 10000000 )
		return 7;
	else if( dwBadFame < 50000000 )
		return 8;
	else if( dwBadFame < 100000000 )
		return 9;
	else
		return 10;
//#endif
}

int CPKManager::GetLootingItemNum( DWORD dwBadFame )
{
//#ifdef _HK_LOCAL_
//#ifndef _TW_LOCAL
//	if( dwBadFame < 100000 )
//		return 1;
//	else if( dwBadFame < 200000 )
//		return 2;
//	else if( dwBadFame < 500000 )
//		return 3;
//	else if( dwBadFame < 1000000 )
//		return 4;
//	else
//		return 5;
//#else
//	if( dwBadFame < 100000000 )
//		return 1;
//	else if( dwBadFame < 400000000 )
//		return 2;
//	else if( dwBadFame < 700000000 )
//		return 3;
//	else if( dwBadFame < 1000000000 )
//		return 4;
//	else
//		return 5;
//#endif
//#else
	if( dwBadFame < 100000000 )
		return 1;
	else if( dwBadFame < 400000000 )
		return 2;
	else if( dwBadFame < 700000000 )
		return 3;
	else if( dwBadFame < 1000000000 )
		return 4;

	else
		return 5;
//#endif
}



