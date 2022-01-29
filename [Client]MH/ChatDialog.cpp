







// ChatDialog.cpp: implementation of the CChatDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cPushupButton.h"
#include "cImeEx.h"
#include "GlobalEventFunc.h"
#include "./Input/UserInput.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "OptionDialog.h"
#include "ShoutchatDialog.h"
#include "cScriptManager.h"                // 2014-12-15 表情资源使用头

#include "./Interface/cFont.h"

#include "input/UserInput.h"
#include "GameResourceManager.h"
#include "cChangeCharCode.h"
#include "cSendItemInfoDlg.h"	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "ItemManager.h"
#include "CheckResourceFile.h"
extern HWND _g_hWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatDialog::CChatDialog()
{
	m_type				= WT_CHATDIALOG;
	m_pChatEditBox		= NULL;

	m_bHideChatDialog	= FALSE;
//	m_bShowMenu			= FALSE;
	m_bShowGuildTab		= FALSE;

	m_cPreWord[0] = 0;
	m_cPreWord[1] = '@';
	m_cPreWord[2] = '#';
	m_cPreWord[3] = '$';
	m_cPreWord[4] = '%';
}

CChatDialog::~CChatDialog()
{

}
BOOL CChatDialog::IsChatFocus()
{
	if(m_pChatEditBox)
		return m_pChatEditBox->IsFocus();
	else
		return FALSE;
}
void CChatDialog::Linking()
{

	m_pChatEditBox	= (cEditBox *)GetWindowForID(MI_CHATEDITBOX);
	m_pChatEditBox->SetValidCheck( VCM_DEFAULT );
	m_pChatEditBox->SetEditFunc( GAMEIN_ChatFunc );

	m_ImageGrid =(cIconGridDialog*)GetWindowForID(CTI_ICONGRID_IMAGE); // 2014-12-15 表情框

	m_pSheet[WHOLE]		= (cListDialog*)GetWindowForID(CTI_SHEET1);
	m_pSheet[PARTY]		= (cListDialog*)GetWindowForID(CTI_SHEET2);
	m_pSheet[GUILD]		= (cListDialog*)GetWindowForID(CTI_SHEET3);
	m_pSheet[ALLIANCE]	= (cListDialog*)GetWindowForID(CTI_SHEET4);

	m_pSheet[SHOUT]		= (cListDialog*)GetWindowForID(CTI_SHEET5);
	m_pSheet[ITEMINFO]  = (cListDialog*)GetWindowForID(CTI_SHEET6);
	m_pPBMenu[WHOLE]	= (cPushupButton*)GetWindowForID(CTI_BTN_WHOLE);
	m_pPBMenu[PARTY]	= (cPushupButton*)GetWindowForID(CTI_BTN_PARTY);
	m_pPBMenu[GUILD]	= (cPushupButton*)GetWindowForID(CTI_BTN_MUNPA);
	m_pPBMenu[ALLIANCE]	= (cPushupButton*)GetWindowForID(CTI_BTN_ALLMUNPA);
	m_pPBMenu[SHOUT]	= (cPushupButton*)GetWindowForID(CTI_BTN_WORLD);

	m_pAllShout = (cPushupButton*)GetWindowForID( CTI_BTN_ALLWORLD1 );

	m_pAllShout->SetPush( TRUE );

	m_pSheet[WHOLE]->SetActive( TRUE );
	m_pSheet[PARTY]->SetActive( FALSE );
	m_pSheet[GUILD]->SetActive( FALSE );
	m_pSheet[ALLIANCE]->SetActive( FALSE );
	m_pSheet[SHOUT]->SetActive( FALSE );
	m_pSheet[ITEMINFO]->SetActive( TRUE );
	m_pSheet[ITEMINFO]->SetRelXY(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-247,m_pSheet[ITEMINFO]->GetRelY());
	m_nCurSheetNum = WHOLE;
	SelectMenu( m_nCurSheetNum );

    LoadFaceList(); 

	m_pChatEditBox->SetRenderFaceImg(TRUE);
	m_pSheet[WHOLE]->SetRenderFaceImg( TRUE );
	m_pSheet[PARTY]->SetRenderFaceImg(TRUE );
	m_pSheet[GUILD]->SetRenderFaceImg(TRUE);
	m_pSheet[ALLIANCE]->SetRenderFaceImg(TRUE );
	m_pSheet[SHOUT]->SetRenderFaceImg( TRUE );
}

void CChatDialog::LoadFaceList()
{
	// 2014-12-15 表情到内存!

	for(int i=0;i<CHARFACELIST; i++)
	{
		SCRIPTMGR->GetImage(i+FACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);

		cIcon * pIcon;

		pIcon=new cIcon;

		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+5000+i);
				
		pIcon->SetData(i+FACELISTSTARTIDX);

		pIcon->SetDisable(TRUE);

		pIcon->SetDepend(FALSE);
        
		m_ImageGrid->AddIcon(i,pIcon);
	}

	m_ImageGrid->SetActive(FALSE);

}

DWORD CChatDialog::ActionEvent( CMouse * mouseInfo )
{
    DWORD we = cDialog::ActionEvent( mouseInfo );

	cListDialog* pCurSheet = m_pSheet[m_nCurSheetNum];
	WORD width = pCurSheet->GetWidth();
	WORD height = pCurSheet->cWindow::GetHeight();
	if( KEYBOARD->GetKeyPressed(KEY_CONTROL) )
	{
		pCurSheet->SetWH(410, height);
		if( mouseInfo->LButtonDown() )
		{
			GetSelectedName(mouseInfo);
		}
	}
	else
		pCurSheet->SetWH(410, height);

	return we;
}




void CChatDialog::AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str )
{
//#ifdef _TL_LOCAL_
//	int nLen = strlen( str );
//	char buf[256];
//
//	BYTE brk[512];
//	int nBrkNum = g_TBreak.FindThaiWordBreak( str, nLen, brk, 512, FTWB_SEPARATESYMBOL );
//	int nCutPos = 0;
//
//	int nCount = 0;
//
//	for( int i = 0 ; i < nBrkNum ; ++i )
//
//	{
//		++nCount;	
//		nCutPos += brk[i];
//		if( CFONT_OBJ->GetTextExtentEx(0, str, nCutPos ) > CHATLIST_TEXTEXTENT )
//		{
//			if( nCount == 1 )
//			{
//				for( int k = 1 ; k <= nCutPos ; ++k )
//				{
//					if( CFONT_OBJ->GetTextExtentEx(0, str, k ) > CHATLIST_TEXTEXTENT )
//					{
//						nCutPos = nCutPos - brk[i] + k-1; 
//						strncpy( buf, str, nCutPos );
//						buf[nCutPos] = 0;
//						GetSheet(ChatLimit)->AddItem( buf, MsgColor );
//						str += nCutPos;
//						brk[i] -= nCutPos;
//						nCutPos = 0;
//						nCount = 0;
//						--i;
//						break;
//					}
//				}
//			}
//
//			else
//			{
//				nCutPos -= brk[i];
//				strncpy( buf, str, nCutPos );
//				buf[nCutPos] = 0;
//				GetSheet(ChatLimit)->AddItem( buf, MsgColor );
//				--i;
//				str += nCutPos;
//				nCutPos = 0;
//				nCount = 0;
//			}
//
//			if( *str == ' ' )
//			{
//				++str;
//				++i;
//			}
//		}
//	}
//
//	if( nCutPos > 0 )
//		GetSheet(ChatLimit)->AddItem( str, MsgColor );
//#else

	int nLen = strlen( str );


	char buf[256];
	int nCpyNum = 0;

	BOOL btwoline = FALSE;
	if(ChatLimit==ITEMINFO)
	{
		while( nLen > CHATLISTFORITEM_TEXTLEN )
		{
			if( ( str + CHATLISTFORITEM_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLISTFORITEM_TEXTLEN ) ) )
				nCpyNum = CHATLISTFORITEM_TEXTLEN - 1;
			else
				nCpyNum = CHATLISTFORITEM_TEXTLEN;

			strncpy( buf, str, nCpyNum );
			buf[nCpyNum] = 0;
			GetSheet(ChatLimit)->AddItem( buf, MsgColor );
			nLen -= nCpyNum;
			str += nCpyNum;
			if( *str == ' ' ) ++str;

			btwoline = TRUE;
		}
	}
	else
	{
		while( nLen > CHATLIST_TEXTLEN )
		{
			if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
				nCpyNum = CHATLIST_TEXTLEN - 1;
			else
				nCpyNum = CHATLIST_TEXTLEN;

			strncpy( buf, str, nCpyNum );
			buf[nCpyNum] = 0;
			GetSheet(ChatLimit)->AddItem( buf, MsgColor );
			nLen -= nCpyNum;
			str += nCpyNum;
			if( *str == ' ' ) ++str;

			btwoline = TRUE;
		}
	}


//#ifdef _HK_LOCAL_
//
//	if( nLen > 0 )
//	{
//		if( btwoline )
//		{
//			char buf[256]={ 0, };
//			sprintf( buf, "--%s", str );
//			GetSheet(ChatLimit)->AddItem( buf, MsgColor );
//		}
//		else
//			GetSheet(ChatLimit)->AddItem( str, MsgColor );
//	}
//
//#else

	if( nLen > 0 )
		GetSheet(ChatLimit)->AddItem( str, MsgColor );

//#endif // _HK_LOCAL_
//
//#endif // _TL_LOCAL_

}

void CChatDialog::AddMsgAll( DWORD MsgColor, char* str )
{
//#ifdef _TL_LOCAL_
//	int nLen = strlen( str );
//	char buf[256];
//
//	BYTE brk[512];
//	int nBrkNum = g_TBreak.FindThaiWordBreak( str, nLen, brk, 512, FTWB_SEPARATESYMBOL );
//	int nCutPos = 0;
//
//	for( int i = 0 ; i < nBrkNum ; ++i )
//	{
//		nCutPos += brk[i];
//		if( CFONT_OBJ->GetTextExtentEx(0, str, nCutPos ) > CHATLIST_TEXTEXTENT )
//		{
//			nCutPos -= brk[i];
//			strncpy( buf, str, nCutPos );
//			buf[nCutPos] = 0;
//			GetSheet(WHOLE)->AddItem( buf, MsgColor );
//			GetSheet(PARTY)->AddItem( buf, MsgColor );
//			GetSheet(GUILD)->AddItem( buf, MsgColor );
//			--i;
//			str += nCutPos;
//
//			nCutPos = 0;
//			if( *str == ' ' )
//			{
//				++str;
//				++i;
//			}
//		}
//	}
//
//	if( nCutPos > 0 )
//	{
//		GetSheet(WHOLE)->AddItem( str, MsgColor );
//		GetSheet(PARTY)->AddItem( str, MsgColor );
//		GetSheet(GUILD)->AddItem( str, MsgColor );
//	}
//#else

	int nLen = strlen( str );

	char buf[128];
	int nCpyNum = 0;


	while( nLen > CHATLIST_TEXTLEN )
	{

		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;

		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		GetSheet(WHOLE)->AddItem( buf, MsgColor );
		GetSheet(PARTY)->AddItem( buf, MsgColor );
		GetSheet(GUILD)->AddItem( buf, MsgColor );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;	
	}

	if( nLen > 0 )
	{
		GetSheet(WHOLE)->AddItem( str, MsgColor );
		GetSheet(PARTY)->AddItem( str, MsgColor );
		GetSheet(GUILD)->AddItem( str, MsgColor );
	}

//#endif
}

void CChatDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
		case CTI_BTN_TALLER:
			{
				m_pSheet[WHOLE]->onSize( TRUE );
				m_pSheet[PARTY]->onSize( TRUE );
				m_pSheet[GUILD]->onSize( TRUE );
				m_pSheet[ALLIANCE]->onSize( TRUE );
				m_pSheet[SHOUT]->onSize( TRUE );
				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->RefreshPosition();
				GAMEIN->GetShoutchatDlg()->SetActive( FALSE );
				GAMEIN->GetShoutchatDlg()->SetActive( TRUE );
			}
			break;
		case CTI_BTN_SMALLER:
			{

				m_pSheet[WHOLE]->onSize( FALSE );
				m_pSheet[PARTY]->onSize( FALSE );
				m_pSheet[GUILD]->onSize( FALSE );
				m_pSheet[ALLIANCE]->onSize( FALSE );
				m_pSheet[SHOUT]->onSize( FALSE );
				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->RefreshPosition();
				GAMEIN->GetShoutchatDlg()->SetActive( FALSE );
				GAMEIN->GetShoutchatDlg()->SetActive( TRUE );
			}
			break;
		case CTI_BTN_OPTION:
			{
					GAMEIN->GetOptionDialog()->SetActive( TRUE );
					GAMEIN->GetOptionDialog()->SelectTab( 1 );
			}
			break;
		case CTI_BTN_ALLWORLD1:
			{
				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->SetActive( !GAMEIN->GetShoutchatDlg()->IsActive() );
			}
			break;
		case  CTI_BTN_IMAGE: 
			{
                 if(m_ImageGrid->IsActive())
				 {
					 m_ImageGrid->SetActive(FALSE);
				 }
				 else
				 {
					 m_ImageGrid->SetActive(TRUE);
				 }
			}
			break;
		}
	}
	else if( we & WE_PUSHDOWN  )

	{
		switch( lId )
		{
		case CTI_BTN_WHOLE:
			SelectMenu(WHOLE);
			break;
		case CTI_BTN_PARTY:
			SelectMenu(PARTY);
			break;
		case CTI_BTN_MUNPA:
			SelectMenu(GUILD);
			break;
		case CTI_BTN_ALLMUNPA:
//#ifdef _JAPAN_LOCAL_
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(710) );
//			SelectMenu(m_nCurSheetNum);
//#else
			SelectMenu(ALLIANCE);
//#endif
			break;
		case CTI_BTN_WORLD:
			SelectMenu(SHOUT);
			break;
		case CTI_BTN_ALLWORLD1:
			if( GAMEIN->GetShoutchatDlg() )
				GAMEIN->GetShoutchatDlg()->SetActive( TRUE );
			break;
		}

		m_pChatEditBox->SetFocusEdit( TRUE );		
	}
	else if( we & WE_PUSHUP)
	{
		SelectMenu(m_nCurSheetNum);

		m_pChatEditBox->SetFocusEdit( TRUE );
		
		switch( lId )
		{
		case CTI_BTN_ALLWORLD1:	
			if( GAMEIN->GetShoutchatDlg() )
				GAMEIN->GetShoutchatDlg()->SetActive( FALSE );
			break;
		}
	}
	else if( we & WE_LBTNDBLCLICK )
	{
		int k = 0;

	}
	else if( we & WE_LBTNCLICK) 
	{
        switch(lId)
		{
		case CTI_ICONGRID_IMAGE:
			{
				int Pos=m_ImageGrid->GetCurSelCellPos();

				if(Pos>=0)
				{
					char ImgCode[10];

					sprintf(ImgCode,"{%02d",Pos);

					m_pChatEditBox->SetEditText(strcat(m_pChatEditBox->GetEditText(),ImgCode));
				}
			}
		}
	}
}
void CChatDialog::SendItemInfoParesing(char * strItemMsg)
{
	std::vector<std::string> v_Str;
	std::string str = strItemMsg;
	v_Str = FILECHECK->split(str, "&");
	if (v_Str.size() != 3) return;

	DWORD ObjID = atoi(v_Str[2].c_str());
	int ItemDBIdx = atoi(v_Str[1].c_str());

	CItem * pItem = NULL;
	pItem = ITEMMGR->GetItem(ItemDBIdx);
	if(GAMEIN->GetSendItemInfoDlg()->IsSameItem(ItemDBIdx))
	{
		if(!GAMEIN->GetSendItemInfoDlg()->IsActive())
			GAMEIN->GetSendItemInfoDlg()->Show();
		else
			return;
	}
	else if(pItem!=NULL)
	{
		GAMEIN->GetSendItemInfoDlg()->SetItem(pItem->GetDBIdx());
		GAMEIN->GetSendItemInfoDlg()->SetItemTip(pItem->GetToolTip());
		GAMEIN->GetSendItemInfoDlg()->Show();
		if(pItem->IsActive())
			pItem->GetToolTip()->SetItemDBIdx(pItem->GetDBIdx());
	}
	else
	{
		MSG_GETSENDITEMINFO msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEM_SENDINFO_SYN;
		msg.dwObjectID=HEROID;
		msg.dwObject = ObjID;
		msg.dwItemDBIdx = ItemDBIdx;
		NETWORK->Send(&msg,sizeof(msg));
	}
}
void CChatDialog::SetEditBoxPreWord()
{
	char buf[256] = {0,};
	
	SafeStrCpy( buf, m_pChatEditBox->GetEditText(), 128 );

	int nLen = strlen( buf );
	if( nLen == 0 )
	{
		buf[0] = m_cPreWord[m_nCurSheetNum];
	}
	else if( buf[0] != '/' )
	{

		if( m_nCurSheetNum != WHOLE )
		{
			if( IsPreWord( buf[0] ) == FALSE )
			{
				memmove( buf+1, buf, nLen );
			}
			buf[0] = m_cPreWord[m_nCurSheetNum];
		}
		else
		{
			if( IsPreWord( buf[0] ) == TRUE )
			{
				memmove( buf, buf+1, nLen );
			}
		}
	}
#ifdef _BIG5SYSTEM_
	if (strlen(buf)>=2)
	{
		CODEMGR->ChangeCharCode(buf);
	}
#endif
	m_pChatEditBox->SetEditText( buf );
}

BOOL CChatDialog::IsPreWord( char c )
{
	for( int i = PARTY ; i <= SHOUT ; ++i )
	{
		if( m_cPreWord[i] == c ) return TRUE;
	}

	return FALSE;		
}

void CChatDialog::SelectMenu( int nSheet )
{
	if( nSheet >= 5 ) return;

	for( int i = 0 ; i < 5 ; ++i )
	{
		if( nSheet == i )
			m_pPBMenu[i]->SetPush(TRUE);
		else
			m_pPBMenu[i]->SetPush(FALSE);
	}

	m_nCurSheetNum = nSheet;
	SetEditBoxPreWord();
}


void CChatDialog::HideChatDialog( BOOL bHide )
{
	m_bHideChatDialog = bHide;

	if( bHide )
	{
		//m_pSheet[m_nCurSheetNum]->SetActive( FALSE );
		m_pSheet[WHOLE]->SetActive( FALSE );
	}
	else
	{
		//m_pSheet[m_nCurSheetNum]->SetActive( TRUE );
		m_pSheet[WHOLE]->SetActive( TRUE );
	}

}


void CChatDialog::ShowGuildTab( BOOL bShow )
{
//	ShowMenu( FALSE );

//	if( bShow == FALSE )
//	if( m_nCurSheetNum == GUILD )
//	{
//		SelectMenu( WHOLE );
//	}
//
//	m_bShowGuildTab = bShow;
}

BYTE CChatDialog::GetLineNum()
{ 
	return m_pSheet[m_nCurSheetNum]->GetMiddleImageNum();
}

WORD CChatDialog::GetSheetPosY()
{
	return (WORD)m_pSheet[m_nCurSheetNum]->GetAbsY();
}

WORD CChatDialog::GetSheetHeight()
{
	DWORD height = 0;

	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_TOP );
	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_DOWN );
	height += ( m_pSheet[m_nCurSheetNum]->GetHeight( LD_MIDDLE )*m_pSheet[m_nCurSheetNum]->GetMiddleImageNum() );

	return (WORD)height;	
}

void CChatDialog::SetAllShoutBtnPushed( BOOL val )
{
	if( m_pAllShout )
		m_pAllShout->SetPush( val );
}

void CChatDialog::GetSelectedName(CMouse* mouseInfo)
{
	memset(m_SelectedName, 0, MAX_NAME_LENGTH+1);

	cListDialog* pCurSheet = m_pSheet[m_nCurSheetNum];

	ITEM* pItem = pCurSheet->GetSelectedITEM(mouseInfo);
	if(!pItem)
		return;

   	if( pItem->rgb != dwChatTextColor[CTC_GENERALCHAT] && 
		pItem->rgb != dwChatTextColor[CTC_PARTYCHAT] && 
		pItem->rgb != dwChatTextColor[CTC_GUILDCHAT] && 

		pItem->rgb != dwChatTextColor[CTC_ALLIANCE] && 
		pItem->rgb != dwChatTextColor[CTC_SHOUT] && 
		pItem->rgb != dwChatTextColor[CTC_WHISPER] )
		return;
		
	int nLen = strlen(pItem->string);

	char* pStr = NULL;
	if( nLen )
		pStr = pItem->string;

	int nCpyNum = 0;
	if (pStr != NULL)
	{
		if (*pStr == '[')
		{
			++pStr;
		}
		else
		{
			return;
		}
	}



	BOOL bNeedCpy = FALSE;
	BOOL bDevelopid = FALSE;
	for( int i = 0; i < nLen; ++i )
	{
		if( pStr[i] == ']' )
		{
			bNeedCpy = TRUE;			
			char tbuf[MAX_NAME_LENGTH+1] = { 0, };
			SafeStrCpy( tbuf, pStr, nCpyNum+1 );

			if( (strncmp( pStr, "[没锋", 5 ) == 0 || strncmp( pStr, "[炔锋", 5 ) == 0) &&
				!bDevelopid )
			{
                bDevelopid = TRUE;
				++nCpyNum;
				continue;
			}
			break;
		}

		++nCpyNum;
	}

	if( nCpyNum > MAX_NAME_LENGTH )
		return;
	
	if( bNeedCpy )
	{
		if( bDevelopid )
			SafeStrCpy(m_SelectedName, pStr, nCpyNum + 2);
		else
			SafeStrCpy(m_SelectedName, pStr, nCpyNum + 1);

		if( OpenClipboard(_g_hWnd) )
		{
			EmptyClipboard();

			HGLOBAL hMem = NULL;
			hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(m_SelectedName) + 1);
			if( hMem )
			{
				LPTSTR pClipData = NULL;
				pClipData = (LPTSTR)GlobalLock(hMem);
				if (pClipData != NULL)
				{
					pClipData[0] = 0;
					SafeStrCpy(pClipData, m_SelectedName, sizeof(m_SelectedName) + 1);
					SetClipboardData(CF_OEMTEXT, hMem);
					GlobalUnlock(hMem);
				}
				GlobalFree(hMem);
			}
			CloseClipboard();


			//ChatMsg眠啊
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1630), m_SelectedName );
		}		

	}

	else
		return;

}




