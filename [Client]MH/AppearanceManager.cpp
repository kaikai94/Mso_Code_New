// AppearanceManager.cpp: implementation of the CAppearanceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppearanceManager.h"

#include "Player.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Npc.h"
#include "ItemGround.h"
#include "TacticObject.h"

#include "Titan.h"

#include ".\Engine\EngineObject.h"
#include ".\Engine\EngineEffect.h"
#include "ObjectStateManager.h"

#include "ObjectManager.h"
#include "GameResourceManager.h"
#include "ItemManager.h"
#include "PeaceWarModeManager.h"
#include "MotionManager.h"
#include "CharMakeManager.h"
#include "MainGame.h"
#include "MHMap.h"
#include "Pet.h"
#include "MapObject.h"
#include "StreetStallManager.h"

#include "MHCamera.h"
#include "OptionManager.h"

#include "GameIn.h"
#include "StallFindDlg.h"

#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "battle.h"
#include "BattleSystem_Client.h"
#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"
#endif


enum
{
	eConfirmAddPlayer_NoErr=0,
	eConfirmAddPlayer_NoData,
	eConfirmAddPlayer_DontAddPlayer,
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CAppearanceManager)

CAppearanceManager::CAppearanceManager()
{

}

CAppearanceManager::~CAppearanceManager()
{
	Release();
}

void CAppearanceManager::Init()
{
	GAMERESRCMNGR->LoadModFileList(GAMERESRCMNGR->m_ModFileList);
	GAMERESRCMNGR->LoadHairModFileList(GAMERESRCMNGR->m_HairModList);
	GAMERESRCMNGR->LoadFaceModFileList(GAMERESRCMNGR->m_FaceModList);
	GAMERESRCMNGR->LoadAvatarEquipList();
	GAMERESRCMNGR->LoadTitanModFileList(&GAMERESRCMNGR->m_TitanModFileList);

	m_AvatarItemExceptionTable.Initialize(50);
	LoadAvatarItemExcetionList();

#ifndef _RESOURCE_WORK_NOT_APPLY_
	m_bUseReservation			= FALSE;	
	
	m_pReserItemPlayer = new RESERVATION_ITEM_PLAYER[MAX_RESERVATION_ITEM_PLAYER];

	int i = 0;
	for (i = 0; i < MAX_RESERVATION_ITEM_PLAYER; i++)
	{
		m_lstFreeReserItemPlayer.AddTail(&m_pReserItemPlayer[i]);
	}

	m_dwUsedReserItemPlayerNum	= 0;

#endif	
}

void CAppearanceManager::Release()
{
	AVATARITEM_EXCEPTION* pAvatarItemException = NULL;
	m_AvatarItemExceptionTable.SetPositionHead();
	while(pAvatarItemException = m_AvatarItemExceptionTable.GetData())
	{
		SAFE_DELETE(pAvatarItemException);
	}
	m_AvatarItemExceptionTable.RemoveAll();

#ifndef _RESOURCE_WORK_NOT_APPLY_	
	m_lstFreeReserItemPlayer.RemoveAll();


	if (NULL != m_pReserItemPlayer)
	{
		delete[] m_pReserItemPlayer;
		m_pReserItemPlayer = NULL;
	}
#endif
}

void CAppearanceManager::InitAppearanceToEffect(CObject* pObject,CEngineEffect* pEffect)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		SetCharacterAppearanceToEffect(pPlayer,pEffect);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Monster)
	{
		ASSERT(0);	
	}
	else if(pObject->GetObjectKind() == eObjectKind_Npc)
	{
		ASSERT(0);	
	}
}

BOOL CAppearanceManager::LoadAvatarItemExcetionList()
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	sprintf(filename,"Resource/Client/AvatarItemException.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		AVATARITEM_EXCEPTION* pAvatarItemExcetion = new AVATARITEM_EXCEPTION;
		pAvatarItemExcetion->dwItemIndex = file.GetDword();
		pAvatarItemExcetion->Kind = file.GetDword();
		
		if( NULL != m_AvatarItemExceptionTable.GetData(pAvatarItemExcetion->dwItemIndex) )
		{
			SAFE_DELETE(pAvatarItemExcetion);
			continue;
		}

		m_AvatarItemExceptionTable.Add(pAvatarItemExcetion, pAvatarItemExcetion->dwItemIndex);
	}		
	file.Release();

	return TRUE;
}

CAppearanceManager::AVATARITEM_EXCEPTION* CAppearanceManager::GetAvatarItemExcetion(WORD wItemIndex)
{
	return m_AvatarItemExceptionTable.GetData(wItemIndex);
}

BYTE CAppearanceManager::GetGenderFromMap(CPlayer* pPlayer)
{
	BYTE gender;

	if((MAP->IsMapKind(eTitanMap)) && (MAP->GetMapNum() != 101))	
	{
		if(pPlayer->m_CharacterInfo.Gender == GENDER_MALE)			
			gender = GENDER_FEMALE;		
		else	
			gender = GENDER_MALE;		
	}
	else
	{
		gender = pPlayer->m_CharacterInfo.Gender;
	}
#define CHACRACTER_TALL	170
	pPlayer->SetObjectBalloonTall( (LONG)(CHACRACTER_TALL * pPlayer->GetCharacterTotalInfo()->Height) );	

	ASSERT(gender < 2);
	if( gender >= 2 ) 
		gender = 0;

	return gender;
}

BOOL CAppearanceManager::SetCharacterAppearanceToEngineObject(CPlayer* pPlayer,CEngineObject* pEngineObject)
{
	WORD AvatarItemIdx[eAvatar_Max] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};	
	if( pPlayer->m_CharacterInfo.bNoAvatarView == FALSE)
	{
		memcpy( AvatarItemIdx, pPlayer->GetShopItemStats()->Avatar, sizeof(WORD)*eAvatar_Max );	
	}
	else
	{
		for(int i = eAvatar_Weared_Gum ; i <= eAvatar_Weared_Amgi ; i++)
		{
			AvatarItemIdx[i] = pPlayer->GetShopItemStats()->Avatar[i];
		}		
	}

	APPEARANCEDATA_INFO DataInfo;	
	ZeroMemory(&DataInfo, sizeof(DataInfo));
	DataInfo.gender = GetGenderFromMap(pPlayer);		
	DataInfo.hair = pPlayer->m_CharacterInfo.HairType;
	DataInfo.face = pPlayer->m_CharacterInfo.FaceType;
	DataInfo.WearedItemIdx = pPlayer->m_CharacterInfo.WearedItemIdx;
	DataInfo.pHairList = &GAMERESRCMNGR->m_HairModList[DataInfo.gender];
	DataInfo.pFaceList = &GAMERESRCMNGR->m_FaceModList[DataInfo.gender];
	DataInfo.pModList = &GAMERESRCMNGR->m_ModFileList[DataInfo.gender];
	DataInfo.AvatarItemIdx = AvatarItemIdx;

	ASSERT(DataInfo.pHairList->MaxModFile > DataInfo.hair);
	if( DataInfo.pHairList->MaxModFile <= DataInfo.hair )
		DataInfo.hair = 0;

	pEngineObject->Init(DataInfo.pModList->BaseObjectFile,pPlayer,eEngineObjectType_Character);	
	pEngineObject->ChangePart(eAppearPart_Face,"NULL.MOD");	

	SetCharacterAvatarAppearance(pPlayer, pEngineObject, &DataInfo);
	SetCharacterSkinAppearance(pPlayer, pEngineObject, &DataInfo);	
	SetCharacterWearedAppearance(pPlayer, pEngineObject, &DataInfo);
	SetCharacterEventAppearance(pPlayer, pEngineObject, &DataInfo);		

	if(pPlayer->GetState() != eObjectState_Ungijosik && pPlayer->GetState() != eObjectState_StreetStall_Owner)

		ShowWeapon(pPlayer,pEngineObject);

#define CHACRACTER_TALL	170
	pPlayer->SetObjectBalloonTall( (LONG)(CHACRACTER_TALL * pPlayer->GetCharacterTotalInfo()->Height) );	

	VECTOR3 scale;	
	
	if( pPlayer->m_CharacterInfo.Width > 1.1 ) pPlayer->m_CharacterInfo.Width = 1.1f;
	else if( pPlayer->m_CharacterInfo.Width < 0.9 ) pPlayer->m_CharacterInfo.Width = 0.9f;

	if( pPlayer->m_CharacterInfo.Height > 1.1 ) pPlayer->m_CharacterInfo.Height = 1.1f;
	else if( pPlayer->m_CharacterInfo.Height < 0.9 ) pPlayer->m_CharacterInfo.Height = 0.9f;
		
	SetVector3(&scale,
		pPlayer->m_CharacterInfo.Width,
		pPlayer->m_CharacterInfo.Height,
		pPlayer->m_CharacterInfo.Width);

	pEngineObject->SetScale(&scale);
		
	pEngineObject->ApplyHeightField(TRUE);
	pPlayer->SetPosition(&pPlayer->m_MoveInfo.CurPosition);
	pPlayer->SetAngle(pPlayer->m_RotateInfo.Angle.ToRad());
	
	if(pPlayer->GetID() == HEROID)
	{
		pPlayer->GetEngineObject()->DisablePick();
	}

	ITEMMGR->SetItemEffectProcess(pPlayer);

	return TRUE;
}

void CAppearanceManager::SetCharacterEventAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo)
{
	BOOL rt;

	if( pPlayer->GetFullMoonEventIndex() != eFULLMOONEVENT_NONE )
	{
		if(pDataInfo->gender == 0)
			pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_M.MOD");
		else
			pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_W.MOD");

		CEngineObject* pPart = new CEngineObject;
		if( pPlayer->GetFullMoonEventIndex() == eFULLMOONEVENT_BUN )
		{
			if( pPlayer->GetGender() == GENDER_MALE )
				pPart->Init("M_mandoo.MOD", NULL, eEngineObjectType_Weapon);
			else
				pPart->Init("W_mandoo.MOD", NULL, eEngineObjectType_Weapon);
		}
		else if( pPlayer->GetFullMoonEventIndex() == eFULLMOONEVENT_CHINESECAKE )
		{
			if( pPlayer->GetGender() == GENDER_MALE )
				pPart->Init("M_wb.MOD", NULL, eEngineObjectType_Weapon);
			else
				pPart->Init("W_wb.MOD", NULL, eEngineObjectType_Weapon);
		}
		pEngineObject->AttachDress(pPart,"Bip01 Head");
	}	
	else if( pPlayer->GetFullMoonEventIndex() == eFULLMOONEVENT_NONE )
	{
		WORD wItemIndex = 0;
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pDataInfo->WearedItemIdx[eWearedItem_Hat]);
		if(pInfo != NULL)
			wItemIndex = pInfo->ItemIdx;

		if( (pDataInfo->AvatarItemIdx[eAvatar_Weared_Face]) )	
		{
			AVATARITEM* pAvatarItem = NULL;
			BOOL bFlag = TRUE;

			WORD* wSkinItem = pPlayer->GetShopItemStats()->wSkinItem;						

			pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( wSkinItem[eSkinItem_Mask] );
			if( (pAvatarItem != NULL) && ((pAvatarItem->Position == 3) || (pAvatarItem->Position == 4)) )
			{				
				if( (pAvatarItem->Item[eAvatar_Weared_Face] == 0) && (wSkinItem[eSkinItem_Mask] > 0) )
				{
					if( (pDataInfo->AvatarItemIdx[eAvatar_Weared_Face] != 0) && ((pDataInfo->AvatarItemIdx[eAvatar_Mask] > 0) || (pDataInfo->AvatarItemIdx[eAvatar_Glasses] > 0)) )
						bFlag = TRUE;
					else
						bFlag = FALSE;
				}
				else
				{
					if( (pDataInfo->AvatarItemIdx[eAvatar_Weared_Face] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Mask] > 0) )
						bFlag = FALSE;					
				}			
			}
			else
			{
				AVATARITEM_EXCEPTION* pAvetarItemException = GetAvatarItemExcetion(wItemIndex);
				if( (pAvetarItemException != NULL) )
				{
					if( pAvetarItemException->Kind == 0)
					{
						if( pDataInfo->AvatarItemIdx[eAvatar_Dress] != 0 )
						{
							pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
							if(pAvatarItem != NULL)
							{
								if( pAvatarItem->Item[eAvatar_Hat] == 0 )
									bFlag = TRUE;
								else if( (pDataInfo->AvatarItemIdx[eAvatar_Hat] == 0) && (wSkinItem[eSkinItem_Hat] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Mask] == 0) 
									      && (pDataInfo->AvatarItemIdx[eAvatar_Glasses] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Hair] == 0) )
									bFlag = FALSE;
							}							
						}
						else if( (pDataInfo->AvatarItemIdx[eAvatar_Hat] == 0) && (wSkinItem[eSkinItem_Hat] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Mask] == 0)
							      && (pDataInfo->AvatarItemIdx[eAvatar_Glasses] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Hair] == 0) )
                            bFlag = FALSE;
					}
				}				
			}			
			
			if(bFlag == FALSE)
				return;
			
			CEngineObject* pFace = new CEngineObject;

			ASSERT( pDataInfo->pFaceList->MaxModFile > pDataInfo->face );
			if( pDataInfo->pFaceList->MaxModFile <= pDataInfo->face )		
				pDataInfo->face = 0;
			pFace->Init(pDataInfo->pFaceList->ModFile[pDataInfo->face],NULL,eEngineObjectType_Weapon);
			rt = pEngineObject->AttachDress(pFace,"Bip01 Head");			
		}		
	}
}

void CAppearanceManager::SetCharacterWearedAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo)
{
	int PartType,PartModelNum;	
	BOOL rt;

	WORD* wSkinItem = pPlayer->GetShopItemStats()->wSkinItem;

	for(int n=0;n<eWearedItem_Max;++n)
	{
		if(pDataInfo->WearedItemIdx[n])
		{
			if( n<eWearedItem_Shoes && !pDataInfo->AvatarItemIdx[n+eAvatar_Weared_Hat] && n != eWearedItem_Weapon )
				continue;
			else if( n==eWearedItem_Cape && !pDataInfo->AvatarItemIdx[eAvatar_Weared_Hat] )
				continue;

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pDataInfo->WearedItemIdx[n]);
			if(!pInfo)
				continue;
			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			if(PartType == -1)
				continue;

			if(pDataInfo->gender == 1 && PartType == 0)
			{
				PartType = 6;
			}

			if(PartType == 7)
			{
				if(pDataInfo->gender == 0)
					pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_M.MOD");
				else
					pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_W.MOD");

				PartType = 6;
			}

			if(PartType == 6)	
			{
				CEngineObject* pHairBand = NULL;

				if( pDataInfo->AvatarItemIdx[eAvatar_Dress] > 0 )					
				{
					AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
					if( pAvatarItem )
					{
						if( !pAvatarItem->Item[eAvatar_Hat] )
							continue;
						if( !pAvatarItem->Item[eAvatar_Weared_Hat] )
							continue;
					}
				}

				if( (pDataInfo->AvatarItemIdx[eAvatar_Hair] != 0) || (pDataInfo->AvatarItemIdx[eAvatar_Mask] != 0) 
					|| (pDataInfo->AvatarItemIdx[eAvatar_Glasses] != 0) )
					continue;

				if( (wSkinItem[eSkinItem_Hat] != 0) || (wSkinItem[eSkinItem_Mask] != 0) )
					continue;
				
				if( pPlayer->GetFullMoonEventIndex() != eFULLMOONEVENT_NONE )
					continue;


				pHairBand = new CEngineObject;
				pHairBand->Init(pDataInfo->pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
				rt = pEngineObject->AttachDress(pHairBand,"Bip01 Head");


				BYTE stage = pPlayer->GetStage();
				if( stage == eStage_Normal )
					pHairBand->SetMaterialIndex( 0 );
				else if( stage == eStage_Hwa || stage == eStage_Hyun )
					pHairBand->SetMaterialIndex( 1 );
				else
					pHairBand->SetMaterialIndex( 2 );


			}
			else if(PartType == 3)	
			{
				if(pDataInfo->AvatarItemIdx[eAvatar_Weared_Gwun] > 1 && pInfo->WeaponType > 0)
				{
					ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pDataInfo->AvatarItemIdx[eAvatar_Weared_Gwun]);
					if(!pInfo)
						continue;				

					PartType = pInfo->Part3DType;
					PartModelNum = pInfo->Part3DModelNum;
				}

				pEngineObject->ChangePart(
					PartType
					,pDataInfo->pModList->ModFile[PartModelNum]);
			}
			else if(PartType != 5)
			{
				if( pDataInfo->AvatarItemIdx[eAvatar_Dress] > 0 )					
				{
					AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
					if( pAvatarItem )
					{
						if( pAvatarItem->Item[eAvatar_Hand] == 0 )
							pEngineObject->ChangePart(eAppearPart_Hand, "NULL.MOD");
						if( pAvatarItem->Item[eAvatar_Shoes] == 0 )
							pEngineObject->ChangePart(eAppearPart_Foot, "NULL.MOD");

						if( PartType == 3 )
						{
							WORD weapon = pPlayer->GetWeaponEquipType();
							for(int i=eAvatar_Weared_Gum; i<=eAvatar_Weared_Amgi; ++i)
							{
								if( pAvatarItem->Item[i] == 0 && weapon == i-(eAvatar_Weared_Gum-1) )
								{
									HideWeapon( pPlayer, pEngineObject );
									goto _NotChangePart;
								}
							}
						}
						else if( PartType == 4 )
						{
							if( !pDataInfo->AvatarItemIdx[eAvatar_Shoes] )
								goto _NotChangePart;

							if( wSkinItem[eSkinItem_Shoes] != 0 )
								goto _NotChangePart;							
						}
					}
				}

				if( wSkinItem[eSkinItem_Dress] != 0 )
					goto _NotChangePart;				

				pEngineObject->ChangePart(
					PartType
					,pDataInfo->pModList->ModFile[PartModelNum]);

	_NotChangePart:
				BYTE stage = pPlayer->GetStage();
				if( stage == eStage_Normal )
					pEngineObject->SetMaterialIndex( 0 );
				else if( stage == eStage_Hwa || stage == eStage_Hyun )
					pEngineObject->SetMaterialIndex( 1 );				
				else
					pEngineObject->SetMaterialIndex( 2 );

			}
		}
	}
}

void CAppearanceManager::SetCharacterSkinAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo)
{
	int PartType,PartModelNum;
	BOOL rt;

	WORD* wSkinItem = pPlayer->GetShopItemStats()->wSkinItem;
	
	WORD wItemIndex = wSkinItem[eSkinItem_Hat];

	if( (pDataInfo->AvatarItemIdx[eAvatar_Hat] == 0) && (wItemIndex != 0) )
	{		
		BOOL bFlag = TRUE;
		if(pDataInfo->AvatarItemIdx[eAvatar_Dress] != 0)
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
			if(pAvatarItem != NULL)
			{
				if( pAvatarItem->Item[eAvatar_Hat] == 0 )
					bFlag = FALSE;
			}			
		}		


		if(bFlag == TRUE)
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIndex);
			if(!pInfo)
				return;

			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			CEngineObject* pHairBand = NULL;
			pHairBand = new CEngineObject;
			pHairBand->Init(pDataInfo->pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
			rt = pEngineObject->AttachDress(pHairBand,"Bip01 Head");
		}
	}
	
	wItemIndex = wSkinItem[eSkinItem_Mask];

	if( (pDataInfo->AvatarItemIdx[eAvatar_Mask] == 0) && (pDataInfo->AvatarItemIdx[eAvatar_Glasses] == 0) && (wItemIndex != 0) )
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIndex);
		if(!pInfo)
			return;

		PartType = pInfo->Part3DType;
		PartModelNum = pInfo->Part3DModelNum;		

		CEngineObject* pPart = new CEngineObject;
		pPart->Init(pDataInfo->pModList->ModFile[PartModelNum], NULL, eEngineObjectType_Weapon);
		pEngineObject->AttachDress(pPart,"Bip01 Head");
	}

	wItemIndex = wSkinItem[eSkinItem_Dress];

	if( (pDataInfo->AvatarItemIdx[eAvatar_Dress] == 0) && (wItemIndex != 0) )
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIndex);
		if(!pInfo)
			return;

		PartType = pInfo->Part3DType;
		PartModelNum = pInfo->Part3DModelNum;

		pEngineObject->ChangePart(PartType, pDataInfo->pModList->ModFile[PartModelNum]);
	}

	wItemIndex = wSkinItem[eSkinItem_Shoes];

	if( wItemIndex != 0 )
	{
		if(pDataInfo->AvatarItemIdx[eAvatar_Dress] != 0)
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
			if(pAvatarItem == NULL)
				return;

			if(pAvatarItem->Item[eAvatar_Shoes] == 0)
				return;
		}	

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIndex);
		if(!pInfo)
			return;

		PartType = pInfo->Part3DType;
		PartModelNum = pInfo->Part3DModelNum;

		pEngineObject->ChangePart(PartType, pDataInfo->pModList->ModFile[PartModelNum]);
	}
}

void CAppearanceManager::SetCharacterAvatarAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo)
{
	int PartType,PartModelNum;

	WORD* wSkinItem = pPlayer->GetShopItemStats()->wSkinItem;
	AVATARITEM_EXCEPTION* pAvetarItemException = GetAvatarItemExcetion(pDataInfo->WearedItemIdx[eWearedItem_Hat]);	
	if( pAvetarItemException != NULL)
	{
		BOOL bFlag = TRUE;

		if( (pAvetarItemException->Kind == 0) || (pAvetarItemException->Kind == 1) )
			bFlag = FALSE;

		if(pDataInfo->AvatarItemIdx[eAvatar_Dress] != 0)
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[eAvatar_Dress] );
			if(pAvatarItem != NULL)
			{
				if( pAvatarItem->Item[eAvatar_Hat] == 0 )
					bFlag = TRUE;
			}			
		}

		if( (pDataInfo->AvatarItemIdx[eAvatar_Mask] > 0) || (pDataInfo->AvatarItemIdx[eAvatar_Weared_Hair] == 0) 
			|| (pDataInfo->AvatarItemIdx[eAvatar_Hat] > 0) || (pDataInfo->AvatarItemIdx[eAvatar_Glasses] > 0))		
			bFlag = TRUE;

		if(wSkinItem[eSkinItem_Hat] != 0 || wSkinItem[eSkinItem_Mask] != 0)
			bFlag = TRUE;

		if(TRUE == bFlag)
		{
			pEngineObject->ChangePart(eAppearPart_Hair,pDataInfo->pHairList->ModFile[pDataInfo->hair]);
		}
		else
		{
			if(pDataInfo->gender == 0)		
				pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_M.MOD");
			else
				pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_W.MOD");
		}		
	}
	else if( pDataInfo->AvatarItemIdx[eAvatar_Weared_Hair] )
		pEngineObject->ChangePart(eAppearPart_Hair,pDataInfo->pHairList->ModFile[pDataInfo->hair]);

	for(int i=0; i<=eAvatar_Effect; i++)
	{
		if( pDataInfo->AvatarItemIdx[i] )
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pDataInfo->AvatarItemIdx[i]);
			if(!pInfo)
				continue;
			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pDataInfo->AvatarItemIdx[i] );
			if( i>=eAvatar_Hat && i<=eAvatar_Mustache )
			{
				if( i==eAvatar_Hat || i==eAvatar_Hair )
				{
					if( pAvatarItem && pAvatarItem->Item[eAvatar_Weared_Hair]==0 )
					{
						if(pDataInfo->gender == 0)
							pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_M.MOD");
						else
							pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_W.MOD");
					}
					else if( pAvatarItem && pAvatarItem->Item[eAvatar_Weared_Hair] )
						pEngineObject->ChangePart( eAppearPart_Hair, pDataInfo->pHairList->ModFile[pDataInfo->hair] );
				}

				if( i==eAvatar_Hat && !pAvatarItem->Item[eAvatar_Hat] )
					continue;

				if( pPlayer->GetFullMoonEventIndex() != eFULLMOONEVENT_NONE )
				{
					if(pDataInfo->gender == 0)
						pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_M.MOD");
					else
						pEngineObject->ChangePart(eAppearPart_Hair,"NULLHAIR_W.MOD");
				}
				else
				{
					CEngineObject* pPart = new CEngineObject;
					pPart->Init(pDataInfo->pModList->ModFile[PartModelNum], NULL, eEngineObjectType_Weapon);
					pEngineObject->AttachDress(pPart,"Bip01 Head");
				}
			}
			else if( i==eAvatar_Shoulder )
			{//[增加背部模型显示，修复翅膀位置][By:十里坡剑神][QQ:112582793][2018/9/4]
				CEngineObject* pPart = new CEngineObject;
				pPart->Init(pDataInfo->pModList->ModFile[PartModelNum], NULL, eEngineObjectType_Weapon);
				pEngineObject->AttachDress(pPart,"Bip01 Spine2");
			}
			else if(i == eAvatar_Effect)
			{
			}
			else if(i == eAvatar_Back)
			{
			}
			else
				pEngineObject->ChangePart( PartType, pDataInfo->pModList->ModFile[PartModelNum] );

			if(  i == eAvatar_Dress )
			{
				if( pAvatarItem->Item[eAvatar_Hand] == 0 )
					pEngineObject->ChangePart(eAppearPart_Hand, "NULL.MOD");
				if( pAvatarItem->Item[eAvatar_Shoes] == 0 )
					pEngineObject->ChangePart(eAppearPart_Foot, "NULL.MOD");
			}
		}

	}

	if( pDataInfo->AvatarItemIdx[eAvatar_Hat] == 55576 || pDataInfo->AvatarItemIdx[eAvatar_Dress] == 55577 )
	{
		CEngineObject* pPart = new CEngineObject;
		pPart->Init( "gm_mona.MOD", NULL, eEngineObjectType_Effect );
		pEngineObject->AttachDress( pPart, "Bip01 Head" );
	}
}

BOOL CAppearanceManager::SetTitanAppearanceToEngineObject( CPlayer* pPlayer, CEngineObject* pEngineObject )
{
	int PartType, PartModelNum;

	TITAN_APPEARANCEINFO* pAppearInfo = NULL;

	if( pPlayer->IsTitanPreView() )
		pAppearInfo = pPlayer->GetTitanPreViewInfo();
	else
		pAppearInfo = pPlayer->GetTitanAppearInfo();
	BASE_TITAN_LIST* pBaseInfo = GAMERESRCMNGR->GetTitanListInfo( pAppearInfo->TitanKind );
	if( !pBaseInfo )
		return FALSE;

	TITANINFO_GRADE* pGradeInfo = &pBaseInfo->GradeInfo[pAppearInfo->TitanGrade - 1];
	if( !pGradeInfo )
		return FALSE;
	pPlayer->SetObjectBalloonTall(LONG(pGradeInfo->Tall*pGradeInfo->Scale));

	MOD_LIST* pModList = &GAMERESRCMNGR->m_TitanModFileList;
	
	pEngineObject->Init( pBaseInfo->TitanBaseChxName, 
		pPlayer, eEngineObjectType_Titan, eAttachObjectType_TargetObject );
	
	BOOL rt;	
	pEngineObject->ChangePart( eAppearPart_Face, "NULL.MOD" );
	pEngineObject->ChangePart( 5, "NULL.MOD" );

	if( !pAppearInfo->WearedItemIdx[eTitanWearedItem_HelMet] )
	{
		CEngineObject* pFace = new CEngineObject;	
		pFace->Init( pBaseInfo->TitanfaceModName, NULL, eEngineObjectType_Titan);
		rt= pEngineObject->AttachDress( pFace, "Bip01 Head" );
	}
	
	GXOBJECT_HANDLE hGxObject = pEngineObject->GetGXOHandle();
	GXOBJECT_HANDLE hGxAttachObject = NULL;

	for(int i=0; i<eTitanWearedItem_Max; ++i)
	{
		if( pAppearInfo->WearedItemIdx[i] )
		{

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pAppearInfo->WearedItemIdx[i] );
			if(!pInfo)
				continue;
			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			if( PartType == -1 )
				continue;

			if( i == eTitanWearedItem_HelMet )
			{
				pEngineObject->ChangePart( PartType, pModList->ModFile[PartModelNum] );

				char buf[30] = { 0, };
				SafeStrCpy( buf, pModList->ModFile[PartModelNum], strlen(pModList->ModFile[PartModelNum])+1 );
				char* p = strstr( buf, "_h02_");
				int tlen = strlen(buf);
				int clen =strlen(p);
				buf[tlen-clen+3] = '1';

				CEngineObject* pFace = new CEngineObject;	
				pFace->Init( buf, NULL, eEngineObjectType_Titan);
				rt= pEngineObject->AttachDress( pFace, "Bip01 Head" );
			}			
			else if( i == eTitanWearedItem_Cloak )
			{
				pEngineObject->ChangePart( 5, pModList->ModFile[PartModelNum] );
			}
			else if( i == eTitanWearedItem_Shield )
			{
				CEngineObject* pShield = NULL;
				pShield = new CEngineObject;
				pShield->Init( pModList->ModFile[PartModelNum], NULL, eEngineObjectType_Titan );
				rt = pEngineObject->AttachDress( pShield, "Bone_Forearm" );
			}
			else
			{
				if( PartType != 5 )
					pEngineObject->ChangePart( PartType, pModList->ModFile[PartModelNum] );
			}
		}
	}

	if(pPlayer->GetState() != eObjectState_Ungijosik && pPlayer->GetState() != eObjectState_StreetStall_Owner)
		ShowWeapon(pPlayer, pEngineObject);


	VECTOR3 scale = { 1.0f, 1.0f, 1.0f };
	if( pAppearInfo->TitanScale > 95 )		pAppearInfo->TitanScale = 95;
	else if( pAppearInfo->TitanScale < 85 )		pAppearInfo->TitanScale = 85;
	scale = scale * (pAppearInfo->TitanScale*0.01f); 
	pEngineObject->SetScale( &scale );

	pEngineObject->ApplyHeightField( TRUE );
	pPlayer->SetPosition( &pPlayer->m_MoveInfo.CurPosition );
	pPlayer->SetAngle( pPlayer->m_RotateInfo.Angle.ToRad() );

	if( pPlayer->GetID() == HEROID )
		pPlayer->GetEngineObject()->DisablePick();


	ITEMMGR->SetItemEffectProcess(pPlayer);

	return TRUE;
}

void CAppearanceManager::ShowWeapon(CPlayer* pPlayer)
{
	if( pPlayer->InTitan() )
		SetTitanAppearanceToEngineObject( pPlayer, &pPlayer->m_EngineObject );
	else
		SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);

	ShowWeapon(pPlayer,pPlayer->GetEngineObject());
	
	if(pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide))
	{
		OBJECTMGR->SetHide(pPlayer, TRUE);
	}
	else if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
	}

	OBJECTMGR->ApplyShadowOption(pPlayer);

	
	WORD* AvatarItemIdx = pPlayer->GetShopItemStats()->Avatar;
	if( AvatarItemIdx[eAvatar_Dress] > 0 )
	{		
		AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( AvatarItemIdx[eAvatar_Dress] );
		if( pAvatarItem )
		{
			WORD weapon = pPlayer->GetWeaponEquipType();

			for(int i=eAvatar_Weared_Gum; i<=eAvatar_Weared_Amgi; ++i)
			{
				if( pAvatarItem->Item[i] == 0 && weapon == i-(eAvatar_Weared_Gum-1) && AvatarItemIdx[i] < 2 )
				{
					HideWeapon( pPlayer, &pPlayer->m_EngineObject );
					break;
				}				
			}
		}
	}
}
void CAppearanceManager::HideWeapon(CPlayer* pPlayer)
{
	if( pPlayer->InTitan() )
		SetTitanAppearanceToEngineObject( pPlayer, &pPlayer->m_EngineObject );
	else
		SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);
	HideWeapon(pPlayer,pPlayer->GetEngineObject());
	
	if(pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide))
	{
		OBJECTMGR->SetHide(pPlayer, TRUE);
	}
	else if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
	}

	OBJECTMGR->ApplyShadowOption(pPlayer);
}

void CAppearanceManager::SetCharacterAppearance(CPlayer* pPlayer)
{
	SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);

	if(pPlayer->GetState() == eObjectState_None)
		OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);
	if( pPlayer->GetState() == eObjectState_Move &&
		pPlayer->m_MoveInfo.KyungGongIdx == 0)
		pPlayer->SetState(eObjectState_Move);

	if(pPlayer->IsHide())
		OBJECTMGR->SetHide(pPlayer, TRUE);
	
	if(pPlayer->IsDetect())
		OBJECTMGR->SetDetect(pPlayer, TRUE);
	
	if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
	}
	
	OBJECTMGR->ApplyShadowOption(pPlayer);
}

void CAppearanceManager::SetBossMonsterAppearance(CBossMonster* pMonster)
{
	pMonster->m_EngineObject.Release();
	
	WORD mkind = pMonster->GetMonsterKind();
	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(mkind);
	
	pMonster->m_EngineObject.Init(pList->ChxName, pMonster, eEngineObjectType_Monster);
	pMonster->m_EngineObject.ApplyHeightField(TRUE);
	
	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pMonster->m_EngineObject.SetScale(&Scale);
}

void CAppearanceManager::SetMonsterAppearance(CMonster* pMonster)
{
	pMonster->m_EngineObject.Release();
	
	WORD mkind = pMonster->GetMonsterKind();

	if(mkind == 145)
		mkind = 145;

	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(mkind);
	pMonster->m_EngineObject.Init(pList->ChxName,pMonster,eEngineObjectType_Monster);
	pMonster->m_EngineObject.ApplyHeightField(TRUE);

	MOTIONMGR->SetMotionInfo(pMonster, pList->MotionID);

	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pMonster->m_EngineObject.SetScale(&Scale);
}
void CAppearanceManager::SetNpcAppearance(CNpc* pNpc)
{
	pNpc->m_EngineObject.Release();
		
	WORD nkind = pNpc->GetNpcKind();
	NPC_LIST* pList = GAMERESRCMNGR->GetNpcInfo(nkind);
	int ChxNum = pList->ModelNum;
	char* pNpcChxName = GAMERESRCMNGR->GetNpcChxName(ChxNum);
	
	pNpc->m_EngineObject.Init(pNpcChxName,pNpc,eEngineObjectType_Npc);
	pNpc->SetPosition(&pNpc->m_MoveInfo.CurPosition);
	pNpc->m_EngineObject.ApplyHeightField(TRUE);
	pNpc->SetAngle(pNpc->m_RotateInfo.Angle.ToRad());
	
	pNpc->m_EngineObject.ChangeMotion(1);

	pNpc->m_EngineObject.RandMotionFrame();
	
	if( pNpc->GetNpcJob() == 0 )
		pNpc->m_EngineObject.DisablePick();

	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pNpc->m_EngineObject.SetScale(&Scale);
}

void CAppearanceManager::SetPetAppearance(CPet* pPet)
{
	pPet->m_EngineObject.Release();

	WORD mKind = pPet->GetPetKind();
	WORD PetGrade = pPet->GetPetGrade();

	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(mKind);
	
	pPet->m_EngineObject.Init(pList->ChxName_PerGrade[PetGrade-1],pPet,eEngineObjectType_Pet);

	pPet->SetPosition(&pPet->m_MoveInfo.CurPosition);
	pPet->m_EngineObject.ApplyHeightField(TRUE);
	pPet->SetAngle(pPet->m_RotateInfo.Angle.ToRad());

	pPet->m_EngineObject.ChangeMotion(1);

	MOTIONMGR->SetMotionInfo(pPet, pList->MotionID);

	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pPet->m_EngineObject.SetScale(&Scale);
}
void CAppearanceManager::SetTitanAppearance(CPlayer* pPlayer)
{
	SetTitanAppearanceToEngineObject( pPlayer, &pPlayer->m_EngineObject );

	if(pPlayer->GetState() == eObjectState_None)
		OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);

	if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
		pPlayer->GetEngineObject()->HideWithScheduling();
	}

	OBJECTMGR->ApplyShadowOption( pPlayer );
}


void CAppearanceManager::SetMapObjectAppearance(CMapObject* pMapObject, char* strData, float fScale )
{
	pMapObject->m_EngineObject.Release();

	pMapObject->m_EngineObject.Init( strData, pMapObject, eEngineObjectType_MapObject) ;

	pMapObject->SetPosition( &pMapObject->m_MoveInfo.CurPosition );
	pMapObject->m_EngineObject.ApplyHeightField(TRUE);
	pMapObject->SetAngle( pMapObject->m_RotateInfo.Angle.ToRad() );

	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = fScale;
	pMapObject->m_EngineObject.SetScale(&Scale);
}

void CAppearanceManager::SetDecorationAppearance(CMapObject* pMapObject, CPlayer* pPlayer, DWORD dwStreetStallKind)
{
	pMapObject->m_EngineObject.Release();
	DWORD dwItemIdx = pPlayer->GetShopItemStats()->dwStreetStallDecoration;
	if( 0 == dwItemIdx )
		return;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo((WORD)dwItemIdx);
	if( !pInfo )
		return;
	char* strTemp = NULL;
	if(dwStreetStallKind == eSK_SELL)
	{
		strTemp = GAMERESRCMNGR->GetItemChxName(pInfo->Part3DModelNum);
	}
	else
	{
		strTemp = GAMERESRCMNGR->GetItemChxName((DWORD)pInfo->LifeRecoverRate);
	}
		
	pMapObject->m_EngineObject.Init( strTemp, NULL, eEngineObjectType_Npc);
	pMapObject->m_EngineObject.ApplyHeightField(TRUE);
	VECTOR3 pos = pPlayer->GetCurPosition();
	pos.x += 30;
	pos.z += 30;
	pMapObject->SetPosition( &pos );

	pMapObject->SetAngle(pPlayer->GetAngle());

	pMapObject->m_EngineObject.ChangeMotion(1);
}

void CAppearanceManager::SetCharacterAppearanceToEffect(CPlayer* pPlayer,CEngineEffect* pRtEffect)
{
	SetCharacterAppearanceToEngineObject(pPlayer,pRtEffect);
}

#define TITAN_EFFECT_MAINTAIN	3620
void CAppearanceManager::InitAppearance(CObject* pObject)
{
	BYTE objectKind = pObject->GetObjectKind();
	if(objectKind == eObjectKind_Player)
	{
		if( ((CPlayer*)pObject)->InTitan() || ((CPlayer*)pObject)->IsTitanPreView() )
		{
			SetTitanAppearance( (CPlayer*)pObject );
		}
		else
		{
			SetCharacterAppearance( (CPlayer*)pObject );
		}
	}
	else if( (objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster) )
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
	else if(objectKind == eObjectKind_Pet)
	{
		SetPetAppearance((CPet*)pObject);
	}
	else if(objectKind == eObjectKind_Npc)
	{
		SetNpcAppearance((CNpc*)pObject);
	}
	else if(objectKind == eObjectKind_Tactic)
	{
	}
	else if(objectKind == eObjectKind_BossMonster)
	{
		SetBossMonsterAppearance((CBossMonster*)pObject);
	}
	else if(objectKind == eObjectKind_Mining)
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
	else if(objectKind == eObjectKind_Collection)
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
	else if(objectKind == eObjectKind_Hunt)
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
}

void CAppearanceManager::SetScalebyGuageBar(DWORD PlayerID)
{
	CObject* pPlayer;	
	pPlayer = OBJECTMGR->GetObject(PlayerID);
	if(pPlayer == NULL)
	{
		ASSERT(0);
		return;
	}
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

	((CPlayer*)pPlayer)->m_CharacterInfo.Width = CHARMAKEMGR->GetCharacterMakeInfo()->Width;
	((CPlayer*)pPlayer)->m_CharacterInfo.Height = CHARMAKEMGR->GetCharacterMakeInfo()->Height;
}

void CAppearanceManager::AddCharacterPartChange(DWORD PlayerID)
{
	m_PtrList.AddHead((void*)PlayerID);
}

void CAppearanceManager::ProcessAppearance()
{
	DWORD PlayerID= 0;
	CPlayer* pPlayer;
	while( PlayerID = (DWORD)m_PtrList.RemoveTail() )
	{
		pPlayer = (CPlayer*)OBJECTMGR->GetObject( PlayerID );
		if( pPlayer == NULL )
			continue;
		ASSERT( pPlayer->GetObjectKind() == eObjectKind_Player );

		if( pPlayer->InTitan() || pPlayer->IsTitanPreView() )
		{
			pPlayer->SetTitanMoveSpeed();
			SetTitanAppearance( pPlayer );
		}
		else
			SetCharacterAppearance( pPlayer );		
	}
}


void CAppearanceManager::ShowWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject)
{
	WORD WeaponIdx = 0;
	MOD_LIST* pModList = NULL;
	BYTE gender = pPlayer->m_CharacterInfo.Gender;
	ASSERT(gender < 2);
	WORD EquipType = 0;
	int PartType = 0;
	int PartModelNum = 0;
	CEngineObject* pWeapon1 = NULL;
	CEngineObject* pWeapon2 = NULL;

	if( pPlayer->InTitan() || pPlayer->IsTitanPreView() )		
	{
		WeaponIdx = pPlayer->GetTitanWearedItemIdx(eTitanWearedItem_Weapon);
		pModList = &GAMERESRCMNGR->m_TitanModFileList;

		HideWeapon(pPlayer,pEngineObject);

		EquipType = pPlayer->GetTitanWeaponEquipType();

		if( WeaponIdx )
		{
			PartType = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DType;
			PartModelNum = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DModelNum;
		}
		else
			return;

		if( PartType != -1 && PartType != 3 )
		{
			if( EquipType == WP_AMGI )
			{
				pWeapon1 = new CEngineObject;
				pWeapon1->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Titan);
				pEngineObject->AttachWeapon(pWeapon1,"Bip01 R Forearm");
			}
			else if( EquipType == WP_GWUN )
			{
				pWeapon1 = new CEngineObject;
				pWeapon2 = new CEngineObject;

				char temp[256],*pSrc;
				pSrc = pModList->ModFile[PartModelNum];
				int nLen = strlen(pSrc);

				for(int n=0;n<nLen;++n)

				{
					if( pSrc[n] == '.' )
					{
						temp[n] = '_';
						temp[n+1] = 'R';
						strcpy(&temp[n+2],&pSrc[n]);
						break;
					}
					else
						temp[n] = pSrc[n];
				}
				pWeapon1->Init(temp,NULL,eEngineObjectType_Titan);
				BOOL rt = pEngineObject->AttachWeapon(pWeapon1,"Bip01 R Forearm");

				for(int n=0;n<nLen;++n)
				{
					if( pSrc[n] == '.')
					{
						temp[n] = '_';
						temp[n+1] = 'L';
						strcpy(&temp[n+2],&pSrc[n]);
						break;
					}
					else
						temp[n] = pSrc[n];
				}
					
				pWeapon2->Init(temp,NULL,eEngineObjectType_Titan);
				rt = pEngineObject->AttachWeapon(pWeapon2,"Bip01 L Forearm");
			}
			else
			{
				pWeapon1 = new CEngineObject;
				pWeapon1->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Titan);
				pEngineObject->AttachWeapon(pWeapon1,WEAPON_ATTACH_BONENAME_R);
			}
		}
	}

	else
	{
		WeaponIdx = pPlayer->GetWearedItemIdx(eWearedItem_Weapon);
		pModList = &GAMERESRCMNGR->m_ModFileList[gender];


		HideWeapon(pPlayer,pEngineObject);

		EquipType = pPlayer->GetWeaponEquipType();

		if( EquipType < WP_KEY && pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Gum+EquipType-1] > 1)
		{
			ITEM_INFO* pinfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Gum+EquipType-1] );
			if( pinfo )
			{
				PartType = pinfo->Part3DType;
				PartModelNum = pinfo->Part3DModelNum;
			}
			else
			{
				PartType = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DType;
				PartModelNum = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DModelNum;
			}
		}
		else if( WeaponIdx )
		{
			PartType = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DType;
			PartModelNum = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DModelNum;
		}
		else
			return;

		WORD wAvartarCheck = 0;
		switch(EquipType)
		{
		case WP_AMGI:
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Amgi];
			}
			break;
		case WP_GUNG:	
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Gung];
			}
			break;
		case WP_GWUN:	
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Gwun];
			}
			break;
		case WP_DO:	
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Do];
			}
			break;
		case WP_CHANG:	
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Chang];
			}
			break;

		case WP_GUM:	
			{
				wAvartarCheck = pPlayer->GetShopItemStats()->Avatar[eAvatar_Weared_Gum];
			}
			break;
		}

		if( PartType != -1 && PartType != 3 )
		{
			if( EquipType == WP_AMGI )
			{				
				UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(WeaponIdx);
				if(pInfo && wAvartarCheck <= 1)
				{
					CEngineObject* pObject = NULL;
					char buf[MAX_CHXNAME_LENGTH+1] = { 0, };

					if(gender == 0)
						SafeStrCpy(buf, pInfo->strManEffectName, MAX_CHXNAME_LENGTH+1);
					else
						SafeStrCpy(buf, pInfo->strWomanEffectName, MAX_CHXNAME_LENGTH+1);

					if( strcmp(buf, "nofile") != 0 )
					{
						pObject = new CEngineObject;
						pObject->Init( buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni );
						pEngineObject->AttachWeapon(pObject,"Bip01 L Forearm");

						for( int i = 0; i < (int)strlen(buf); i++ )
						{
							if( buf[i] == '.' )
							{
								buf[i-1] = 'R';
								break;
							}
						}

						pObject = new CEngineObject;
						pObject->Init( buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni );
						pEngineObject->AttachWeapon(pObject,"Bip01 R Forearm");
					}
				}
				else
				{
					char temp[256],*pSrc;
					pSrc = pModList->ModFile[PartModelNum];
					int nLen = strlen(pSrc);

					for(int n=0;n<nLen;++n)
					{
						if( pSrc[n] == '.' )
						{
							temp[n] = '_';
							temp[n+1] = 'R';
							strcpy(&temp[n+2],&pSrc[n]);
							break;
						}
						else
							temp[n] = pSrc[n];
					}
					pWeapon1 = new CEngineObject;
					pWeapon1->Init(temp,NULL,eEngineObjectType_Weapon);
					BOOL rt = pEngineObject->AttachWeapon(pWeapon1,"Bip01 R Forearm");
					SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 R Forearm");	

					for(int n=0;n<nLen;++n)
					{
						if( pSrc[n] == '.')
						{
							temp[n] = '_';
							temp[n+1] = 'L';
							strcpy(&temp[n+2],&pSrc[n]);
							break;
						}
						else
							temp[n] = pSrc[n];
					}
					pWeapon2 = new CEngineObject;	
					pWeapon2->Init(temp,NULL,eEngineObjectType_Weapon);
					rt = pEngineObject->AttachWeapon(pWeapon2,"Bip01 L Forearm");
					SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 L Forearm");	
				}
			}
			else if( EquipType == WP_GUNG )
			{				

				UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(WeaponIdx);
				if(pInfo && wAvartarCheck <= 1)
				{
					CEngineObject* pObject = NULL;
					char buf[MAX_CHXNAME_LENGTH+1] = { 0, };

					if(gender == 0)
						SafeStrCpy(buf, pInfo->strManEffectName, MAX_CHXNAME_LENGTH+1);
					else
						SafeStrCpy(buf, pInfo->strWomanEffectName, MAX_CHXNAME_LENGTH+1);

					if( strcmp(buf, "nofile") != 0 )
					{
						pObject = new CEngineObject;
						pObject->Init( buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni );
						pEngineObject->AttachWeapon(pObject,WEAPON_ATTACH_BONENAME_L);
					}
				}
				else
				{
					pWeapon1 = new CEngineObject;
					pWeapon1->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					pEngineObject->AttachWeapon(pWeapon1,WEAPON_ATTACH_BONENAME_L);					
					SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, WEAPON_ATTACH_BONENAME_L);
				}
			}
			else if( EquipType == WP_GWUN )
			{
				pWeapon1 = new CEngineObject;
				pWeapon1->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
				pEngineObject->AttachWeapon(pWeapon1,WEAPON_ATTACH_BONENAME_R);
				SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 R Forearm");
				
				char temp[256],*pSrc;
				pSrc = pModList->ModFile[PartModelNum];
				for(DWORD n=0;n<strlen(pSrc);++n)
				{
					if(pSrc[n] == '.')
					{
						temp[n] = '_';
						temp[n+1] = 'L';
						strcpy(&temp[n+2],&pSrc[n]);
						break;
					}
					else
						temp[n] = pSrc[n];
				}
				pWeapon2 = new CEngineObject;
				pWeapon2->Init(temp,NULL,eEngineObjectType_Weapon);
				pEngineObject->AttachWeapon(pWeapon2,WEAPON_ATTACH_BONENAME_L);
				SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 L Forearm");
			}
			else
			{				
				UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(WeaponIdx);				
				if(pInfo && wAvartarCheck <= 1)
				{
					CEngineObject* pObject = NULL;
					char buf[MAX_CHXNAME_LENGTH+1] = { 0, };


					if(gender == 0)
						SafeStrCpy(buf, pInfo->strManEffectName, MAX_CHXNAME_LENGTH+1);
					else
						SafeStrCpy(buf, pInfo->strWomanEffectName, MAX_CHXNAME_LENGTH+1);

					if( strcmp(buf, "nofile") != 0 )
					{
						pObject = new CEngineObject;
						pObject->Init( buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni );
						pEngineObject->AttachWeapon(pObject,WEAPON_ATTACH_BONENAME_R);
					}
				}
				else
				{
					pWeapon1 = new CEngineObject;
					pWeapon1->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					pEngineObject->AttachWeapon(pWeapon1,WEAPON_ATTACH_BONENAME_R);
					SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, WEAPON_ATTACH_BONENAME_R);	
				}
			}
		}
		else if( PartType == 3 && EquipType == WP_GWUN )
		{			
			UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(WeaponIdx);

			if(pInfo && wAvartarCheck <= 1)
			{
				char buf[MAX_CHXNAME_LENGTH+1] = { 0, };

				if(gender == 0)
					SafeStrCpy(buf, pInfo->strManEffectName, MAX_CHXNAME_LENGTH+1);
				else
					SafeStrCpy(buf, pInfo->strWomanEffectName, MAX_CHXNAME_LENGTH+1);

				if( strcmp(buf, "nofile") != 0 )
				{
					CEngineObject* pObject = new CEngineObject;
					pObject->Init(buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni);
					pEngineObject->AttachWeapon(pObject,"Bip01 L Forearm");

					for( int i = 0; i < (int)strlen(buf); i++ )
					{
						if( buf[i] == '.' )
						{
							buf[i-1] = 'R';
							break;
						}
					}

					CEngineObject* pObject2 = new CEngineObject;
					pObject2->Init(buf, NULL, eEngineObjectType_Weapon, eAttachObjectType_withAni);
					pEngineObject->AttachWeapon(pObject2,"Bip01 R Forearm");
				}				
			}
			else
			{
				SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 R Forearm");	
				SetPlusItemEffect(pPlayer, pEngineObject, WeaponIdx, "Bip01 L Forearm");	
			}
		}
	}	
}

void CAppearanceManager::SetPlusItemEffect(CPlayer* pPlayer, CEngineObject* pEngineObject, DWORD dwItemIdx, char* pObjectName)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(dwItemIdx);
	if(pItemInfo == NULL)
		return;

	for(int i = eAvatar_Weared_Gum ; i <= eAvatar_Weared_Amgi ; i++)
	{
		if(pPlayer->GetShopItemStats()->Avatar[i] > 1)
			return;
	}		

	if(pItemInfo->wPulsItemEffect >= ePLUSITEM_EFFECT_10)
	{
		PLUSITEM_EFFECT_INFO* pPlusItemEffectInfo = GAMERESRCMNGR->GetPlusItemEffectInfo(pPlayer->GetWeaponEquipType());
		if(pPlusItemEffectInfo == NULL)
			return;

		WORD wIndex = pItemInfo->wPulsItemEffect - ePLUSITEM_EFFECT_10;
		if(wIndex < 0)
			return;

		if(!pPlusItemEffectInfo->szCHXName[wIndex])
		{
			return;
		}
		char szCHXName[MAX_CHXNAME_LENGTH+1];
		memset(szCHXName, 0, sizeof(szCHXName));
		if(pPlayer->GetGender() == GENDER_MALE)
			sprintf(szCHXName, "m_%s", pPlusItemEffectInfo->szCHXName[wIndex]);
		else
			sprintf(szCHXName, "w_%s", pPlusItemEffectInfo->szCHXName[wIndex]);

		CEngineObject* pEffectObject = new CEngineObject;		
		pEffectObject->Init(szCHXName, NULL, eEngineObjectType_Weapon);
		pEngineObject->AttachWeapon(pEffectObject, pObjectName);
	}			
}

void CAppearanceManager::HideWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject)
{
	if( !pPlayer->InTitan() )
		pEngineObject->RemoveAllAttachedWeapon();
}

void CAppearanceManager::SetAvatarItem( CPlayer* pPlayer, WORD ItemIdx, BOOL bAdd )
{
	WORD* pAvatarItem = pPlayer->GetShopItemStats()->Avatar;
	AVATARITEM* pAvatarUseInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if( !pAvatarUseInfo )		return;
	
	if( bAdd )
	{
		for(int i=0; i<eAvatar_Max; i++)
		{
			if( i >= eAvatar_Weared_Hair )
			{
				if( !pAvatarUseInfo->Item[i] )
					pAvatarItem[i] = 0;
			}
			else if( i == pAvatarUseInfo->Position )
			{
				if( pAvatarItem[i] )				
				if( pPlayer->GetID() == HERO->GetID() )
				{
					MSG_WORD2 msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_CHANGE;
					msg.dwObjectID = HERO->GetID();
					msg.wData1 = i;					
					msg.wData2 = pAvatarItem[i];					
					NETWORK->Send( &msg, sizeof(msg) );
				}

				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatarItem[i] );
				if( !pTemp )		continue;
				for(int k=0; k<eAvatar_Max; k++)
				{
					if( k >= eAvatar_Weared_Hair )
					if( !pTemp->Item[k] )
						pAvatarItem[k] = 1;
				}

				pAvatarItem[i] = ItemIdx;
			}
			else if( !pAvatarUseInfo->Item[i] && pAvatarItem[i] )
			{				
				if( pPlayer->GetID() == HERO->GetID() )
				{
					MSG_WORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
					msg.dwObjectID = HERO->GetID();
					msg.wData = ItemIdx;								
					NETWORK->Send( &msg, sizeof(msg) );
				}				
				
				AVATARITEM* pAvatarUseInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatarItem[i] );
				if( !pAvatarUseInfo )		continue;
				for(int k=0; k<eAvatar_Max; k++)
				{
					if( k >= eAvatar_Weared_Hair )
					if( !pAvatarUseInfo->Item[k] )
						pAvatarItem[k] = 1;					
				}

				pAvatarItem[i] = 0;
			}
		}
	}
	else
	{		
		for(int i=0; i<eAvatar_Max; i++)
		{
			if( i < eAvatar_Weared_Hair )
			{
				if( i == pAvatarUseInfo->Position )				
					pAvatarItem[i] = 0;
			}			
			else
			{
				if( !pAvatarUseInfo->Item[i] )
					pAvatarItem[i] = 1;
			}
		}		
	}
	
	AddCharacterPartChange( pPlayer->GetID() );
}



#ifndef _RESOURCE_WORK_NOT_APPLY_



VOID CAppearanceManager::CancelReservation(CObject* pObject)
{
	if (FALSE == m_bUseReservation)
	{
		return;
	}

	if (NULL == pObject)
	{
		return;
	}

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		RESERVATION_ITEM_PLAYER* pReservItem = (RESERVATION_ITEM_PLAYER*)pObject->GetEngineObject()->GetCacheListData();

		if (NULL != pReservItem)
		{
			m_lstReserveToAddPlayerList.Remove((void*)pReservItem);
		}

		pObject->GetEngineObject()->SetCacheListData(NULL);
	}
}

VOID CAppearanceManager::CancelAlphaProcess(CObject* pObject)
{
	if (FALSE == m_bUseReservation)
	{
		return;
	}

	if (NULL == pObject)
	{
		return;
	}

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		pObject->GetEngineObject()->SetAlpha(1.0f);
		m_lstAlphaProcessing.Remove(pObject);
	}
}

BOOL CAppearanceManager::ReserveToAppearObject(CObject* pObject, void* pMsg)
{
	if (FALSE == m_bUseReservation)
	{
		InitAppearance(pObject);
		return FALSE;
	}
		
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{	

		RESERVATION_ITEM_PLAYER* pReservItem = AllocReservationItem();
		if (NULL != pReservItem)
		{
			pReservItem->pObject	= (CPlayer*)pObject;
			pReservItem->Msg		= *(SEND_CHARACTER_TOTALINFO*)pMsg;

			m_lstReserveToAddPlayerList.AddTail((void*)pReservItem);	
			
			pObject->GetEngineObject()->SetCacheListData((void*)pReservItem);			
		}
		else
		{
			pObject->GetEngineObject()->SetCacheListData(NULL);
		}
		
	}
	else
	{
		InitAppearance(pObject);
	}
	
	return TRUE;
}


#define REACTION_PROCESS_RESERVATION_TIME	200
#define LOAD_OBJECT_NUMBER_PER_PROCESS		3


VOID CAppearanceManager::ProcessReservation(void)
{
	if (FALSE == m_bUseReservation)
	{		
		return;
	}	
	

	DWORD	dwCurrentTick = GetTickCount();
	int		iLoopLimit = 0;
	int		i = 0;

	if (dwCurrentTick >= m_dwLastProcessReservationTime + REACTION_PROCESS_RESERVATION_TIME)
	{	
		for(int i=0; i<LOAD_OBJECT_NUMBER_PER_PROCESS; ++i)
		{
			if (0 < m_lstReserveToAddPlayerList.GetCount())
			{
				RESERVATION_ITEM_PLAYER* pReservItem = NULL;
				pReservItem = (RESERVATION_ITEM_PLAYER*)m_lstReserveToAddPlayerList.RemoveHead();

				if (NULL != pReservItem)
				{
					CPlayer* pPlayer = pReservItem->pObject;

					if (NULL != pPlayer)
					{
						InitAppearance(pPlayer);
						
						PostProcessAddPlayer(pPlayer, &pReservItem->Msg);
						
						if( pPlayer->GetCharacterTotalInfo()->bVisible )
						{
							pPlayer->GetEngineObject()->SetAlpha(0.2f);
							m_lstAlphaProcessing.AddTail(pPlayer);
						}
					}

					FreeReservationItem(pReservItem);
					pReservItem = NULL;

					pPlayer->GetEngineObject()->SetCacheListData(NULL);

				}
			}
		}
		m_dwLastProcessReservationTime = dwCurrentTick;
	}
}



RESERVATION_ITEM_PLAYER* CAppearanceManager::AllocReservationItem(VOID)
{

	if (m_dwUsedReserItemPlayerNum < MAX_RESERVATION_ITEM_PLAYER)
	{
		++m_dwUsedReserItemPlayerNum;
		return (RESERVATION_ITEM_PLAYER*)m_lstFreeReserItemPlayer.RemoveHead();
	}

	return NULL;
}

BOOL CAppearanceManager::FreeReservationItem(RESERVATION_ITEM_PLAYER* pItem)
{
	if (NULL == pItem)
	{
		return FALSE;
	}

	if (0 < m_dwUsedReserItemPlayerNum)
	{
		--m_dwUsedReserItemPlayerNum;
		m_lstFreeReserItemPlayer.AddTail((void*)pItem);
	}

	return TRUE;
}


VOID CAppearanceManager::ProcessAlphaAppearance(VOID)
{		
	float fAlpha = 0.0f;

	cPtrList removelist;

	PTRLISTPOS pos = m_lstAlphaProcessing.GetHeadPosition();
	
	while( pos )
	{
		CObject* pObject = (CObject*)m_lstAlphaProcessing.GetNext(pos);
		
		if( pObject )
		{
			fAlpha = pObject->GetEngineObject()->GetAlpha();			
			
			if (0.9f <= fAlpha)
			{
				pObject->GetEngineObject()->SetAlpha(1.0f);					
				removelist.AddTail(pObject);
			}
			else
			{
				fAlpha += 0.025f;
				
				pObject->GetEngineObject()->SetAlpha(fAlpha);			
			}
		}
	}

	pos = removelist.GetHeadPosition();
	while( pos )
	{
		CObject* pObject = (CObject*)removelist.GetNext(pos);
		if( pObject )
			m_lstAlphaProcessing.Remove( pObject );

	}
	removelist.RemoveAll();
}



DWORD ConfirmAddPlayer( CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pmsg )
{
	if( !pPlayer || !pmsg )		return eConfirmAddPlayer_NoData;

	WORD TournamentTeam = 0;

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)			
			yCASE(CAddableInfoList::GTournament)
				iter.GetInfoData( &TournamentTeam );
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	if( TournamentTeam == 2 )			return eConfirmAddPlayer_DontAddPlayer;

	return eConfirmAddPlayer_NoErr;
}

void PostProcessAddPlayer(CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pmsg)
{
	if (NULL == pPlayer)
	{
		return;
	}

	if (NULL == pmsg)
	{
		return;
	}


	OBJECTSTATEMGR->InitObjectState(pPlayer);
	OBJECTMGR->ApplyOverInfoOption( pPlayer );
	OBJECTMGR->ApplyShadowOption( pPlayer );

	BOOL bVimu = FALSE;
	BOOL bPartyWar = FALSE;
	BOOL bGTournament = FALSE;
	BOOL bSiegeWar = FALSE;
	int wPartyTeam = 0;
	int wGTournamentTeam = 0;
	WORD StallKind = 0;
	SEIGEWAR_CHARADDINFO SiegeWarInfo;
	COMPRESSEDPOS VimuPos;
	static char StreetStallTitle[MAX_STREETSTALL_TITLELEN+1] = {0,};

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)			
			yCASE(CAddableInfoList::VimuInfo)
				bVimu = TRUE;
				iter.GetInfoData(&VimuPos);
			yCASE(CAddableInfoList::StreetStall)
				StallKind = eSK_SELL;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::StreetBuyStall)
				StallKind = eSK_BUY;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::PartyWar)
				bPartyWar = TRUE;
				iter.GetInfoData( &wPartyTeam );
			yCASE(CAddableInfoList::GTournament)
				bGTournament = TRUE;
				iter.GetInfoData( &wGTournamentTeam );
			yCASE(CAddableInfoList::SiegeWar)
				bSiegeWar = TRUE;
				iter.GetInfoData( &SiegeWarInfo );	
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	if( pmsg->TotalInfo.bPKMode )
	{
		if( pPlayer->InTitan() )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
			pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );						
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
			pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
		}
	}

	if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
		||(!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
	{
		if( pmsg->TotalInfo.bMussangMode )
		{
//#ifdef _JAPAN_LOCAL_
//			OBJECTEFFECTDESC desc(FindEffectNum("maintain_mussang.beff"));
//			pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID, &desc, 1, pPlayer);
//#else
			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				{
					OBJECTEFFECTDESC desc(1035);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
				}
				break;
			case eStage_Hwa:		
			case eStage_Hyun:
				{
					OBJECTEFFECTDESC desc(888);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
				}
				break;
			case eStage_Geuk:
			case eStage_Tal:
				{
					OBJECTEFFECTDESC desc(889);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
				}
				break;
			}
//#endif			
		}

	}
	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pPlayer;
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pPlayer,&set,0,pPlayer->GetID());
		pPlayer->ChangeBaseMotion( pPlayer->GetStandardMotion() );
	}
	else
	{
		DWORD	dwObjectIndex = GAMEIN->GetStallFindDlg()->GetSelectedObjectIndex();
		DWORD	dwPrevType = GAMEIN->GetStallFindDlg()->GetPrevSelectedType();
		if( pmsg->BaseObjectInfo.ObjectState == eObjectState_StreetStall_Owner )
		{
			if(StallKind == eSK_SELL)
				pPlayer->ShowStreetStallTitle( TRUE, StreetStallTitle );
			else if(StallKind == eSK_BUY)
				pPlayer->ShowStreetBuyStallTitle( TRUE, StreetStallTitle );
			if( pPlayer->GetShopItemStats()->dwStreetStallDecoration )
				STREETSTALLMGR->AddStreetStallDecoration(pPlayer, StallKind);

			if( dwObjectIndex == pPlayer->GetID() )
			{
				if( StallKind == dwPrevType )
                    pPlayer->SetStreetStallBalloonImage( dwPrevType, TRUE );
			}
		}
		else
		{ 
			if( dwObjectIndex == pPlayer->GetID() )
			{
				pPlayer->SetStreetStallBalloonImage( dwPrevType, FALSE );
				GAMEIN->GetStallFindDlg()->SetSelectedObjectIndex(0);
				GAMEIN->GetStallFindDlg()->SetPrevSelectedType(0);
			}
		}
	}

	if( bVimu )
	{
		if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam1 )
		{
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	
			}
		}
		else if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam2 )
		{
			if( pPlayer->InTitan() )

			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
		}
	}

	if( bPartyWar )
	{
		if( wPartyTeam == 1 )
		{
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
		}
		else if( wPartyTeam == 2 )
		{
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
		}
	}
	
	if( bGTournament )
	{
		if( wGTournamentTeam == 0 )
		{
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			pPlayer->GetCharacterTotalInfo()->bVisible = true;
		}
		else if( wGTournamentTeam == 1 )
		{
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			}

			pPlayer->GetCharacterTotalInfo()->bVisible = true;
		}
		else if( wGTournamentTeam == 2 )
		{
			pPlayer->GetCharacterTotalInfo()->bVisible = false;
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
				pPlayer->GetEngineObject()->SetAlpha( 0.3f );
			else
				pPlayer->GetEngineObject()->HideWithScheduling();
		}		
	}

	if( bSiegeWar && HERO )
	{
		((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName( pPlayer, SiegeWarInfo.Team, pPlayer->GetGuildIdx() );

		if( SiegeWarInfo.bEngrave )
		{
			OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_Engrave);
			pPlayer->ChangeMotion(329, TRUE);

			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
			pPlayer->AddObjectEffect( ENGRAVE_EFFECTID, &desc, 1, HERO );			
		}

		if( pPlayer->IsRestraintMode() )
		{
			pPlayer->SetRestraintMode( TRUE );
			OBJECTMGR->ApplyOverInfoOption( pPlayer );

			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );						
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
			}
		}
	}

	if( pPlayer->GetState() == eObjectState_Engrave )
	{
		OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
		pPlayer->AddObjectEffect( ENGRAVE_EFFECTID, &desc, 1, HERO );
	}
}




#endif




