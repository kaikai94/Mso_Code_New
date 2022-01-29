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
class cGuageBar;
class cButton;
class cCheckBox;
class cOnlineVoiceDlg : public cDialog
{
	cGuageBar* m_MicVoice;
	cGuageBar* m_SpeakVoice;
	cButton*   m_JoinRoom;
	cButton*   m_QuitRoom;
	cCheckBox* m_MicCheck;
	cCheckBox* m_SpkCheck;
public:
	cOnlineVoiceDlg(void);
	~cOnlineVoiceDlg(void);
	void Linking();
	void UpdateDailog();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void OnVoiceSetEvent(LONG lId, void * p, DWORD we,long Value);
};
