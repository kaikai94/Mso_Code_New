////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改					  //
////////////////////////////////////////////////////////////////////
#pragma once
#include "skillinfo.h"
#include "Monster.h"
#include "InventoryExDialog.h"
#include "AiStructDef.h"
#include <map>
#define sCPlayerAI AIManager::GetInstance()

class AIManager
{
	CYHHashTable<SKILLSET> m_SkillSetInfo;
	CYHHashTable<sUseItem> m_UseItemInfo;

	std::map<WORD, AiLimitInfo> m_AiLimitInfo;
	std::map<WORD, WORD> m_AiSkillInfo;
	std::map<WORD, WORD> m_PetItemInfo;

	DWORD TimeCount[eMaxTime];
	DWORD TimerUse[eSwitchMax];
	DWORD m_dwOpenTime;
	DWORD m_ReBuildTime;
	DWORD m_ReviveCount;
	DWORD m_PlayerDealTime;
	DWORD m_dwAutoAttTime;
	DWORD m_AttrPlayerID;

	WORD  m_SkillUseCount;

	BOOL m_IsChangeMap;
	BOOL m_IsTooFar;
	BOOL m_IsNeedInit;
	BOOL m_NoAction;
	WORD m_MonsterAttrTime;
	BOOL DobuleSkillFlg;

	BOOL ReBuildIsMovePoint;
	BOOL m_IsWanted;
	bool m_bUseHunXue;
	AIManager(void);
public:
	VECTOR3 Heropos;

	AICFIG sAiConfig;


public:

	//[全局变量][By:十里坡剑神][QQ:112582793][2017/12/5]
	static BOOL CheckKyunKong;
	~AIManager(void);
	//[单例模式][By:十里坡剑神][QQ:112582793][2017/12/5]
	GETINSTANCE(AIManager);
	//[脚本初始化][BY:十里坡剑神][QQ:112582793][2019-8-2][13:10:23]
	void LoadSetFile();
	//[主进程][By:十里坡剑神][QQ:112582793][2017/12/5]
	void process();   
	//[狩猎进程][By:十里坡剑神][QQ:112582793][2017/12/5]
	BOOL FindSkillStatusFLg(DWORD dwFlg, CMonster* pMonster);  
	void ProcessActtacMonster();	 
	void AttactMonster(LPOBJECT lpObj,double distance1);   
	//[打坐进程][By:十里坡剑神][QQ:112582793][2017/12/5]
	void ProcessDaZuo();  
	void EndingDaZuo();	
	BOOL NeedDaZuo();
	BOOL CanEndingDaZuo();
	//[自动无双][By:十里坡剑神][QQ:112582793][2017/12/5]
	void AutoMussang();  
	//[自动复活][By:十里坡剑神][QQ:112582793][2017/12/5]
	void ProcessReBuild();
	void MovePoint();
	//[自动反][By:十里坡剑神][QQ:112582793][2017/12/5]
	void ProcessAutoAttrPlay();  
	void StartAutoAttrPalyProcess(CObject *AttrPlayer);  
	//[辅助技能][By:十里坡剑神][QQ:112582793][2017/12/5]
	void LoadSkillSetInfo();
	CYHHashTable<SKILLSET> * GetSkillSetInfo(){return &m_SkillSetInfo;}
	void UseNotMainSkill();
	WORD GetSkillStateIconForName(char * SkillName);
	CMugongBase * CheckHeightOrLow(int SkillIdx1,int SkillIdx2);
	//[保护进程][By:十里坡剑神][QQ:112582793][2017/12/5]
	void LoadUseItemInfo();
	CYHHashTable<sUseItem> * GetUseItemInfo(){return &m_UseItemInfo;}
	void ProtectProcess();
	BOOL UsePacketItem(DWORD ItemIdx,BOOL val=FALSE);
	//[辅助功能][By:十里坡剑神][QQ:112582793][2017/12/5]
	DOUBLE GetLife();
	DOUBLE GetShield();
	DOUBLE GetNaeRyuk();
	DOUBLE GetObjLife(LPOBJECT lpObj);
	BOOL PlayerUseSkill(CSkillInfo* pInfo); 
	BOOL IsOpen(){return sAiConfig.AutoSwitch[eOpen];}
	BOOL SetOpen(BOOL val);
	void AddMonsterAttrTime(){m_MonsterAttrTime++;}
	WORD GetAIMap(){return sAiConfig.SetValue[eLastMap];}
	//void SetFistRun(BOOL val){IsFirstRun = val;}
	void SaveConfig();
	void ReadConfig();
	void InitNotMainSkillTime();
	//[轻功][By:十里坡剑神][QQ:112582793][2017/12/5]
	void CheckKyunkong();
	void DufualtUsekyungGong();
	void SetChangeMap(BOOL Val){m_IsChangeMap = Val;}
	void SetTooFar(BOOL val);
	void LoadAiSkillInfo();
	CMugongBase * GetNotMainSkillInfo(WORD wIndex);
	void AutoCheckPetState();//检查宠物状态
	void CheckTimer();
	//void CheckMonsterStat(CObject* pAttacker);
	void SetUseSkill(){ DobuleSkillFlg = !DobuleSkillFlg;}
	BOOL CheckNotMainSkill();
	//[内挂限制信息][BY:十里坡剑神][QQ:112582793][2019-8-2][12:05:45]
	void LoadAiLimitInfo();
	bool IsCanUse();
	bool CheckUseLimit(const AiLimitInfo& info);
	bool UseHunXue();
	void SetUseHunXue(){ m_bUseHunXue  = !m_bUseHunXue; }
};
