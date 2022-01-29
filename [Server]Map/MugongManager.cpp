



// MugongManager.cpp: implementation of the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "SkillManager_server.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongManager::CMugongManager()
{

}

CMugongManager::~CMugongManager()
{

}
void CMugongManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_MUGONG_MOVE_SYN:
		{
			MSG_MUGONG_MOVE_SYN * msg = (MSG_MUGONG_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);			
			if(!pPlayer) return;
			if(MoveMugong(pPlayer, msg->FromPos, msg->ToPos))
			{
				MSG_MUGONG_MOVE_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_MUGONG_MOVE_SYN));
				msg2.Protocol = MP_MUGONG_MOVE_ACK;
				pPlayer->SendMsg(&msg2, sizeof(MSG_MUGONG_MOVE_ACK));
			}
			else
			{
				MSGBASE Msg;
				Msg.Category = MP_MUGONG;
				Msg.Protocol = MP_MUGONG_MOVE_NACK;
				pPlayer->SendMsg(&Msg, sizeof(Msg));
			}
		}
		break;
/*
			case MP_MUGONG_ADD_SYN:
				{
					MSG_MUGONG_ADD * msg = (MSG_MUGONG_ADD *)pMsg;
					CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
					if(pPlayer == NULL)
						return;
					MUGONGMGR_OBJ->AddMugong(pPlayer, msg);
				}
				break;*/
	case MP_MUGONG_REM_SYN:
		{
			MSG_MUGONG_REM_SYN * msg = (MSG_MUGONG_REM_SYN *)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(pPlayer == NULL)
				return;
			if(RemMugong(pPlayer, msg->wMugongIdx, msg->TargetPos,eLog_MugongDiscard))
			{
				MSG_MUGONG_REM_ACK ToMsg;
				ToMsg.Category = MP_MUGONG;
				ToMsg.Protocol = MP_MUGONG_REM_ACK;
				ToMsg.dwObjectID = msg->dwObjectID;
				ToMsg.wMugongIdx = msg->wMugongIdx;
				ToMsg.TargetPos = msg->TargetPos;

				pPlayer->SendMsg(&ToMsg, sizeof(ToMsg));
			}

			else
			{
				MSGBASE Msg;
				Msg.Category = MP_MUGONG;
				Msg.Protocol = MP_MUGONG_REM_NACK;
				pPlayer->SendMsg(&Msg, sizeof(Msg));
			}
		}
		break;
	case MP_MUGONG_OPTION_SYN:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSG_WORD4 msg;
			msg.Category = MP_MUGONG;
			msg.wData1 = pmsg->wData1;
			msg.wData2 = pmsg->wData2;
			msg.wData3 = pmsg->wData3;
			msg.wData4 = pmsg->wData4;

			if( SetOption(pPlayer, pmsg->wData1, pmsg->wData2, pmsg->wData3, pmsg->wData4) )
				msg.Protocol = MP_MUGONG_OPTION_ACK;
			else
				msg.Protocol = MP_MUGONG_OPTION_NACK;
			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	case MP_MUGONG_OPTION_CLEAR_SYN:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSG_WORD4 msg;
			msg.Category = MP_MUGONG;
			msg.wData1 = pmsg->wData1;
			msg.wData2 = pmsg->wData2;
			msg.wData3 = pmsg->wData3;
			msg.wData4 = pmsg->wData4;

			if( ClearOption(pPlayer, pmsg->wData1, pmsg->wData2, pmsg->wData3, pmsg->wData4) )
				msg.Protocol = MP_MUGONG_OPTION_CLEAR_ACK;
			else
				msg.Protocol = MP_MUGONG_OPTION_CLEAR_NACK;

			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	default:
		break;
	} 
}
BOOL CMugongManager::RemMugong(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, BYTE bType)
{
	MUGONGBASE * pMugong = pPlayer->GetMugongBase(TargetPos);
	if(!pMugong || pMugong->wIconIdx != wMugongIdx)
		return FALSE;

	// db
	MugongDeleteToDB(pMugong->dwDBIdx);

	InsertLogMugong( bType, pPlayer->GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, pMugong->Sung, pMugong->ExpPoint );

	memset(pMugong, 0, sizeof(MUGONGBASE));

	return TRUE;
}

BOOL CMugongManager::MoveMugong(CPlayer * pPlayer, POSTYPE FromPos, POSTYPE ToPos)
{	
	if(FromPos == ToPos)
		return FALSE;

//#ifdef _JAPAN_LOCAL_
//
//		if( FromPos < TP_JINBUB_START )
//		{
//			DWORD EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//			if( ToPos > EndPos )
//				return FALSE;
//		}
//
//#elif defined _HK_LOCAL_
//		if( FromPos < TP_JINBUB_START )
//		{
//			DWORD EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//			if( ToPos > EndPos )
//				return FALSE;
//		}
//
//#elif defined _TL_LOCAL_
//		if( FromPos < TP_JINBUB_START )
//		{
//			DWORD EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//			if( ToPos > EndPos )
//				return FALSE;
//		}
//
//#else

	if( FromPos >= TP_JINBUB_START && FromPos < TP_JINBUB_END )
	{
		if( ToPos >= TP_MUGONG1_START && ToPos < TP_MUGONG2_END )
			return FALSE;
		if( ToPos >= TP_TITANMUGONG_START && ToPos < TP_TITANMUGONG_END )
			return FALSE;
	}
	else if( FromPos >= TP_MUGONG1_START && FromPos < TP_MUGONG2_END )
	{
		if( ToPos >= TP_JINBUB_START && ToPos < TP_JINBUB_END )
			return FALSE;
		if( ToPos >= TP_TITANMUGONG_START && ToPos < TP_TITANMUGONG_END )
			return FALSE;
	}
	else if( FromPos >= TP_TITANMUGONG_START && FromPos < TP_TITANMUGONG_END )
	{
		if( ToPos >= TP_MUGONG1_START && ToPos < TP_MUGONG2_END )
			return FALSE;
		if( ToPos >= TP_JINBUB_START && ToPos < TP_JINBUB_END )
			return FALSE;
	}

//#endif	// _JAPAN_LOCAL_	
	
	MUGONGBASE FromMugong = *pPlayer->GetMugongBase(FromPos);
	MUGONGBASE ToMugong = *pPlayer->GetMugongBase(ToPos);

	if( ToMugong.Position == 0 ) 
	{
		FromMugong.Position = ToPos;
		pPlayer->SetMugongBase(FromPos, &ToMugong);
		pPlayer->SetMugongBase(FromMugong.Position, &FromMugong);

		MugongUpdateToDB(&FromMugong,"MOVE_NULL");

	}
	else
	{
		if( FromMugong.dwDBIdx == 0 )
			return FALSE;
		
		SWAPVALUE(FromMugong.Position, ToMugong.Position)
		ASSERT(FromMugong.Position);
		pPlayer->SetMugongBase(FromMugong.Position, &FromMugong);
		pPlayer->SetMugongBase(ToMugong.Position, &ToMugong);

		MugongMoveUpdateToDB(FromMugong.dwDBIdx, FromMugong.Position, ToMugong.dwDBIdx, ToMugong.Position, "MOVE_FROM");

	}

	//CharacterMugongUpdate(FromMugong);
	//CharacterMugongUpdate(ToMugong);

	return TRUE;
}

// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
/*
MUGONGBASE* CMugongManager::GetTitanMugongBase( CPlayer * pPlayer, WORD idx )

{
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(idx);
	if( pInfo == NULL )
		return NULL;

	WORD mugongIdx = pInfo->GetSkillInfo()->LinkSkillIdx;
	// ÀÌ¹Ì ¹è¿üÀ¸¸é Ã³¸®¾ÈÇÔ
	MUGONGBASE* pTitanMugong = pPlayer->GetMugongBaseByMugongIdx(mugongIdx);
	if( pTitanMugong )
        return pTitanMugong;

	return NULL;
}
*/

void CMugongManager::AddMugongDBResult(CPlayer * pPlayer, MUGONGBASE * pMugongBase)
{
	MSG_MUGONG_ADD_ACK msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_ADD_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.MugongBase = *pMugongBase;

	pPlayer->SetMugongBase(pMugongBase->Position, pMugongBase);
	pPlayer->SendMsg(&msg, sizeof(MSG_MUGONG_ADD_ACK));

	/*if(pMugongBase->QuickPosition)
	{
		MSG_QUICK_ADD_ACK msg;
		msg.Category = MP_QUICK;
		msg.Protocol = MP_QUICK_ADD_ACK;
		msg.OldSrcItemIdx = 0;
		msg.OldSrcPos = 0;
		msg.SrcPos = pMugongBase->Position;
		msg.SrcItemIdx = pMugongBase->wIconIdx;
		msg.QuickPos = pMugongBase->QuickPosition;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}*/
	InsertLogMugong( eLog_MugongLearn, pPlayer->GetID(), pMugongBase->wIconIdx, pMugongBase->dwDBIdx, pMugongBase->Sung, pMugongBase->ExpPoint );
}
BOOL CMugongManager::AddMugong( CPlayer * pPlayer, WORD wMugongIdx, WORD ItemKind, POSTYPE QuickPos, BYTE bSung, WORD Option)
{
	if(pPlayer->GetMugongBaseByMugongIdx(wMugongIdx))
		return FALSE;
	
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(wMugongIdx);
	if( pInfo == NULL )
		return FALSE;

	SKILL_CHANGE_INFO * pChangeInfo = SKILLMGR->GetSkillChangeInfo(wMugongIdx);
	if(pChangeInfo)
	{
		if(pPlayer->GetMugongBaseByMugongIdx(pChangeInfo->wTargetMugongIdx))
			return FALSE;
	}

	POSTYPE StartPos,EndPos;
	if(ItemKind == eMUGONG_ITEM_JINBUB)
	{
		StartPos = TP_JINBUB_START;
		EndPos = TP_JINBUB_END;
	}
	else if(ItemKind == eMUGONG_ITEM_TITAN)
	{
		StartPos = TP_TITANMUGONG_START;
		EndPos = TP_TITANMUGONG_END;
	}
	else
	{		
//#ifdef _JAPAN_LOCAL_
//		StartPos = TP_MUGONG_START;
//		EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//#elif defined _HK_LOCAL_
//		StartPos = TP_MUGONG_START;
//		EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//#elif defined _TL_LOCAL_
//		StartPos = TP_MUGONG_START;
//		EndPos = TP_MUGONG_START + GIVEN_MUGONG_SLOT + ( MUGONG_SLOT_ADDCOUNT*pPlayer->GetExtraMugongSlot() );
//#else
		StartPos = TP_MUGONG1_START;
		EndPos = TP_MUGONG1_END;
		if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;
//#endif	// _JAPAN_LOCAL_
	}

	POSTYPE ToPos = EndPos;
	MUGONGBASE* pMugong;
	for( POSTYPE n = StartPos ; n < EndPos ; ++n )
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong->dwDBIdx == 0)
		{
			ToPos = n;
			break;
		}
	}
	if(ToPos == EndPos)
		return FALSE;

	BYTE weared = 0;
	switch(ConvAbsPos2MugongPos(ToPos))
	{
//#ifdef _JAPAN_LOCAL_
//	case TP_MUGONG_START:
//#elif defined _HK_LOCAL_
//	case TP_MUGONG_START:
//#elif defined _TL_LOCAL_
//	case TP_MUGONG_START:
//#else
	case TP_MUGONG1_START:
//#endif
	case TP_JINBUB_START:
		weared = 1;
		break;
	}

	MUGONGBASE MugongBase;
	MugongBase.Position			= ToPos;
	MugongBase.dwDBIdx			= 9;
	MugongBase.ExpPoint			= 9;
	MugongBase.QuickPosition	= QuickPos;
	MugongBase.Sung				= 9;
    MugongBase.wIconIdx			= wMugongIdx;
	MugongBase.OptionIndex		= Option;

	pPlayer->SetMugongBase(MugongBase.Position, &MugongBase);

	ASSERTMSG(ToPos, "0Â¹Ã¸ Ã†Ã·ÃÃ¶Â¼Ã‡Â¿Â¡ Â¹Â«Â°Ã¸?Â» Â³Ã–?Â¸Â·Ã Ã‡Ã•Â´ÃÂ´Ã™.");
	//db
	
	if( pInfo->GetNeedExp( 0 ) == (DWORD)(-1) )
	{
		bSung = 12;
	}

	if( pInfo->GetSkillKind() == SKILLKIND_TITAN )
	{
		bSung = 1;
	}

	WORD wSkillKind = pInfo->GetSkillKind();
	if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
	{
		WORD wSkillIndex = pInfo->GetSkillIndex();
		if( (wSkillIndex == 6001) || (wSkillIndex == 6003) || (wSkillIndex == 6005) )
		{
			bSung = 13;
		}
		else
		{
			bSung = 1;
		}
	}


	MugongInsertToDB( pPlayer->GetID(), wMugongIdx, ToPos, weared, bSung, Option,QuickPos );

	return TRUE;
}

// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
/*
BOOL CMugongManager::AddTitanMugong( CPlayer * pPlayer, WORD wMugongIdx, POSTYPE QuickPos, BYTE bSung, WORD Option)
{
	// ½ºÅ³Á¤º¸
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(wMugongIdx);
	if( pInfo == NULL )
		return FALSE;

	// ÇÃ·¹ÀÌ¾î°¡ ¹è¿î ½ºÅ³Á¤º¸
	MUGONGBASE* pMugongInfo = pPlayer->GetMugongBaseByMugongIdx(wMugongIdx);
	if(!pMugongInfo)
		return FALSE;

	WORD mugongIdx = pInfo->GetSkillInfo()->LinkSkillIdx;
	// ÀÌ¹Ì ¹è¿üÀ¸¸é Ã³¸®¾ÈÇÔ
	if(pPlayer->GetMugongBaseByMugongIdx(mugongIdx))
		return FALSE;

	BYTE weared = 0;

	MUGONGBASE MugongBase;
	MugongBase.Position			= pMugongInfo->Position;
	MugongBase.dwDBIdx			= 9; 
	MugongBase.ExpPoint			= 9;
	MugongBase.QuickPosition	= 9;
	MugongBase.Sung				= 9;
	MugongBase.wIconIdx			= mugongIdx;
	MugongBase.OptionIndex		= Option;

	pPlayer->SetMugongBase(MugongBase.Position, &MugongBase);

	ASSERTMSG(pMugongInfo->Position, "MugongManager.cpp -> Func : AddTitanMugong()");
	//db

	if( pInfo->GetNeedExp( 0 ) == (EXPTYPE)(-1) )
	{
		bSung = 12;
	}


	MugongInsertToDB( pPlayer->GetID(), mugongIdx, pMugongInfo->Position, weared, bSung, Option );

	return TRUE;
}
*/

void CMugongManager::ChangeMugong(CPlayer* pPlayer, WORD wRemMugongIdx, POSTYPE TargetPos, WORD wAddMugongIdx, WORD LogType)

{
	MUGONGBASE* pMugong = pPlayer->GetMugongBase(TargetPos);
	POSTYPE QPos = pMugong->QuickPosition;
	WORD Option = pMugong->OptionIndex;
	if(RemMugong(pPlayer, wRemMugongIdx, TargetPos, (BYTE)LogType))
	{
		MSG_MUGONG_REM_ACK ToMsg;
		ToMsg.Category = MP_MUGONG;
		ToMsg.Protocol = MP_MUGONG_REM_ACK;
		ToMsg.dwObjectID = pPlayer->GetID();

		ToMsg.wMugongIdx = wRemMugongIdx;
		ToMsg.TargetPos = TargetPos;
		
		pPlayer->SendMsg(&ToMsg, sizeof(ToMsg));
	}
	// [Îä¹¦³ö¾øºó±£³Ö¿ì½ÝÀ¸Î»ÖÃ] [by:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/12/21]
	AddMugong(pPlayer, wAddMugongIdx, eMUGONG_ITEM, QPos, 12, Option);
}

//#ifdef _JAPAN_LOCAL_
//BOOL CMugongManager::DeleteSkill( CPlayer* pPlayer, WORD wSkillIdx )
//{
//	MUGONGBASE* pMugong = pPlayer->GetMugongBaseByMugongIdx( wSkillIdx );
//	if( !pMugong || pMugong->wIconIdx != wSkillIdx )	return FALSE;
//
//	// db & Log 
//	MugongDeleteToDB( pMugong->dwDBIdx );	
//	InsertLogMugong( eLog_MugongDestroyByGetNextLevel, pPlayer->GetID(), pMugong->wIconIdx, pMugong->dwDBIdx,
//					 pMugong->Sung, pMugong->ExpPoint );
//
//	MSG_MUGONG_REM_ACK Msg;
//	Msg.Category = MP_MUGONG;
//	Msg.Protocol = MP_MUGONG_REM_ACK;
//	Msg.dwObjectID = pPlayer->GetID();
//	Msg.wMugongIdx = wSkillIdx;
//	Msg.TargetPos = pMugong->Position;
//	pPlayer->SendMsg( &Msg, sizeof(Msg) );
//
//	memset( pMugong, 0, sizeof(MUGONGBASE) );	
//
//	return TRUE;
//}
//#endif

BOOL CMugongManager::SetOption(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos)
{
	MUGONGBASE * pMugong = pPlayer->GetMugongBase(TargetPos);
	if(!pMugong || pMugong->wIconIdx != wMugongIdx)
		return FALSE;

	CSkillInfo* pInfo = NULL;
	SKILLINFO* pSInfo = NULL;
	SKILLOPTION* pOption = NULL;
	SKILLOPTION* pOldOption = NULL;

	// ½ºÅ³ Á¤º¸¸¦ °¡Á®¿Â´Ù
	pInfo = SKILLMGR->GetSkillInfo(wMugongIdx);
	if(!pInfo)	return FALSE;
 	pSInfo = pInfo->GetSkillInfo();
	if(!pSInfo)	return FALSE;


	// º¯È¯ ºÒ°¡ ½ºÅ³ÀÌ¸é ½ÇÆÐ
	if(pSInfo->ChangeKind == eSkillChange_None) return FALSE;

	// ¾ÆÀÌÅÛ Á¤º¸¸¦ °¡Á®¿Â´Ù
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	pOption = SKILLMGR->GetSkillOptionByItemIndex(ItemIdx);
	if(!pOption) return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(ItemIdx);
	if(!pItemInfo) return FALSE;

	// ·¹º§ Á¦ÇÑ
	if(pPlayer->GetLevel() < pItemInfo->LimitLevel)
		return FALSE;

	// °æÁö Á¦ÇÑ
	switch(pOption->OptionGrade)
	{
	case 1:
		// 1´Ü°è´Â ÀÏ¹Ý
		break;
	case 2:
		// 2´Ü°è´Â È­°æ ±Ø¸¶ ÀÌ»ó
		if(!(pPlayer->GetStage() & eStage_Hwa || pPlayer->GetStage() & eStage_Geuk))
			return FALSE;
		break;

	case 3:
		// 3´Ü°è´Â Çö°æ Å»¸¶ ÀÌ»ó
		if(!(pPlayer->GetStage() == eStage_Hyun || pPlayer->GetStage() == eStage_Tal))
			return FALSE;
		break;
	}

	// ¸ðµç º¯È¯ °¡´É ½ºÅ³ÀÌ ¾Æ´Ï¸é º¯È¯ °¡´É Á¾·ù¿Í ¾ÆÀÌÅÛÀÇ Á¾·ù°¡ ´Ù¸£¸é ½ÇÆÐÇÑ´Ù.
	if(pSInfo->ChangeKind != eSkillChange_All && pSInfo->ChangeKind != pOption->SkillKind)
		return FALSE;

	// ÀÌ¹Ì º¯È¯µÈ ¹«°øÀÎÁö È®ÀÎÇÑ´Ù
	pOldOption = SKILLMGR->GetSkillOption(pMugong->OptionIndex);
	if(pOldOption)
	{
		// ÀÌ¹Ì Àû¿ëµÈ º¯È¯ÀÇ Á¾·ù¿Í ´Ù¸£¸é ½ÇÆÐÇÑ´Ù.
		if(pOldOption->OptionKind != pOption->OptionKind)
			return FALSE;

		// ÀÌ¹Ì Àû¿ëµÈ º¯È¯º¸´Ù ³·°Å³ª °°Àº µî±ÞÀÌ¸é ½ÇÆÐÇÑ´Ù.
		if(pOldOption->OptionGrade >= pOption->OptionGrade)
			return FALSE;
	}

	if( EI_TRUE != ITEMMGR->DiscardItem(pPlayer, ItemPos, ItemIdx, 1) )
		return FALSE; 

	pMugong->OptionIndex = pOption->Index;

	InsertLogMugong( eLog_MugongOption, pPlayer->GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, pMugong->OptionIndex, pMugong->ExpPoint );
	MugongUpdateToDB(pMugong, "OPTION");

	return TRUE;
}

BOOL CMugongManager::ClearOption(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos)
{
	MUGONGBASE * pMugong = pPlayer->GetMugongBase(TargetPos);
	if(!pMugong || pMugong->wIconIdx != wMugongIdx)
		return FALSE;

	// ¾ÆÀÌÅÛ Á¤º¸¸¦ °¡Á®¿Â´Ù
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	// ÀÌ¹Ì º¯È¯µÈ ¹«°øÀÎÁö È®ÀÎÇÑ´Ù
	if(pMugong->OptionIndex == eSkillOption_None)
		return FALSE;

	ITEMBASE itembase = *pItem;
	
	if( EI_TRUE != ITEMMGR->DiscardItem(pPlayer, ItemPos, ItemIdx, 1) )
		return FALSE;

	if(ItemIdx == 10750)
	{		
		pPlayer->MugongLevelDown(pMugong->wIconIdx, eLog_MugongDestroyByOptionClear);

		InsertLogMugong( eLog_MugongOptionClear, pPlayer->GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, pMugong->OptionIndex, pMugong->ExpPoint );
	}
	// 06.09.15 RaMa - ¹«°øº¯È¯ÃÊ±âÈ­¾ÆÀÌÅÛ Ãß°¡(¾ÆÀÌÅÛ¸ô)
	else if( ItemIdx == eIncantation_MugongOptionReset )
	{
		InsertLogMugong( eLog_MugongOptionClearbyShopItem, pPlayer->GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, pMugong->OptionIndex, pMugong->ExpPoint );

		//
		SEND_SHOPITEM_BASEINFO usemsg;
		SetProtocol( &usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
		usemsg.ShopItemPos = ItemPos;
		usemsg.ShopItemIdx = ItemIdx;
		pPlayer->SendMsg( &usemsg, sizeof(usemsg) );


		LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",

			eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			ItemIdx, itembase.dwDBIdx, ItemPos, 0, itembase.Durability, pPlayer->GetPlayerExpPoint());
	}

	pMugong->OptionIndex = 0;	
	MugongUpdateToDB(pMugong, "OPTION");
	return TRUE;
}


