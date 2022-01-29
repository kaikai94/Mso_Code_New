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
#include "./interface/cStatic.h"
#include "cEditBox.h"

class RestPwd : public cDialog
{
	cButton* m_RegBtn;
	cButton* m_RegNo;
	char m_Pwd1[20];
public:
	RestPwd();
	~RestPwd();
	void Linking();
	void Show();
	void Close();
	BOOL IsInvalidCharInclude( char* pStr );
	BOOL TelIsInvalidCharInclude( char* pStr );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);	
	void EncryptData(char*msg);
};



