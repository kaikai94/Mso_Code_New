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
#include "./interface/cDialog.h"
#include "./interface/cIconGridDialog.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "./interface/cButton.h"
#include "./interface/cStatic.h"
enum PayBtnType
{
	ePayBtn1,
	ePayBtn2,
	ePayBtn3,
	ePayBtn4,
	ePayBtn5,
	ePayBtnMax,
};
class cRechargeManager: public cDialog
{
	sCHARRECHARGEINFO	RechargeInfo;
	std::map<int,int> m_RechargeGiftInfo;
	DWORD PayMoney[5];
	cIconGridDialog * IconDlg[2];
	cButton*	m_PayBtn[ePayBtnMax];
	cButton*	m_btnPay;
	cStatic*	m_PayMoney;
	cStatic*	m_PayMoneyCount;
	cStatic*    m_Residual;
public:
	cRechargeManager(void);
	~cRechargeManager(void);
	void Linking();
	void SetActive(BOOL val);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void LoadInfo();
	void ShowWnd();
	void HideWnd();
	void SetRechargeInfo(sCHARRECHARGEINFO &info);
	void InitBtnStat();
};
