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
#include <map>
#include <vector>
#include "..\[CC]Header\GameResourceStruct.h"
#define RECHARGEMGR RechargeRecordManager::GetInstance()
class CPlayer;
class RechargeRecordManager
{
	std::map<WORD, WORD> m_RechargeGiftInfo;
	std::map<WORD, sCHARRECHARGEINFO> m_CharacterRechargeInfo;
	std::vector<WORD> m_GiftMoneyInfo;
	//WORD m_GiftMoneyInfo[5];
public:
	RechargeRecordManager(void);
	~RechargeRecordManager(void);
	GETINSTANCE(RechargeRecordManager);
	void LoadRechargeInfo();
	void SetCharcterRechargeInfo(DWORD Character_idx,DWORD PayMoney,DWORD UsedMoney,DWORD PayMoneyCount);
	void GetRechargeGift(CPlayer* pPlayer,WORD type);
	void RemoveRechargeInfo(DWORD Character_idx);
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void SendInfoToClient(CPlayer* pPlayer);
	WORD GetGiftCount(WORD Type);
	void UpdateRecharge();
};
