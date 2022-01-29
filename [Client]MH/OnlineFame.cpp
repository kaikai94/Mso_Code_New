#include "stdafx.h"
#include "OnlineFame.h"
#include "./interface/cWindowManager.h"
#include "./interface/cButton.h"
#include "./interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "../[CC]Header/CommonDefine.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "BillBoardManager.h"
OnlineFame::OnlineFame(void)
{

}
void OnlineFame::Linking()
{
	//cImage imgToolTip;
	//SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	//((cButton*)this->GetWindowForID(BAR_PH_BTN_OPEN))->SetToolTip(CHATMGR->GetChatMsg(1795),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	//((cButton*)this->GetWindowForID(BAR_PH_GUILD_OPEN))->SetToolTip(CHATMGR->GetChatMsg(1796),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	//((cButton*)this->GetWindowForID(SINGED_MAINBTN))->SetToolTip(CHATMGR->GetChatMsg(1369),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	//((cButton*)this->GetWindowForID(RECHARGE_BTN_DLG))->SetToolTip(CHATMGR->GetChatMsg(2290),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	//((cButton*)this->GetWindowForID(BAR_PH_KILL_OPEN))->SetToolTip(CHATMGR->GetChatMsg(2279),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);	
}
OnlineFame::~OnlineFame(void)
{
}
