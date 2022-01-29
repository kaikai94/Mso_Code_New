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
struct sRESTCFG
{
	BOOL	ReSetPower;
	WORD	MaxReSetTime;
	WORD	MinReSetLevel;
	DWORD	ReSetMoney;
};
struct RSETPOINT
{
	WORD wBase;
	WORD wLevel;
};
struct  TopSendInterval
{
	WORD wHour;
	WORD wMinute;
};
#define RESTDMGR sRestManager::GetInstance()
class sRestManager
{
	stTIME m_RestTime;
	stTIME m_GiveProfitTime;
	WORD   m_wInterval;
	TopSendInterval   m_TopSend;

	sRESTCFG m_sRestCfg;
	SPROFIT	 m_sSprofit;
	WORD	 m_MapNum;

	std::map<WORD,RSETPOINT> m_RsetSetStaut;
public:
	sRestManager(void);
	~sRestManager(void);
	GETINSTANCE(sRestManager);
	void InitRestConfig(WORD mapnum,BOOL bReSetPower=FALSE,WORD wMaxReSetTime=0,WORD wMinReSetLevel=60,DWORD dwReSetMoney=1000000);
	void LoadFirstRestTime();
	void LoadSprofitInfo();
	void LoadRestSetStaut();
	void SetRestTime(DWORD val);
	void SetInterval(WORD val){m_wInterval = val;}
	void NetWorkMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
	void Process();
	void AddRestTimes();
	void BeingSendSprofit();
	void ShowConfigInformation();
	WORD GetRestSetStaut(WORD pLayerLevel);
	WORD GetRestSetLevel(WORD pLayerLevel);
	void ReLoadRestTime();
};
