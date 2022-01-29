#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_
#include "HeroMove.h"
#include "SkillManager_Client.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "ChatDialog.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "MousePointer.h"
#include "Interface/cEditBox.h"
#include "GameResourceManager.h"
#include "ExchangeManager.h"
#include "chatmanager.h"
#include "NpcScriptManager.h"
#include "BattleSystem_Client.h"
#include "Input/UserInput.h"
#include "MouseCursor.h"
#include "Interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "GuildUnion.h" 
#include "ObjectStateManager.h"  
#include "PetBase.h"
#include "MiniMarryDialog.h"
#include "AIManager.h"
#include "CharacterRBTN.h"      //[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "FBTipDlg.h"			//[副本时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "cChangeCharCode.h"
#include "cSendItemInfoDlg.h"	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "ChannelDialog.h"		//[过图读秒引用][By:十里坡剑神][QQ:112582793][2018/5/1 20:52]
#include "AIManager.h"
#include "FortWarManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeroMove::CHeroMove()
{
	
}

CHeroMove::~CHeroMove()
{

}

void CHeroMove::OnMouseOver( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseOver( pObject );
	
	if( HERO )
	{
		CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
		if( pBattle->IsEnemy( HERO, pObject ) )
		{
			BYTE bObjectKind = pObject->GetObjectKind();
			if( bObjectKind == eObjectKind_Mining )
			{
				CURSOR->SetCursor( eCURSOR_MINING );
			}
			else if( bObjectKind == eObjectKind_Collection )
			{
				CURSOR->SetCursor( eCURSOR_COLLECTION );
			}
			else if( bObjectKind == eObjectKind_Hunt )
			{

				CURSOR->SetCursor( eCURSOR_HUNT );
			}
			else
			{
				CURSOR->SetCursor( eCURSOR_ATTACK );
			}
		}
		else
		{
			if( pObject->GetObjectKind() == eObjectKind_Npc )
			{
				if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
				{
					if( ((CNpc*)pObject)->GetNpcKind() != 30 && ((CNpc*)pObject)->GetNpcKind() != 31 )
						CURSOR->SetCursor( eCURSOR_DEALER );
				}
			}
			else if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
                CURSOR->SetCursor( eCURSOR_DEFAULT );
		}			
	}
}

void CHeroMove::OnMouseLeave( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseLeave( pObject );
}

void CHeroMove::OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if(HEROID == pPlayer->GetID()) return;
	

	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_MENU ) )
	if( GAMEIN->GetChatDialog() )
	{
		static char str[MAX_NAME_LENGTH+3];
		wsprintf( str, "/%s ", pPlayer->GetObjectName() );
#ifdef _BIG5SYSTEM_
		if(strlen(str)>=2)
		{
			CODEMGR->ChangeCharCode(str);
		}
#endif
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetEditText( str );
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetFocusEdit( TRUE );
		return;
	}

	OnAttack( pPlayer, MouseEvent );

	// 2014-05-08 Marry double click select character add name!
		
	if ( GAMEIN->GetMiniMaryDialog()->IsActive())
	{
		GAMEIN->GetMiniMaryDialog()->SetName(pPlayer->GetObjectName());
	}
}

void CHeroMove::OnClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	OnAttack( pMonster, MouseEvent );
}


void CHeroMove::OnClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	
	BYTE BattleKind = BATTLESYSTEM->GetBattle(HERO)->GetBattleKind();
	if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
			return;
		}
		
		CActionTarget Target;
		Target.InitActionTarget( pNpc, NULL );

		CAction MoveAction;
		float fDistance = NPC_TALKING_DISTANCE;
		if( pNpc->GetNpcJob() == SYMBOL_ROLE )
			fDistance = NPC_TALKING_DISTANCE/2;
		MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pNpc->GetID() )
		{
			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			CActionTarget Target;
			Target.InitActionTarget( pNpc, NULL );

			CAction MoveAction;
			float fDistance = NPC_TALKING_DISTANCE;
			if( pNpc->GetNpcJob() == SYMBOL_ROLE )
				fDistance = NPC_TALKING_DISTANCE/2;
			MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
	}
}


void CHeroMove::OnClickSkillObject(DWORD MouseEvent,CSkillObject* pObj)
{
	OnAttack( pObj, MouseEvent );
}


void CHeroMove::OnClickGround(DWORD MouseEvent,VECTOR3* pPos)
{

#ifdef _CHEATENABLE_
	if(KEYBOARD->GetKeyPressed(KEY_MENU) && KEYBOARD->GetKeyPressed(KEY_SPACE))
	{
#ifndef _GMTOOL_
		if( CHEATMGR->IsCheatEnable() )	
#endif
		{
			MOVE_POS msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOVE_SYN;
			msg.dwObjectID = HEROID;

			msg.dwMoverID = HEROID;
			msg.cpos.Compress(pPos);
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else
#endif	//_CHEATENABLE_
	{
		if(HERO)
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
			{
				MOVEMGR->SetHeroTarget(pPos,TRUE);
			}
			else
			{
				MOVEMGR->SetHeroTarget(pPos,FALSE);
			}
			
			HERO->DisableAutoAttack();
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);
		}
	}
}

void CHeroMove::OnClickPet(DWORD MouseEvent, CPetBase* pPet)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		CActionTarget Target;
		Target.InitActionTarget( pPet, NULL );

		CAction MoveAction;
		MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPet->GetID() )
		{

			CActionTarget Target;
			Target.InitActionTarget( pPet, NULL );

			CAction MoveAction;
			MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
	}
}


void CHeroMove::OnClickMapObject(DWORD MouseEvent, CObject* pObj)
{

}



#define ATTACK_TICK	100  // 2015-04-06 间隔 400 改为100 以免造成手动延迟!

BOOL IsEventAttackable(CObject* pTargetObj)

{
	if( HERO->GetWeaponEquipType() == WP_EVENT )
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		
		if( !pItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
			return	FALSE;
		}
	}
	else
	{
		
		if( pTargetObj->GetObjectKind() & eObjectKind_Monster )
		{
			CMonster* pMonster = (CMonster*)pTargetObj;
			
			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
				)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(584) );

				return FALSE;
			}

		}
	}

	return TRUE;
}

void CHeroMove::OnAttack(CObject* pTargetObj,DWORD MouseEvent)
{	
	static CObject* pLastObj		= NULL;
	static DWORD	dwAttackTime	= 0;
	
	if( HERO && pTargetObj )
	{
		if( HERO->GetCharacterTotalInfo()->bVisible )	
		{
			if( pLastObj != pTargetObj || gCurTime - dwAttackTime > ATTACK_TICK )
			{
				if( pTargetObj->GetState() == eObjectState_Die )
					return;

				if( MouseEvent & MOUSEEVENT_LCLICK )
				{
					if( OBJECTMGR->GetSelectedObjectID() == pTargetObj->GetID() )
					{
						if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
							return;
						}
						if( !IsEventAttackable(pTargetObj) )
							return;

						if( HERO->IsPKMode())
						{
							if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
							{ 
							// 同队伍成员 2014-08-27

								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
								return;
							}   
							// 同帮会 2014-08-27 
							if(GUILDMGR->IsSameGuild( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
								return ;
							}
							//  同盟   2014-08-27
							if( GUILDUNION->IsSameUnion( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
								return ;
							}

						}

						
						CActionTarget Target;
						Target.InitActionTarget(pTargetObj,NULL);
						
						HERO->EnableAutoAttack(&Target);		
						SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
						pLastObj		= pTargetObj;
						dwAttackTime	= gCurTime;
					}
				}
				else if( MouseEvent & MOUSEEVENT_DBLCLICK )
				{
					if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
						return;
					}
					if( !IsEventAttackable(pTargetObj) )
						return;

					if( HERO->IsPKMode())
					{
						if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
						{ 
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
							return;
						}   
						if( GUILDMGR->IsSameGuild( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
							return ;
						}
						if( GUILDUNION->IsSameUnion( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
							return ;
						}
					}

					CActionTarget Target;
					Target.InitActionTarget(pTargetObj,NULL);
					

					HERO->EnableAutoAttack(&Target);		
					SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
					pLastObj		= pTargetObj;
					dwAttackTime	= gCurTime;
				}
				else if(MouseEvent & MOUSEEVENT_RCLICK&&g_UserInput.GetKeyboard()->GetKeyPressed(KEY_SHIFT))	//角色右键菜单	by:十里坡剑神	QQ:112582793
				{//[角色右键菜单弹出][By:十里坡剑神][QQ:112582793][2017/11/23]
					if(sCPlayerAI->sAiConfig.AutoSwitch[eOpen]) 
					{
						CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2225));
						return;
					}
					if(pTargetObj->GetObjectKind()!=eObjectKind_Player) return;
					CObject* obj = OBJECTMGR->GetSelectedObject();
					if(!obj)
					{
						CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2226));
						return;
					}
					CMouse* mouse = g_UserInput.GetMouse();
					GAMEIN->GetCharacterRBTNDlg()->SetAbsXY(mouse->GetMouseX(),mouse->GetMouseY());
					char buf[64];
					sprintf(buf,"%s[%d]",obj->GetObjectName(),obj->GetID());
					GAMEIN->GetCharacterRBTNDlg()->SetCharName(buf);
					GAMEIN->GetCharacterRBTNDlg()->Show();
					return;
				}
			}
		}
	}
}


void CHeroMove::OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)

{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;


	if( OBJECTMGR->GetSelectedObjectID() )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPlayer->GetID() )
		{
			if( pPlayer->GetState() == eObjectState_StreetStall_Owner )
			{
				CActionTarget Target;
				Target.InitActionTarget( pPlayer, NULL );


				CAction MoveAction;
				MoveAction.InitStallAction( STREETSTALL_ENTER_DISTANCE, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
			}
			else
			{
				OnAttack( pPlayer, MOUSEEVENT_DBLCLICK );
			}
		}
	}
}

void CHeroMove::OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	OnAttack( pMonster, MouseEvent );
}

void CHeroMove::OnDbClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}

	CActionTarget Target;
	Target.InitActionTarget( pNpc, NULL );
				
	CAction MoveAction;
	float fDistance = NPC_TALKING_DISTANCE;
	if( pNpc->GetNpcJob() == SYMBOL_ROLE )
		fDistance = NPC_TALKING_DISTANCE/2;
	MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
	MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
}

void CHeroMove::AttackSelectedObject()
{
	if(sCPlayerAI->IsOpen()) return;
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if( !pTargetObj ) return;

	OnAttack(pTargetObj,MOUSEEVENT_LCLICK);
}

void CHeroMove::SelectLoginPoint( WORD LoginPointIndex )
{
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_SAVEPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = LoginPointIndex;
				
	NETWORK->Send(&msg, sizeof(msg));

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(161) );
}

void CHeroMove::MapChange( WORD MapIndex, DWORD ItemIdx, DWORD ItemPos,DWORD state )
{
	GAMEIN->UpdataGoldMoney();    //[在线充值刷新元宝][By:十里坡剑神][QQ:112582793][2017/12/5]
	sCPlayerAI->SetChangeMap(TRUE);
	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = MapIndex;
	msg.dwData2 = ItemIdx;
	msg.dwData3 = ItemPos;
	msg.dwData4 = state;
		
	g_UserInput.SetInputFocus(FALSE);
	//[内挂配置保存][By:十里坡剑神][QQ:112582793][2017/12/8]
	//sCPlayerAI->SetOpen(FALSE);
	sCPlayerAI->SaveConfig();
	sCPlayerAI->InitNotMainSkillTime();
	//[删除发送物品信息][By:十里坡剑神][QQ:112582793][2018/4/3]
	GAMEIN->GetSendItemInfoDlg()->ClearUseItem();
	if(GAMEIN->GetFBTipDlg()->IsActive())   //[副本时间提示][By:十里坡剑神][QQ:112582793][2017/12/8]
	{//[此处判断是否处于副本地图，如果是的话不进行读秒直接移出][By:十里坡剑神][QQ:112582793][2017/12/8]
		//GAMEIN->GetFBTipDlg()->SetActive(FALSE);
		//if(GAMEIN->GetFBTipDlg()->IsStartTip())
		//	GAMEIN->GetFBTipDlg()->SetStartTip(FALSE);
		NETWORK->Send(&msg, sizeof(msg));
		return;
	}
	CHANNEL->SetMoveMsg(msg);  //[过图读秒数据包保存][By:十里坡剑神][QQ:112582793][2018/5/1 20:53]
	CHANNEL->SetMove(TRUE);	   //[过图读秒开始][By:十里坡剑神][QQ:112582793][2018/5/1 20:53]
	FORTWARMGR->SetWarClose();
}




