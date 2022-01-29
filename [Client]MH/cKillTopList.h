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
#include "./interface/cTabDialog.h"
#include <map>
class cIconGridDialog;
class cListCtrl;
class cKillTopList: public cTabDialog
{
	cIconGridDialog * m_IconDlg;
	SINGEDINFO		  m_HeroKillTopInfo;
	eSINGED	m_GetType;
	std::map<std::string,SINGEDGIFTINFO> m_GiftInfo;
	std::map<eSINGED,std::string> m_DayInfo;
public:
	cKillTopList(void);
	~cKillTopList(void);
	virtual void SetActive( BOOL val );
	virtual void Add(cWindow * window);
	void Linking();
	void OpenKillBoard();
	void EndDialog();

	void ShowListDlg();
	void ShowKillTopList();
	void SetTableState(WORD count);
	cListCtrl* m_memdlg[5]; 
	cStatic*   m_sTime;
	void SetHeroKillTopInfo(SINGEDINFO& info);
	void InitDlgForHero();
	void LoadGiftInfo();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};
