



// LootingManager.cpp: implementation of the CLootingManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LootingManager.h"
#include "LootingRoom.h"
#include "Player.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLootingManager::CLootingManager()
{
	m_htLootingRoom.Initialize( 100 );
	m_htDeleteRoom.Initialize( 10 );
	m_mpLootingRoom = new CMemoryPoolTempl<CLootingRoom>;
	m_mpLootingRoom->Init( 100, 10, "CLootingRoom" );
}

CLootingManager::~CLootingManager()
{
	m_htLootingRoom.SetPositionHead();
	CLootingRoom* pRoom = NULL;
	while( pRoom = m_htLootingRoom.GetData() )
	{
		m_mpLootingRoom->Free( pRoom );
	}
	m_htLootingRoom.RemoveAll();
	m_htDeleteRoom.RemoveAll();
	SAFE_DELETE( m_mpLootingRoom );	

}

BOOL CLootingManager::IsLootingSituation( CPlayer* pDiePlayer, CObject* pAttacker )
{
	if( pAttacker->GetObjectKind() != eObjectKind_Player )
		return FALSE;


	CPlayer* pAttackPlayer = (CPlayer*)pAttacker;

	if( pAttackPlayer->GetState() == eObjectState_Die )
		return FALSE;

	if( pAttackPlayer->IsPKMode() )		
		return TRUE;
	
	if( pDiePlayer->IsPKMode() )	
		return TRUE;

	if( pAttackPlayer->IsWantedOwner( pDiePlayer->GetWantedIdx() ) )
		return TRUE;

	return FALSE;
}

int CLootingManager::AutoLooting( CPlayer* pDiePlayer, CPlayer* pAttackPlayer )
{

	DWORD dwDiePlayerID = pDiePlayer->GetID();
	CloseLootingRoom( dwDiePlayerID, FALSE );
	CLootingRoom* pRoom = m_mpLootingRoom->Alloc();
	pRoom->MakeLootingRoom( pDiePlayer, pAttackPlayer );
	
	int nArrayNum = rand()%PKLOOTING_ITEM_NUM;
	pRoom->Loot( nArrayNum );

	int nLootedItemNum = pRoom->GetLootedItemCount();

	m_mpLootingRoom->Free( pRoom );

	return nLootedItemNum;
}

void CLootingManager::CreateLootingRoom( CPlayer* pDiePlayer, CPlayer* pAttackPlayer )
{
	CloseLootingRoom( pDiePlayer->GetID(), FALSE );
	CLootingRoom* pRoom = m_mpLootingRoom->Alloc();
	pRoom->MakeLootingRoom( pDiePlayer, pAttackPlayer );

	m_htLootingRoom.Add( pRoom, pDiePlayer->GetID() );

	MSG_DWORD msg;
	msg.Category	= MP_PK;
	msg.Protocol	= MP_PK_LOOTING_START;
	msg.dwData		= pDiePlayer->GetID();
	pAttackPlayer->SendMsg( &msg, sizeof(msg) );

	msg.Protocol	= MP_PK_LOOTING_BEINGLOOTED;
	msg.dwData		= pAttackPlayer->GetID();
	pDiePlayer->SendMsg( &msg, sizeof(msg) );

	pRoom->SetLootingStart();
}


CLootingRoom* CLootingManager::GetLootingRoom( DWORD dwDiePlayer )
{
	return m_htLootingRoom.GetData( dwDiePlayer );
}


void CLootingManager::Loot( DWORD dwDiePlayer, int nArrayNum )
{
	CLootingRoom* pRoom = m_htLootingRoom.GetData( dwDiePlayer );

	if( pRoom->Loot( nArrayNum ) == FALSE )	
	{
		CloseLootingRoom( dwDiePlayer, TRUE );	
		return;
	}
}



int CLootingManager::GetLootingChance( DWORD dwBadFame )
{
	if( dwBadFame < 100000 )
		return 3;
	else if( dwBadFame < 500000 )
		return 4;
	else if( dwBadFame < 1000000 )
		return 5;
	else if( dwBadFame < 5000000 )
		return 6;
	else if( dwBadFame < 10000000 )
		return 7;
	else if( dwBadFame < 50000000 )
		return 8;
	else if( dwBadFame < 100000000 )
		return 9;
	else
		return 10;
}

int CLootingManager::GetLootingItemNum( DWORD dwBadFame )
{
	if (dwBadFame < 30)
		return 1;
	else if( dwBadFame < 100000000 )
		return 2;
	else if( dwBadFame < 400000000 )
		return 3;
	else if( dwBadFame < 700000000 )
		return 4;
	else if( dwBadFame < 1000000000 )
		return 5;
	else
		return 6;
}


int CLootingManager::GetWearItemLootingRatio( DWORD dwBadFame )
{

	if( dwBadFame < 30 )
		return 0;
	if( dwBadFame < 50 )
		return 1;
	if( dwBadFame < 500 )
		return 2;
	else if( dwBadFame < 4000 )
		return 10;
	else if( dwBadFame < 20000 )
		return 20;
	else if( dwBadFame < 80000 )
		return 30;
	else if( dwBadFame < 400000 )
		return 40;
	else if( dwBadFame < 1600000 )
		return 50;
	else if( dwBadFame < 8000000 )
		return 60;
	else if( dwBadFame < 32000000 )
		return 70;
	else if( dwBadFame < 100000000 )
		return 85;
	else if( dwBadFame < 500000000 )
		return 100;
	else
		return 100;
}

void CLootingManager::CloseLootingRoom( DWORD dwDiePlayer, BOOL bMsgToClient )
{
	CLootingRoom* pRoom = m_htLootingRoom.GetData( dwDiePlayer );
	if( pRoom )
	{
		pRoom->EndLootingRoomMsg( bMsgToClient );	//end Msg를 보낸다.
		m_htLootingRoom.Remove( dwDiePlayer );
		m_mpLootingRoom->Free( pRoom );
	}
}

BOOL CLootingManager::IsLootedPlayer( DWORD dwPlayerIdx )
{
	CLootingRoom* pRoom = m_htLootingRoom.GetData( dwPlayerIdx );
	if( pRoom )
		return TRUE;
	return FALSE;
}

void CLootingManager::DiePlayerExitPanelty( CPlayer* pPlayer )
{
	CLootingRoom* pRoom = m_htLootingRoom.GetData( pPlayer->GetID() );
	if( !pRoom )
		return;

	pRoom->DiePlayerExitPanelty();
}

void CLootingManager::ProcessTimeOutCheck()
{
	m_htLootingRoom.SetPositionHead();
	CLootingRoom* pRoom = NULL;
	while( pRoom = m_htLootingRoom.GetData() )
	{
		if( pRoom->IsTimeOut() )
		{
			m_htDeleteRoom.Add( pRoom, pRoom->GetDiePlayerID() );
		}
	}

	m_htDeleteRoom.SetPositionHead();
	while( pRoom = m_htDeleteRoom.GetData() )
	{
		CloseLootingRoom( pRoom->GetDiePlayerID(), TRUE );
	}
	m_htDeleteRoom.RemoveAll();
}

void CLootingManager::LootingCancel( CPlayer* pPlayer )
{
	DWORD	dwAttackerID = pPlayer->GetID();

	m_htLootingRoom.SetPositionHead();
	CLootingRoom* pRoom = NULL;
	while( pRoom = m_htLootingRoom.GetData() )
	{
		if( pRoom->GetAttackerID() == dwAttackerID )
		{
			m_htDeleteRoom.Add( pRoom, dwAttackerID );
		}
	}

	m_htDeleteRoom.SetPositionHead();
	while( pRoom = m_htDeleteRoom.GetData() )
	{
		CloseLootingRoom( pRoom->GetDiePlayerID(), TRUE );
	}
	m_htDeleteRoom.RemoveAll();
}


