#include "StdAfx.h"
#include "PartyMatchingDlg.h"
#include "WindowIDEnum.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "PartyManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "BattleSystem_Client.h"
#include "GameIn.h"
#include "cIMEex.h"

CPartyMatchingDlg::CPartyMatchingDlg()

{
	m_pLimitLevelEdit = NULL;
	m_pPartyList = NULL;
	m_dwSelectIdx = -1;
	m_dwReflashDelayTime = 0;
	m_PartyInfoTable.Initialize(MAX_PARTYMATCHING_INFO_NUM);	

}

CPartyMatchingDlg::~CPartyMatchingDlg()
{
	m_pPartyList->RemoveAll();
	this->RemovePartyInfoTable();
}

void CPartyMatchingDlg::Linking()
{
	m_pPartyList = (cListDialog*)GetWindowForID(PARTYMATCHING_LIST);	
	m_pLimitLevelEdit = (cEditBox*)GetWindowForID(PARTYMATCHING_LEVEL_EDIT);	

	m_pPartyList->SetShowSelect(TRUE);	
	m_pLimitLevelEdit->SetEditText("0");
	m_pLimitLevelEdit->SetValidCheck( VCM_NUMBER );
	m_pLimitLevelEdit->SetAlign( TXT_RIGHT );
}

void CPartyMatchingDlg::SetActive( BOOL val )

{
	cDialog::SetActive(val);

	if(val == FALSE)
	{
		m_pPartyList->RemoveAll();
		this->RemovePartyInfoTable();
		m_dwSelectIdx = -1;
		m_pPartyList->SetCurSelectedRowIdx(m_dwSelectIdx);
		m_pLimitLevelEdit->SetEditText("0");
	}
}

BOOL CPartyMatchingDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case PARTYMATCHING_RANDOMIN_BTN:
		{
			PartyRandomSelect();
		}
		break;
	case PARTYMATCHING_REFLESH_BTN:

		{
			if( (m_dwReflashDelayTime != 0) && (m_dwReflashDelayTime > gCurTime) )
			{
				DWORD dwTime = ((m_dwReflashDelayTime - gCurTime) / 1000) + 1;
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043),  dwTime);
				return TRUE;
			}

			PARTYMGR->RefleshPartyMatchingList(atoi(m_pLimitLevelEdit->GetEditText()));
			StartReflashDelayTime();
		}
		break;
	case PARTYMATCHING_JOIN_BTN:
		{
			if(m_dwSelectIdx == -1)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1738));
				return TRUE;
			}

			PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData(m_dwSelectIdx);
			if(pPartyInfo == NULL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
				return TRUE;
			}

			DWORD dwPartyIndex = pPartyInfo->PartyAddOption.dwPartyIndex;
			DWORD dwPartyMasterID = pPartyInfo->dwMemberID[0];
			MasterToPartyRequestSyn(dwPartyIndex, dwPartyMasterID);

		}
		break;
	}

	return TRUE;
}

DWORD CPartyMatchingDlg::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pPartyList->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{		
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pPartyList->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx;
			}
		}
	}	

	return we;
}

void CPartyMatchingDlg::RemovePartyInfoTable()
{
	m_PartyInfoTable.SetPositionHead();
	while (PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData())
	{
		SAFE_DELETE(pPartyInfo);
	}
	m_PartyInfoTable.RemoveAll();
}

void CPartyMatchingDlg::SetPartyMatchingList(MSG_PARTYMATCHING_INFO* pPartyListInfo)
{
	m_pPartyList->RemoveAll();
	this->RemovePartyInfoTable();

	WORD wCount = pPartyListInfo->wCount;	
	
	PartyListSort(pPartyListInfo->PartyMatchingInfo, wCount);
	
	int nTemp = 0;
	int nKey = 0;	

	for(int i = 0 ; i < wCount ; i++)
	{
		BOOL bPublic = pPartyListInfo->PartyMatchingInfo[i].PartyAddOption.bPublic;
		if(bPublic == TRUE)
		{
			PARTY_MATCHINGLIST_INFO* pPartyInfo = new PARTY_MATCHINGLIST_INFO;
			SetPartyInfo(pPartyInfo, pPartyListInfo->PartyMatchingInfo[i]);
			m_PartyInfoTable.Add(pPartyInfo, nKey);
			nKey++;
		}
		else
		{
			pTemp[nTemp] = pPartyListInfo->PartyMatchingInfo[i];
			nTemp++;
		}
	}
	for(int n = 0 ; n < nTemp ; n++)
	{
		PARTY_MATCHINGLIST_INFO* pPartyInfo = new PARTY_MATCHINGLIST_INFO;
		SetPartyInfo(pPartyInfo, pTemp[n]);
		m_PartyInfoTable.Add(pPartyInfo, nKey);		
		nKey++;
	}

	char szListStr[128];
	m_PartyInfoTable.SetPositionHead();

	for(DWORD i = 0 ; i < m_PartyInfoTable.GetDataNum() ; i++)	
	{

		PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData(i);		

		if(pPartyInfo == NULL)
			continue;

		int nLen = 0;

		char szIndexNum[7+1];
		char szTempStr[7+1];
		memset(szIndexNum, 0, sizeof(szIndexNum));
		memset(szTempStr, 0, sizeof(szTempStr));
		sprintf(szTempStr, "%d", i+1);
		nLen = strlen(szTempStr);
		int nCount = 0;
		for( int k = 0 ; k < 7 ; k++ )
		{
			if(k > (3-nLen) )
			{
				if( (nLen > 0) && (nCount < nLen) )
				{
					szIndexNum[k] = szTempStr[nCount];
					nCount++;
				}
				else
				{
					szIndexNum[k] = 0x20;
				}
			}
			else
			{
				szIndexNum[k] = 0x20;				
			}

			szIndexNum[k+1] = NULL;
		}
		
		char szPartyName[MAX_PARTY_NAME+8];
		memset(szPartyName, 0, sizeof(szPartyName));
		SafeStrCpy(szPartyName, pPartyInfo->PartyAddOption.szTheme, MAX_PARTY_NAME+8);
		nLen = strlen(szPartyName);
		for( int k = nLen ; k < MAX_PARTY_NAME+8-1 ; k++ )
		{
			szPartyName[k] = 0x20;
			szPartyName[k+1] = NULL;
		}

		char szPartyMasterName[MAX_NAME_LENGTH+8];
		memset(szPartyMasterName, 0, sizeof(szPartyMasterName));
		SafeStrCpy(szPartyMasterName, pPartyInfo->szMasterName, MAX_NAME_LENGTH+8);
		nLen = strlen(szPartyMasterName);
		for( int k = nLen ; k < MAX_NAME_LENGTH+8-1 ; k++ )
		{
			szPartyMasterName[k] = 0x20;
			szPartyMasterName[k+1] = NULL;
		}

		char szLimitLevel[10+1];	
		memset(szLimitLevel, 0, sizeof(szLimitLevel));
		WORD wMinLevel = pPartyInfo->PartyAddOption.wMinLevel;		
		WORD wMaxLevel = pPartyInfo->PartyAddOption.wMaxLevel;		
		sprintf(szLimitLevel, "%d ~ %d", wMinLevel, wMaxLevel);		
		nLen = strlen(szLimitLevel);
		for( int k = nLen ; k < 9 ; k++ )
		{
			szLimitLevel[k] = 0x20;
			szLimitLevel[k+1] = NULL;
		}

		char szMemberNum[12+1];

		memset(szMemberNum, 0, sizeof(szMemberNum));
		WORD wMaxMemberNum = pPartyInfo->PartyAddOption.wLimitCount;	
		WORD wCurMemberNum = 0;															

		wCurMemberNum = GetPartyMemberNum(pPartyInfo);		

		sprintf(szMemberNum, "%d / %d", wCurMemberNum, wMaxMemberNum);		

		sprintf(szListStr, "%s %s %s %s %s", szIndexNum, szPartyName, szPartyMasterName, szLimitLevel, szMemberNum);		
		
		DWORD dwColor = 0xffffffff;
		if(pPartyInfo->PartyAddOption.bPublic == FALSE)	
			dwColor = RGBA_MAKE(172,182,199,255);

		if( (pPartyInfo->PartyAddOption.dwPartyIndex == HERO->GetPartyIdx()) && (HERO->GetPartyIdx() != 0) )
			dwColor = RGBA_MAKE(0,255,0,255);
		
		m_pPartyList->AddItem(szListStr, dwColor);
	}	
}

void CPartyMatchingDlg::PartyListSort(PARTY_MATCHINGLIST_INFO* pPartyListInfo, WORD wMaxNum)
{	
	PARTY_MATCHINGLIST_INFO temp;

	for(int i = 0 ; i < wMaxNum ; i++)
	{
		for(int j = i+1 ; j < wMaxNum ; j++)
		{
			WORD wCurValue = pPartyListInfo[i].PartyAddOption.wMinLevel;
			WORD wNextValue = pPartyListInfo[j].PartyAddOption.wMinLevel;
			
			if(wCurValue > wNextValue)
			{

				temp = pPartyListInfo[i];
				pPartyListInfo[i] = pPartyListInfo[j];
				pPartyListInfo[j] = temp;
			}
		}
	}
}

WORD CPartyMatchingDlg::GetPartyMemberNum(PARTY_MATCHINGLIST_INFO* pPartyInfo)
{
	WORD wCurMemberNum = 0;
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		if(pPartyInfo->dwMemberID[i] != 0)
			wCurMemberNum++;
	}

	return wCurMemberNum;
}

void CPartyMatchingDlg::PartyRandomSelect()
{	
	DWORD dwRandomIndexArray[MAX_PARTYMATCHING_INFO_NUM];
	memset(&dwRandomIndexArray, -1, sizeof(dwRandomIndexArray));

	DWORD dwPartyNum = m_PartyInfoTable.GetDataNum();
	BOOL bCheck = FALSE;

	for(DWORD i = 0 ; i < dwPartyNum ; i++)
	{
		while(1)
		{
			bCheck = FALSE;

			DWORD dwRandomIndex = rand()%dwPartyNum;

			for(DWORD j = 0 ; j < i ; j++)
			{
				if(dwRandomIndexArray[j] == dwRandomIndex)
				{
					bCheck = TRUE;
					break;
				}
			}
			if(bCheck == FALSE)
			{
				dwRandomIndexArray[i] = dwRandomIndex;
				break;
			}
		}
	}

	PARTY_MATCHINGLIST_INFO* pPartyInfo = NULL;
	bCheck = FALSE;

	for(DWORD i = 0 ; i < dwPartyNum ; i++)
	{
		pPartyInfo = m_PartyInfoTable.GetData(dwRandomIndexArray[i]);
        if(pPartyInfo == NULL)
			continue;

		if(pPartyInfo->PartyAddOption.bPublic == FALSE) 

			continue;
		if(pPartyInfo->PartyAddOption.wLimitCount <= GetPartyMemberNum(pPartyInfo))	

			continue;
		
		LEVELTYPE wLevel = HERO->GetLevel();
		if(pPartyInfo->PartyAddOption.wMinLevel > wLevel)
			continue;
		if(pPartyInfo->PartyAddOption.wMaxLevel < wLevel)
			continue;

        bCheck = TRUE;
		break;
	}

	if(bCheck == TRUE)
	{
		DWORD dwPartyIndex = pPartyInfo->PartyAddOption.dwPartyIndex;
		DWORD dwPartyMasterID = pPartyInfo->dwMemberID[0];
				
		if(CheckPartyRequest(dwPartyIndex) == FALSE)
			return;

		this->MasterToPartyRequestSyn(dwPartyIndex, dwPartyMasterID);		
	}
	else
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1734));

}

BOOL CPartyMatchingDlg::CheckPartyRequest(DWORD dwPartyIDX)
{
	if(PARTYMGR->IsProcessing() == TRUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
		return FALSE;
	}

	DWORD dwPartyNum = m_PartyInfoTable.GetDataNum();
	if(dwPartyNum == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
		return FALSE;
	}

	if(HERO->GetPartyIdx() != 0)

	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1732) );
		return FALSE;
	}

	if( (BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE) &&
		(BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_MUNPAFIELD) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(1737));
		return FALSE;		
	}

	PARTY_MATCHINGLIST_INFO* pPartyInfo = this->GetPartyInfo(dwPartyIDX);
	if(pPartyInfo == NULL)
	{

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
		return FALSE;
	}

	LEVELTYPE wHeroLevel = HERO->GetLevel();
    LEVELTYPE wLimitMinLevel = pPartyInfo->PartyAddOption.wMinLevel;
	LEVELTYPE wLimitMaxLevel = pPartyInfo->PartyAddOption.wMaxLevel;
	WORD wLimitCount = pPartyInfo->PartyAddOption.wLimitCount;
	BOOL bPublic = pPartyInfo->PartyAddOption.bPublic;

	if( (wLimitMinLevel > wHeroLevel) || (wLimitMaxLevel < wHeroLevel) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1731) );
		return FALSE;
	}

	WORD wCurMemberNum = GetPartyMemberNum(pPartyInfo);	
	if(wLimitCount < wCurMemberNum)

	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1733) );
		return FALSE;
	}

	if(bPublic == FALSE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1729) );
		return FALSE;
	}

	return TRUE;
}

void CPartyMatchingDlg::MasterToPartyRequestSyn(DWORD dwPartyIDX, DWORD dwMasterID)
{
	if( (dwPartyIDX == 0) || (dwMasterID == 0) )
		return;

	if(CheckPartyRequest(dwPartyIDX) == FALSE)
		return;

	MSG_NAME_DWORD2 msg;
	msg.Category	= MP_PARTY;
	msg.Protocol	= MP_PARTY_MASTERTOREQUEST_SYN;
	msg.dwObjectID	= HEROID;	
	msg.dwData1		= dwPartyIDX;
	msg.dwData2		= dwMasterID;
	SafeStrCpy(msg.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	NETWORK->Send(&msg,sizeof(msg));

	PARTYMGR->SetIsProcessing(TRUE);	
}

void CPartyMatchingDlg::SetPartyInfo(PARTY_MATCHINGLIST_INFO* pPartyInfo, PARTY_MATCHINGLIST_INFO Info)
{
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		pPartyInfo->dwMemberID[i] = Info.dwMemberID[i];
	}

	SafeStrCpy(pPartyInfo->szMasterName, Info.szMasterName, MAX_NAME_LENGTH+1);

	pPartyInfo->PartyAddOption.bOption = Info.PartyAddOption.bOption;
	pPartyInfo->PartyAddOption.bPublic = Info.PartyAddOption.bPublic;
	pPartyInfo->PartyAddOption.dwPartyIndex = Info.PartyAddOption.dwPartyIndex;
	SafeStrCpy(pPartyInfo->PartyAddOption.szTheme, Info.PartyAddOption.szTheme, MAX_PARTY_NAME+1);
	pPartyInfo->PartyAddOption.wLimitCount = Info.PartyAddOption.wLimitCount;
	pPartyInfo->PartyAddOption.wMaxLevel = Info.PartyAddOption.wMaxLevel;

	pPartyInfo->PartyAddOption.wMinLevel = Info.PartyAddOption.wMinLevel;

	
}

void CPartyMatchingDlg::StartReflashDelayTime()
{
	m_dwReflashDelayTime = eREFLESH_DELAYTIME + gCurTime;
}

PARTY_MATCHINGLIST_INFO* CPartyMatchingDlg::GetPartyInfo(DWORD dwPartyIDX)
{
	PARTY_MATCHINGLIST_INFO* pPartyInfo = NULL;

	m_PartyInfoTable.SetPositionHead();
	while (pPartyInfo = m_PartyInfoTable.GetData())
	{
		if(dwPartyIDX == pPartyInfo->PartyAddOption.dwPartyIndex)
			return pPartyInfo;
	}

	return NULL;
}




