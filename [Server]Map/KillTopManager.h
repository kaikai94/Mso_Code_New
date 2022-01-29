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
//         佛祖保佑       永无BUG     永不修改                    //
////////////////////////////////////////////////////////////////////
#pragma once
#include <map> 
#include <string>
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"
#include "Date.h"

#define KILLTOPMGR KillTopManager::GetInstance()

class CPlayer;

class KillTopManager
{
	static KillTopManager * g_Instance;	//[单例模式实例][By:十里坡剑神][QQ:112582793][2018/4/22]
	
	Date m_NewTime;						//[日期转换星期类][By:十里坡剑神][QQ:112582793][2018/4/22]

	eSINGED m_day;						//[日期枚举][By:十里坡剑神][QQ:112582793][2018/4/22]

	std::string m_dayChar;				//[日期存放变量][By:十里坡剑神][QQ:112582793][2018/4/22]

	std::map<DWORD,SINGEDINFO> m_KillTopList;  //[存储玩家击杀排行信息][By:十里坡剑神][QQ:112582793][2018/4/22]

	std::map<std::string,SINGEDGIFTINFO> m_TopListGiftInfo;  //[礼包信息][By:十里坡剑神][QQ:112582793][2018/4/22]
public:
	KillTopManager(void);
	~KillTopManager(void);
	GETINSTANCE(KillTopManager);
	//[添加排行信息到map][By:十里坡剑神][QQ:112582793][2018/4/22]
	void AddKillTopListInfo(const DWORD& UserId,const SINGEDINFO& info);
	//[移除排行信息][By:十里坡剑神][QQ:112582793][2018/4/22]
	void RemoveKillTopListInfo(const DWORD& UserId);
	//[判断玩家是否可领取礼包][By:十里坡剑神][QQ:112582793][2018/4/22]
	BOOL CanGetGift(const DWORD& UserID,eSINGED Day);
	//[领取后更新数据库][By:十里坡剑神][QQ:112582793][2018/4/22]
	void SetTopListInfo(const DWORD& UserID,eSINGED Day);
	//[加载奖励物品信息][By:十里坡剑神][QQ:112582793][2018/4/23]
	void LoadGiftInfo();
	//[重置击杀排行信息][By:十里坡剑神][QQ:112582793][2018/4/23]
	void RestKillTopListInfo();
	//[奖励发放][By:十里坡剑神][QQ:112582793][2018/4/23]
	eSINGED GiftGiving(CPlayer* pPlayer,eSINGED Day);
	//[网络消息处理][By:十里坡剑神][QQ:112582793][2018/4/23]
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
};
