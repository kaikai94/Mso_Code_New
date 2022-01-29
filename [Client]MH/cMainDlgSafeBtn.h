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
#include "cDialog.h"
class cPushupButton;
class cMainDlgSafeBtn : public cDialog
{
	cPushupButton* m_pBtnClose;
	cPushupButton* m_pBtnOpen;
	BOOL m_bIsSafeLockOpen;
public:
	cMainDlgSafeBtn(void);
	~cMainDlgSafeBtn(void);
	void Linking();
	void Show(BOOL val);
	void ChangeBtnState(BOOL bVal);
	BOOL IsSafeLockOpen(){return m_bIsSafeLockOpen;}
	void SetSafeLockState(BOOL val){m_bIsSafeLockOpen = val;}
};
