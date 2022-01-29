




// ChatDialog.h: interface for the CChatDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_)
#define AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cIconGridDialog.h" 
#include "./Interface/cIcon.h"

class cEditBox;
class cPushupButton;
class cListDialog;
class cIcon;      


enum 
{
	WHOLE,
	PARTY,
	GUILD,
	ALLIANCE,
	SHOUT,
	ITEMINFO,
	MAX_CHAT_COUNT,
};

#define CHATLIST_TEXTLEN	65
#define CHATLISTFORITEM_TEXTLEN	30
//#ifdef _TL_LOCAL_
//#define CHATLIST_TEXTEXTENT	390
//#endif

#define CHARFACELIST        56         //[表情系统][By:十里坡剑神][QQ:112582793][2018/2/19]
#define FACELISTSTARTIDX	148		   //[表情系统][By:十里坡剑神][QQ:112582793][2018/2/19]

class CChatDialog  : public cDialog
{

protected:

	cImage          m_ImageFace[CHARFACELIST];  //[表情系统][By:十里坡剑神][QQ:112582793][2018/2/19]

	cIcon           m_IconFace[CHARFACELIST];   //[表情系统][By:十里坡剑神][QQ:112582793][2018/2/19]

	cIconGridDialog * m_ImageGrid;     //[表情系统][By:十里坡剑神][QQ:112582793][2018/2/19]

	cEditBox*		m_pChatEditBox;

	cListDialog*	m_pSheet[MAX_CHAT_COUNT];
	cPushupButton*	m_pPBMenu[MAX_CHAT_COUNT-1];

	int				m_nCurSheetNum;

	BOOL			m_bHideChatDialog;



	char			m_cPreWord[MAX_CHAT_COUNT];
	
	BOOL			m_bShowGuildTab;
	cPushupButton*	m_pAllShout;
	
	char			m_SelectedName[MAX_NAME_LENGTH+1];
public:


	CChatDialog();
	virtual ~CChatDialog();


	void Linking();
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	void AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddMsgAll( DWORD MsgColor, char* str );
	void OnActionEvent(LONG lId, void * p, DWORD we);

	cEditBox* GetChatEditBox() { return m_pChatEditBox;	}

	cListDialog* GetSheet( int nSheet ) { return m_pSheet[nSheet]; }
	int	GetCurSheetNum()				{ return m_nCurSheetNum; }
	BYTE GetLineNum();

	void SelectMenu( int nSheet );
	void SetEditBoxPreWord();

	BOOL IsPreWord( char c );

	void HideChatDialog( BOOL bHide );

	void ShowGuildTab( BOOL bShow );
	
	WORD GetSheetPosY();
	WORD GetSheetHeight();
	
	void SetAllShoutBtnPushed( BOOL val );

	void GetSelectedName(CMouse* mouseInfo);

	void LoadFaceList();

	void SendItemInfoParesing(char * strItemMsg);	//[物品信息发送][BY:十里坡剑神][QQ:112582793][2019-4-14][12:52:18]
	BOOL IsChatFocus();
};

#endif // !defined(AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_)


