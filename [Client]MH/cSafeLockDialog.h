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
#include "./interface/cDialog.h"
class cStatic;
class cButton;
class cEditBox;
class cSafeLockDialog: public cDialog
{
	cStatic* m_pStPwd;
	cStatic* m_pStNewPwd;
	cStatic* m_pStCheckNewPwd;
	cButton* m_btnOK;
	cEditBox* m_edPwd;
	cEditBox* m_edNewPwd;
	cEditBox* m_edCheckNewPwd;
	BOOL m_IsRestPwd;
	BOOL m_ChangeDialog;
public:
	cSafeLockDialog(void);
	~cSafeLockDialog(void);
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Show(BOOL IsRestPwd);
	void Close(BOOL IsOpen = FALSE);
};
