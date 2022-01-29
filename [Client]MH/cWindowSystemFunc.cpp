#include "stdafx.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "interface/cWindowManager.h"
#include "globaleventfunc.h"
#include "GameIn.h"
#include "CharMake.h"
#include "./Audio/MHAudioManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "PyogukDialog.h"
#include "QuickDialog.h"
#include "MugongDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetStall.h"
#include "MonsterGuageDlg.h"
#include "DebugDlg.h"
#include "ChannelDialog.h"
#include "QuickManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "MugongBase.h"
#include "ExchangeItem.h"
#include "cMsgBox.h"
#include "cImeEx.h"
#include "CharMakeDialog.h"
#include "MainTitle.h"
#include "ServerListDialog.h"
#include "MenuSlotDialog.h"
#include "PKManager.h"
//#ifdef TAIWAN_LOCAL
//#include "ChinaAdviceDlg.h"
//#endif
//#ifdef _KOR_LOCAL_
#include "OptionManager.h"
//#endif
#include "RegID.h"			//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "RestPwd.h"		//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
//#include "MaintainaceNoteDlg.h"//[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
void cbDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY)
{
	cDialog * srcDlg = WINDOWMGR->GetWindowForID(id);
	if(!srcDlg) return;
	ASSERT(srcDlg);

	cDialog * ptDlg = WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX,beforeDragY);
	cDialog * ptardlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
	if(ptDlg && !ptardlg)
	{
		if( ptDlg->GetType() == WT_ITEMSHOPDIALOG )
		{
			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
			if(!srcDlg->GetParent())
				return;
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
			WINDOWMGR->SetMouseInputProcessed();
			return;
		}
	}

	if(srcDlg->GetType() & WT_ICON) 
	{
		WORD wtype = srcDlg->GetType();
		if( !(wtype == WT_ITEM ||
			wtype == WT_MUGONG ||
			wtype == WT_JINBUB ||
			wtype == WT_QUICKITEM ||
			wtype == WT_EXCHANGEITEM ||
			wtype == WT_STALLITEM ||
			wtype == WT_DEALITEM ||
			wtype == WT_ABILITYICON) )
		{
			ASSERT(0);
		}
		cIcon* pIcon = (cIcon*)srcDlg;
		cDialog * targetdlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
		if(targetdlg)
		{
			if( pIcon->CanMoveToDialog(targetdlg) == FALSE )
			{
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
			}
			else
			{				
				if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				{
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
					
				}
				switch(targetdlg->GetType())

				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}				
			}
		}
		else
		{
			if( pIcon->CanDelete() )
			{
				if(GAMEIN->GetGameInInitKind() != eGameInInitKind_SuryunEnter)
					SendDeleteIconMsg((cIcon*)srcDlg);
			}
			else

				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));

			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	
		}
		ASSERT(srcDlg->GetParent());
		if(srcDlg->GetParent())
		{
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
		}
		else if( srcDlg->GetType() == WT_QUICKITEM )
		{
			QUICKMGR->ReleaseQuickItem( (CQuickItem*)srcDlg );
		}

		WINDOWMGR->SetMouseInputProcessed();
	}

}

void SendDeleteIconMsg(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_ITEM)
	{
		if( PKMGR->IsPKLooted() ) return;
        if( ITEMMGR->FakeDeleteItem((CItem *)pIcon) == FALSE )
			return;

		if( (((CItem*)pIcon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_LUCK))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2124) );

			return;
		}
		if (ITEMMGR->GetItemRareOption(((CItem *)pIcon)->GetRareness()) || ITEMMGR->IsOptionValuablesItem(((CItem *)pIcon)->GetDurability()) ||
			ITEMMGR->IsValuablesItem(((CItem *)pIcon)->GetItemIdx()))
		{//[物品删除消息提示][BY:十里坡剑神][QQ:112582793][2019-8-28][12:52:26]
			WINDOWMGR->MsgBoxTip(MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(2408), ((CItem *)pIcon)->GetItemInfo()->ItemName);
		}
		else
			WINDOWMGR->MsgBox( MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(363), ((CItem *)pIcon)->GetItemInfo()->ItemName );
		GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		GAMEIN->GetPyogukDialog()->SetDisable( TRUE );
	}
	else if(pIcon->GetType() == WT_QUICKITEM)
	{
		GAMEIN->GetQuickDialog()->FakeDeleteQuickItem( (CQuickItem *)pIcon );
		AUDIOMGR->Play(67, HERO);
	}

	else if(pIcon->GetType() == WT_MUGONG || pIcon->GetType() == WT_JINBUB )
	{
		GAMEIN->GetMugongDialog()->FakeDeleteMugong(((CMugongBase*)pIcon));
		WINDOWMGR->MsgBox(MBI_MUGONGDELETE, MBT_YESNO, CHATMGR->GetChatMsg(190));
		GAMEIN->GetMugongDialog()->SetDisable( TRUE );
	}
	else if( pIcon->GetType() == WT_EXCHANGEITEM )
	{
		GAMEIN->GetExchangeDialog()->FakeDeleteItem(((CExchangeItem*)pIcon)->GetPosition());
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_STALLITEM )
	{
		GAMEIN->GetStreetStallDialog()->FakeDeleteItem( ((CExchangeItem*)pIcon)->GetPosition() );
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_MENUICON )
	{
		GAMEIN->GetMenuSlotDialog()->FakeDeleteIcon( (CMenuIcon*)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
}

void CreateMainTitle_m()
{
	WINDOWMGR->LoadUserIDInfo();
	BOOL bVal = WINDOWMGR->GetUserCfg()->IsKeep;
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/IDDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos1.y =window->GetAbsY()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);

	cEditBox* pEdit;
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_IDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitID);
	//[账号密码记录读取][By:十里坡剑神][QQ:112582793][2017/11/23]
	((cCheckBox*)((cDialog*)window)->GetWindowForID( MT_KEEPID ))->SetChecked(bVal);
	if(bVal)
		pEdit->SetEditText(WINDOWMGR->GetUserCfg()->ID);
	pEdit->SetValidCheck( VCM_DEFAULT );


	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_PWDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitPWD);
	pEdit->SetValidCheck( VCM_PASSWORD );	
	//[账号密码记录读取][By:十里坡剑神][QQ:112582793][2017/11/23]
	if(bVal)
		pEdit->SetEditText(WINDOWMGR->GetUserCfg()->Pwd);

	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/copyright.bin", "rb");
	WINDOWMGR->AddWindow(window1);
	
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/ServerListDlg.bin", "rb" );

	VECTOR2 Pos2;
	Pos2.x =window3->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos2.y =window3->GetAbsY()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	window3->SetAbsXY(Pos2.x,Pos2.y);
	WINDOWMGR->AddWindow( window3 );

//#ifdef TAIWAN_LOCAL
//	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.bin", "rb" );
//	WINDOWMGR->AddWindow( window4 );
//#elif defined _HK_LOCAL_
//	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.bin", "rb" );
//	WINDOWMGR->AddWindow( window4 );
//#endif

	cWindow* window5 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/IntroReplay.bin", "rb" );

	WINDOWMGR->AddWindow( window5 );
	//[注册账号对话框创建][By:十里坡剑神][QQ:112582793][2017/11/23]
	cWindow* window6 = NULL;
	window6 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/RegDlg.bin", "rb");   //注册账号窗口创建  by:十里坡剑神  QQ:112582793
	VECTOR2 Pos3;
	Pos3.x =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-(window6->GetWidth()/2);
	Pos3.y =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-(window6->GetHeight()/2); 
	window6->SetAbsXY(Pos3.x,Pos3.y);
	WINDOWMGR->AddWindow(window6);

	RegID* pDlg = (RegID*)window6;
	pDlg->Linking();
	GAMEIN->SetRegID( pDlg );
	//[修改密码对话框创建][By:十里坡剑神][QQ:112582793][2017/11/23]
	cWindow* window7 = NULL;

	window7 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/RestPwdDlg.bin", "rb");   //注册账号窗口创建  by:十里坡剑神  QQ:112582793
	VECTOR2 Pos5;
	Pos5.x =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-(window7->GetWidth()/2);
	Pos5.y =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-(window7->GetHeight()/2); 
	window7->SetAbsXY(Pos5.x,Pos5.y);
	WINDOWMGR->AddWindow(window7);

	RestPwd* pDlg1 = (RestPwd*)window7;
	pDlg1->Linking();
	GAMEIN->SetRestPwdDlg( pDlg1 );
	////[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
	//cWindow* window8 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/MaintainaceNoteDlg.bin", "rb");
	//VECTOR2 Pos6;
	//Pos6.x =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-(window8->GetWidth()/2);
	//Pos6.y =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-(window8->GetHeight()/2);  
	//window8->SetAbsXY(Pos6.x,Pos6.y);
	//WINDOWMGR->AddWindow(window8);
	//MaintainaceNoteDlg * pDlg2 = (MaintainaceNoteDlg*)window8;
	//pDlg2->Linking();
	//GAMEIN->SetMaintainaceNoteDlg(pDlg2);
}


void NewCreateCharSelect_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharSelectDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W);
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);
}
void CreateCharMakeDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharMakeNewDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)+5;
	Pos1.y =window->GetAbsY();   
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);

	CHARMAKE->SetCharMakeDialog((cCharMakeDlg*)window);
	CHARMAKE->GetCharMakeDialog()->Linking();

	cEditBox* pEdit = (cEditBox*)CHARMAKE->GetCharMakeDialog()->GetWindowForID( CMID_IDEDITBOX );
	pEdit->SetValidCheck( VCM_CHARNAME );
}

cWindow * CreateGameLoading_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/NewLoadDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-2; 
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);

	return window;
}

void CreateInitLogo_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/InitDlg.bin", "rb");

	cImageScale scale;
	scale.x =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/MIDRSLTN_W;
	scale.y =(float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/MIDRSLTN_H; 
	window->SetScale(&scale);
	WINDOWMGR->AddWindow(window);
}




void CreateMoneyDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/moneyDlg.bin", "rb");

	WINDOWMGR->AddWindow(window);
}

void CreateMonsterGuageDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/51.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);
	GAMEIN->SetMonsterGuageDlg((CMonsterGuageDlg * )window);
	GAMEIN->GetMonsterGuageDlg()->Linking();
}

void CreateChannelDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/Channel.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos1.y =window->GetAbsY()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);

	CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
	if(pChannelDlg)
	{
		pChannelDlg->Linking();
	}
}



