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
#include <string>
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"
#include "Date.h"

#define SINGEDMGR SingedManager::GetInstance()
class CPlayer;
class SingedManager
{
	Date m_NewTime;
	eSINGED m_day;
	std::string m_dayChar;
	std::map<DWORD,SINGEDINFO> m_SingedInfo;
	std::map<DWORD,SINGEDINFO>::iterator iter;
	std::map<std::string,SINGEDGIFTINFO> m_SingedGiftInfo;
	std::map<std::string,SINGEDGIFTINFO>::iterator Giftiter;
	DWORD NeedGoldMoney;
public:
	SingedManager(void);
	~SingedManager(void);
	GETINSTANCE(SingedManager);
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void AddSinged(const DWORD& UserId,const SINGEDINFO& info);
	void RemoveSinged(const DWORD& UserId);
	BOOL CanGetGift(const DWORD& UserID);
	BOOL CanGetGift(const DWORD& UserID,eSINGED Day);

	BOOL CanGetCompeletGift(const DWORD& UserID);
	
	void SetSinged(const DWORD& UserID);
	void SetSinged(const DWORD& UserID,eSINGED Day);

	void UpdateTime(SYSTEMTIME& time);
	void RestSingedInfo();
	void LoadGiftInfo();

	eSINGED GiftGiving(CPlayer* pPlayer);
	eSINGED GiftGiving(CPlayer* pPlayer,eSINGED Day);
	eSINGED GiftGivingCompelet(CPlayer* pPlayer);
	void SetSingedMsgInfo(CPlayer* pPlayer,MSG_SINGEDINFO &info);
};
