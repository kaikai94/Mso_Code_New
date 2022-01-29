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
#include "cListDialogEx.h"
#include "CommonStruct.h"
#include "cListCtrl.h"
#include "./interface/cTabDialog.h"
class GuildTopList:public cTabDialog
{
public:
	cListCtrl* m_pList[5];
	cStatic*   m_sTime;
	GuildTopList(void);
	virtual ~GuildTopList(void);
	virtual void SetActive( BOOL val );
	virtual void Add(cWindow * window);
	void ClearList();
	void Linking();
	void ShowList();
	void OpenGuildTopList();
	void SetTableState(WORD count);
};
