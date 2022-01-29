










#include "stdafx.h"
#include ".\abilityuse_pet.h"
#include "AbilityManager.h"


#ifdef _MHCLIENT_
#include "PetManager.h"

#endif



CAbilityUse_Pet::CAbilityUse_Pet(void)
{
}

CAbilityUse_Pet::~CAbilityUse_Pet(void)
{
}

void CAbilityUse_Pet::Use(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_


	ySWITCH(pAbilityInfo->GetInfo()->Ability_effect_Param1)

		yCASE(eAUKPET_State)

			PETMGR->OpenPetStateDlg();
		yCASE(eAUKPET_Inven)

			PETMGR->OpenPetInvenDlg();
		yCASE(eAUKPET_Skill)

			PETMGR->CheckRestNSkillUse();
		yCASE(eAUKPET_Rest)

			PETMGR->SendPetRestMsg(TRUE);
		yCASE(eAUKPET_Seal)
			PETMGR->SendPetSealMsg();
			yDEFAULT
		yENDSWITCH
#endif


}







