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
#include "./interface/cMultiLineText.h"
#include "Item.h"
#include <vector>
//class cButton;
class cSendItemInfoDlg:public cDialog
{
	cMultiLineText* m_pSendItemInfoTip;
	DWORD m_dwItemDBidx;
	//cButton * m_CloseBtn;
	std::vector<DWORD> m_NeedDeleteItem;
public:
	cSendItemInfoDlg(void);
	~cSendItemInfoDlg(void);
	void SetItem(DWORD ItemDBidx,BOOL bNeedDelete=FALSE);
	void SetItemTip(cMultiLineText *Tip);
	void Linking();
	void Show();
	virtual void SetActive( BOOL val );
	BOOL IsSameItem(DWORD dwItemDBIdx);
	void ClearUseItem();
};
