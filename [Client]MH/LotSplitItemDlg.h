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
#pragma once //[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][8:40:30]
#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include <vector>
class cIconDialog;
class CItem;
class LotSplitItemDlg: public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	std::vector<BYTE> m_EmptySolt;
	
	WORD		m_ItemIdx;
	WORD		m_ItemPos;
	DWORD		m_ItemDurability;
	WORD		m_NeedEmptySolt;
	WORD		m_InputNum;
	DWORD		m_ItemDBIdx;
public:
	LotSplitItemDlg(void);
	~LotSplitItemDlg(void);

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon);
	virtual void SetActive( BOOL val );
	virtual void OnActionEvent(LONG lId, void * p, DWORD we);
	void ReleaseItem();

	void AddVirtualItem( CItem* pItem );

	void ItemSplitSyn();

	void CalcNeedSolt();

	void ClearData();
};
