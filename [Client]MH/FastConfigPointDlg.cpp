#include "stdafx.h"
#include "FastConfigPointDlg.h"
#include "GameIn.h"
#include "cEditBox.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "./input/Keyboard.h"
#include "MainBarDialog.h"
#include "cMsgBox.h"
FastConfigPointDlg::FastConfigPointDlg(void)
{
	m_wItemIdx=0;
	m_dwItemDBIdx=0;
	m_dwItemPos = 0;
}

FastConfigPointDlg::~FastConfigPointDlg(void)
{
}
//[响应键盘输入事件，实时计算属性点分配][By:十里坡剑神][QQ:112582793][2017/11/22]
DWORD FastConfigPointDlg::ActionKeyboardEvent( CKeyboard* keyInfo )
{
	//if(((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->IsFocus()||
	//	((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->IsFocus()||
	//	((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->IsFocus()||
	//	((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->IsFocus())
	//{
		if (keyInfo->GetKeyUp(KEY_PAD0)||keyInfo->GetKeyUp(KEY_PAD1)||keyInfo->GetKeyUp(KEY_PAD2)||
			keyInfo->GetKeyUp(KEY_PAD3)||keyInfo->GetKeyUp(KEY_PAD4)||keyInfo->GetKeyUp(KEY_PAD5)||
			keyInfo->GetKeyUp(KEY_PAD6)||keyInfo->GetKeyUp(KEY_PAD7)||keyInfo->GetKeyUp(KEY_PAD8)||
			keyInfo->GetKeyUp(KEY_PAD9)||keyInfo->GetKeyUp(KEY_0)||keyInfo->GetKeyUp(KEY_1)||
			keyInfo->GetKeyUp(KEY_2)||keyInfo->GetKeyUp(KEY_3)||keyInfo->GetKeyUp(KEY_4)||
			keyInfo->GetKeyUp(KEY_5)||keyInfo->GetKeyUp(KEY_6)||keyInfo->GetKeyUp(KEY_7)||
			keyInfo->GetKeyUp(KEY_8)||keyInfo->GetKeyUp(KEY_9)||keyInfo->GetKeyUp(DIK_BACK))
		{

			char buf[10];
			memset(buf,0,10);
			WORD Residue=0,Power=0,Dex=0,Con=0,Xm=0;
			Power	= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->GetEditText());
			Dex		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->GetEditText());
			Con		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->GetEditText());
			Xm		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->GetEditText());
			if ((Power+Dex+Con+Xm)>HERO->GetHeroLevelUpPoint())
			{
				WINDOWMGR->MsgBox(MBI_OK, MBT_OK, CHATMGR->GetChatMsg(2199));
				goto lbClear;
			}
			else
			{
				Residue = HERO->GetHeroLevelUpPoint()-(Power+Dex+Con+Xm);
				itoa(Residue,buf,10);
				((cEditBox*)GetWindowForID(FASTCFG_EDIT_LEVLEPOINT))->SetEditText(buf);
			}
		}
		else 
		{
			BOOL bCheckRsult = TRUE;
			char Power[5]={0,};

			SafeStrCpy(Power,((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->GetEditText(),5);
			for(int i = 0;i<strlen(Power);i++)
			{
				if(Power[i]<'0'||Power[i]>'9')
					bCheckRsult = FALSE;
			}

			SafeStrCpy(Power,((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->GetEditText(),5);
			for(int i = 0;i<strlen(Power);i++)
			{
				if(Power[i]<'0'||Power[i]>'9')
					bCheckRsult = FALSE;
			}
			SafeStrCpy(Power, ((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->GetEditText(),5);
			for(int i = 0;i<strlen(Power);i++)
			{
				if(Power[i]<'0'||Power[i]>'9')
					bCheckRsult = FALSE;
			}
			SafeStrCpy(Power, ((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->GetEditText(),5);
			for(int i = 0;i<strlen(Power);i++)
			{
				if(Power[i]<'0'||Power[i]>'9')
					bCheckRsult = FALSE;
			}

			if(!bCheckRsult)
			{
				WINDOWMGR->MsgBox(MBI_OK, MBT_OK, CHATMGR->GetChatMsg(2341));
				goto lbClear;
			}
		}
	//}	
	return 0 ;
lbClear:
	char buf[10];
	memset(buf,0,10);
	itoa(HERO->GetHeroLevelUpPoint(),buf,10);
	((cEditBox*)GetWindowForID(FASTCFG_EDIT_LEVLEPOINT))->SetEditText(buf);
	sprintf(buf,"");
	((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->SetEditText(buf);
	((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->SetEditText(buf);
	((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->SetEditText(buf);
	((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->SetEditText(buf);
	return 0 ;
}

void FastConfigPointDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case FASTCFG_BTN_ADDPOINT_OK:
			{
				AddPoint();
			}
			break;
		}
	}

}
void FastConfigPointDlg::SetItemInfo(CItem * pItem)
{
	m_wItemIdx=pItem->GetItemIdx();
	m_dwItemDBIdx=pItem->GetDBIdx();
	m_dwItemPos = pItem->GetPosition();
}
void FastConfigPointDlg::ClearData()
{
	m_wItemIdx=0;
	m_dwItemDBIdx=0;
	m_dwItemPos = 0;
}
void FastConfigPointDlg::AddPoint()
{
	//[快速加点][By:十里坡剑神][QQ:112582793][2017/11/22]

	WORD Residue=0,Power=0,Dex=0,Con=0,Xm=0;
	Residue = HERO->GetHeroLevelUpPoint();
	Power	= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->GetEditText());
	Dex		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->GetEditText());
	Con		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->GetEditText());
	Xm		= atoi(((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->GetEditText());
	if((Power+Dex+Con+Xm)>Residue)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2199));
		return;
	}
	if((Power+Dex+Con+Xm)<=0) return;
	else
	{
		MSG_ADDPOINT msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEM_ADDPOINT_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.GenGol = Power;
		msg.MinChub = Dex;
		msg.CheRyuk = Con;
		msg.SimMek = Xm;
		msg.dwItemDBidx = m_dwItemDBIdx;
		msg.dwItemPos = m_dwItemPos;
		msg.wItemIdx = m_wItemIdx;
		Residue -= (Power+Dex+Con+Xm);
		char buf[10];
		memset(buf,0,10);
		itoa(Residue,buf,10);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_LEVLEPOINT))->SetEditText(buf);
		itoa(0,buf,10);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_POWER))->SetEditText(buf);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_MIN))->SetEditText(buf);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_TIZHI))->SetEditText(buf);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_SIMEK))->SetEditText(buf);
		msg.LevelUpPoint = Residue;
		NETWORK->Send(&msg, sizeof(msg));
		GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_CHARACTERDLGICON,FALSE);//[关闭属图标闪烁][By:十里坡剑神][QQ:112582793][2018/4/21]
	}
}
void FastConfigPointDlg::SetActive( BOOL val )
{
	if(val)
	{
		char buf[10];

		memset(buf,0,10);
		if(HERO)
		{
			itoa(HERO->GetHeroLevelUpPoint(),buf,10);	
		}
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_LEVLEPOINT))->SetEditText(buf);
		((cEditBox*)GetWindowForID(FASTCFG_EDIT_LEVLEPOINT))->SetReadOnly(TRUE);
	}
	cDialog::SetActive(val);
}