#include "stdafx.h"
#include "WindowIDEnum.h"
#include "AIManager.h"
#include "cWindowManager.h"
#include <MMSystem.h>
#include "ObjectManager.h"
#include "QuickItem.h"
#include "QuickManager.h"
#include "OptionDialog.h"
#include "./input/UserInput.h"
#include "SkillManager_client.h"
#include "ChatManager.h"
#include "MoveManager.h"
#include "ExitManager.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "ReviveDialog.h"
#include "StatusIconDlg.h"
#include "MHMap.h"
#include "MHTimeManager.h"
#include "interface/cEditBox.h"
#include "ObjectStateManager.h"
#include "MugongBase.h"
#include "MugongDialog.h"
#include "GameIn.h"
#include "MussangManager.h"
#include "MHNetwork.h"
#include "PetManager.h"
#include "MainGame.h"
#include <time.h>
#include "cMsgBox.h"
#include "SkillDelayManager.h"
#include "WantedManager.h"
#include "MugongManager.h"
#include "InventoryExDialog.h"
#include "SOSDialog.h"
#include "FBTipDlg.h"
#include "MoveManager.h"
#include "FortWarManager.h"
#include "MarryManager.h"
BOOL AIManager::CheckKyunKong;
extern HWND _g_hWnd;
extern int gChannelNum;

AIManager::AIManager(void)
{
	m_IsChangeMap = FALSE;
	m_NoAction = FALSE;
	m_AttrPlayerID = 0;
	CheckKyunKong = TRUE;
	ReBuildIsMovePoint = FALSE;
	m_IsWanted=FALSE;
	m_SkillSetInfo.Initialize(200);
	m_UseItemInfo.Initialize(500);
	InitNotMainSkillTime();
	TimeCount[eReMoveTime]=gCurTime;
	TimeCount[eUsePetItem] = gCurTime;
	m_IsTooFar = FALSE;
	m_PlayerDealTime = 0;
	m_PetItemInfo[100] = 8500;
	m_PetItemInfo[300] = 8501;
	m_PetItemInfo[600] = 8502;
	memset(&TimerUse,gCurTime,sizeof(TimerUse));
	m_IsNeedInit = TRUE;
	m_dwAutoAttTime = gCurTime;
	m_SkillUseCount = 0;
	m_bUseHunXue = false;
}

AIManager::~AIManager(void)
{
	SKILLSET* SkillSetInfo = NULL;
	m_SkillSetInfo.SetPositionHead();
	while(SkillSetInfo = m_SkillSetInfo.GetData())
	{
		delete SkillSetInfo;
		SkillSetInfo=NULL;
	}
	m_SkillSetInfo.RemoveAll();

	sUseItem * UseItemInfo = NULL;
	m_UseItemInfo.SetPositionHead();
	while(UseItemInfo = m_UseItemInfo.GetData())
	{
		delete UseItemInfo;
		UseItemInfo = NULL;
	}
	m_UseItemInfo.RemoveAll();
	//[Map清理][BY:十里坡剑神][QQ:112582793][2019-4-7][12:50:11]
	m_PetItemInfo.clear();
	m_AiSkillInfo.clear();
}
void AIManager::LoadSetFile()
{
	LoadSkillSetInfo();
	LoadUseItemInfo();
	LoadAiSkillInfo();
	LoadAiLimitInfo();
}
void AIManager::process()  //[处理进程实现][By:十里坡剑神][QQ:112582793][2018/5/2 17:09]
{	
	if(m_IsChangeMap||!HERO) return;
	if(EXITMGR->IsExitWaiting()) return;
	ProtectProcess();    //[生命保护进程][By:十里坡剑神][QQ:112582793][2018/5/2 17:09]
	AutoCheckPetState();//检查宠物状态
	CheckTimer();

	if(MAP->GetMapNum()!=sAiConfig.SetValue[eLastMap]) return;

	if (sAiConfig.AutoSwitch[eOpen]) 
	{
		if (!IsCanUse()) return;

		ProcessReBuild();		//[复活进程开始][By:十里坡剑神][QQ:112582793][2018/5/2 17:09]
		if (HERO->IsDied())		//[判断角色是否死亡，死亡则退出进程	][By:十里坡剑神][QQ:112582793][2018/5/2 17:09]
			return;	
		if(NeedDaZuo())
		{
			HERO->DisableAutoAttack();
			ProcessDaZuo();
		}
		UseNotMainSkill();	//[辅助技能进程开始][By:十里坡剑神][QQ:112582793][2018/5/2 17:09]
		ProcessActtacMonster(); //[打怪进程开始][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
		ProcessAutoAttrPlay();  //[自动反击进程开始][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
		EndingDaZuo();			//[打坐结束进程][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
		MovePoint();			//[挂机点传送进程][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
		AutoMussang();			//[自动无双进程][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
	} 
}
void AIManager::CheckTimer()	//计时器
{
	if(!sAiConfig.AutoSwitch[eUseTimer])
	{
		memset(&TimerUse,gCurTime,sizeof(TimerUse));
		return;
	}
	for(int i = 0;i<eSwitchMax;i++)
	{
		if(sAiConfig.TimeSwitch[i])
		{
			if(gCurTime-TimerUse[i]>=sAiConfig.TimeValue[i]*1000)
			{
				QUICKMGR->UseQuickItem( sAiConfig.TimeKey[i]-1,sAiConfig.TimePage[i]-1 );
				TimerUse[i] = gCurTime;
			}
		}
	}
}
void AIManager::AutoCheckPetState()//检查宠物状态
{
	if(!PETMGR->GetCurSummonPet())//未召唤宠物
		return;
	if(gCurTime-TimeCount[eUsePetItem]>=2500)
	{
		WORD wValue = PETMGR->GetPetSummon();
		if(wValue>600)
		{
			if(!UsePacketItem(8502))
				if(!UsePacketItem(8501))
					if(!UsePacketItem(8500))
						CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2325));
			TimeCount[eUsePetItem] = gCurTime;
			return;
		}
		std::map<WORD,WORD>::iterator  iter = m_PetItemInfo.find(wValue);
		if(iter!=m_PetItemInfo.end())
		{
			UsePacketItem(iter->second);
		}
		TimeCount[eUsePetItem] = gCurTime;
	}
}
void AIManager::ProcessActtacMonster()  //[自动狩猎进程][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
{
	if (HERO->IsDied())	
		return;
	VECTOR3 herotmppos,monstertmppos;	

	double distunce = 0;		

	if (HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Immortal)
		return;

	if (HERO->GetState() == eObjectState_SkillUsing)  
		return;
	if (sAiConfig.AutoSwitch[eOpen])  
	{
		if (OBJECTMGR->GetSelectedObject() && 
			OBJECTMGR->GetSelectedObject()->GetObjectKind() == eObjectKind_Monster&&
			!OBJECTMGR->GetSelectedObject()->IsDied()&&!m_IsTooFar)
		{
			HERO->GetPosition(&herotmppos);  
			OBJECTMGR->GetSelectedObject()->GetPosition(&monstertmppos);  
			double distuncetmp =( (monstertmppos.x-herotmppos.x)*(monstertmppos.x-herotmppos.x) + (monstertmppos.z-herotmppos.z)*(monstertmppos.z-herotmppos.z));
			AttactMonster(OBJECTMGR->GetSelectedObject(),distunce);	
			return;
		}
	
		ObjectArray<LPOBJECT> oArray; 
		OBJECTMGR->GetTargetInRange(&Heropos,sAiConfig.SetValue[eDistance]*100,&oArray,FALSE);
		oArray.SetPositionHead();
		LPOBJECT lptmpMons = NULL;

		bool bInit = false; 
		for (int i=0;i<oArray.nObjectNum;i++)
		{
			LPOBJECT lpObj = oArray.GetNextObject();	
			BYTE wJobMonsterKind = lpObj->GetObjectKind();	

			if (wJobMonsterKind & eObjectKind_Monster)	
			{
				HERO->GetPosition(&herotmppos);  
				lpObj->GetPosition(&monstertmppos);	
				
				double distuncetmp =( (monstertmppos.x-herotmppos.x)*(monstertmppos.x-herotmppos.x) + (monstertmppos.z-herotmppos.z)*(monstertmppos.z-herotmppos.z));
				if (!bInit) 
				{
					lptmpMons = lpObj;   
					distunce = distuncetmp;
					bInit = true;
					continue;  
				}

				if (distuncetmp < distunce) 
				{
					distunce = distuncetmp;
					lptmpMons = lpObj;
				}
			}
		}
		AttactMonster(lptmpMons,distunce);
	}
}
//void AIManager::CheckMonsterStat(CObject* pAttacker)
//{
//	if(sAiConfig.AutoSwitch[eOpen])
//	{
//		CMonster* pMonster = (CMonster*)pAttacker; 
//		if (sAiConfig.AutoSwitch[eUsePoHun])	
//		{	
//			BOOL statusflg  = FindSkillStatusFLg(256,pMonster);	
//			if ( !statusflg&&HERO->GetLevel()>=5)	
//			{
//				OBJECTMGR->SetSelectedObject(pAttacker);
//				CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(1202);	
//				if(pInfo)
//				{
//					//TimeCount[eMovePoint] = gCurTime;
//					PlayerUseSkill(pInfo) ;
//					return;
//				}
//			}
//		}
//	}
//}
void AIManager::AttactMonster( LPOBJECT lpObj,double distance1 )	
{
	if (!lpObj) 
	{
		m_NoAction= TRUE;
		return;
	}
	if (sAiConfig.AutoSwitch[eAutoToNa])
	{
		if ( GetLife() < sAiConfig.SetValue[eAutoTuNaLife]&&gCurTime - TimeCount[eTimeForTuna] > 3000)
		{
			HERO->SetNextAction(NULL);
			HERO->DisableAutoAttack();
			return;
		}
	}
	if(NeedDaZuo()&&(GetObjLife(lpObj)>=sAiConfig.SetValue[eDaZuoMonsterLife]))
	{
		return;
	}
	if (CheckNotMainSkill())
	{
		HERO->SetNextAction(NULL);
		HERO->DisableAutoAttack();
		return;
	}
		
	BYTE wJobMonsterKind = lpObj->GetObjectKind(); 

	if (wJobMonsterKind & eObjectKind_Monster)
	{
		if ( lpObj->IsDied() == FALSE )		
		{
			CMonster* pMonster = (CMonster*)lpObj; 
			OBJECTMGR->SetSelectedObject(pMonster);	
			if(GetNaeRyuk()<8)
			{
				g_UserInput.GetHeroMove()->AttackSelectedObject();
				//TimeCount[eMovePoint] = gCurTime;
				TimeCount[eForActtact] = gCurTime;
				return;
			}
			if (sAiConfig.AutoSwitch[eUsePoHun])	
			{	
				BOOL statusflg  = FindSkillStatusFLg(256,pMonster);	
				if ( !statusflg&&HERO->GetLevel()>=5&&
					wJobMonsterKind!=eObjectKind_FieldBossMonster)	
				{
					CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(1202);	
					if(pInfo)
					{
						PlayerUseSkill(pInfo) ;
						return;
					}
				}
			}
			else if (sAiConfig.AutoSwitch[eUseSheXin])   
			{	
				BOOL statusflg  = FindSkillStatusFLg(4096,pMonster);	
				if ( !statusflg&&HERO->GetLevel()>=5)	
				{
					CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(1203); 
					if(pInfo)
					{
						PlayerUseSkill(pInfo) ;	
						return;
					}
				}
			}
#ifdef _GMTOOL_
			if (UseHunXue())
				return;
#endif
			if (HERO->GetLevel() < 5)  
			{
				g_UserInput.GetHeroMove()->AttackSelectedObject(); 
				TimeCount[eForActtact] = gCurTime;
			}
			else
			{
				if (!sAiConfig.AutoSwitch[eAutoSkill1] && 
					!sAiConfig.AutoSwitch[eAutoSkill2])
				{
					g_UserInput.GetHeroMove()->AttackSelectedObject();
					TimeCount[eForActtact] = gCurTime;
				}
				else 	
				{
					if (sAiConfig.AutoSwitch[eAutoSkill1] && 
						sAiConfig.AutoSwitch[eAutoSkill2]) 
					{
						if (!DobuleSkillFlg)
						{
							QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey1]-1,sAiConfig.SetValue[ePressPage1]-1 );
						}
						if (DobuleSkillFlg)
						{
							QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey2]-1,sAiConfig.SetValue[ePressPage2]-1);
						}
					}
					if(sAiConfig.AutoSwitch[eAutoSkill1] && 
						!sAiConfig.AutoSwitch[eAutoSkill2])
					{	
						QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey1]-1,sAiConfig.SetValue[ePressPage1]-1);
					}
					if(!sAiConfig.AutoSwitch[eAutoSkill1] && 
						sAiConfig.AutoSwitch[eAutoSkill2])
					{	
						QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey2]-1,sAiConfig.SetValue[ePressPage2]-1 );
					}				
				}
			}
		}
	}
}
void AIManager::ProcessDaZuo()
{
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 401 );
	if( pAbility == NULL )
		return ;
	if(HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Immortal)
	{
		return;
	}
	if(gCurTime - TimeCount[eForActtact]+500 < QUICKMGR->GetUserSkillTime())
		return;
	pAbility->Use();
	m_MonsterAttrTime = 0;
	TimeCount[eForDaZuo] = gCurTime; //[打坐技能使用计时开始][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
}
BOOL AIManager::CanEndingDaZuo()
{	
	if((int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if(GetLife()>=sAiConfig.SetValue[eDaZuoLife]
			&&GetNaeRyuk()==100&&
			OBJECTMGR->GetSelectedObject()!=NULL)
		{
			return TRUE;
		}
	}
	if((int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if(GetLife()>=100&&GetShield()>=100&&GetNaeRyuk()>=100)
			return TRUE;
	}
	if(m_MonsterAttrTime>5&&(int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if(GetLife()>=30&&GetNaeRyuk()>=100)
		{
			m_MonsterAttrTime = 0;
			return TRUE;
		}
	}
	return FALSE;
}

void AIManager::EndingDaZuo()
{
	TimeCount[eCurTime] = gCurTime;    //[判断打坐技能使用间隔时间][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
	if (HERO->IsDied())
		return;
	if(TimeCount[eCurTime]-TimeCount[eForDaZuo]<2150)
	{
		return;
	}

	if(!CanEndingDaZuo())return;

	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 401 );  
	if( pAbility == NULL )return ;

	if ( (int)HERO->GetState() == eObjectState_Ungijosik)
	{
		pAbility->Use();
		TimeCount[eCurTime] = gCurTime;
		TimeCount[eSkillTime] = gCurTime;     //[辅助技能释放修复检查打坐站起时间][By:十里坡剑神][QQ:112582793][2018/5/2 17:10]
		return;
	}
	if((TimeCount[eCurTime]-TimeCount[eForDaZuo]>2250))
	{
		pAbility->Use();
		TimeCount[eCurTime] = gCurTime;
		TimeCount[eSkillTime] = gCurTime;     //[辅助技能释放修复检查打坐站起时间][By:十里坡剑神][QQ:112582793][2018/5/2 17:11]
		return;
	}
	if(m_AttrPlayerID!=0)
	{
		if((int)HERO->GetState() == eObjectState_Ungijosik)
		{
			pAbility->Use();
			TimeCount[eCurTime] = gCurTime;
			TimeCount[eSkillTime] = gCurTime;    //[辅助技能释放修复检查打坐站起时间][By:十里坡剑神][QQ:112582793][2018/5/2 17:11]
			return;
		}
	}
}

BOOL AIManager::NeedDaZuo()
{	
	CObject* lpObj = OBJECTMGR->GetSelectedObject();

	if (HERO->GetState() == eObjectState_SkillUsing)  
		return FALSE;
	if(GetShield()<=sAiConfig.SetValue[eDaZuoShield]&&
		m_MonsterAttrTime>=5&&GetNaeRyuk()>sAiConfig.SetValue[eDaZuoNaeRyuk])
		return FALSE;
	if(lpObj)
	{
		if(GetObjLife(lpObj)<=sAiConfig.SetValue[eDaZuoMonsterLife]||
			lpObj->GetObjectKind()==eObjectKind_Player)
			return FALSE;
	}
	if(sAiConfig.AutoSwitch[eAutoDazuoH])
	{
		if(GetLife()<=sAiConfig.SetValue[eDaZuoLife]&&sAiConfig.SetValue[eDaZuoLife]!=0)
			return TRUE;
	}
	if(sAiConfig.AutoSwitch[eAutoDazuoL])
	{
		if(GetNaeRyuk()<=sAiConfig.SetValue[eDaZuoNaeRyuk]&&sAiConfig.SetValue[eDaZuoNaeRyuk]!=0)
			return TRUE;
	}
	if(sAiConfig.AutoSwitch[eAutoDazuoT])
	{
		if(GetShield()<=sAiConfig.SetValue[eDaZuoShield]&&sAiConfig.SetValue[eDaZuoShield]!=0)
			return TRUE;
	}
	return FALSE;
}

BOOL AIManager::PlayerUseSkill(CSkillInfo* pInfo)
{
	if( HERO->GetCharacterTotalInfo()->bVisible == FALSE ) return FALSE;	
	CActionTarget Target;

	if( !pInfo )				return FALSE;

	WORD wSkillKind = pInfo->GetSkillKind();

	if( (HERO->InTitan()) && (SKILLMGR->CheckSkillKind(wSkillKind) == FALSE) )
	{
		pInfo = SKILLMGR->GetSkillInfo(pInfo->GetSkillIndex());
	}

	if( pInfo->GetSkillInfo()->TargetKind == 0)
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();

		if( targetObj )
		{
			if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
				return FALSE;

			Target.InitActionTarget(targetObj,NULL);

			if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
			{
				HERO->DisableAutoAttack();
			}
		}
		else
		{
			CHATMGR->AddMsg( CTC_AI_TIP, CHATMGR->GetChatMsg(400) );
			return FALSE;
		}
	}
	else if(pInfo && pInfo->GetSkillInfo()->TargetKind == 1)
	{
		Target.InitActionTarget(HERO,NULL);	
	}

	if(pInfo && !pInfo->GetSpecialState())
	{
		if(HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
		{
			CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(1329));
			return FALSE;
		}
	}

	SKILLMGR->SetSkill(pInfo);
//	CHATMGR->AddMsg(CTC_AI_TIP, "技能释放");
	return SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );;
}

BOOL AIManager::FindSkillStatusFLg( DWORD dwFlg , CMonster* pMonster)
{
	if (!pMonster)
		return FALSE;
	cPtrList& cPtrL = pMonster->GetSkillStatusList();
	PTRLISTPOS pos = cPtrL.GetHeadPosition();
	while( pos )
	{
		CStatus* pData = (CStatus*)cPtrL.GetNext( pos );
		if( pData->GetFlag() == dwFlg)
			return TRUE;
	}
	return FALSE;
}
DOUBLE AIManager::GetLife()
{
	return ((DOUBLE)HERO->GetLife()/(DOUBLE)HERO->DoGetMaxLife())*100;
}
DOUBLE	AIManager::GetShield()
{
	return ((DOUBLE)HERO->GetShield()/(DOUBLE)HERO->DoGetMaxShield())*100;
}
DOUBLE AIManager::GetNaeRyuk()
{
	return ((DOUBLE)HERO->GetNaeRyuk()/(DOUBLE)HERO->DoGetMaxNaeRyuk())*100;
}
DOUBLE AIManager::GetObjLife(LPOBJECT lpObj)
{
	double temp1=(double)lpObj->GetLife();
	double temp2=(double)lpObj->DoGetMaxLife();
	return temp1/temp2*100;
}

void AIManager::DufualtUsekyungGong()
{
	if(HERO->GetKyungGongMode())
	{
		return;
	}
	LEVELTYPE kyungGong = HERO->GetLevel();
	CAbility* pAblilty1 = HERO->GetAbilityGroup()->GetAbility(201);
	CAbility* pAblilty2 = HERO->GetAbilityGroup()->GetAbility(204);
	CAbility* pAblilty3 = HERO->GetAbilityGroup()->GetAbility(207);

	if(kyungGong>=40&&pAblilty3)
	{
		pAblilty3->Use();
		return;
	}
	if(kyungGong>=25&&kyungGong<=39&&pAblilty2)
	{
		pAblilty2->Use();
		return;
	}
	if(kyungGong>=0&&kyungGong<=24)
	{	
		pAblilty1->Use();
		return;
	}
}
void AIManager::CheckKyunkong()
{
	if(!CheckKyunKong) return;
	if(HERO->GetLevel()==1 )
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong();
		CheckKyunKong = FALSE;
		return;
	}
	if(HERO->GetLevel()==25)
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong();
		CheckKyunKong = FALSE;
		return;
	}
	if(HERO->GetLevel()==40)
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong();
		CheckKyunKong = FALSE;
		return;
	}
}

void AIManager::AutoMussang()
{
	if(!sAiConfig.AutoSwitch[eAutoUseWuShuang]) return;

	BOOL IsMussang = MUSSANGMGR->IsMussangMode();  //这里判断是否处于无双模式	by:十里坡剑神 QQ:112582793

	if(IsMussang||HERO == NULL||HERO->IsDied()/*||
		(HERO->GetState() != eObjectState_None &&
		 HERO->GetState() != eObjectState_Immortal)*/)
	{
		return;
	}
	if(!IsMussang&&MUSSANGMGR->IsMussangReady())  //判断如果不处于无双模式，且无双值已满则开启无双	by:十里坡剑神 QQ:112582793
	{
		MUSSANGMGR->SendMsgMussangOn();
	}
}
void AIManager::ProcessReBuild()	
{ 
	if(m_IsNeedInit||HERO->GetState()!=eObjectState_Die) 
	{
		DWORD m_Time = MHTIMEMGR->GetNewCalcCurTime();
		m_ReBuildTime = m_Time-1000;
		m_ReviveCount = sAiConfig.SetValue[eReBuildTime];
		m_IsNeedInit = FALSE;
		return;
	}
	if(m_ReBuildTime>gCurTime)
		m_ReBuildTime = gCurTime;
	if(sAiConfig.AutoSwitch[eAutoReBuild]==TRUE)  
	{
		if(gCurTime-m_ReBuildTime>=1000)
		{
			m_ReBuildTime += 1000;
			if(m_ReviveCount<=0)  
			{
				if(m_PlayerDealTime<sAiConfig.SetValue[eBuildTime]) 
				{
					cDialog* WantReg_Dialog = WINDOWMGR->GetWindowForID(WANTREG_DLG); 
					if(WantReg_Dialog) 
					{
						if(sAiConfig.AutoSwitch[eAutoWant]&&!m_IsWanted) 
						{
							WANTEDMGR->RegistKillerSyn(sAiConfig.SetValue[eWantMoney]);
							TimeCount[eCurTime]  = gCurTime;//打坐技能使用计时开始在这里计时防止复活后玩家手动吃药	by:十里坡剑神 QQ:112582793
							TimeCount[eReMoveTime] = gCurTime;
							m_IsWanted = TRUE;
							return;
						}
						else
						{
							if(WantReg_Dialog->IsActive())
								WantReg_Dialog->SetActive(FALSE);
						}
					}
					if( EXITMGR->IsExitWaiting() )	
					{
						CHATMGR->AddMsg( CTC_AI_TIP, CHATMGR->GetChatMsg(643) );
						GAMEIN->GetReviveDialog()->SetActive( FALSE );
						return;
					}
					MSGBASE msg;
					msg.Category = MP_CHAR_REVIVE;
					if(sAiConfig.AutoSwitch[eReBuildType])
						msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
					else
						msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
					msg.dwObjectID = HEROID;			
					NETWORK->Send(&msg, sizeof(msg));	
					cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
					if( pDlg )
					{
						WINDOWMGR->AddListDestroyWindow( pDlg );
						ITEMMGR->ReviveOtherConfirm( FALSE );
					}
					m_PlayerDealTime +=1; 
					ReBuildIsMovePoint = TRUE;
					TimeCount[eCurTime]  = gCurTime;//打坐技能使用计时开始在这里计时防止复活后玩家手动吃药	by:十里坡剑神 QQ:112582793
					TimeCount[eReMoveTime] = gCurTime;
					TimeCount[eMovePoint] = gCurTime;
					m_IsWanted=FALSE;
					m_IsNeedInit = TRUE;

				}
				else//否则当复活次数超过限制将玩家传送回兰州1线!	by:十里坡剑神 QQ:112582793
				{  
					DWORD wMapNum;
					wMapNum=17;
					stMOVEPOINT stMovePoint;
					stMovePoint.SetMovePoint(8625, 9825);
					MARRYMGR->SetTargerPos(stMovePoint.value);
					MARRYMGR->SetChannelIdx(0);
					g_UserInput.GetHeroMove()->MapChange( 17, 0, 2019, eMapChange_SOS );
					gChannelNum = 0;
					m_PlayerDealTime=0;  
					TimeCount[eCurTime]  = gCurTime;//打坐技能使用计时开始在这里计时防止复活后玩家手动吃药	by:十里坡剑神 QQ:112582793
					TimeCount[eReMoveTime] = gCurTime;
					m_IsWanted=FALSE;
					sAiConfig.AutoSwitch[eOpen] = FALSE;
					m_IsNeedInit = TRUE;
					SaveConfig();
				}
			}
			else
			{
				CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2152), m_ReviveCount);
			}
			if(m_ReviveCount>0)
				--m_ReviveCount;
			if(m_ReviveCount<0)
				m_ReviveCount=0;
		}
	}
}
void AIManager::SetTooFar(BOOL val)
{
	m_IsTooFar=val;
	m_NoAction=val;
	TimeCount[eMovePoint]=gCurTime;
}
void AIManager::MovePoint()
{
	if(Heropos.x==0||Heropos.z==0)
		return;
	if (HERO->GetLevel() < 5) return;
	if(ReBuildIsMovePoint&&((HERO->GetState() == eObjectState_None &&
		HERO->GetState() == eObjectState_Immortal)))
	{
		if(gCurTime - TimeCount[eReMoveTime]<15000)
			return;
		MOVEMGR->SetHeroTarget( &Heropos,1.f );
		ReBuildIsMovePoint = FALSE;
		return;
	}
	if(m_NoAction==TRUE)
	{	
		if(gCurTime-TimeCount[eMovePoint]<15000)
			return;

		if(HERO->GetState() == eObjectState_SkillUsing)
		{
			m_NoAction  = FALSE;
			return;
		}
		if(HERO!=NULL)
		{
			if(HERO->GetState() == eObjectState_None ||
				HERO->GetState() == eObjectState_Immortal)
			{

				/*MOVE_POS_USER msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_USERRECONN_MOVEPOINT_SYN;
				msg.dwObjectID	= HEROID;
				SafeStrCpy( msg.Name, HERO->GetObjectName(), MAX_NAME_LENGTH + 1 );
				msg.cpos.Compress(&Heropos);
				NETWORK->Send(&msg,sizeof(msg));*/
				MOVEMGR->SetHeroTarget( &Heropos, 1.f);
				ReBuildIsMovePoint = FALSE;
				m_NoAction  = FALSE;
				TimeCount[eMovePoint]= gCurTime;
				return;
			}
		}
	}
}
void AIManager::StartAutoAttrPalyProcess(CObject *AttrPlayer)   //开始自动狩猎进程	by:十里坡剑神 QQ:112582793
{
	if(AttrPlayer)
		m_AttrPlayerID=AttrPlayer->GetID();
	else
		m_AttrPlayerID = 0;
}
void AIManager::ProcessAutoAttrPlay()   //自动反击进程	by:十里坡剑神 QQ:112582793
{
	if(m_AttrPlayerID==0) return;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(m_AttrPlayerID);
	if(!pPlayer) return;
	if(sAiConfig.AutoSwitch[eAutoAttrPlay]==FALSE||
		HERO->GetState()==eObjectState_Deal)  
		return;
	if(pPlayer->IsDied()||!((CHero*)pPlayer)->IsPKMode())
	{
		m_AttrPlayerID = 0;
		return;
	}

	OBJECTMGR->SetSelectedObject(pPlayer); 

	VECTOR3 herotmppos,AttrPlayerpos;  
	double distunce = 0;   

	HERO->GetPosition(&herotmppos);	
	pPlayer->GetPosition(&AttrPlayerpos);

	double distuncetmp =( (AttrPlayerpos.x-herotmppos.x)*(AttrPlayerpos.x-herotmppos.x) + (AttrPlayerpos.z-herotmppos.z)*(AttrPlayerpos.z-herotmppos.z));
	if(gCurTime-m_dwAutoAttTime<2500) return;

	if(sAiConfig.AutoSwitch[eAutoSkill1])	
	{
		if(HERO->GetState() != eObjectState_SkillUsing)	
		{ 
			QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey1]-1,sAiConfig.SetValue[ePressPage1]-1);	
			m_dwAutoAttTime = gCurTime;
		}
	}
	else if (sAiConfig.AutoSwitch[eAutoSkill2])
	{
		if(HERO->GetState() != eObjectState_SkillUsing)  
		{ 
			QUICKMGR->UseQuickItem( sAiConfig.SetValue[ePressKey1]-2,sAiConfig.SetValue[ePressPage2]-1 );	
			m_dwAutoAttTime = gCurTime;
		}
	}
	else
	   g_UserInput.GetHeroMove()->AttackSelectedObject(); 
}
void AIManager::SaveConfig()
{
	if (!HERO) return;
	FILE * fp;
	char buf[64];
	WORD ID = HERO->GetID();
	sprintf(buf,"./Data/AiConfig_%d.dat",ID);
	fopen_s(&fp,buf,"w+");
	if(fp)
	{
		fwrite(&sAiConfig,sizeof(sAiConfig),1,fp);
		fclose(fp);
	}
}
void AIManager::ReadConfig()
{
	if (!HERO) return;
	FILE * fp;
	char buf[64];
	WORD ID = HERO->GetID();
	sprintf(buf,"./Data/AiConfig_%d.dat",ID);
	fopen_s(&fp,buf,"r");
	if(fp)
	{
		memset(&sAiConfig,0,sizeof(sAiConfig));
		fread(&sAiConfig,sizeof(sAiConfig),1,fp);
		fclose(fp);
		Heropos.x=sAiConfig.SetValue[ePointX]*100;
		Heropos.z=sAiConfig.SetValue[ePointY]*100;
	}
}
void AIManager::LoadSkillSetInfo()
{
	CMHFile file;
	if(!file.Init("Resource/Client/SillSetInfo.bin", "rb"))
		return;
	SKILLSET * pInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;

		pInfo = new SKILLSET;
		pInfo->StateIcon = file.GetWord();
		pInfo->SkillIndex = file.GetWord();
		file.GetString(pInfo->SkillName);
		m_SkillSetInfo.Add(pInfo,pInfo->SkillIndex);
		pInfo=NULL;
	}
	file.Release();
}
BOOL AIManager::CheckNotMainSkill()
{
	for (int i = 9; i < eMaxSkill; i++)
	{
		if (sAiConfig.SkillInfo[i] != 0)
		{
			if (!STATUSICONDLG->CheckSkillState(i))
			{
				CSkillInfo* SkillInfo = NULL;
				CMugongBase * pMugongBase = GetNotMainSkillInfo(sAiConfig.SkillInfo[i]);
				if (pMugongBase == NULL)
					continue;
				SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetSkillInfo()->GetSkillIndex());
				if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
				{
					if (HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, SkillInfo->GetSkillIndex()) == 0)
					{
						if (m_SkillUseCount < 1)
						{
							m_SkillUseCount++;
							return FALSE;
						}
						PlayerUseSkill(SkillInfo);
						TimeCount[eSkillTime] = gCurTime;
						m_SkillUseCount = 0;
						return TRUE;
					}
				}
				else 
					continue;
			}
		}
	}
	return FALSE;
}
void AIManager::UseNotMainSkill()
{
	if(sAiConfig.AutoSwitch[eAutoToNa])
	{
		if(GetLife()<sAiConfig.SetValue[eAutoTuNaLife])
		{
			 CMugongBase * pMugongBase = CheckHeightOrLow(900,910);
			 if(pMugongBase)
			 {
				 CSkillInfo* SkillInfo = NULL;
				 SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetSkillInfo()->GetSkillIndex());
				 if(SkillInfo&&HERO->GetLevel()>=SkillInfo->GetSkillInfo()->RestrictLevel)
				 {
					 if(HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill,SkillInfo->GetSkillIndex())==0)
					 {
						// if(gCurTime-TimeCount[eSkillTime]<2000)return;
						 if (m_SkillUseCount < 1)
						 {
							 m_SkillUseCount++;
							 return;
						 }
						 PlayerUseSkill(SkillInfo);
						 TimeCount[eSkillTime] = gCurTime; 
						 m_SkillUseCount = 0;
					 }
				 }
			 }
		}
	}
}
WORD AIManager::GetSkillStateIconForName(char * SkillName)
{
	SKILLSET* SkillInfo = NULL;
	m_SkillSetInfo.SetPositionHead();
	while(SkillInfo = m_SkillSetInfo.GetData())
	{
		if(strcmp(SkillName,SkillInfo->SkillName)==0)
			return SkillInfo->StateIcon;
	}
	return 0;
}
CMugongBase * AIManager::GetNotMainSkillInfo(WORD wIndex)
{
	std::map<WORD,WORD>::iterator iter;
	iter = m_AiSkillInfo.find(wIndex);
	if(iter!=m_AiSkillInfo.end())
		return CheckHeightOrLow(iter->first,iter->second);
	else
		return MUGONGMGR->GetMugongByMugongIdx(wIndex);
}
CMugongBase * AIManager::CheckHeightOrLow(int SkillIdx1,int SkillIdx2)
{
	CMugongBase* pMugongBase1 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx1);

	CMugongBase* pMugongBase2 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx2);

	if(pMugongBase1&&pMugongBase2 || !pMugongBase1 && pMugongBase2)
	{
		return pMugongBase2;
	}
	else if(pMugongBase1)
	{
		return pMugongBase1;
	}
	else
	{
		return NULL;
	}
}

void AIManager::InitNotMainSkillTime()
{
	memset(&TimeCount,0,sizeof(TimeCount));
}

void AIManager::LoadUseItemInfo()
{
	CMHFile file;
	if(!file.Init("Resource/Client/UseItemInfo.bin", "rb"))
		return;
	sUseItem * pInfo = NULL;
	for (;;)
	{
		if(file.IsEOF())
			break;
		pInfo = new sUseItem;
		pInfo->ItemType = file.GetWord();
		pInfo->ItemIdx = file.GetDword();
		file.GetString(pInfo->ItemName);
		m_UseItemInfo.Add(pInfo,pInfo->ItemIdx);
		pInfo=NULL;
	}
	file.Release();
}
BOOL AIManager::UsePacketItem(DWORD ItemIdx,BOOL val)
{
	ITEM_INFO * pItemInfo = NULL;
	CItem * item = NULL;
	if(!val)
	{
		item=GAMEIN->GetInventoryDialog()->GetItemLike(ItemIdx);
		if(item)
		{
			GAMEIN->GetInventoryDialog()->UseItem(item);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(gCurTime-TimeCount[eUseItemTime]>=1500)
	{
		pItemInfo = ITEMMGR->GetItemInfo(ItemIdx);
		if (pItemInfo)
		{
			WORD Pos;
			if(!STATUSICONDLG->CheckSkillState(pItemInfo->BuyPrice+1))
			{
				item=GAMEIN->GetInventoryDialog()->GetItemLike(ItemIdx,Pos);
				if(item)
				{
					if(item->GetItemParam() & ITEM_PARAM_SEAL)
						GAMEIN->GetInventoryDialog()->ItemUnSealingSync(Pos);
					GAMEIN->GetInventoryDialog()->UseItem(item);
					TimeCount[eUseItemTime]=gCurTime;
					return TRUE;
				}
				else
					CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2126),pItemInfo->ItemName);
			}
		}
	}
	return FALSE;
}
BOOL AIManager::SetOpen(BOOL val)
{
	/*if(gCurTime-m_dwOpenTime>=10000)
	{*/
	if (!IsCanUse())
	{
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2321));
		return FALSE;
	}

	sAiConfig.AutoSwitch[eOpen] = val;
	m_IsNeedInit = val;

	if (sAiConfig.AutoSwitch[eOpen])
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2176));
	else
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2175));

	return TRUE;

}
void AIManager::ProtectProcess()
{
	
	if(sAiConfig.AutoSwitch[eProtectLife]&&
		GetLife()<sAiConfig.SetValue[eProtectLifeVaule]
	    &&gCurTime-TimeCount[eProtectLifeTime]>3000)
	{
		for (int i = 0;i<eMaxLife;i++)
		{
			if (sAiConfig.ProtectLife[i]!=0)
			{
				if(UsePacketItem(sAiConfig.ProtectLife[i]))
				{
					TimeCount[eProtectLifeTime]=gCurTime;
					break;
				}
			}
		}
	}
	if(sAiConfig.AutoSwitch[eProtectNaeRyuk]&&
		GetNaeRyuk()<sAiConfig.SetValue[eProtectNaeRyukVaule]
		 &&gCurTime-TimeCount[eProtectNaeRyukTime]>3000)
	{
		for (int i = 0;i<eMaxNaeRyuk;i++)
		{
			if (sAiConfig.ProtectNaeRyuk[i]!=0)
			{
				if(UsePacketItem(sAiConfig.ProtectNaeRyuk[i]))
				{
					TimeCount[eProtectNaeRyukTime] = gCurTime;
					break;
				}
			}
		}	
	}
	if (sAiConfig.AutoSwitch[eProtectShield]&&
		GetShield()<sAiConfig.SetValue[eProtectShieldVaule]
		 &&gCurTime-TimeCount[eProtectShieldTime]>3000)
	{
		for (int i = 0;i<eMaxShield;i++)
		{
			if (sAiConfig.ProtectShield[i]!=0)
			{
				if(UsePacketItem(sAiConfig.ProtectShield[i]))
				{
					TimeCount[eProtectShieldTime] = gCurTime;
					break;
				}
			}
		}
	}
	if (sAiConfig.AutoSwitch[eProtectItem])
	{
		for (int i = 0;i<eMaxEtcItem;i++)
		{
			if (sAiConfig.EtcItem[i]!=0)
			{
				UsePacketItem(sAiConfig.EtcItem[i],TRUE);
			}
		}
	}
}
void AIManager::LoadAiSkillInfo()
{
	CMHFile file;
	if(!file.Init("./Resource/Client/AiSkillInfo.bin","rb"))
		return;
	for (;;)
	{
		if(file.IsEOF())
			break;
		WORD idx1 = file.GetWord();
		WORD idx2 = file.GetWord();
		m_AiSkillInfo[idx1] = idx2;
	}
	file.Release();
}
//[内挂限制信息][BY:十里坡剑神][QQ:112582793][2019-8-2][12:06:42]
void AIManager::LoadAiLimitInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/Client/AiSetInfo.bin", "rb"))
		return;
	for (;;)
	{
		if (file.IsEOF())
			break;
		AiLimitInfo Info;
		Info.wMapNum = file.GetWord();
		Info.bIsLimit = file.GetBool();
		Info.wBeginH = file.GetWord();
		Info.wBeginM = file.GetWord();
		Info.wEndH = file.GetWord();
		Info.wEndM = file.GetWord();
		m_AiLimitInfo[Info.wMapNum] = Info;
	}
	file.Release();
}

bool AIManager::IsCanUse()
{
	std::map<WORD, AiLimitInfo>::iterator iter;
	iter = m_AiLimitInfo.find(MAP->GetMapNum());
	if (iter != m_AiLimitInfo.end())
	{
		if (!iter->second.bIsLimit)
			return false; //[未设置时间范围标志的表示全天限制][BY:十里坡剑神][QQ:112582793][2019-8-2][13:16:45]
		else
			return CheckUseLimit(iter->second);
	}
	else //[未添加进脚本的地图不限制内挂使用][BY:十里坡剑神][QQ:112582793][2019-8-2][13:16:24]
		return true;
}
bool AIManager::CheckUseLimit(const AiLimitInfo& info)
{
	SYSTEMTIME sysTime,sysBegin,sysEnd;
	GetLocalTime(&sysTime);
	GetLocalTime(&sysBegin);
	GetLocalTime(&sysEnd);

	sysBegin.wHour = info.wBeginH;
	sysBegin.wMinute = info.wBeginM;

	sysEnd.wHour = info.wEndH;
	sysEnd.wMinute = info.wEndM;

	stTIME stBegin, stEnd, stCur;

	stCur.SetTime(sysTime);
	stBegin.SetTime(sysBegin);
	stEnd.SetTime(sysEnd);

	if (stCur.value >= stBegin.value&&stCur.value < stEnd.value)
		return true;//[在时间范围内允许使用内挂否则不能使用][BY:十里坡剑神][QQ:112582793][2019-8-2][13:18:25]
	else
	{
		if (sAiConfig.AutoSwitch[eOpen])
		{
			sAiConfig.AutoSwitch[eOpen] = FALSE;
			SaveConfig();
			CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2376));
		}	
		return false;
	}
		

}
bool AIManager::UseHunXue()
{
	if (!m_bUseHunXue) return false;
	CMugongBase* pMugongBase = MUGONGMGR->GetMugongByMugongIdx(5053);
	if (pMugongBase)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			if (HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, SkillInfo->GetSkillIndex()) == 0)
			{

				PlayerUseSkill(SkillInfo);
				return true;
			}
		}
	}
	return false;
}