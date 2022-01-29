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
//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:17:15]
#pragma once
#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "Item.h"
class cIconDialog;
class cStatic;
class cCheckBox;
class cEquipItemInherit : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	cStatic* m_TitleStatic;
	cStatic* m_TipStatic;
	cStatic* m_GrowStatic;
	cCheckBox* m_GrowCheck;
	CItem* m_pItem;
public:
	cEquipItemInherit();
	~cEquipItemInherit();
	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void ReleaseItem();
	void AddVirtualItem(CItem* pItem);
	void SetShopItemInfo(CItem* pItem);
	BOOL IsInherit(){ return m_pItem->GetRareness() == 0; }
	DWORD GetInheritItemRare(){ return m_pItem->GetRareness(); }
	void ItemInheritCreateSyn();
	void ItemInheritCreateAck(MSG_ITEM_RAREITEM_GET* pmsg);
	void ItemInheritCreateNAck(eInheritErrorKind kind);
	void Show(BOOL bIsWeapon=FALSE);
};

