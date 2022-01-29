




//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../[CC]Header/GameResourceManager.h"
#include "TitanManager.h"
#include "ServerSystem.h"
#include "PackedData.h"
#include "MHFile.h"
#include "Titan.h"
#include "Player.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "CheckRoutine.h"
#include "StatsCalcManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//DWORD CTitanManager::m_gTitanSpellDecrease = 0;

CTitanManager::CTitanManager()
{
	m_TitanInfoList.Initialize(4);
	m_ItemEnduranceList.Initialize(4);
	m_ItemUsingEnduranceList.Initialize(4);

	m_ItemEndurancePool.Init( 50, 10, "TitanEnduranceItem" );

	Init(NULL);
	/*
	m_pCurRidingTitan	= NULL;
	m_pMaster			= NULL;

	m_dwCurRegistTitanCallItemDBIdx = 0;

	m_dwRecallCheckTime = 0;

	TitanScaleForNewOne = 1000;

	m_bAvaliableEndurance = FALSE;*/

	m_dwTitanRecallProcessTime = 0;	
	m_dwCurrentTime = 0;
	m_bTitanRecall = FALSE;	
	m_bTitanRecallClient = FALSE;


	m_dwTitanEPTime = 0;
	m_dwTitanMaintainTime = 0;	// magi82(23)
}

CTitanManager::~CTitanManager()
{

}

void CTitanManager::Init( CPlayer* pPlayer )
{
	m_pMaster = pPlayer;
	m_pCurRidingTitan	= NULL;

	m_dwCurRegistTitanCallItemDBIdx = 0;

	m_dwRecallCheckTime = 0;

	TitanScaleForNewOne = 100;

	m_bAvaliableEndurance = FALSE;


	memset( m_TitanWearedInfo, 0, sizeof(TitanWearedInfo)*eTitanWearedItem_Max );

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::Release()
{
	UpDateCurTitanAndEquipItemInfo();

	m_TitanInfoList.SetPositionHead();
	TITAN_TOTALINFO* pTotalInfo = NULL;
	while( pTotalInfo = m_TitanInfoList.GetData() )
	{
		//delete pTotalInfo;
		//pTotalInfo = NULL;
		g_pServerSystem->RemoveTitanInfo(pTotalInfo);
	}
	m_TitanInfoList.RemoveAll();

	m_ItemUsingEnduranceList.RemoveAll();

	m_ItemEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
	while( pEnduranceInfo = m_ItemEnduranceList.GetData() )
	{
		delete pEnduranceInfo;
		pEnduranceInfo = NULL;
	}
	m_ItemEnduranceList.RemoveAll();

	if( m_pCurRidingTitan )
	{
		//delete m_pCurRidingTitan;
		//m_pCurRidingTitan = NULL;
		RemoveTitan();
	}

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::RemoveTitan()
{
	if(!m_pCurRidingTitan)	return;

	g_pServerSystem->RemoveTitan(m_pCurRidingTitan);

	m_pCurRidingTitan = NULL;
}

void CTitanManager::AddTitanTotalInfo( TITAN_TOTALINFO* pTitanInfo, int flagSendMsgTo/* = eServerOnly*/ )
{
	if( !pTitanInfo )	return;

	TITAN_TOTALINFO* pInfo = NULL;
	if( pInfo = m_TitanInfoList.GetData(pTitanInfo->TitanCallItemDBIdx) )
	{
		*pInfo = *pTitanInfo;
		return;
	}

	if(eServerNClient == flagSendMsgTo)
	{
		SEND_ADDTITAN_FROMITEM msg;
		memcpy(&msg.Info, pTitanInfo, sizeof(TITAN_TOTALINFO));
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_ADDNEW_FROMITEM;
		msg.dwObjectID = m_pMaster->GetID();

		m_pMaster->SendMsg(&msg, sizeof(SEND_ADDTITAN_FROMITEM));
	}

	//TITAN_TOTALINFO* pNewInfo = new TITAN_TOTALINFO;
	//memcpy(pNewInfo, pTitanInfo, sizeof(TITAN_TOTALINFO));
	TITAN_TOTALINFO* pNewInfo = g_pServerSystem->MakeTitanInfo(pTitanInfo);
	if( NULL == pNewInfo )
	{
		ASSERT(0);
		return;
	}
	if( FALSE == AddTitanTotalInfoList(pNewInfo) )
	{
		g_pServerSystem->RemoveTitanInfo( pNewInfo );
	}
}

BOOL CTitanManager::AddTitanTotalInfoList( TITAN_TOTALINFO* pTitanInfo )
{
	if(m_TitanInfoList.GetData(pTitanInfo->TitanCallItemDBIdx))
	{
		ASSERT(0);
		return FALSE;
	}

	m_TitanInfoList.Add(pTitanInfo, pTitanInfo->TitanCallItemDBIdx);
	return TRUE;
}


TITAN_TOTALINFO* CTitanManager::GetTitanTotalInfo( DWORD callItemDBIdx )
{
	TITAN_TOTALINFO* pTitanInfo = m_TitanInfoList.GetData(callItemDBIdx);

	return pTitanInfo;
}

void CTitanManager::RemoveTitanTotalInfo( DWORD callItemDBIdx )
{// ��ȯ�� ����Ʈ �󿡼��� ����
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(callItemDBIdx);

	ASSERT(pInfo);

	m_TitanInfoList.Remove(callItemDBIdx);
}

void CTitanManager::DeleteTitanTotalInfo( DWORD callItemDBIdx )
{// ��ȯ�����ۻ���, �α׾ƿ���.
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(callItemDBIdx);

	ASSERT(pInfo);
	//SAFE_DELETE(pInfo);
	if(pInfo)
		g_pServerSystem->RemoveTitanInfo(pInfo);

	m_TitanInfoList.Remove(callItemDBIdx);
}

WORD CTitanManager::GetTitanInfoList( TITAN_TOTALINFO* RtInfo )
{
	WORD TitanCount = 0;
	m_TitanInfoList.SetPositionHead();
	TITAN_TOTALINFO* pInfo = NULL;
	while(pInfo = m_TitanInfoList.GetData())
	{
		memcpy(&RtInfo[TitanCount], pInfo, sizeof(TITAN_TOTALINFO));
		++TitanCount;
	}

	return TitanCount;
}

void CTitanManager::DeleteTitan( DWORD callItemDBIdx )
{//Ÿ��ź ��ȯ ������ ������. DB ������ ���� ����
	DeleteTitanTotalInfo( callItemDBIdx );

	TitanDeleteToDB( callItemDBIdx );
}

void CTitanManager::CreateNewTitan( CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx, WORD wTitanKind, WORD wTitanGrade /*= 1*/ )
{
	BASE_TITAN_LIST* pTitanListInfo = NULL;

	pTitanListInfo = GAMERESRCMNGR->GetTitanListInfo(wTitanKind);
	if( NULL == pTitanListInfo )	return;

	TITAN_TOTALINFO TotalInfo;
	TotalInfo.TitanCallItemDBIdx = dwCallItemDBIdx;
	TotalInfo.TitanKind = wTitanKind;

	//�� ��ȹ���� ���� ����� 1�� �ƴѰ��� ����. ������ ��޺��� �������� ���� �����ϱ⶧����...
	//ASSERT(1 == wTitanGrade);
	if( 1 > wTitanGrade || 3 < wTitanGrade )
	{
		ASSERT(0);
		wTitanGrade = 1;
	}

	TotalInfo.Fuel = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxFuel;
	TotalInfo.Spell = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxSpell;
	TotalInfo.TitanGrade = wTitanGrade;
	TotalInfo.TitanMasterUserID = pMaster->GetUserID();
	TotalInfo.RegisterCharacterIdx = 0;
	TotalInfo.RecallTime = 0;
	TotalInfo.bRiding = FALSE;
	//TotalInfo.bRegistered = FALSE;
	TotalInfo.Scale = TitanScaleForNewOne;

	TitanInsertToDB(pMaster->GetUserID(), pMaster->GetID(), dwItemIdx, &TotalInfo);
}

void CTitanManager::CreateUpgradeTitan( CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx )
{
	// �ε����� ����� dwItemIdx�� ���� itemlist ���� �����κ��� �����´�.

	BASE_TITAN_LIST* pTitanListInfo = NULL;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( (WORD)dwItemIdx );

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	WORD wTitanKind = pInfo->Plus_MugongIdx;
	WORD wTitanGrade = pInfo->Plus_Value;

	pTitanListInfo = GAMERESRCMNGR->GetTitanListInfo(wTitanKind);
	if( NULL == pTitanListInfo )	return;

	TITAN_TOTALINFO TotalInfo;
	TotalInfo.TitanCallItemDBIdx = dwCallItemDBIdx;
	TotalInfo.TitanKind = wTitanKind;

	//�� ��ȹ���� ���� ����� 1�� �ƴѰ��� ����. ������ ��޺��� �������� ���� �����ϱ⶧����...
	//ASSERT(1 == wTitanGrade);
	if( 1 > wTitanGrade || 3 < wTitanGrade )
	{
		ASSERT(0);
		wTitanGrade = 1;
	}

	TotalInfo.Fuel = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxFuel;
	TotalInfo.Spell = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxSpell;
	TotalInfo.RecallTime = 0;
	TotalInfo.TitanGrade = wTitanGrade;
	TotalInfo.TitanMasterUserID = pMaster->GetUserID();
	TotalInfo.RegisterCharacterIdx = pMaster->GetID();
	TotalInfo.bRiding = FALSE;
	//TotalInfo.bRegistered = FALSE;
	TotalInfo.Scale = (WORD)TitanScaleForNewOne;

	TitanInsertToDB(pMaster->GetUserID(), pMaster->GetID(), dwItemIdx, &TotalInfo);
}

DWORD CTitanManager::GetTitanSpellDecrease()
{
//	static DWORD titanSpellDecrease = 0;
//	titanSpellDecrease = GAMERESRCMNGR->GetTitanSpellCost(g_pServerSystem->GetMapNum());
//	// Ÿ��ź ���� �ɼ����� ���� ����
//	return titanSpellDecrease;

	return GAMERESRCMNGR->GetTitanSpellCost(g_pServerSystem->GetMapNum());
}

void CTitanManager::ReduceTitanSpell( DWORD spell )
{
	if(m_pCurRidingTitan)
	{
		m_pCurRidingTitan->ReduceSpell(spell);
	}
}

// Ÿ��ź ���μ���
// ���°���ó��
// ���������� ������ ó��
void CTitanManager::TitanProcess()
{
	if( m_pCurRidingTitan
		&& FALSE == g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
		//&& GAMERESRCMNGR->GetLoadMapNum() != 101
		//&& GAMERESRCMNGR->GetLoadMapNum() != 102
		//&& GAMERESRCMNGR->GetLoadMapNum() != 104
		//&& GAMERESRCMNGR->GetLoadMapNum() != 105 )
	{
		// magi82(23)
		if( GetCurTime() > m_dwTitanMaintainTime )
		{
            GetOffTitan();
			m_dwTitanMaintainTime = 0;
		}
		//m_pCurRidingTitan->Process();
	}

	TitanRecallProcess();

}

void CTitanManager::UpDateCurTitanAndEquipItemInfo()
{
	UpdateCurTitanInfo();

	UpdateUsingEquipItemEnduranceInfo();
}

void CTitanManager::UpdateCurTitanInfo()
{
	if( !m_pCurRidingTitan )	return;

	const TITAN_TOTALINFO* pCurTitanInfo = m_pCurRidingTitan->GetTotalInfo();
	DWORD dwCallItemDBIdx = pCurTitanInfo->TitanCallItemDBIdx;
	TITAN_TOTALINFO* TInfo = GetTitanTotalInfo(dwCallItemDBIdx);
	if(!TInfo)
	{
		ASSERT(0);
		return;
	}

	*TInfo = *pCurTitanInfo;

	// ���̵��� ���� ��ȯ ������
	if( m_dwRecallCheckTime > GetCurTime() )
	{
		TInfo->RecallTime = m_dwRecallCheckTime;
	}
	else
	{
		TInfo->RecallTime = 0;
	}

	// magi82(23)
	//if( GAMERESRCMNGR->GetLoadMapNum() == 101
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 102
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 104
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 105 )
	if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
	{
		stTIME stMaintainTime, stCurTime;
		DWORD dwMilliSec = GAMERESRCMNGR->GetTitanRule()->TitanMaintainTime + m_pMaster->GetAbilityStats()->dwTitanRidingPlusTime;
		DWORD dwSec = 0;
		DWORD dwMin = 0;

		dwSec = dwMilliSec / SECTOMILLISEC;
		dwMin = dwSec / MINUTETOSECOND;
		dwSec = dwSec % MINUTETOSECOND;

		stMaintainTime.SetTime(0,0,0,0,dwMin,dwSec);
		stCurTime.SetTime(GetCurTime());
		stCurTime += stMaintainTime;
		m_dwTitanMaintainTime = stCurTime.value;

		TInfo->MaintainTime = m_dwTitanMaintainTime;
	}

	TitanUpdateToDB(m_pMaster->GetUserID(), TInfo);
}

// Ÿ��ź ž��
BOOL CTitanManager::RideInTitan( DWORD callItemDBIdx, BOOL bSummonFromUser/* = TRUE*/ )
{
	if( FALSE == CheckBeforeRideInTitan(callItemDBIdx) )
		return FALSE;

	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);

	if(!pInfo)
	{
		char szBuf[128] = {0,};
		sprintf(szBuf, "Player Have No TitanData with Item! PlayerID :%d\t ItemDBIdx:%d\n"
			,m_pMaster->GetID(),callItemDBIdx);
		ASSERTMSG(0,szBuf);
		return FALSE;
	}

	if(NULL == m_pCurRidingTitan)
	{
		//m_pCurRidingTitan = new CTitan;	// �޸�Ǯ�� �ٲٴ���..
		m_pCurRidingTitan = g_pServerSystem->MakeTitan(m_pMaster);
	}
	else
	{
		ASSERT(0);
	}

	// ��ȯ ���� �ð� �ʱ�ȭ.
	//pInfo->RecallTime = 0;

	// magi82(23)
	m_dwTitanMaintainTime = pInfo->MaintainTime;
    
	m_pCurRidingTitan->InitTitan(pInfo, m_pMaster);

	//Ŭ���̾�Ʈ�� Ÿ��ź ���� ������ �� ����.
	SetTitanStats();

	//m_pCurRidingTitan->SendTitanInfoMsg();

	//SendTitanStats();	//���ο��� ������ Ÿ��ź ���� ����

	// ��ȿ �׸��� ���� ������
	MSG_TITAN_RIDEIN msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_RIDEIN_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.OwnerID = m_pMaster->GetID();
	msg.TitanTotalInfo = *pInfo;
	GetAppearanceInfo(&msg.AppearanceInfo);

	// magi82(23)
	stTIME stCurTime, stMaintainTime;
	stMaintainTime.SetTime(m_dwTitanMaintainTime);
	stCurTime.SetTime(GetCurTime());
	stMaintainTime -= stCurTime;
	msg.MaintainTime = Convert2MilliSecond( stMaintainTime.GetDay(), stMaintainTime.GetHour(), stMaintainTime.GetMinute(), stMaintainTime.GetSecond() );

	if(TRUE == bSummonFromUser)
	{
		PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );
	}
	else
	{
		m_pMaster->SendMsg(&msg, sizeof(msg));
	}

	return TRUE;
}

void CTitanManager::CheckRidingTitan()
{
	TITAN_TOTALINFO* pInfo = NULL;

	m_TitanInfoList.SetPositionHead();
	while( pInfo = m_TitanInfoList.GetData() )
	{
		if( TRUE == pInfo->bRiding )
		{
			RideInTitan(pInfo->TitanCallItemDBIdx, FALSE);

			SetTitanRiding(FALSE);

			return;
		}
	}
}

void CTitanManager::SetTitanRiding( BOOL bVal )
{
	if(!m_pCurRidingTitan)	return;

	m_pCurRidingTitan->SetTitanRiding(bVal);
}

// Ÿ��ź ž�� ���� �˻�
BOOL CTitanManager::CheckBeforeRideInTitan( DWORD callItemDBIdx )
{
	// ��ϵ� Ÿ��ź ����
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
		return FALSE;

	if( m_pMaster->GetID() != pInfo->RegisterCharacterIdx )
		return FALSE;

	// ���� ž����
	if( m_pCurRidingTitan )
	{
		if( m_pCurRidingTitan->GetCallItemDBIdx() == callItemDBIdx )
			GetOffTitan(eFromUser);
		else
		{	//Client ���� ó��.
			ASSERT(0);

		}

		return FALSE;
	}

	// ���ȯ ������ �˻�
	// Ÿ��ź ���ᰡ �ִ���, �Ǵ� ���� �ٴ����� ȸ�� ���ȯ ������ ������
	if( !CheckTimeRecallAvailable() )
		return FALSE;

	// ������ ü���� 10% �̻�����
	DWORD perc = m_pMaster->GetLife() * 100 / m_pMaster->GetMaxLife();
	if( perc < GAMERESRCMNGR->GetTitanRule()->MasterLifeRate_forTitanCall )
		return FALSE;

	// Ÿ��ź ���� �������� ���� �ִ���
	// Ÿ��ź ���� ������ �� �������� 1% �̸��� �������� 3�� �̸�����
	// 070621 ��ȹ����
	/*if( !CheckEquipState() )
		return FALSE;*/

	return TRUE;
}

void CTitanManager::SetRecallCheckTime(DWORD checkTime /*= 0*/)
{
	if(checkTime)
	{
		if(m_dwRecallCheckTime < checkTime)
			m_dwRecallCheckTime = checkTime;	// RCharacterTitanInfo�� ���� �ߺ� �� ����
	}
	else
	{
		if(!m_dwCurRegistTitanCallItemDBIdx)
		{
			m_dwRecallCheckTime = checkTime;	// ��� ������ �̵�.
			return;
		}
		
		//TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(m_dwCurRegistTitanCallItemDBIdx);
		TITAN_TOTALINFO* pInfo = m_pCurRidingTitan->GetTotalInfo();

		if(!pInfo)
		{
			ASSERT(0);
			return;
		}

		stTIME tmp,penalTime;

		DWORD penalSec = 0;
		DWORD penalMin = 0;
		
		// magi82 - Titan(070914) Ÿ��ź ���� ������
		TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pInfo->TitanKind, pInfo->TitanGrade);
		if( !pGrade )
			return;

		// magi82(24) - 
		//if( GAMERESRCMNGR->GetLoadMapNum() == 101
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 102
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 104
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 105 )
		if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
		{
			// ��Ÿ�� ���϶�
			// ü���������� ������ ���
			if( 0 == pInfo->Fuel || 0 == pInfo->Spell )
			{
				penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_ZeroFuel - pGrade->RecallReduceTime;
			}
			else
			{
				penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_Seal - pGrade->RecallReduceTime;
			}
		}
		else
		{
			// �߿� ���϶�
			penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_General - pGrade->RecallReduceTime;
		}

		if(!penalSec)
			return;

		// magi82(26)
		if( GetTitanShopitemOption()->dwRecallReduceRate > 0.0f)
		{
			penalSec = (DWORD)(penalSec * GetTitanShopitemOption()->dwRecallReduceRate);
		}

		penalSec /= SECTOMILLISEC;
		penalMin = penalSec / MINUTETOSECOND;
		penalSec %= MINUTETOSECOND;
		penalTime.SetTime(GetCurTime());
		tmp.SetTime(0,0,0,0,penalMin,penalSec);
		penalTime += tmp;
		m_dwRecallCheckTime = penalTime.value;

		pInfo->RecallTime = m_dwRecallCheckTime;
	}
}

BOOL CTitanManager::CheckTimeRecallAvailable()
{
	BOOL bAble = FALSE;

	if(!m_dwCurRegistTitanCallItemDBIdx)	return bAble;

	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(m_dwCurRegistTitanCallItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return bAble;
	}

	bAble = (GetCurTime() > m_dwRecallCheckTime);

	// ü���������� ������ ���
	if( TRUE == bAble )
	{
		if( 0 == pInfo->Fuel )//|| 0 == pInfo->Spell )
		{
			//ü�� ȸ��
			if(bAble)
			{
				BASE_TITAN_LIST* pList = GAMERESRCMNGR->GetTitanListInfo(pInfo->TitanKind);
				//pInfo->Fuel = (pList->GradeInfo[pInfo->TitanGrade-1]).MaxFuel;
				//pInfo->Spell = (pList->GradeInfo[pInfo->TitanGrade-1]).MaxSpell;
				pInfo->Fuel = GetTitanStats()->MaxFuel;
			}
		}
	}

	
	//�����ڴ� �ð� üũ ���
	if( m_pMaster->GetUserLevel() < eUSERLEVEL_GM )
		bAble = TRUE;

	return bAble;
}

BOOL CTitanManager::CheckEquipState()
{
	BOOL bPass = ( CheckUsingEquipItemNum() && CheckUsingEquipItemEndurance() );
	return bPass;
}

// Ÿ��ź ž������
BOOL CTitanManager::GetOffTitan( int reason  /*= eNormal*/)
{
	if( !m_pCurRidingTitan )
		return FALSE;

	SetRecallCheckTime();

	UpdateCurTitanInfo();

	// Ŭ���̾�Ʈ�� ���� ���� ������
	MSG_DWORD2 msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_GETOFF_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1 = m_pMaster->GetID();
	msg.dwData2 = reason;
	PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );

	//SAFE_DELETE(m_pCurRidingTitan);
	g_pServerSystem->RemoveTitan(m_pCurRidingTitan);

	m_pCurRidingTitan = NULL;

	return TRUE;
}

void CTitanManager::ExchangeTitan( DWORD callItemDBIdx, CPlayer* pNewMaster )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->TitanMasterUserID != m_pMaster->GetUserID() )
	{
		ASSERT(0);
	}

	pInfo->TitanMasterUserID = pNewMaster->GetUserID();

	TitanUpdateToDB(pNewMaster->GetUserID(), pInfo);

	pNewMaster->GetTitanManager()->AddTitanTotalInfoList(pInfo);

	RemoveTitanTotalInfo(pInfo->TitanCallItemDBIdx);
}

// Ÿ��ź ��޾�
// ������ ����ó��(��޺� �������� �ٸ�.���� ��ȯ������ �����.���� ��� ��ȯ������ �����ϱ�)
// Ÿ��ź ��� ����ó��(DB)
void CTitanManager::UpgradeTitan( DWORD callItemDBIdx, DWORD newCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->TitanGrade >= MAX_TITANGRADE )
	{
		ASSERT(0);
		return;
	}
	pInfo->TitanGrade++;

	pInfo->TitanCallItemDBIdx = newCallItemDBIdx;

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);
}

// Ÿ��ź ���
// Ÿ��ź ���ó��(DB)
BOOL CTitanManager::RegistTitan( DWORD callItemDBIdx)

{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	if( pInfo->RegisterCharacterIdx )
	{
		ASSERT(0);
		return FALSE;
	}

	//����� ĳ���� idx 
	pInfo->RegisterCharacterIdx = m_pMaster->GetID();

	//������� ������ DBIdx
	//m_dwCurRegistTitanCallItemDBIdx = callItemDBIdx;
	SetRegistTitanCallItemDBIdx(callItemDBIdx);

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);
	
	return TRUE;
}

// Ÿ��ź �������
// Ÿ��ź �������ó��(DB)
BOOL CTitanManager::CancleTitanRegister( DWORD callItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	if( 0 == pInfo->RegisterCharacterIdx )
	{
		ASSERT(0);
		return FALSE;
	}

	//����� ĳ���� idx �ʱ�ȭ
	pInfo->RegisterCharacterIdx = 0;

	// ���ȯ ���� �ð� �ʱ�ȭ

	pInfo->RecallTime = 0;

	//������� ������ DBIdx �ʱ�ȭ
	m_dwCurRegistTitanCallItemDBIdx = 0;

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);

	return TRUE;
}

#define TITAN_POTION_DELAY 29000	// Ŭ���̾�Ʈ���� 1�� ������.. (����ȭ)
int CTitanManager::ApplyYoungYakItemToCurTitan( ITEM_INFO* pItemInfo )
{
	if(NULL == m_pCurRidingTitan)
	{
		char tmp[128];
		ASSERTMSG(0,tmp);
		return eItemUseErr_Err;
	}

	if( gCurTime - m_dwTitanEPTime <= TITAN_POTION_DELAY )
		return eItemUseErr_TitanItemTime;

	m_dwTitanEPTime = (DWORD)(gCurTime - (TITAN_POTION_DELAY * GetTitanShopitemOption()->dwEPReduceRate));	// magi82(26)

	if(pItemInfo->LifeRecoverRate)
	{
		//���� �ִ�ġ ȸ��
		AddCurTitanFuelAsRate( pItemInfo->LifeRecoverRate );
	}
	else if(pItemInfo->LifeRecover)
	{
		//Ÿ��ź ���� �߰�
		AddCurTitanFuel(pItemInfo->LifeRecover);
	}

	if(pItemInfo->NaeRyukRecoverRate)
	{
		//���� �ִ�ġ ȸ��
		AddCurTitanSpellAsRate( pItemInfo->NaeRyukRecoverRate );
	}
	else if(pItemInfo->NaeRyukRecover)
	{
		//Ÿ��ź ���� �߰�
		AddCurTitanSpell(pItemInfo->NaeRyukRecover);
	}

	return eItemUseSuccess;
}

void CTitanManager::AddCurTitanFuel( WORD amount )
{
	if(!m_pCurRidingTitan)	return;

	DWORD curFuel = m_pCurRidingTitan->GetLife();

	m_pCurRidingTitan->SetLife(curFuel + amount);
}

void CTitanManager::AddCurTitanSpell( WORD amount )
{
	if(!m_pCurRidingTitan)	return;

	DWORD curSpell = m_pCurRidingTitan->GetNaeRyuk();

	m_pCurRidingTitan->SetNaeRyuk(curSpell + amount);
}

void CTitanManager::AddCurTitanFuelAsRate( float recoverRate )
{
	if(!m_pCurRidingTitan)	return;
	
	DWORD maxFuel = m_pCurRidingTitan->GetMaxLife();

	DWORD recover = DWORD(maxFuel * recoverRate);

	m_pCurRidingTitan->SetLife(m_pCurRidingTitan->GetLife() + recover);
}


void CTitanManager::AddCurTitanSpellAsRate( float recoverRate )
{
	if(!m_pCurRidingTitan)	return;

	DWORD maxSpell = m_pCurRidingTitan->GetMaxNaeRyuk();

	DWORD recover = DWORD(maxSpell * recoverRate);

	m_pCurRidingTitan->SetNaeRyuk(m_pCurRidingTitan->GetNaeRyuk() + recover);
}

void CTitanManager::SetTitanStats()
{
	titan_calc_stats* pStats = GetTitanItemStats();

	m_titanStats = *pStats;

	// Ÿ��ź ž�� ���̸� Ÿ��ź �⺻ ������ �����ش�.
	if( m_pCurRidingTitan )
	{
		const BASE_TITAN_LIST* pList = m_pCurRidingTitan->GetTitanBaseInfo();

		DWORD grade = m_pCurRidingTitan->GetTitanGrade();

		if(pList)
		{
			const TITANINFO_GRADE* pGradeInfo = &pList->GradeInfo[grade -1];

			m_titanStats.MaxFuel	+= pGradeInfo->MaxFuel;
			m_titanStats.MaxSpell	+= pGradeInfo->MaxSpell;

			m_titanStats.Critical	+= pGradeInfo->Critical;
			m_titanStats.AttackRange	+= pGradeInfo->AttackRange;

			m_titanStats.MoveSpeed	+= pGradeInfo->Speed;

			m_titanStats.MeleeAttackPowerMin	+= pGradeInfo->MeleeAttackPowerMin;
			m_titanStats.MeleeAttackPowerMax	+= pGradeInfo->MeleeAttackPowerMax;
			m_titanStats.RangeAttackPowerMin	+= pGradeInfo->RangeAttackPowerMin;
			m_titanStats.RangeAttackPowerMax	+= pGradeInfo->RangeAttackPowerMax;
			m_titanStats.PhysicalDefense	+=	(WORD)pGradeInfo->PhyDef;
	
			ATTRIBUTEATTACK	atk = pGradeInfo->AttAtk;
			ATTRIBUTEREGIST reg = pGradeInfo->AttReg;
			// �Ӽ� ���ݷºκ�.
			m_titanStats.AttributeAttack.AddATTRIBUTE_VAL(atk);	//ApplyRate = 1 // ������ ���� ����. ��ȹ�� Ÿ��ź ������ ��� ����.
			m_titanStats.AttributeResist.AddATTRIBUTE_VAL(reg);
		}
	}
}

//=================================================================================
//������ ó�� ����
//=================================================================================

// ���������� ������ ���� ����
void CTitanManager::MakeNewEnduranceInfo( CPlayer* pOwner, ITEMBASE* pItemInfo, EnduranceException eException )
{
	// DBInsert ������ ó����..

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;

	// magi82(33)
	if( eException == eExceptionPyoguk )
        EnduranceInfo.CharacterID = 0;	// â�� ������ 0 ����
	else
		EnduranceInfo.CharacterID = pOwner->GetID();


	EnduranceInfo.ItemDBIdx = pItemInfo->dwDBIdx;
	EnduranceInfo.ItemIdx = pItemInfo->wIconIdx;
	//EnduranceInfo.bEquip = FALSE;
	EnduranceInfo.Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

	// magi82(33)
	if( eException == eExceptionPyoguk )
		EnduranceInfo.UserIdx = pOwner->GetUserID();	// â�� ������ �����Ѵ�.
	else
		EnduranceInfo.UserIdx = 0;
	

	//EnduranceInfo.OwnerUserID = pOwner->GetUserID();
	//EnduranceInfo.OwnerCharacterID = pOwner->GetID();

	// magi82(33)
	if( eException == eExceptionNone )
	{

		// ������ ���������� Ÿ��ź ���� �������� ȹ�� �Ͽ�����..
		TitanEquipItemInfoInsertToDB(&EnduranceInfo);
	}
	else if( eException == eExceptionPyoguk )
	{
		// �̺�Ʈ �� �ٸ� ���ܻ������� â�� ��� Ÿ��ź ������������ DB�� ��� �ؾ��Ҷ�..
		TitanEquipInsertExceptionPyogukToDB(pOwner->GetID(), &EnduranceInfo);
	}
	else
	{
		// �̺�Ʈ �� �ٸ� ���ܻ������� ���ӿ� ���ӽ� Ÿ��ź ������������ DB�� ��� �ؾ��Ҷ�..
		TitanEquipInsertExceptionToDB(&EnduranceInfo);
	}
	//AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
}

void CTitanManager::DeleteTitanEquip( DWORD itemDBIdx )
{//Ÿ��ź ���� ������ ������. DB ������ ���� ����
	TitanEquipItemInfoDeleteToDB(itemDBIdx);

	RemoveTitanUsingEquipItemList(itemDBIdx);
	DeleteTitanEquipItemInfo(itemDBIdx);
}

// Ÿ��ź ���� ������ ������ó�� ����
void CTitanManager::AddTitanEquipItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int flagSendMsgTo /*= eServerOnly*/ )
{
	if( !pEnduranceInfo )	return;

	if( m_ItemEnduranceList.GetData(pEnduranceInfo->ItemDBIdx) )
	{
		return;
	}

	if(eServerNClient == flagSendMsgTo)
	{
		SEND_ADDTITANEQUIP_FROMITEM msg;
		memcpy(&msg.Info, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_ADDNEW_EQUIP_FROMITEM;
		msg.dwObjectID = m_pMaster->GetID();

		m_pMaster->SendMsg(&msg, sizeof(SEND_ADDTITANEQUIP_FROMITEM));
	}

	//TITAN_ENDURANCE_ITEMINFO* pNewInfo = m_ItemEndurancePool.Alloc();
	TITAN_ENDURANCE_ITEMINFO* pNewInfo = new TITAN_ENDURANCE_ITEMINFO;
	if( NULL == pNewInfo )
	{
		ASSERT(0);
		return;
	}
	memcpy(pNewInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));

	if( FALSE == AddTitanEquipItemEnduranceInfoList(pNewInfo) )
	{
		SAFE_DELETE(pNewInfo);
	}
}

BOOL CTitanManager::AddTitanEquipItemEnduranceInfoList( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo )
{
	if(m_ItemEnduranceList.GetData(pEnduranceInfo->ItemDBIdx))
	{
		return FALSE;
	}

	m_ItemEnduranceList.Add(pEnduranceInfo, pEnduranceInfo->ItemDBIdx);
	return TRUE;
}


TITAN_ENDURANCE_ITEMINFO* CTitanManager::GetTitanItemEnduranceInfo(DWORD equipItemDBIdx)
{
	return m_ItemEnduranceList.GetData(equipItemDBIdx);
}

void CTitanManager::RemoveTitanEquipItemInfo( DWORD equipItemDBIdx )
{// ��ȯ�� ����Ʈ �󿡼��� ����
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(equipItemDBIdx);

	ASSERT(pInfo);

	m_ItemEnduranceList.Remove(equipItemDBIdx);
}

void CTitanManager::DeleteTitanEquipItemInfo( DWORD equipItemDBIdx )
{// ���������� �ı�, �Ǵ� ������.
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(equipItemDBIdx);

	ASSERT(pInfo);
	SAFE_DELETE(pInfo);

	m_ItemEnduranceList.Remove(equipItemDBIdx);
}

WORD CTitanManager::GetTitanEquipItemInfoList( TITAN_ENDURANCE_ITEMINFO* RtInfo )
{
	WORD infoCount = 0;
	m_ItemEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	while(pInfo = m_ItemEnduranceList.GetData())
	{
		memcpy(&RtInfo[infoCount], pInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
		++infoCount;
	}

	return infoCount;
}

void CTitanManager::DiscardTitanEquipItem( POSTYPE whatPos, WORD whatItemIdx, DWORD titanEquipItemDBIdx )
{
	if( EI_TRUE == ITEMMGR->DiscardItem(m_pMaster, whatPos, whatItemIdx, 1) )
	{
		DeleteTitanEquip(titanEquipItemDBIdx);

		MSG_ITEM_DISCARD_ACK msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_TITAN_DISCARD_EQUIPITEM;
		msg.wItemIdx = whatItemIdx;
		msg.TargetPos = whatPos;
		msg.ItemNum = 1;
		m_pMaster->SendMsg( &msg, sizeof(msg) );
	}
	else
	{
		ASSERT(0);		
	}
}

//== Ÿ��ź ����� ���� ������ ���� (for DBUpdate)
void CTitanManager::AddTitanUsingEquipItemList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo)
{
	if( !pEnduranceInfo )	return;

	if( m_ItemUsingEnduranceList.GetData(pEnduranceInfo->ItemDBIdx) )
	{
		return;
	}

	m_ItemUsingEnduranceList.Add(pEnduranceInfo, pEnduranceInfo->ItemDBIdx);
}

void CTitanManager::RemoveTitanUsingEquipItemList(DWORD equipItemDBIdx)
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemUsingEnduranceList.GetData(equipItemDBIdx);

	//ASSERT(pInfo);

	if(pInfo)
		m_ItemUsingEnduranceList.Remove(equipItemDBIdx);

}

void CTitanManager::UpdateUsingEquipItemEnduranceInfo()
{
	m_ItemUsingEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	while(pInfo = m_ItemUsingEnduranceList.GetData())
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
	}
}

void CTitanManager::GetAppearanceInfo( TITAN_APPEARANCEINFO* RtInfo )
{
	if( m_pCurRidingTitan )
	{
		RtInfo->TitanKind = (WORD)(m_pCurRidingTitan->GetTitanKind());
		RtInfo->TitanGrade = (WORD)(m_pCurRidingTitan->GetTitanGrade());
		RtInfo->TitanScale = (WORD)(m_pCurRidingTitan->GetTotalInfo()->Scale);
	}
	for( int i = 0; i < eTitanWearedItem_Max; ++ i )
	{
		RtInfo->WearedItemIdx[i] = GetWearedInfo( i )->TitanEquipItemIdx;
	}
}

BOOL CTitanManager::CheckUsingEquipItemNum()
{// ����. ������ ���� �ִ���
	BOOL bTrue = (m_ItemUsingEnduranceList.GetDataNum() == eTitanWearedItem_Max);
	return bTrue;
}

BOOL CTitanManager::CheckUsingEquipItemEndurance()
{// ������ 1% 2�� ��������
	DWORD perc = 0;
	int cnt = 0;
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemUsingEnduranceList.SetPositionHead();
	while(pInfo = m_ItemUsingEnduranceList.GetData())
	{
		perc = pInfo->Endurance * 10000 / TITAN_EQUIPITEM_ENDURANCE_MAX ;
		if( perc < 100 )	// 1% �̸�
			cnt++;
	}

	m_bAvaliableEndurance = ( cnt < 3 );
	return m_bAvaliableEndurance;
}

// ���������� ������ ����
void CTitanManager::DoRandomDecrease( int flgPoint )
{
	DWORD Rnd = rand()%100;

	// 1�ܰ� Ȯ��.
	if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseEndurance )
		return;

	Rnd = rand()%100;

	DWORD targetItemDBIdx = 0;

	if( AtAtk == flgPoint )
	{// 2�ܰ� Ȯ��.
		if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseArmorEndurance_Atk )
		{
			//���� ������ �϶�
			targetItemDBIdx = GetWearedInfo(eTitanWearedItem_Weapon)->TitanEquipItemDBIdx;
		}
		else
		{
			//�� ������ �϶�
			targetItemDBIdx = GetWearedInfo( rand()% eTitanWearedItem_Weapon )->TitanEquipItemDBIdx;
		}
	}
	else if( AtDef == flgPoint )
	{
		if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseArmorEndurance_Def )
		{
			//���� ������ �϶�
			targetItemDBIdx = GetWearedInfo(eTitanWearedItem_Weapon)->TitanEquipItemDBIdx;
		}
		else
		{
			//�� ������ �϶�
			targetItemDBIdx = GetWearedInfo( rand()% eTitanWearedItem_Weapon )->TitanEquipItemDBIdx;
		}	
	}

	if( 0 == targetItemDBIdx )
	{
		int i = 0;
		while(!targetItemDBIdx)
		{
			targetItemDBIdx = GetWearedInfo(i)->TitanEquipItemDBIdx;
			++i;

			if(i > eTitanWearedItem_Max)
				break;
		}
	}

	if(targetItemDBIdx)
	{
		TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(targetItemDBIdx);

		if(pInfo)
		{
			DWORD amount = GetDecreaseEnduranceFromItemIdx(pInfo->ItemIdx);
			ASSERT(amount);
			MinusItemEndurance( targetItemDBIdx, amount );
		}
	}
}

DWORD CTitanManager::GetDecreaseEnduranceFromItemIdx( DWORD titanEquipItemIdx )
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)titanEquipItemIdx );

	if(pItemInfo)
	{
		return pItemInfo->AllPlus_Kind;
	}
	else
		return 0;
}

void CTitanManager::PlusItemEndurance( DWORD titanEquipItemDBIdx, DWORD increaseAmount )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(titanEquipItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( 0 == increaseAmount )
		return;

	// 070621 ��ȹ����
	DWORD tmp = pInfo->Endurance;
	
	pInfo->Endurance += increaseAmount;

	if(pInfo->Endurance > TITAN_EQUIPITEM_ENDURANCE_MAX)
		pInfo->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

	//TitanEquipItemInfoUpdateToDB(pInfo);

	//if( increaseAmount ) %�ٲ𶧸� �������� ����
	if( m_pMaster->GetUserLevel() > eUSERLEVEL_GM )
	if( (tmp*100)/TITAN_EQUIPITEM_ENDURANCE_MAX != ((pInfo->Endurance)*100)/TITAN_EQUIPITEM_ENDURANCE_MAX )
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
		// ������ ���� ������
		SendItemEnduranceInfo(pInfo);
	}
}

void CTitanManager::MinusItemEndurance( DWORD titanEquipItemDBIdx, DWORD decreaseAmount )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(titanEquipItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( 0 == decreaseAmount )
		return;

	DWORD tmp = pInfo->Endurance;
	// 070621 ��ȹ����
	/*

	if( tmp - decreaseAmount > pInfo->Endurance )
	{
		pInfo->Endurance = 0;
		// �ش� ������ ����.
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)pInfo->ItemIdx );
		POSTYPE pos = TP_TITANWEAR_START + (pItemInfo->ItemKind - eTITAN_EQUIPITEM_HELMET);

		DiscardTitanEquipItem(pos,pItemInfo->ItemIdx,titanEquipItemDBIdx);
	}
	else
		pInfo->Endurance -= decreaseAmount;*/
	if( pInfo->Endurance < decreaseAmount )
	{
		pInfo->Endurance = 0;
		STATSMGR->CalcTitanItemStats(m_pMaster);
	}
	else
		pInfo->Endurance -= decreaseAmount;

	//TitanEquipItemInfoUpdateToDB(pInfo);

	//if( decreaseAmount )
	if( m_pMaster->GetUserLevel() > eUSERLEVEL_GM )
	if( (tmp*100)/TITAN_EQUIPITEM_ENDURANCE_MAX != ((pInfo->Endurance)*100)/TITAN_EQUIPITEM_ENDURANCE_MAX )
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
		// ������ ���� ������
		SendItemEnduranceInfo(pInfo);
	}
}

void CTitanManager::SendItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	MSG_DWORD2 msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_ENDURANCE_UPDATE;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1 = pInfo->ItemDBIdx;
	msg.dwData2 = pInfo->Endurance;

	m_pMaster->SendMsg(&msg, sizeof(msg));
}

void CTitanManager::SetWearedInfo( POSTYPE pos, ITEMBASE* pItemBase, BOOL bIn )
{
	WORD wearNum = pos - TP_TITANWEAR_START;

	ASSERT(wearNum>=0 && wearNum<eTitanWearedItem_Max);

	WORD itemIdx = 0;
	DWORD itemDBIdx = 0;

	if(pItemBase)
	{
		itemIdx = pItemBase->wIconIdx;
		itemDBIdx = pItemBase->dwDBIdx;
	}
	else
	{
		itemIdx = 0;
		itemDBIdx = 0;
	}

	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
	if(!pInfo)
	{
///		ASSERT(0);
	}

	if(bIn)
	{
		AddTitanUsingEquipItemList(pInfo);
		m_TitanWearedInfo[wearNum].TitanEquipItemIdx = itemIdx;
		m_TitanWearedInfo[wearNum].TitanEquipItemDBIdx = itemDBIdx;
	}
	else
	{
		RemoveTitanUsingEquipItemList(pItemBase->dwDBIdx);
		m_TitanWearedInfo[wearNum].TitanEquipItemIdx = 0;
		m_TitanWearedInfo[wearNum].TitanEquipItemDBIdx = 0;
	}
}

TitanWearedInfo* CTitanManager::GetWearedInfo( int equipKind )
{
	return &m_TitanWearedInfo[equipKind];
}

void CTitanManager::ExchangeTitanEquipItem( DWORD dwItemDBIdx, CPlayer* pNewOwner )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(dwItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->CharacterID != m_pMaster->GetID() )
	{
		ASSERT(0);
	}

	pInfo->CharacterID = pNewOwner->GetID();

	TitanEquipItemInfoUpdateToDB(pInfo);	// magi82 - DB�� ����

	pNewOwner->GetTitanManager()->AddTitanEquipItemEnduranceInfoList(pInfo);	// magi82 - ���ο� ������ ������ ����Ʈ�� �߰�

	RemoveTitanEquipItemInfo(pInfo->ItemDBIdx);		// magi82 - ���������� ����Ʈ���� ����
}

void CTitanManager::SendTitanStats()
{
	SEND_TITANSTATINFO msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_STATINFO;
	msg.dwObjectID = m_pMaster->GetID();
	msg.Info = m_titanItemStats;

	m_pMaster->SendMsg(&msg, sizeof(SEND_TITANSTATINFO));
}

void CTitanManager::SendTitanPartsChange()
{
	SEND_TITAN_APPEARANCEINFO InfoMsg;
	InfoMsg.Category = MP_ITEM;
	InfoMsg.Protocol = MP_ITEM_TITAN_APPEARANCE_CHANGE;
	InfoMsg.OwnerID = m_pMaster->GetID();
    GetAppearanceInfo(&InfoMsg.titanAppearanceInfo);

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pMaster, &InfoMsg, sizeof(InfoMsg));
}

// magi82 - Titan(070619)
void CTitanManager::SetTitanRepairEquipItem( CPlayer* pPlayer, MSG_DWORD2* pmsg )
{
	// ������ ��ȿ�� �˻�
	if( !CHKRT->ItemOf(pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 0, 0, CB_EXIST|CB_ICONIDX) )
	{
		ASSERT(0);
		return;
	}

	ITEMBASE* pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	DWORD dwDBIdx = pItemBase->dwDBIdx;
	TITAN_ENDURANCE_ITEMINFO* pEndurance = GetTitanItemEnduranceInfo(dwDBIdx);
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);

	if(pEndurance->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
	{
        // magi82 - Titan(070619) ������ ����
		float fEnduranceRate = (float)pEndurance->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
		if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
		{
			fEnduranceRate = 1.0f;
		}
		DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pInfo->BuyPrice / 100) * 0.09f );

		if( pPlayer->GetMoney() < dwRepairPrice )
		{
			// Ŭ�󿡰� ������ �����ϴٰ� ����
			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData = 0;	// 0�̸� ������ ����, 1�̸� �ش�������� ������ Ǯ, 2�� ��� �������� ������ Ǯ
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		else
		{
			// ������ ����
			pPlayer->SetMoney(dwRepairPrice, MONEY_SUBTRACTION);

			// ����
			pEndurance->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

			// DB�� ����
			TitanEquipItemInfoUpdateToDB(pEndurance);

			// Ŭ�󿡰� �����޼��� ����
			MSG_TITAN_REPAIR_EQUIPITEM_SYN msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_ACK );
			msg.dwObjectID = pPlayer->GetID();
			CopyMemory( &msg.RepairInfo, pEndurance, sizeof(TITAN_ENDURANCE_ITEMINFO) );
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
	}
	else
	{
		// Ŭ�󿡰� �̹� �������� Ǯ�� �������̶�� �˷���
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 1;	// 0�̸� ������ ����, 1�̸� �ش�������� ������ Ǯ, 2�� ��� �������� ������ Ǯ
		pPlayer->SendMsg(&msg,sizeof(msg));
	}
}

// magi82 - Titan(070619)
void CTitanManager::SetTitanRepairTotalEquipItem( CPlayer* pPlayer, MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* pmsg )
{

	// ������ ��ȿ�� �˻�
	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		if( !CHKRT->ItemOf(pPlayer, (POSTYPE)pmsg->sRepairInfo[i].dwItemPos, (WORD)pmsg->sRepairInfo[i].dwItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		{
			ASSERT(0);
			return;
		}
	}

	MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK msg;
	msg.Init();
	SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_ACK );
	msg.dwObjectID = pPlayer->GetID();
	DWORD dwMoney = 0;

	// ������ ���
	ITEMBASE* pItemBase = NULL;
	TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
	ITEM_INFO* pInfo = NULL;

	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->sRepairInfo[i].dwItemPos);
		if( !pItemBase )
		{
			ASSERT(0);
			continue;
		}

		pEnduranceInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
		if( !pEnduranceInfo )
		{
			ASSERT(0);
			continue;
		}

		pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if( !pInfo )
		{
			ASSERT(0);
			continue;
		}

		if(pEnduranceInfo->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
		{
			float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
			if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
			{
				fEnduranceRate = 1.0f;
			}
			DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pInfo->BuyPrice / 100) * 0.09f );

			dwMoney += dwRepairPrice;
		}
	}

	if( pPlayer->GetMoney() < dwMoney )
	{
		// Ŭ�󿡰� ������ �����ϴٰ� ����
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 0;	// 0�̸� ������ ����, 1�̸� �ش�������� ������ Ǯ, 2�� ��� �������� ������ Ǯ
		pPlayer->SendMsg(&msg,sizeof(msg));
		return;
	}

	// ����
	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->sRepairInfo[i].dwItemPos);
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);

		if(pEnduranceInfo->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
		{
			// ����
            pEnduranceInfo->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

			// DB�� ����
			TitanEquipItemInfoUpdateToDB(pEnduranceInfo);

			msg.AddRepairInfo(pEnduranceInfo);
		}
	}

	if(msg.wRepairNum == 0)
	{
		// Ŭ�󿡰� �̹� �������� Ǯ�� �������̶�� �˷���
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 2;// 0�̸� ������ ����, 1�̸� �ش�������� ������ Ǯ, 2�� ��� �������� ������ Ǯ
		pPlayer->SendMsg(&msg,sizeof(msg));
	}
	else
	{
		// ������ ����
		pPlayer->SetMoney(dwMoney, MONEY_SUBTRACTION);
		pPlayer->SendMsg(&msg,msg.GetSize());
	}
}
//////////////////////////////////////////////////////////////////////////

///// 2007. 9. 13. CBH
BOOL CTitanManager::TitanRecallStartSyn( DWORD callItemDBIdx, BOOL bSummonFromUser/* = TRUE*/ )	//Ÿ��ź ��ȯ ����
{
	if( FALSE == CheckBeforeRideInTitan(callItemDBIdx) )
		return FALSE;

	MSGBASE msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_RECALL_ACK;
	msg.dwObjectID = m_pMaster->GetID();
			
	PACKEDDATA_OBJ->QuickSend(m_pMaster, &msg, sizeof(msg));
	
	return TRUE;
}

void CTitanManager::TitanRecallProcess()
{
	if(m_bTitanRecall == FALSE)
	{
		return;
	}

	m_dwCurrentTime = gCurTime;
	if(m_dwTitanRecallProcessTime < m_dwCurrentTime)		
	{
		//Ŭ���̾�Ʈ���� ī��Ʈ�� �Ϸ�Ǹ� ž��
		if(m_bTitanRecallClient == TRUE)
		{
			RideInTitan();
			InitTitanRecall();		
		}		
	}
}

void CTitanManager::InitTitanRecall()
{
	m_dwTitanRecallProcessTime = 0;
	m_dwCurrentTime = 0;
	m_bTitanRecall = FALSE;
	m_bTitanRecallClient = FALSE;
}

void CTitanManager::SetRecallProcessTime(DWORD dwRecallProcessTime)
{
	m_dwTitanRecallProcessTime = dwRecallProcessTime;
}

void CTitanManager::StartTitanRecall()
{	
	m_dwCurrentTime = gCurTime;
	m_bTitanRecall = TRUE;
}

BOOL CTitanManager::IsTitanRecall()
{
	return m_bTitanRecall;
}

// Ÿ��ź ž��
BOOL CTitanManager::RideInTitan()
{
	DWORD callItemDBIdx = GetRegistTitanCallItemDBIdx();
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);	

	if( pInfo == NULL)
	{
		return FALSE;
	}

	if(NULL == m_pCurRidingTitan)
	{
		//m_pCurRidingTitan = new CTitan;	// �޸�Ǯ�� �ٲٴ���..
		m_pCurRidingTitan = g_pServerSystem->MakeTitan(m_pMaster);
	}
	else
	{
		ASSERT(0);
	}

	// magi82(23)
	stTIME stMaintainTime, stCurTime;
	DWORD dwMilliSec = GAMERESRCMNGR->GetTitanRule()->TitanMaintainTime + m_pMaster->GetAbilityStats()->dwTitanRidingPlusTime;
	DWORD dwSec = 0;
	DWORD dwMin = 0;

	dwSec = dwMilliSec / SECTOMILLISEC;
	dwMin = dwSec / MINUTETOSECOND;
	dwSec = dwSec % MINUTETOSECOND;

	stMaintainTime.SetTime(0,0,0,0,dwMin,dwSec);
	stCurTime.SetTime(GetCurTime());
	stCurTime += stMaintainTime;
	m_dwTitanMaintainTime = stCurTime.value;

	pInfo->MaintainTime = m_dwTitanMaintainTime;
	m_pCurRidingTitan->InitTitan(pInfo, m_pMaster);

	//Ŭ���̾�Ʈ�� Ÿ��ź ���� ������ �� ����.
	SetTitanStats();

	// ��ȿ �׸��� ���� ������
	MSG_TITAN_RIDEIN msg;
	msg.Category = MP_TITAN;

	msg.Protocol = MP_TITAN_RIDEIN_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.OwnerID = m_pMaster->GetID();
	msg.MaintainTime = dwMilliSec;
	msg.TitanTotalInfo = *pInfo;
	GetAppearanceInfo(&msg.AppearanceInfo);

	PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );
	
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////

void CTitanManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_TITAN_RECALL_SYN:
		{
			m_bTitanRecallClient = TRUE;
		}		
		break;
	case MP_TITAN_RECALL_CANCEL_SYN:
		{
			//Ÿ��ź ��ȯ ī��Ʈ���̸� ��� �Ϸ�
			if(m_bTitanRecall == TRUE)
			{
				InitTitanRecall();

				MSGBASE msg;
				msg.Category = MP_TITAN;
				msg.Protocol = MP_TITAN_RECALL_CANCEL_ACK;
				msg.dwObjectID = m_pMaster->GetID();
								
				PACKEDDATA_OBJ->QuickSend(m_pMaster, &msg, sizeof(msg));
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_TITAN;
				msg.Protocol = MP_TITAN_RECALL_CANCEL_NACK;
				msg.dwObjectID = m_pMaster->GetID();

				m_pMaster->SendMsg(&msg, sizeof(msg));				
			}
		}
		break;	
	default:
		break;
	}
}

void CTitanManager::MoveTitanEquipItemUpdateToDB(ITEMBASE* pFromItem, ITEMBASE* pToItem, POSTYPE FromPos, POSTYPE ToPos)
{
	// From -> To
	ITEM_INFO* pFromItemInfo = ITEMMGR->GetItemInfo(pFromItem->wIconIdx);
	if(pFromItemInfo && pFromItemInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = m_pMaster->GetTitanManager()->GetTitanItemEnduranceInfo(pFromItem->dwDBIdx);
		if( pEnduranceInfo )
		{
			BOOL bFlag = FALSE;

			// â���� �κ����� �ű涧(�ű���� �κ��� ������ ����ִ�.)
			if( (TP_INVENTORY_START <= pFromItem->Position && pFromItem->Position < TP_INVENTORY_END) && (TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END) )
			{
				pEnduranceInfo->CharacterID = m_pMaster->GetID();
				pEnduranceInfo->UserIdx = 0;	// â�� �������� ����
				bFlag = TRUE;
			}
			// �κ����� â��� �ű涧(�ű���� â���� ������ ����ִ�.)
			else if( (TP_PYOGUK_START <= pFromItem->Position && pFromItem->Position < TP_PYOGUK_END) && (TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END) )
			{
				pEnduranceInfo->CharacterID = 0;	// ĳ���� ������ �������� ����
				pEnduranceInfo->UserIdx = m_pMaster->GetUserID();
				bFlag = TRUE;
			}

			if( bFlag == TRUE )
			{
				TitanEquipItemInfoUpdateToDB( pEnduranceInfo );
			}
		}
	}

	// ToPos -> FromPos
	ITEM_INFO* pToItemInfo = ITEMMGR->GetItemInfo(pToItem->wIconIdx);
	if(pToItemInfo && pToItemInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = m_pMaster->GetTitanManager()->GetTitanItemEnduranceInfo(pToItem->dwDBIdx);
		if( pEnduranceInfo )
		{
			BOOL bFlag = FALSE;

			// â���� �κ����� �ű涧(�ű���� �κ��� ������ ����ִ�.)
			if( (TP_INVENTORY_START <= pToItem->Position && pToItem->Position < TP_INVENTORY_END) && (TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END) )
			{
				pEnduranceInfo->CharacterID = m_pMaster->GetID();
				pEnduranceInfo->UserIdx = 0;	// â�� �������� ����
				bFlag = TRUE;
			}
			// �κ����� â��� �ű涧(�ű���� â���� ������ ����ִ�.)
			else if( (TP_PYOGUK_START <= pToItem->Position && pToItem->Position < TP_PYOGUK_END) && (TP_INVENTORY_START <= ToPos && ToPos < TP_INVENTORY_END) )
			{
				pEnduranceInfo->CharacterID = 0;	// ĳ���� ������ �������� ����
				pEnduranceInfo->UserIdx = m_pMaster->GetUserID();
				bFlag = TRUE;
			}

			if( bFlag == TRUE )
			{
				TitanEquipItemInfoUpdateToDB( pEnduranceInfo );
			}
		}
	}
}

// magi82(26)
void CTitanManager::SetTitanShopitemOption( DWORD dwItemIdx, BOOL bAdd )
{
    ITEM_INFO* pInfo = ITEMMGR->GetItemInfo((WORD)dwItemIdx);
	if( !pInfo || pInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP )
		return;

	// bAdd�� TRUE �̸� �����ְ� FALSE �̸� ���ش�.
	int nAddMark = 1;
	if( bAdd == FALSE )
		nAddMark = -1;

	if( pInfo->GenGol )
	{
		m_TitanShopitemOption.dwSummonReduceTime += (pInfo->GenGol * nAddMark);

		if( m_TitanShopitemOption.dwSummonReduceTime < 0 )
			m_TitanShopitemOption.dwSummonReduceTime = 0;
	}

	if( pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) )
	{
        m_TitanShopitemOption.dwRecallReduceRate += (pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) * nAddMark);

		if( m_TitanShopitemOption.dwRecallReduceRate < 0.0f )
			m_TitanShopitemOption.dwRecallReduceRate = 0.0f;
	}

	if( pInfo->AttrRegist.GetElement_Val(ATTR_WATER) )
	{
        m_TitanShopitemOption.dwEPReduceRate += (pInfo->AttrRegist.GetElement_Val(ATTR_WATER) * nAddMark);

		if( m_TitanShopitemOption.dwEPReduceRate < 0.0f )
			m_TitanShopitemOption.dwEPReduceRate = 0.0f;
	}
}


