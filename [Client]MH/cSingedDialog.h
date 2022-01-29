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
#include "./interface/cDialog.h"
#include "./interface/cIconGridDialog.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "./interface/cButton.h"
#include "Date.h"

class cSingedDialog: public cDialog
{
	Date m_Date;
	eSINGED m_day;
	std::map<std::string,SINGEDGIFTINFO> m_SingedGiftInfo;
	std::map<std::string,SINGEDGIFTINFO>::iterator Giftiter;
	std::map<int,std::string> m_DayInfo;
	cIconGridDialog * IconDlg;
	SINGEDINFO sSingedInfo;
	cButton*	m_btnMonDay;
	cButton*	m_btnTuesDay;
	cButton*	m_btnWednesDay;
	cButton*	m_btnThursday;
	cButton*	m_btnFriDay;
	cButton*	m_btnSaturDay;
	cButton*	m_btnSunDay;
	cButton*	m_btnCompelet;
	MSG_SINGED m_SendMsg;
	DWORD NeedGoldMoney;
public:
	cSingedDialog(void);
	~cSingedDialog(void);
	void Linking();
	void LoadGiftInfo();
	void Show();
	void ClearItem();
	void SetActive(BOOL val);
	void SetSingedInfo(SINGEDINFO &info);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void InitButtonInfo();
	void SendLaterMsg();
	void ClearLaterMsg(){memset(&m_SendMsg,0,sizeof(m_SendMsg));}
	void Process();
};
