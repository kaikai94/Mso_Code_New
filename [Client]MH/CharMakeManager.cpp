







#include "stdafx.h"
#include "Hero.h"
#include "CharMakeManager.h"
#include "GameResourceManager.h"
#include "CharMake.h"
#include "windowidenum.h"
#include "cComboBoxEx.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "HelperManager.h"
#include "CharMakeDialog.h"
#include "AppearanceManager.h"
#include "interface/cGuageBar.h"
#include "HelperSpeechDlg.h"
#include "MHFile.h"
#include "interface/cStatic.h"


GLOBALTON(cCharMakeManager)
cCharMakeManager::cCharMakeManager()
{
	m_bInit = FALSE;
}

cCharMakeManager::~cCharMakeManager()
{
}

void cCharMakeManager::Init()
{
	m_pNewPlayer = NULL;

	memset(&m_CharMakeInfo, 0, sizeof(CHARACTERMAKEINFO));
	memset(&m_dwCurIndex, 0, sizeof(DWORD)*CE_MAX);	// �� �ɼǺ��� ���� ���õ� �ε����� ������ �ִ´�.

	LoadUserSelectOption();
//	SetUserSelectOption();
	SetUserSelections();


	m_fAngle = 70;
	m_fOldX = 0.0f;

	m_bInit = TRUE;
	
	m_pHeight = (cGuageBar*)CHARMAKE->GetCharMakeDialog()->GetWindowForID(CMID_Height);
	m_pWidth = (cGuageBar*)CHARMAKE->GetCharMakeDialog()->GetWindowForID(CMID_Width);
	InitConfig();
}
void cCharMakeManager::InitConfig()
{
	char buff[64] = {0,};
	CMHFile file;
	if(file.Init("./Resource/Client/CharMake.bin","rb")==FALSE)
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
			COMPARE("#CREATEPOS")
				sCharMake.vCreatePos.x = file.GetFloat();
				sCharMake.vCreatePos.y = file.GetFloat();
				sCharMake.vCreatePos.z = file.GetFloat();
			COMPARE("#MAPNUM")
				sCharMake.wMapNum = file.GetWord();
			COMPARE("#CAMERAPOS")
				sCharMake.vCameraPos.x = file.GetFloat();
				sCharMake.vCameraPos.y = file.GetFloat();
				sCharMake.vCameraPos.z = file.GetFloat();
			COMPARE("#CAMERANUM")
				sCharMake.wCameraNum = file.GetWord();
			COMPARE("#ANGLEPOS")
				sCharMake.vAnglePos.x = file.GetFloat();
				sCharMake.vAnglePos.y = file.GetFloat();
				sCharMake.vAnglePos.z = file.GetFloat();
			COMPARE("#EYEPOS")
				sCharMake.vEyePos.x = file.GetFloat();
				sCharMake.vEyePos.y = file.GetFloat();
				sCharMake.vEyePos.z = file.GetFloat();
			COMPARE("#ROTATE")
				sCharMake.vRotate.x = file.GetFloat();
				sCharMake.vRotate.y = file.GetFloat();
				sCharMake.vRotate.z = file.GetFloat();
			COMPARE("#CURCAMERA")
				sCharMake.wCurCamera = file.GetWord();
			COMPARE("#CAMERAMODE")
				sCharMake.wCameraMode = file.GetWord();
			COMPARE("#CAMERAEYE")
				sCharMake.vCameraEye.x = file.GetFloat();
				sCharMake.vCameraEye.y = file.GetFloat();
				sCharMake.vCameraEye.z = file.GetFloat();
			COMPARE("#CAMERATO")
				sCharMake.vCameraTo.x = file.GetFloat();
				sCharMake.vCameraTo.y = file.GetFloat();
				sCharMake.vCameraTo.z = file.GetFloat();
			COMPARE("#CAMERAUP")
				sCharMake.vCameraUp.x = file.GetFloat();
				sCharMake.vCameraUp.y = file.GetFloat();
				sCharMake.vCameraUp.z = file.GetFloat();
			COMPARE("#ZOOM")
				sCharMake.fZoom[0] = file.GetFloat();
				sCharMake.fZoom[1] = file.GetFloat();
			COMPARE("#ROTPITCH")
				sCharMake.fRotPitch[0] = file.GetFloat();
				sCharMake.fRotPitch[1] = file.GetFloat();
			COMPARE("#CAMERAANGLEX")
				sCharMake.fCameraAngleX = file.GetFloat();
			COMPARE("#MOVEDISTANCE")
				sCharMake.fMonveDistance = file.GetFloat();
			COMPARE("#ROTATEANGLE")
				sCharMake.fRotateAngle = file.GetFloat();
			COMPARE("PI")
				sCharMake.fPI[0] = file.GetFloat();
				sCharMake.fPI[1] = file.GetFloat();
		END_COMPARE
	}
	file.Release();
}
void cCharMakeManager::Release()
{
	for(int i=0;i<CE_MAX;++i)
	{
		if( !m_UserSelectOption[i].IsEmpty() )
		{
			PTRLISTSEARCHSTART(m_UserSelectOption[i],CM_OPTION*,Value)
				delete Value;
			PTRLISTSEARCHEND

			m_UserSelectOption[i].RemoveAll();
		}
	}
}
/*SW050812
void cCharMakeManager::ComboBoxResetAll()
{

	cComboBoxEx* pCombo = NULL;

	for(int i=0;i<CE_MAX;++i)
	{
		if( !m_UserSelectOption[i].IsEmpty() && i != CE_SEX  && i != CE_AREA)
		{
			PTRLISTPOS pos = m_UserSelectOption[i].GetHeadPosition();
			CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[i].GetAt(pos);
			pCombo = (cComboBoxEx*)Value->pDescCombo;

			pCombo->SelectComboText(0);
		}
	}
}*/

void cCharMakeManager::ListBoxResetAll()
{
	cStatic* pStatic = NULL;


	for(int i=0;i<CE_MAX;++i)
	{
//#ifdef _JAPAN_LOCAL_
//		if( !m_UserSelectOption[i].IsEmpty() && i != CE_SEX  && i != CE_AREA && i != CE_ATTRIBUTE )
//		{
//
//			PTRLISTPOS pos = m_UserSelectOption[i].GetHeadPosition();
//			CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[i].GetAt(pos);
//			pStatic = (cStatic*)Value->pDescStatic;
//			pStatic->SetStaticText(Value->strName);
//		}
//#else
		if( !m_UserSelectOption[i].IsEmpty() && i != CE_SEX  && i != CE_AREA && i != CE_WEAPON )
		{
			PTRLISTPOS pos = m_UserSelectOption[i].GetHeadPosition();
			CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[i].GetAt(pos);
			pStatic = (cStatic*)Value->pDescStatic;
			pStatic->SetStaticText(Value->strName);
		}
//#endif
	}	
}

void cCharMakeManager::RotateSelection( int idx, int Dir )
{
	cStatic* pStatic = NULL;

	PTRLISTPOS pos = m_UserSelectOption[idx].FindIndex(m_dwCurIndex[idx]);

	if( Dir == CM_PREV )
	{
		if( pos == m_UserSelectOption[idx].GetHeadPosition() )
		{
			m_dwCurIndex[idx] = m_UserSelectOption[idx].GetCount() - 1;
		}
		else
			m_dwCurIndex[idx]--;
	}

	else if( Dir == CM_NEXT )
	{
		if( pos == m_UserSelectOption[idx].GetTailPosition() )
		{
			m_dwCurIndex[idx] = 0; 
		}
		else
			m_dwCurIndex[idx]++;
	}
	
	else
		ASSERT(0);

	if(ReplaceCharMakeInfo( idx, m_dwCurIndex[idx] ))
	{
		APPEARANCEMGR->AddCharacterPartChange(1);
		APPEARANCEMGR->SetScalebyGuageBar(1);
	}

	pos = m_UserSelectOption[idx].FindIndex(m_dwCurIndex[idx]);
	CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[idx].GetAt(pos);
	pStatic = (cStatic*)Value->pDescStatic;
	pStatic->SetStaticText(Value->strName);
}
void cCharMakeManager::LoadUserSelectOption()
{
	int idx = 0;
	int num = 0;
	char StaticId[64];

	CMHFile file;
	if(!file.Init(PATH_UOPTION, "rb")) return;
	char temp[256];
	file.GetLine(temp, 256);
	while(1)
	{
		if( idx >= CE_MAX || file.IsEOF()) break;

		num = file.GetInt(); 
		strcpy(StaticId, file.GetString());


		for( int i=0;i<num;++i)
		{
			CM_OPTION*	option = new CM_OPTION;
			strcpy(option->strName, file.GetString());
			option->dwData = file.GetDword();
			option->dwHelperData = file.GetDword();
			option->pDescStatic = (cStatic*)CHARMAKE->GetCharMakeDialog()->GetWindowForID( IDSEARCH(StaticId) );
			if(option->pDescStatic != NULL)
			{
				option->pDescStatic->SetAlign(TXT_MIDDLE);
			}


			m_UserSelectOption[idx].AddTail(option);

		}

		++idx;
	}
}

/*SW050812
void cCharMakeManager::SetUserSelectOption()
{
	cComboBoxEx* pCombo = NULL;
	OPTIONITEM * pItem = NULL;

	for(int i=0;i<CE_MAX;++i)
	{
		if( !m_UserSelectOption[i].IsEmpty() )
		{
			WORD MaxOption = 0;
			PTRLISTSEARCHSTART(m_UserSelectOption[i],CM_OPTION*,Value)
//				pCombo = (cComboBoxEx*)Value->pDescCombo;

				pItem = new OPTIONITEM;
				pItem->wComboType = i;
				strcpy(pItem->string, Value->strName);
				pItem->dwData = Value->dwData;
				if( Value->dwHelperData != 0 )
					pItem->nExtData = Value->dwHelperData;
			pCombo->AddItem(pItem);
				++MaxOption;
			PTRLISTSEARCHEND

			// �� ���� ǥ�ø� ���� �ұ�?
			if(pItem->wComboType == CE_AREA)
			{
				WORD randarea = MaxOption-1;	//µ·È²(³·)?» ±âº»°ª?¸·Î ¹Ù²Û´Ù.
				pCombo->SelectComboText(randarea);
				ReplaceCharMakeInfo(i, randarea);

			}

			else
			{
				pCombo->SelectComboText(0);
				ReplaceCharMakeInfo(i, 0);
			}
		}
	}

//	for(int i=0;i<CE_MAX;++i)
//		ReplaceCharMakeInfo(i, 0);

	m_CharMakeInfo.Height = 1;
	m_CharMakeInfo.Width = 1;
}*/

void cCharMakeManager::SetUserSelections()
{
	cStatic*	pStatic = NULL;

	for(int i=0;i<CE_MAX;++i)
	{

		PTRLISTPOS pos = m_UserSelectOption[i].GetHeadPosition();

		if(pos)
		{
			CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[i].GetAt(pos);
			pStatic = (cStatic*)Value->pDescStatic;
			if(pStatic)	// ���� ���� ǥ�� ����.
			{
				pStatic->SetStaticText(Value->strName);	
			}
			ReplaceCharMakeInfo(i, 0);
		}
	}

	m_CharMakeInfo.Height = 1;
	m_CharMakeInfo.Width = 1;
}


BOOL cCharMakeManager::ReplaceCharMakeInfo(int idx, WORD wValue )
{
	PTRLISTPOS pos = m_UserSelectOption[idx].FindIndex( wValue );
	if( !pos ) return FALSE;

	CM_OPTION* option = (CM_OPTION*)m_UserSelectOption[idx].GetAt(pos);
	if( !option ) return FALSE;

	DWORD value = option->dwData;

	switch(idx)
	{


		/*
	case CE_JOB: 
		{
			if( m_CharMakeInfo.JobType == value ) return FALSE;

			m_CharMakeInfo.JobType = (BYTE)value;

			if( HELPERMGR->IsCompleteInit() )
				HELPERMGR->ChangeSpeechDlg( option->dwHelperData );

			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->Job = (BYTE)value;
		}
		break;
		*/
//#ifdef _JAPAN_LOCAL_
//	case CE_ATTRIBUTE:
//		{
//			if( m_CharMakeInfo.wCharAttr == (WORD)value )	return FALSE;
//			m_CharMakeInfo.wCharAttr = (WORD)value;
//			if( m_pNewPlayer )
//				m_pNewPlayer->GetCharacterTotalInfo()->nMainAttr = (int)value;
//		}
//		break;
//#endif
		
	case CE_SEX:
		{
			if( m_CharMakeInfo.SexType == value ) return FALSE;
			
			m_CharMakeInfo.SexType = (BYTE)value;
			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->Gender = (BYTE)value;
			
			ReplaceCharMakeInfo( CE_MHAIR, 0 );
			ReplaceCharMakeInfo( CE_MFACE, 0 );
			ReplaceCharMakeInfo( CE_WEAR, 0 );

			ReplaceCharMakeInfo( CE_BOOT, 0 );
			
			///			ComboBoxResetAll();
			ListBoxResetAll();
			CHARMAKE->GetCharMakeDialog()->ChangeComboStatus((unsigned short)value);

//			CreateNewCharacter();

			if( m_pNewPlayer ) 
			{
				OBJECTMGR->AddGarbageObject( (CObject*)m_pNewPlayer );
				m_pNewPlayer = NULL;
			}
		}
		return FALSE; // »õ·Î »ý¼ºÇÒ °æ¿ì´Â AddCharacterPartChange ¸¦ ÇÏÁö ¾Ê´Â´Ù.
	case CE_MHAIR: 
	case CE_WMHAIR:
		{
			if( m_CharMakeInfo.HairType == value ) return FALSE;

			m_CharMakeInfo.HairType = (BYTE)value;
			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->HairType = (BYTE)value;
			return TRUE;
		}
		break;
	case CE_MFACE: 
	case CE_WMFACE: 
		{
			if( m_CharMakeInfo.FaceType == value ) return FALSE;

			m_CharMakeInfo.FaceType = (BYTE)value;
			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->FaceType = (BYTE)value;

			return TRUE;
		}
		break;
	case CE_WEAR: 
		{
			if( m_CharMakeInfo.WearedItemIdx[eWearedItem_Dress] == value ) return FALSE;

			m_CharMakeInfo.WearedItemIdx[eWearedItem_Dress] = (WORD)value;

			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->WearedItemIdx[eWearedItem_Dress] = (WORD)value;

			return TRUE;
		}
		break;
	case CE_BOOT: 
		{
			if( m_CharMakeInfo.WearedItemIdx[eWearedItem_Shoes] == value ) return FALSE;

			m_CharMakeInfo.WearedItemIdx[eWearedItem_Shoes] = (WORD)value;
			if( m_pNewPlayer )
				m_pNewPlayer->GetCharacterTotalInfo()->WearedItemIdx[eWearedItem_Shoes] = (WORD)value;

			return TRUE;
		}
		break;
	case CE_WEAPON: 
		{
			if( m_CharMakeInfo.WearedItemIdx[eWearedItem_Weapon] == value ) return FALSE;

			m_CharMakeInfo.WearedItemIdx[eWearedItem_Weapon] = (WORD)value;

//			if( HELPERMGR->IsCompleteInit() )
			if( m_bInit )

			{
				cHelperSpeechDlg* pHelpDlg = HELPERMGR->GetHelperDlg();


				if( HELPERMGR->GetHelper() && pHelpDlg )	//Ŭ���ص� ����â ������� �ʰ�..
				{
					HELPERMGR->SetActive(TRUE);

					if(pHelpDlg->IsListEmpty())
					{
						HELPERMGR->ChangeSpeechDlg( option->dwHelperData, FALSE );
					}
				}
			}

			if( m_pNewPlayer )
			{
				m_pNewPlayer->GetCharacterTotalInfo()->WearedItemIdx[eWearedItem_Weapon] = (WORD)value;

				m_pNewPlayer->ChangeMotion(eMotion_Battle_Gum_Standard + wValue, 1);
				m_pNewPlayer->ChangeBaseMotion(eMotion_Battle_Gum_Standard + wValue);	
			}

			return TRUE;
		}
		break;
	case CE_AREA: 
		{

			m_CharMakeInfo.StartArea = (BYTE)value;
		}
		break;
	}

	return FALSE;
}

void cCharMakeManager::CreateNewCharacter(VECTOR3* pv3Pos)
{
	if (NULL == pv3Pos)
	{
		return;
	}

	VECTOR3 pos;


	m_fAngle = 40;

	pos = *pv3Pos;
	

	BASEOBJECT_INFO sBaseInfo;
	CHARACTER_TOTALINFO sTotalInfo;

	memset(&sBaseInfo, 0, sizeof(BASEOBJECT_INFO));
	memset(&sTotalInfo, 0, sizeof(CHARACTER_TOTALINFO));

	sBaseInfo.dwObjectID = 1;
	strcpy( sBaseInfo.ObjectName, "NewCharacter" );
	sBaseInfo.ObjectState = 1;

//	sTotalInfo.Job = m_CharMakeInfo.JobType;
	sTotalInfo.HairType = m_CharMakeInfo.HairType;
	sTotalInfo.FaceType = m_CharMakeInfo.FaceType;
	sTotalInfo.Gender = m_CharMakeInfo.SexType;
	sTotalInfo.WearedItemIdx[eWearedItem_Dress] = m_CharMakeInfo.WearedItemIdx[eWearedItem_Dress];
	sTotalInfo.WearedItemIdx[eWearedItem_Shoes] = m_CharMakeInfo.WearedItemIdx[eWearedItem_Shoes];
	sTotalInfo.WearedItemIdx[eWearedItem_Weapon] = m_CharMakeInfo.WearedItemIdx[eWearedItem_Weapon];
	sTotalInfo.Height = m_CharMakeInfo.Height;
	sTotalInfo.Width = m_CharMakeInfo.Width;
	sTotalInfo.bVisible = TRUE;

	//m_pNewPlayer = OBJECTMGR->AddPlayer(&sBaseInfo,NULL,&sTotalInfo,NULL);
	//SW070127 Ÿ��ź
	m_pNewPlayer = OBJECTMGR->AddPlayer(&sBaseInfo,NULL,&sTotalInfo,NULL,NULL,FALSE);
	m_pNewPlayer->GetEngineObject()->ApplyHeightField(FALSE);
	m_pNewPlayer->SetOverInfoOption(0);
	
//	m_pNewPlayer->SetPosition(&pos);
//	m_pNewPlayer->SetAngle(m_fAngle);	// ¹æÇâ µ¹¸®±â
	MOVEMGR->SetPosition( m_pNewPlayer, &pos );
	MOVEMGR->SetAngle( m_pNewPlayer, m_fAngle, 0 );
	
	m_pNewPlayer->ChangeMotion(eMotion_Battle_Gum_Standard + m_dwCurIndex[CE_WEAPON],1);
	m_pNewPlayer->ChangeBaseMotion(eMotion_Battle_Gum_Standard + m_dwCurIndex[CE_WEAPON]);	
}

void cCharMakeManager::NewCharacterRotate( int x )
{
	if( !m_pNewPlayer ) return;

	m_fAngle -= x*CHARROTATE_RATE*30;

	
	if( m_fAngle >= 360 )
		m_fAngle = 0;

//	m_pNewPlayer->SetAngle( m_fAngle );
	MOVEMGR->SetAngle( m_pNewPlayer, m_fAngle, 0 );
}


void cCharMakeManager::ApplyScale()
{
	m_CharMakeInfo.Height = 0.9 + m_pHeight->GetCurRate()*0.2;
	m_CharMakeInfo.Width = 0.9 + m_pWidth->GetCurRate()*0.2;

	VECTOR3 scale;
	SetVector3(&scale,
		m_CharMakeInfo.Width,
		m_CharMakeInfo.Height,
		m_CharMakeInfo.Width);
	m_pNewPlayer->GetEngineObject()->SetScale(&scale);
}




