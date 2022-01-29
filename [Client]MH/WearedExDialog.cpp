








// WearedExDialog.cpp: implementation of the CWearedExDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WearedExDialog.h"

#include "ObjectManager.h"
#include "AppearanceManager.h"

#include "ItemManager.h"
#include "StatsCalcManager.h"
#include "GameIn.h"
#include "MugongManager.h"


#include "CharacterDialog.h"
#include "QuickDialog.h"
#include "TitanManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWearedExDialog::CWearedExDialog()

{

	m_type = WT_WEAREDDIALOG;

	m_nIconType = WT_ITEM;
}

CWearedExDialog::~CWearedExDialog()
{

}
BOOL CWearedExDialog::AddItem(WORD relPos, cIcon * InIcon)
{
	CItem * item = (CItem * )InIcon;
	CHero* pHero = OBJECTMGR->GetHero();
	if( AddIcon(relPos, InIcon) )
	{
		if( item->GetItemKind() & eTITAN_EQUIPITEM )

		{
			TITAN_APPEARANCEINFO* pAprInfo = pHero->GetTitanAppearInfo();
			// magi82 - Titan(070912) 타이탄 무공업데이트
			pAprInfo->WearedItemIdx[relPos] = item->GetItemIdx();	// 앞에�?이미 배열 첨자�?TP_TITANWEAR_START 값을 빼줌
			pHero->SetBaseMotion();

			APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());

			STATSMGR->CalcTitanStats(TITANMGR->GetRegistedTitanItemDBIdx());

			// magi82 - Titan(070912) 타이탄 무공업데이트
			MUGONGMGR->RefreshMugong();
			GAMEIN->GetQuickDialog()->RefreshIcon();			

		}
		else
		{
			pHero->SetWearedItemIdx(relPos, item->GetItemIdx());
			pHero->SetCurComboNum(SKILL_COMBO_NUM);	// ¹«±â¸¦ ¹Ù²Ù¸é Ã¹¹øÂ° ÄÞº¸ºÎÅÍ ½ÃÀÛÇÏµµ·Ï

			APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());


			STATSMGR->CalcItemStats(HERO);

			GAMEIN->GetCharacterDialog()->SetAttackRate();
			GAMEIN->GetCharacterDialog()->SetDefenseRate();

			GAMEIN->GetCharacterDialog()->SetGenGol();
			GAMEIN->GetCharacterDialog()->SetMinChub();
			GAMEIN->GetCharacterDialog()->SetCheRyuk();
			GAMEIN->GetCharacterDialog()->SetSimMek();
			GAMEIN->GetCharacterDialog()->SetCritical();
			GAMEIN->GetCharacterDialog()->UpdateData();

			//KES 031128 ¹«°ø¾ÆÀÌÅÛ ÀåÂø°¡´É ÅøÆÁ º¯°æ	//°¡Àå ¾Æ·¡¼­ ÇÏ½Ã¿À.
			MUGONGMGR->RefreshMugong();

			GAMEIN->GetQuickDialog()->RefreshIcon();
		}

		return TRUE;
	}
	else
		return FALSE;
}


BOOL CWearedExDialog::DeleteItem(WORD relPos, cIcon ** outIcon)
{
	CHero* pHero = OBJECTMGR->GetHero();

	//KES 031128 ¹«°ø¾ÆÀÌÅÛ ÀåÂø°¡´É ÅøÆÁ º¯°æ
	if( DeleteIcon(relPos, outIcon) )
	{
		CItem* item = (CItem*)*outIcon;
		if(item->GetItemKind() & eTITAN_EQUIPITEM)
		{

			TITAN_APPEARANCEINFO* pAprInfo = pHero->GetTitanAppearInfo();
			// magi82 - Titan(070912) 타이탄 무공업데이트
			pAprInfo->WearedItemIdx[relPos] = 0;	// 앞에�?이미 배열 첨자�?TP_TITANWEAR_START 값을 빼줌
			pHero->SetBaseMotion();

			APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());

			STATSMGR->CalcTitanStats(TITANMGR->GetRegistedTitanItemDBIdx());

			// magi82 - Titan(070912) 타이탄 무공업데이트

			MUGONGMGR->RefreshMugong();
			GAMEIN->GetQuickDialog()->RefreshIcon();
		}

		else
		{
			pHero->SetWearedItemIdx(relPos, 0);
			pHero->SetCurComboNum(SKILL_COMBO_NUM);	// ¹«±â¸¦ ¹Ù²Ù¸é Ã¹¹øÂ° ÄÞº¸ºÎÅÍ ½ÃÀÛÇÏµµ·Ï


			APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());


			STATSMGR->CalcItemStats(HERO);


			GAMEIN->GetCharacterDialog()->SetAttackRate();
			GAMEIN->GetCharacterDialog()->SetDefenseRate();

			GAMEIN->GetCharacterDialog()->SetGenGol();
			GAMEIN->GetCharacterDialog()->SetMinChub();
			GAMEIN->GetCharacterDialog()->SetCheRyuk();
			GAMEIN->GetCharacterDialog()->SetSimMek();
			GAMEIN->GetCharacterDialog()->SetCritical();
			GAMEIN->GetCharacterDialog()->UpdateData();

			MUGONGMGR->RefreshMugong();					//°¡Àå ¹Ø¿¡¼­ È£Ãâ ÇÏ½Ã¿À
			GAMEIN->GetQuickDialog()->RefreshIcon();
		}

		return TRUE;
	}
	else
		return FALSE;
}




