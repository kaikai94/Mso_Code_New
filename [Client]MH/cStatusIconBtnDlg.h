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
//         ���汣��       ����BUG     �����޸�					  //
////////////////////////////////////////////////////////////////////
#pragma once
#include "./interface/cDialog.h"
class cPushupButton;
class cButton;
class cStatusIconBtnDlg: public cDialog
{
	cPushupButton * pShowStatuIcoBtn;

	//[¼����][By:ʮ���½���][QQ:112582793][2018/9/17]
	cButton*	m_pVideoCaptureStartBtn;
	cButton*	m_pVideoCaptureStopBtn;
public:
	cStatusIconBtnDlg(void);
	~cStatusIconBtnDlg(void);
	void Linking();
	//[¼����][By:ʮ���½���][QQ:112582793][2018/9/17]
	void ShowVideoCaptureStartBtn( BOOL bShow );
};
