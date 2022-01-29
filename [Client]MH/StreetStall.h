




// StreetStall.h: interface for the CStreetStall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREETSTALL_H__0553CA73_956D_4CA4_95ED_3F8280AF8C6B__INCLUDED_)
#define AFX_STREETSTALL_H__0553CA73_956D_4CA4_95ED_3F8280AF8C6B__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000



#include "./Interface/cDialog.h"
#include "./Interface/cIconGridDialog.h"   // 2014-12-15 引用 cIconGridDialog 为表情系统
#include "./Interface/cIcon.h"
#define CHARFACELIST        56         // 表情系统 56个 2014-12-15
#define FACELISTSTARTIDX	148        // 表情位置ID 148 为第一个表情位置 2014-12-15

class cIcon;
class cIconGridDialog;
class cEditBox;
class cTextArea;
class cButton;

enum STALL_DLG_STATE
{
	eSDS_NOT_OPENED,
	eSDS_OPENED,
	eSDS_BUY,

};

enum STALL_OPTION
{
	eSO_DIVE,
	eSO_INPUTMONEY,
	eSO_INPUTMONEY_DUP,
	eSO_INPUTGOLD,      // 2014-10-30 摆摊操作
	eSO_INPUTGOLD_DUP,  // 2014-10-30 摆摊操作
};

#define DEFAULT_TITLE_TEXT	CHATMGR->GetChatMsg(366)
#define DEFAULT_MONEY_TEXT	"0"

class CExchangeItem;

class CStreetStall : public cDialog  
{
	DWORD m_DlgState;
	
	cEditBox* m_pTitleEdit;
	cEditBox* m_pMoneyEdit;
	cEditBox* m_pGoldEdit; 
	cButton* m_pEnterBtn;
	cButton* m_pBuyBtn;
	cButton* m_pEditBtn;
	cIconGridDialog * m_pStallGrid;
	cImage          m_ImageFace[CHARFACELIST];   

	cIcon           m_IconFace[CHARFACELIST];    

	cIconGridDialog * m_ImageGrid;            

	cButton*		 m_pImgBtn;              

	DWORD m_MoneyArray[SLOT_STREETSTALL_NUM];
	DWORD m_GoldArray[SLOT_STREETSTALL_NUM];  
	DWORD m_TotalMoney;
	DWORD m_TotalGold;                       
	char m_OldTitle[MAX_STREETSTALL_TITLELEN+1];		
	int m_nCurSelectedItem;
	DWORD m_dwCurSelectedItemDBIdx;
	
	DWORD m_dwOwnnerId;								

	void* m_pData;										

public:
	CStreetStall();
	virtual ~CStreetStall();

	void Linking();

	void SetDisable( BOOL val );


	void ShowSellStall();
	void ShowBuyStall();
	
	void OnCloseStall(BOOL bDelOption = FALSE);

	void OnMoneyEditClick();

	void OnGoldEditClick(); 

	void OnTitleEditClick();

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	virtual void FakeMoveItem(LONG mouseX, LONG mouseY, cIcon * icon) {}

	void FakeDeleteItem( POSTYPE pos );

	void OnActionEvnet(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
	virtual void SetActive( BOOL val );

	void RegistMoney();
	void RegistMoney( POSTYPE pos, DWORD dwMoney );

	// 2014-10-30 注册

    void RegistGold();
	void RegistGold( POSTYPE pos, DWORD dwGold );

	void RegistTitle( char* title, BOOL bSucess );
	void EditTitle( char* title );

public:
	void GetTitle( char* pStrTitle );

	int GetCurSelectedItemNum() { return m_nCurSelectedItem; }

	void SetCurSelectedItemNum(int num) { m_nCurSelectedItem = num; }


	cIcon* GetCurSelectedItem();

	DWORD GetCurSelectedItemDBidx();
	DWORD GetCurSelectedItemDur();

	DWORD GetCurSelectedItemIdx();

	DWORD GetCheckDBIdx() { return m_dwCurSelectedItemDBIdx; }
	
	cIcon* GetItem( POSTYPE pos );
	cIcon* GetItem( DWORD dwDBIdx );

	DWORD GetItemMoney( POSTYPE pos ) { return m_MoneyArray[pos]; }

	DWORD GetItemGold( POSTYPE pos ) { return m_GoldArray[pos]; } 

	int AddItem( cIcon* pItem );
	void DeleteItem( ITEMBASE* pbase );
	void DeleteItemAll(BOOL bDelOption = FALSE);
	void ResetItemInfo( DWORD dwDBIdx, DWORD count );
	CExchangeItem* FindItem( DWORD dwDBIdx );

	void ChangeItemStatus( POSTYPE pos, DWORD money,DWORD gold, BOOL bLock );

	void ChangeItemStatus( ITEMBASE* pbase, DWORD money,DWORD gold, BOOL bLock, int nDivideKind = 0 );  // 2014-10-30 重载

	BOOL SelectedItemCheck();

	BOOL MoneyEditCheck();

	BOOL GoldEditCheck();  // 2014-10-30 检查

	DWORD GetDlgState() { return m_DlgState; }

	DWORD GetStallOwnerId() { return m_dwOwnnerId; }
	void SetStallOwnerId( DWORD dwId ) { m_dwOwnnerId = dwId; }

	void ResetDlgData();
	int GetGridposForItemIdx(WORD wIdx);

	void ShowDivideBox(DWORD dwOption = eSO_DIVE, int x = 0, int y = 0, int nDivideKind = 0 ); 
	static void OnDivideItem( LONG iId, void* p, DWORD param1,void * vData1, void * vData2 );
	static void OnDivideItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeRegistItem( LONG iId, void* p, DWORD param1,DWORD param2, void * vData1, void * vData2 ); // 2014-10-30 
	static void OnRegistItemCancel( LONG iId, void* p, DWORD param1,DWORD param2, void * vData1, void * vData2 ); // 2014-10-30 
	
	void* GetData() { return m_pData; }
	void SetData( void* pData ) { m_pData = pData; }

	cIconGridDialog* GetGridDialog() { return m_pStallGrid; }

	void LoadFaceList();
};

#endif // !defined(AFX_STREETSTALL_H__0553CA73_956D_4CA4_95ED_3F8280AF8C6B__INCLUDED_)


