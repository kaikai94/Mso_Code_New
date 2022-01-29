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
#include "interface\cdialog.h"
#include "WindowIDEnum.h"

class CharacterRBTN :public cDialog
{
	cButton*	AddFriend;
	cButton*	InviteMember;
	cButton*	ViewTargeteQ;
	cButton*	Report;
	cStatic*	CharName;
	cButton*	AddPartyMember;
	cButton*	ApplyDeal;
	cButton*	ApplyPK;
	cButton*	MiniNote;
	cButton*	SendWhisper;
	cButton*	MarryMe;
	cButton*	DelMarry;
public:
	CharacterRBTN(void);
	virtual ~CharacterRBTN(void);

	void Show();
	void Close();
	void SetCharName(char *buf);
	void Linking();
	void SendReportMsg();
};




