#include "stdafx.h"
#include "OtherCharacterEquip.h"
#include "CharacterDialog.h"
#include "ObjectManager.h"
#include "./Interface/CWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cButton.h"
#include "WindowIDEnum.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "MunpaMarkDialog.h"
#include "SkillManager_Client.h"
#include "CheatMsgParser.h"
#include "StatsCalcManager.h"

#include "GameIn.h"
#include "MainBarDialog.h"
#include "ChatManager.h"
#include "itemManager.h"
#include "DealItem.h"
#include "./interface/cIconGridDialog.h"
#include "GoldShopSubDialog.h"



void CQuipInfoDlg::Init( LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID/*=0*/ )
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
}
void CQuipInfoDlg::SetAttDef(MSG_QUERY_EQUIPINF* msg)
{
	char buf[16];
	memset(buf,0,16);
	sprintf(buf,"%d%%",(WORD)(msg->Att_Fire*100));
	m_pAttDef[0]->SetStaticText(buf);

	sprintf(buf,"%d%%",(WORD)(msg->Att_Earth*100));
	m_pAttDef[1]->SetStaticText(buf);

	sprintf(buf,"%d%%",(WORD)(msg->Att_Iron*100));
	m_pAttDef[2]->SetStaticText(buf);

	sprintf(buf,"%d%%",(WORD)(msg->Att_Water*100));
	m_pAttDef[3]->SetStaticText(buf);

	sprintf(buf,"%d%%",(WORD)(msg->Att_Tree*100));
	m_pAttDef[4]->SetStaticText(buf);
	
}
void CQuipInfoDlg::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_GOLDSHOPSUB_DLG||
		window->GetType() == WT_DIALOG)
	{
		AddTabSheet(curIdx2++, window);
	}
	else 
		cDialog::Add(window);
}
void CQuipInfoDlg::SendGetInfoMsg()
{
	if(gCurTime-m_dwTime>10000)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_QUERY_EQUI_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.dwData = OBJECTMGR->GetSelectedObject()->GetID();
		NETWORK->Send(&msg, sizeof(msg));
		m_dwTime = gCurTime;
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2322),10-(gCurTime-m_dwTime)/1000);
	}
}
void CQuipInfoDlg::ClearShopItem()
{
	cIcon* pIcon;
	for(int i = 0;i<3;i++)
	{
		for(int j = 0; j<35;j++)
		{
			cIconGridDialog * dlg = (cIconGridDialog *)SubDlg->GetTabSheet(i);
			pIcon = dlg->GetIconForIdx(j);
			if(pIcon != NULL)
			{
				ITEMMGR->ReleaseGenerateIndex(pIcon->GetID());
				dlg->DeleteIcon(pIcon);
				pIcon->SetActive( FALSE );
				WINDOWMGR->AddListDestroyWindow( pIcon );
				pIcon = NULL;
			}
		}
	}
}
void CQuipInfoDlg::AddShopItem(MSG_QUERY_EQUIPINF * msg)
{
	cImage tmpImage;
	WORD wCount = 0;
	WORD wGridCount=0;
	for(int i = 0;i<3;++i)
	{
		for(int j = 0;j<35;++j)
		{			
			if(wCount>=msg->ItemCount) return;
			ITEM_INFO * pInfo= ITEMMGR->GetItemInfo(msg->Item[wCount].ItemBase.wIconIdx);
			if(pInfo)
			{
				CDealItem* pItem = NULL;
				cImage tmpImage;
				pItem = new CDealItem;
				pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(msg->Item[wCount].ItemBase.wIconIdx, &tmpImage),ITEMMGR->GetItemGenerateIndex());
				pItem->SetData(msg->Item[wCount].ItemBase.wIconIdx);
				pItem->SetItemIdx(msg->Item[wCount].ItemBase.wIconIdx);
				pItem->m_wIdx=msg->Item[wCount].ItemBase.wIconIdx;
				ITEMMGR->SetGoldShopToolTipIcon(pItem);
				WINDOWMGR->AddWindow(pItem);
				cIconGridDialog * dlg = (cIconGridDialog *)SubDlg->GetTabSheet(i);
				if (j>=wGridCount)
					dlg->AddIcon(j-wGridCount,pItem);
				dlg->SetDisable(TRUE);
			}
			else
			{
				wGridCount++;
			}
			wCount++;
		}
		wGridCount = 0;
	}
}
void CQuipInfoDlg::SetActive( BOOL val )
{
	if( m_bDisable ) return;
	if(val)
	{
		CObject* pLastSelectedObject = OBJECTMGR->GetSelectedObject();
		if(pLastSelectedObject && pLastSelectedObject->GetObjectKind() == eObjectKind_Player) 
		{
			CPlayer* pPlayer = (CPlayer*)pLastSelectedObject;
			BASEOBJECT_INFO ObjInfo;
			CHARACTER_TOTALINFO ChaInfo;
			pPlayer->GetBaseObjectInfo(&ObjInfo);
			pPlayer->GetCharacterTotalInfo(&ChaInfo);


			SetLevel(pPlayer->GetLevel());
			SetLife(pPlayer);
			SetShield(pPlayer->GetShield());
			SetNaeRyuk(pPlayer->GetNaeRyuk());
			SetGenGol(ChaInfo.genGoal);
			SetMinChub(ChaInfo.minchub);
			SetCheRyuk(ChaInfo.cheryuk);
			SetSimMek(ChaInfo.simmak);
			SetCritical(ChaInfo.critical);
			SetReset(ChaInfo.ReStage);
			//[攻击力][By:十里坡剑神][QQ:112582793][2018/3/28]
			char buf1[66];
			DWORD minv,maxv;
			minv = ChaInfo.minAttack;
			maxv = ChaInfo.maxAttack;
			wsprintf(buf1, "%d ~ %d", minv, maxv);
			m_ppStatic.meleeattack->SetStaticText(buf1);
			//[防御力][By:十里坡剑神][QQ:112582793][2018/3/28]
			WORD value = (WORD)ChaInfo.defenseRate;
			m_ppStatic.defense->SetStaticValue(value);
			m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
			//[角色名][By:十里坡剑神][QQ:112582793][2018/3/28]
			wsprintf(buf1,"[%s]角色信息",ObjInfo.ObjectName);
			m_ppStatic.name->SetStaticText(buf1);
			wsprintf(buf1,"[%s]百宝信息",ObjInfo.ObjectName);
			CharShopInfo->SetStaticText(buf1);
		}
	}
	else
		ClearShopItem();
	
	cDialog::SetActiveRecursive(val);
}

void CQuipInfoDlg::Linking()
{
	
	m_pHeroWearDlg = (CWearedExDialog*)((cDialog*)GetTabSheet(0))->GetWindowForID(TQ_WEAREDDLG);
	
	m_ppStatic.name = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARNAME);
	m_ppStatic.genGoal = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARGENGOAL);
	m_ppStatic.simmak = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARSIMMAK);
	m_ppStatic.minchub = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARDEX);
	m_ppStatic.cheryuk = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARSTA);
	m_ppStatic.level = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARLEVEL );

	m_ppStatic.meleeattack = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARATTACK);
	m_ppStatic.critical = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCRITICAL);

	m_ppStatic.defense = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARDEFENSE);
	m_ppStatic.life = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARLIFE);
	m_ppStatic.Shield = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETHOSINDEFENSE);
	m_ppStatic.naeryuk = (cStatic *)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARNAERYUK);
	m_ppStatic.reset = (cStatic*)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGETCHARRESET);
	SubDlg = (CGoldShopSubDialog *)((cDialog*)GetTabSheet(1));
	CharShopInfo = (cStatic *)SubDlg->GetWindowForID(CI_TARGETSHOPITEMINFO);
	for(int i=0;i<5;i++)
		m_pAttDef[i] = (cStatic*)((cDialog*)GetTabSheet(0))->GetWindowForID(CI_TARGET_DEFENCE_HWA+i);
}

CQuipInfoDlg::CQuipInfoDlg( void )
{
	m_type = WT_SHOWEQUIPDLG;
	SubDlg = NULL;
}

CQuipInfoDlg::~CQuipInfoDlg( void )
{

}
BOOL CQuipInfoDlg::AddItem(CItem* pItem)
{
	if (!pItem)
		return FALSE;

	if(TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		return m_pHeroWearDlg->AddIcon(pItem->GetPosition()-TP_WEAR_START, pItem);
	}
	return FALSE;
}

BOOL CQuipInfoDlg::DeleteItem( CItem** ppItem)
{
	POSTYPE Pos;
	Pos = (*ppItem)->GetPosition();
	if(TP_WEAR_START <= Pos && Pos < TP_WEAR_END)
	{
		if(!m_pHeroWearDlg->IsAddable(Pos - TP_WEAR_START))
		{
			return m_pHeroWearDlg->DeleteIcon(Pos-TP_WEAR_START, (cIcon **)ppItem);
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CQuipInfoDlg::SetLevel(WORD level)
{
	m_ppStatic.level->SetStaticValue(level);
}
void CQuipInfoDlg::SetLife(CPlayer* pPlayer)
{
	char buf1[66];
	wsprintf(buf1, "%d/%d", pPlayer->GetLife(), pPlayer->GetMaxLife());
	m_ppStatic.life->SetStaticText(buf1);
}
void CQuipInfoDlg::SetShield(DWORD Shield)
{
	m_ppStatic.Shield->SetStaticValue(Shield);
}
void CQuipInfoDlg::SetReset(WORD resVal)
{
	m_ppStatic.reset->SetStaticValue(resVal);
}
void CQuipInfoDlg::SetNaeRyuk(DWORD naeryuk)
{
	m_ppStatic.naeryuk->SetStaticValue(naeryuk);
}

void CQuipInfoDlg::SetGenGol(WORD resVal)
{
	m_ppStatic.genGoal->SetStaticValue(resVal);
	m_ppStatic.genGoal->SetFGColor(RGB_HALF(221, 217, 158));
}
void CQuipInfoDlg::SetMinChub(WORD resVal)
{
	m_ppStatic.minchub->SetStaticValue(resVal);
	m_ppStatic.minchub->SetFGColor(RGB_HALF(221, 217, 158));
}
void CQuipInfoDlg::SetCheRyuk(WORD resVal)
{
	m_ppStatic.cheryuk->SetStaticValue(resVal);
	m_ppStatic.cheryuk->SetFGColor(RGB_HALF(221, 217, 158));
}
void CQuipInfoDlg::SetSimMek(WORD resVal)
{

	m_ppStatic.simmak->SetStaticValue(resVal);


	m_ppStatic.simmak->SetFGColor(RGB_HALF(221, 217, 158));
}

void CQuipInfoDlg::SetAttackRate()
{

}
void CQuipInfoDlg::SetDefenseRate(WORD value)
{
	m_ppStatic.defense->SetStaticValue(value);
	m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
}

void CQuipInfoDlg::SetCritical(DWORD resVal1)
{
	char buf1[66];
	wsprintf(buf1, "%d", resVal1);
	m_ppStatic.critical->SetStaticText(buf1);

	m_ppStatic.critical->SetFGColor(RGB_HALF(221, 217, 158));
}
void CQuipInfoDlg::SetAttackRange(WORD val)
{
	m_ppStatic.attackdistance->SetStaticValue((LONG)(val));
}