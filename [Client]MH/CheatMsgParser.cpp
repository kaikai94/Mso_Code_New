







// CheatMsgParser.cpp: implementation of the CCheatMsgParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatManager.h"
#include "CheatMsgParser.h"
#include "mhnetwork.h"
#include "objectmanager.h"
#include "Movemanager.h"
#include "ItemManager.h"
#include "DebugDlg.h"
#include "GameIn.h"
#include "MAINGAME.h"
#include "StatsCalcManager.h"
#include "PeaceWarModeManager.h"
#include "ObjectStateManager.h"
#include "./Audio/MHAudioManager.h"
#include "FilteringTable.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "mhfile.h"

#include "InventoryExDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "cDivideBox.h"
#include "QuickManager.h"
#include "UserInfoManager.h"

#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "GMNotifyManager.h"
#include "EventNotifyDialog.h"

#include "GuildFieldWarDialog.h"
#include "PartyWar.h"
#include "GuildUnion.h"

#include "cJackpotManager.h"
#include "MHMap.h"

#ifdef _GMTOOL_
#include "MHCamera.h"
#endif

#include "CharacterDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"

#include "PetManager.h"
#include "MugongDialog.h"

#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"

#include "GuildManager.h"
#include "BobusangManager.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
#include "ChannelDialog.h"
#include "FortWarDialog.h"
#include "CheckResourceFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCheatMsgParser)


#ifdef _CHEATENABLE_

BOOL CheatFunc(char* cheat)
{
	char code[256];
	char buf[256] = {0,};
	sscanf(cheat,"%s",code);
	strupr(code);
	DWORD R=0,G=0,B=0;
	DWORD Colors=0;
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_
	if( strcmp(code,"MONEYBEGETSMONEY") == 0 )
	{
		CHEATMGR->SetCheatEnable( TRUE );
		
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001010010101110100101010101010010010011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "0100101001 [ 欢迎进入新世界 ] 01101101001" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "11001101010100111010100001010010101111011" );
		
		return TRUE;
	}

	if( !CHEATMGR->IsCheatEnable() ) return FALSE;
	if(strcmp(code,"COLOR")==0)
	{
		sscanf(cheat,"%s %d %d %d",code,&R,&G,&B);
		Colors = RGB(R,G,B);
		ITEMMGR->SetDwordColor(Colors);
		return TRUE;
	}
	else if(strcmp(code,"CAMERAMODE")==0)
	{
		int onoff;
		sscanf(cheat,"%s %d",code,&onoff);

		if(onoff)
			HERO->GetEngineObject()->Show();
		else
			HERO->GetEngineObject()->HideWithScheduling();


		MSG_DWORD2 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_SETVISIBLE;
		msg.dwObjectID = HEROID;
		msg.dwData1 = HEROID;
		msg.dwData2 = (DWORD)onoff;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "ITEMREADY")==0)
	{
		char Dum[128] = {0,};
		sscanf(cheat, "%s %s", code, Dum);
		strupr(Dum);
		if(strcmp(Dum, "THIS") == 0)
		{
			CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_Reinforce;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]开启强化模式，请将欲强化物品放入强化窗口");
		}
		else
		{
			CHEATMGR->m_wTargetItemIdx = atoi(Dum);
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(atoi(Dum));
			if( !pItemInfo )
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "输入的物品编号不存在，请重新输入");
				return TRUE;
			}
			else
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "设定物品名称 [%s]", pItemInfo->ItemName);
			}
			CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_Rare;
		}
		memset(&CHEATMGR->m_OptionInfo, 0, sizeof(ITEM_OPTION_INFO));
		CHEATMGR->m_bReadyToOptionCheat = TRUE;
		return TRUE;
	}
	else if(strcmp(code, "OPT") == 0)
	{
		if(!CHEATMGR->m_bReadyToOptionCheat)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]未设定物品信息，不能设定强化\绿化参数");
			return TRUE;
		}
		if( CHEATMGR->m_wItemOptionKind == CCheatMsgParser::eKindOption_Reinforce && !CHEATMGR->m_wTargetItemIdx )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]请打开强化窗口，放入需强化物品");
			return TRUE;
		}
		DWORD OptionValue = 0;
		sscanf(cheat, "%s %s %d", code, buf, &OptionValue);
		strupr(buf);
		if( OptionValue > 1000 )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]数值设定限制为1000");
			return TRUE;
		}
		if( strcmp(buf,"G") == 0 )
		{
			CHEATMGR->m_OptionInfo.GenGol = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定力量=%d",OptionValue);
		}
		else if( strcmp(buf,"M") == 0 )
		{
			CHEATMGR->m_OptionInfo.MinChub = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定敏捷=%d",OptionValue);
		}
		else if( strcmp(buf,"C") == 0 )
		{
			CHEATMGR->m_OptionInfo.CheRyuk = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定体质=%d",OptionValue);
		}
		else if( strcmp(buf,"S") == 0 )
		{
			CHEATMGR->m_OptionInfo.SimMek = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定心脉=%d",OptionValue);
		}
		else if( strcmp(buf,"CR") == 0 )
		{
			CHEATMGR->m_OptionInfo.Life = OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定生命值=%d",OptionValue);
		}
		else if( strcmp(buf,"NR") == 0 )
		{
			CHEATMGR->m_OptionInfo.NaeRyuk = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定内力值=%d",OptionValue);
		}
		else if( strcmp(buf,"HS") == 0 )
		{
			CHEATMGR->m_OptionInfo.Shield = OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定护体值=%d",OptionValue);
		}
		else if( strcmp(buf,"PA") == 0 )
		{
			CHEATMGR->m_OptionInfo.PhyAttack = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定物理攻击=%d",OptionValue);
		}
		else if( strcmp(buf,"PD") == 0 )
		{
			CHEATMGR->m_OptionInfo.PhyDefense = (WORD)OptionValue;
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定物理防御=%d",OptionValue);
		}
		else if( strcmp(buf,"CT") == 0 )
		{
			if(CHEATMGR->m_wItemOptionKind == CCheatMsgParser::eKindOption_Rare)
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]绿化物品不能设置一击属性");
				return TRUE;
			}
			CHEATMGR->m_OptionInfo.CriticalPercent = (WORD)OptionValue;
		}
		else if( strcmp(buf,"TA") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定木属性攻击=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"FA") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定火属性攻击=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"EA") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定土属性攻击=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"IA") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定金属性攻击=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"WA") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定水属性攻击=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"TD") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定木属性防御=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"FD") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定火属性防御=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"ED") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定土属性防御=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"ID") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定金属性防御=%f",OptionValue/100.f);
		}
		else if( strcmp(buf,"WD") == 0 )
		{
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, OptionValue/100.f);
			CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定水属性防御=%f",OptionValue/100.f);
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]强化\绿化参数无效");
			return TRUE;
		}

		++CHEATMGR->m_OptionCount;

		return TRUE;
	}
	else if(strcmp(code,"WISHCHARID")==0)
	{
		sscanf(cheat, "%s %d", code,&CHEATMGR->m_WishItemCharID);
		CHATMGR->AddMsg(CTC_CHEAT_2, "[提示]设定物品获得角色ID=%d",CHEATMGR->m_WishItemCharID);
		return TRUE;
	}
	else if( strcmp(code, "CLEAROPTION") == 0 )
	{
		if(CHEATMGR->m_bReadyToOptionCheat)
			memset(&CHEATMGR->m_OptionInfo, 0, sizeof(ITEM_OPTION_INFO));
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Nothing to Clear, Do it Step by Step");
			return TRUE;
		}

		CHEATMGR->m_OptionCount = 0;

		return TRUE;
	}

	else if( strcmp(code, "WISHITEM") == 0 )
	{
		if(!CHEATMGR->m_OptionCount)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]未设定物品强化\绿化信息，请设定后重新获取" );
			return TRUE;
		}
		if(!CHEATMGR->m_bReadyToOptionCheat || !CHEATMGR->m_wItemOptionKind ) 
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "[错误]未设定获取物品信息，请重新设置物品信息后再获取");
			return TRUE;
		}
		SEND_CHEAT_ITEM_OPTION msg;
		if(CHEATMGR->m_WishItemCharID==0)
			CHEATMGR->m_WishItemCharID = HEROID;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_OPTION_SYN;
		msg.dwObjectID = CHEATMGR->m_WishItemCharID;
		msg.bPosition = CHEATMGR->m_ItemAbsPos;
		msg.wItemIdx = CHEATMGR->m_wTargetItemIdx;
		msg.wOptionKind = CHEATMGR->m_wItemOptionKind;

		msg.OptionInfo = CHEATMGR->m_OptionInfo;
		NETWORK->Send(&msg,sizeof(msg));

		CHEATMGR->m_bReadyToOptionCheat = FALSE;
		CHEATMGR->m_wTargetItemIdx = 0;
		CHEATMGR->m_ItemAbsPos = 0;
		CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_None;
		CHEATMGR->m_WishItemCharID = 0 ;
		return TRUE;
	}
	else if(strcmp(code, "ANSVH")==0)
	{
		DWORD PlayerID = 0;
		DWORD GuildIdx = 0;
		CObject* TObj = CHEATMGR->GetCheatTargetObject();
		if(TObj)
		{
			if(TObj->GetObjectKind() != eObjectKind_Player && TObj->GetObjectKind() != eObjectKind_Npc)
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetKind is not a Player");
				return TRUE;
			}

			if(TObj->GetObjectKind() == eObjectKind_Player)
			{
				PlayerID = ((CPlayer*)TObj)->GetID();
				GuildIdx = ((CPlayer*)TObj)->GetGuildIdx();
			}
			else if(TObj->GetObjectKind() == eObjectKind_Npc)
			{
				PlayerID = HEROID;
				GuildIdx = HERO->GetGuildIdx();
			}

			if( 0 == GuildIdx )
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetPlayer is not in a guild");
				return TRUE;
			}
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : NonTarget" );
			return TRUE;
		}

		DWORD GuildPoint = 0;
		sscanf(cheat, "%s %d", code, &GuildPoint);

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GUILDPOINT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = PlayerID;
		msg.dwData2 = GuildIdx;
		msg.dwData3 = GuildPoint;

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "LEVELUP") == 0)
	{
		int val = 1;
		sscanf(cheat,"%s %d",code,&val);
		if(val <= HERO->GetLevel())
			val = HERO->GetLevel()+1;
		if(CHEATMGR->m_WishItemCharID==0)
			CHEATMGR->m_WishItemCharID = HEROID;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg.dwObjectID = CHEATMGR->m_WishItemCharID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	else if(strcmp(code, "RMS") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GENGOL_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "ALS") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MINCHUB_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "CP") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHERYUK_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
#endif
#endif

	return FALSE;
}

#endif //_CHEATENABLE_


CCheatMsgParser::CCheatMsgParser()
{
	m_bCheatEnable = FALSE;

	m_pTargetObj = NULL;
	m_wItemOptionKind		= 0;
	m_bReadyToOptionCheat	= FALSE;
	m_wTargetItemIdx		= 0;
	m_ItemAbsPos			= 0;
	m_OptionCount			= 0;
	m_WishItemCharID		= 0;
}

CCheatMsgParser::~CCheatMsgParser()
{

}

void CCheatMsgParser::NetworkMsgParse(BYTE Category,BYTE Protocol,MSGBASE* pMsg)
{
	ASSERT(Category == MP_CHEAT);

	switch(Protocol) 
	{
	case MP_CHEAT_BOBUSANGINFO_GET_ACK:
		{//[客户端获取包袱商人信息][BY:十里坡剑神][QQ:112582793][2019-6-12][13:40:03]
			MSG_BOBUSANG_INFO_GET *pmsg = (MSG_BOBUSANG_INFO_GET*)pMsg;
			stTIME StartTime;
			StartTime.SetTime(pmsg->dwAppearanceTime);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2373), StartTime.GetHour(), StartTime.GetMinute(), pmsg->strMapName, pmsg->vPos.x, pmsg->vPos.y);
		}
		break;
	case MP_CHEAT_LOACLINFO_GET:
		{//[获取地理位置信息发送到服务端][BY:十里坡剑神][QQ:112582793][2019-9-15][12:42:21]
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
			MSG_WHERE_INFO msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_LOCALINFO_SYN;
			FILECHECK->GetLocalInfo(msg.LocalInfo);
			msg.dwSenderID = pmsg->dwSenderID;
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;
#ifdef _CHEATENABLE_
	case MP_CHEAT_CHANGEMAP_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			MAPTYPE mapnum = pmsg->wData;
			if(mapnum < 200)							
				PEACEWARMGR->Peace_WarMode(HERO, TRUE);

			MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&mapnum,4);
		}
		break;

	case MP_CHEAT_CHANGEMAP_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			if( pmsg->dwData == 0 )
			{
			}
			else
			{
				gChannelNum = pmsg->dwData;
			}
		}
		break;

	case MP_CHEAT_WHEREIS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
#ifdef _GMTOOL_
			GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );	
#endif
		}
		break;

	case MP_CHEAT_LOCALINFO_ACK:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
			GMTOOLMGR->DisplayLocalInfo(pmsg->LocalInfo);
		}
		break;
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;

			VECTOR3 vPos;
			pmsg->cpos.Decompress( &vPos );

			char* strMapName = MAP->GetMapName( pmsg->wMapNum );
			if( strMapName[0] == 0 )
			{
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
#ifdef _GMTOOL_
				GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );
#endif
			}
			else
			{
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
					strMapName, pmsg->bChannel,
					(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );
#ifdef _GMTOOL_
				GMTOOLMGR->DisplayWhereInfo( strMapName, pmsg->bChannel,
					(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );
#endif
			}
		}
		break;

	case MP_CHEAT_WHEREIS_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
			case 1:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 433 ) );
				break;
			default:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 434 ) );
				break;
			}

		}
		break;

	case MP_CHEAT_BLOCKCHARACTER_ACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 435 ) );

		}
		break;

	case MP_CHEAT_BLOCKCHARACTER_NACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_ACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 436 ) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
		}
		break;
	case MP_CHEAT_AGENTCHECK_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "CHEAT_AGENT_ACK!" );
		}
		break;

#endif	//_CHEATENABLE_
	case MP_CHEAT_ITEM_ACK:
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

			CItem* pItem = NULL;
			CItem* pPreItem = NULL;

			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
				}
				else
				{
					pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_CHEAT_ITEM_ACK");	
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );
			}


			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 121 ), pItem->GetItemInfo()->ItemName );

			ITEMMGR->ItemDropEffect( pItem->GetItemIdx() );

			QUICKMGR->RefreshQickItem();

			//[物品掉落开箱子只能在这里判断][By:十里坡剑神][QQ:112582793][2017/12/6]
			if(ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
			{
				ITEMMGR->FakeDeleteItem(pItem);

				ITEMMGR->SendDeleteItem();
				//ITEMMGR->SendSellItem(pItem);

				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 2094 ), pItem->GetItemInfo()->ItemName );
			}


		}
		break;
	case MP_CHEAT_MOVE_ACK:		
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			VECTOR3 pos;			
			pmsg->cpos.Decompress(&pos);

			BASEMOVE_INFO info;
			info.bMoving = FALSE;
			info.KyungGongIdx = 0;
			info.MoveMode = eMoveMode_Run;
			info.CurPosition = pos;
			MOVEMGR->InitMove(HERO,&info);

			if(HERO->GetPet())
			{
				MOVEMGR->InitMove(HERO->GetPet(),&info);
			}

			if(HERO->GetState() == eObjectState_Immortal || HERO->GetState() == eObjectState_Deal)
			{
                 OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MP_CHEAT_HIDE_ACK:	
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if( !pPlayer ) return;
			
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
			
			if(pmsg->dwData2 == 0 )
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
					pPlayer->GetEngineObject()->SetAlpha( 1.0f );
				else

#endif
					pPlayer->GetEngineObject()->Show();
				
				pPlayer->GetCharacterTotalInfo()->bVisible = TRUE;				
			}
			else
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
					pPlayer->GetEngineObject()->SetAlpha( 0.3f );
				else
#endif
					pPlayer->GetEngineObject()->HideWithScheduling();

				
				pPlayer->GetCharacterTotalInfo()->bVisible = FALSE;
				if( pPlayer->GetID() == OBJECTMGR->GetSelectedObjectID() )
				{
					OBJECTMGR->SetSelectedObject( NULL );	
				}
			}

			OBJECTMGR->ApplyShadowOption(pPlayer);	
			OBJECTMGR->ApplyOverInfoOption(pPlayer);

		}
		break;
		
	case MP_CHEAT_LIFE_ACK:
		{
		}
		break;
		
	case MP_CHEAT_MP_ACK:
		{
		}
		break;
		
#ifdef _GMTOOL_
	case MP_CHEAT_GM_LOGIN_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			{
				GMTOOLMGR->Login( TRUE, (int)pmsg->dwData );
			}
		}
		break;


	case MP_CHEAT_GM_LOGIN_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			{
				GMTOOLMGR->Login( FALSE, (int)pmsg->bData );
			}
		}
		break;
	case MP_CHEAT_PARTYINFO_ACK:
		{
			PARTY_INFO * pmsg = (PARTY_INFO*)pMsg;
			for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
			{
				if(pmsg->Member[i].dwMemberID == 0)
					SafeStrCpy(pmsg->Member[i].Name, "!!!", MAX_NAME_LENGTH+1);
				CHATMGR->AddMsg( CTC_SYSMSG, "%s, %d, %d, %d", 
					pmsg->Member[i].Name, pmsg->Member[i].Level, pmsg->Member[i].bLogged,
					pmsg->Member[i].LifePercent);
			}
		}
		break;
		
	case MP_CHEAT_EVENTMAP_RESULT:
		{
			MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
			if( pMsg->dwObjectID == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
			}
			else if( pMsg->dwObjectID == 1 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
			}
			else if( pMsg->dwObjectID == 2 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 717 ), pmsg->wData );
			}

			EVENTMAP->Init();
		}
		break;
#endif

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

			NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			NOTIFYMGR->SetEventNotify( TRUE );

			if( GAMEIN->GetEventNotifyDialog() )
			{
				GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
				GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );
				GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
				NOTIFYMGR->SetEventNotifyChanged( FALSE );
			}
			else
			{
				NOTIFYMGR->SetEventNotifyChanged( TRUE );
			}

			NOTIFYMGR->ResetEventApply();
			for(int i=0; i<eEvent_Max; ++i)
			{
				if( pmsg->EventList[i] )
					NOTIFYMGR->SetEventApply( i );
			}
		}
		break;
		
	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			NOTIFYMGR->SetEventNotify( FALSE );
			NOTIFYMGR->SetEventNotifyChanged( FALSE );

			if( GAMEIN->GetEventNotifyDialog() )
			{
				GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );	
			}
		}
		break;
	case MP_CHEAT_PET_SKILLREADY_ACK:
		{
			PETMGR->SetSkillRechargeAmount(PET_MAX_SKILL_CHARGE);
			PETMGR->SetSkillGuageFull(TRUE);
			PETMGR->SetReadyToSendSkillMsg(TRUE);
		}
	case MP_CHEAT_PET_FRIENDSHIP_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 1)
				CHATMGR->AddMsg( CTC_SYSMSG, "There is No Summoned Pet" );
			else if(pmsg->dwData == 2)
				CHATMGR->AddMsg( CTC_SYSMSG, "Friendship Limit Error" );
		}
		break;
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			char tmp[256];
			wsprintf(tmp, "TargetPet'sFriendship has changed from %d to %d", pmsg->dwData1/100000, pmsg->dwData2/100000 );
			CHATMGR->AddMsg( CTC_SYSMSG, tmp );
		}
		break;
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			enum tempflag{Err_PFR_OverValue = 0xfffffa, Err_PFR_NonSummon = 0xfffffb};

			if(pmsg->dwData1 == Err_PFR_OverValue)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "Friendship Limit Error" );
			}
			else if(pmsg->dwData2 == Err_PFR_NonSummon)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "There is No Summoned Pet" );
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_ACK:
		{
			MSG_BOBUSANG_INFO_REQUEST* pmsg = (MSG_BOBUSANG_INFO_REQUEST*)pMsg;
#ifdef _GMTOOL_ 
			BOBUSANGMGR->PrintInfo(&pmsg->bobusangInfo[0],"CUR", pmsg->appearanceState);
			BOBUSANGMGR->PrintInfo(&pmsg->bobusangInfo[1],"NEXT");
#endif
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[BobusangInfo] Request failed : Channel Num is not valiable" );
		}
		break;
	case MP_CHEAT_BOBUSANG_LEAVE_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[BobusangInfo] Request Succeed" );
		}
		break;
	case MP_CHEAT_BOBUSANG_LEAVE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( 99 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : Channel Num is not valiable" );
			else if( 999 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : Map Num is not valiable" );
			else if( 9999 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : There is no Bobusang in that channel" );
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_CHANGE_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[Change BobusangInfo] Request Succeed" );
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_CHANGE_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[Change BobusangInfo] Request failed : Channel Num is not valiable" );
		}
		break;
	case MP_CHEAT_BOBUSANG_ONOFF_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( TRUE == pmsg->dwData )
			{

				CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSANG is ACTIVED.." );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSNAG is DEACTIVED.." );
		}
		break;
	case MP_CHEAT_BOBUSANG_ONOFF_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSNAG ACTIVATION is NOT CHANGED" );
		}
		break;
//#ifdef _JAPAN_LOCAL_
//	case MP_CHEAT_CHANGE_MAINATTR_ACK:
//		{
//			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
//			if( HEROID == pmsg->dwObjectID )
//			{
//				HERO->SetMainCharAttr( LOWORD(pmsg->dwData) );
//				HERO->SetSubCharAttr( 0 );
//
//				CHATMGR->AddMsg( CTC_SYSMSG, "Change Character Main Attribute" );
//				
//				ITEMMGR->RefreshAllItem();
//				GAMEIN->GetCharacterDialog()->UpdateData();
//			}
//		}
//		break;
//	case MP_CHEAT_CHANGE_SUBATTR_ACK:
//		{
//			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
//			if( HEROID == pmsg->dwObjectID )
//			{
//				HERO->SetMainCharAttr( LOWORD(pmsg->dwData) );
//				HERO->SetSubCharAttr( HIWORD(pmsg->dwData) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "Change Character Sub Attribute" );
//
//				ITEMMGR->RefreshAllItem();
//				GAMEIN->GetCharacterDialog()->UpdateData();
//			}
//		}
//		break;
//	case MP_CHEAT_CHANGE_MAINATTR_NACK:
//	case MP_CHEAT_CHANGE_SUBATTR_NACK:
//		break;
//#endif
	case MP_CHEAT_NPCHIDE_ACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

//#ifndef _HK_LOCAL_	//hk block
			OBJECTMGR->HideNpc(pmsg->wData1, (BOOL)pmsg->wData2);
//#endif
		}
		break;
	case MP_CHEAT_NPCHIDE_NACK:
		{
		}
		break;

	case MP_CHEAT_MAP_CONDITION:
		{
			MSG_PROSESSINFO* pmsg = (MSG_PROSESSINFO*)pMsg;

			int k = 0;
		}
		break;
/*
	case MP_CHEAT_FBOSSMONSTER_POSITION_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			CHATMGR->AddMsg( CTC_SYSMSG, "FBossMonster Position : [ %d ] [ %d ]", pmsg->dwData1/100, pmsg->dwData2/100);
		}
		break;

	case MP_CHEAT_FBOSSMONSTER_POSITION_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "FeildBossMonster Not Regen." );
		}
		break;*/
	case MP_CHEAT_ITEMLIMIT_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			if( pmsg->dwData1 == 1 )			// on/off
			{
				if( pmsg->dwData3 == 0 )
                    CHATMGR->AddMsg( CTC_SYSMSG, "酒捞袍[%d]包访 捞亥飘啊 Off登菌嚼聪促.", pmsg->dwData2 );
				else if( pmsg->dwData3 == 1 )
					CHATMGR->AddMsg( CTC_SYSMSG, "酒捞袍[%d]包访 捞亥飘啊 On登菌嚼聪促..", pmsg->dwData2 );

			}
			else if( pmsg->dwData1 == 2 )		// count
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "酒捞袍[%d]狼 力茄荐啊 %d俺肺 汲沥登菌嚼聪促.", pmsg->dwData2, pmsg->dwData3 );
			}
		}
		break;
	case MP_CHEAT_ITEMLIMIT_NACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if( pmsg->dwData1 == 1 )			// on/off
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "酒捞袍[%d]狼 力茄汲沥捞 阂啊瓷钦聪促.", pmsg->dwData2 );
			}
			else if( pmsg->dwData1 == 2 )		// count
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "酒捞袍[%d]狼 力茄汲沥捞 阂啊瓷钦聪促.", pmsg->dwData2 );
			}
		}
		break;
	case MP_CHEAT_AUTONOTE_SETTING_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			switch( pmsg->dwData1 )
			{
			case 0:
				{
					if( pmsg->dwData2 == 0 )			// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "混积何 荤侩阂啊瓷茄 甘栏肺 汲沥登菌嚼聪促." );
					}
					else if( pmsg->dwData2 == 1 )		// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "混积何 荤侩啊瓷茄 甘栏肺 汲沥登菌嚼聪促." );
					}
				}
				break;
			case 1:	CHATMGR->AddMsg( CTC_SYSMSG, "混积何 览翠矫埃捞 %d檬肺 汲沥登菌嚼聪促.", pmsg->dwData2 );	break;
			case 2:	CHATMGR->AddMsg( CTC_SYSMSG, "混积何 览翠扁雀啊 %d锅栏肺 汲沥登菌嚼聪促.", pmsg->dwData2 );	break;
			case 3:	CHATMGR->AddMsg( CTC_SYSMSG, "混积何 犁荤侩矫埃捞 %盒栏肺 汲沥登菌嚼聪促.", pmsg->dwData2 );	break;
			case 4:	CHATMGR->AddMsg( CTC_SYSMSG, "混积何 喉钒矫埃捞 %盒栏肺 汲沥登菌嚼聪促.", pmsg->dwData2 );	break;
			case 5:
				{
					if( pmsg->dwData2 == 0 )			// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "混积何 荤侩捞 阂啊瓷窍档废 汲沥登菌嚼聪促." );
					}
					else if( pmsg->dwData2 == 1 )		// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "混积何 荤侩捞 啊瓷窍档废 汲沥登菌嚼聪促." );
					}
				}
				break;
			}			
		}
		break;
#ifdef _GMTOOL_
	case MP_CHEAT_GAMECFG_GET_ACK:
		{
			MSG_GAMECFG * pmsg = (MSG_GAMECFG*)pMsg;
			GMTOOLMGR->UpdateGameConfig(pmsg);
		}
		break;
#endif  //_GMTOOL_
	default:
		{
		}
		break;

	}
}






