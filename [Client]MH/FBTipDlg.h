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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./interface/cDialog.h"
#include "./interface/cEditBox.h"

class FBTipDlg : public cDialog
{
	cStatic *TimeTip;
	cEditBox* m_TimeEdit;
	int Minute;
	int Hour;
	int Sec;
	int Day;
	int Remaintime;

	static DWORD m_TipTime;
	DWORD m_TipCurTime;
	BOOL m_StartTip;
	WORD m_wUseItemIdx;

public:
	FBTipDlg();
	virtual ~FBTipDlg();
	void Open();
	void UpDataTime();
	void Close();
	void Linking();
	BOOL IsStartTip(){return m_StartTip;}
	void SetStartTip(BOOL val){m_StartTip = val;}
	void StartTip();
	void SetUseItemTime(int nTime,WORD wItemIdx);
	void CalcAddTime();
	void AddTime(int nTime){ Remaintime += nTime; }
	WORD GetInputTime(){ return atoi(m_TimeEdit->GetEditText()); }
};




