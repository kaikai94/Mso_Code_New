#include "stdafx.h"

#include "AutoNoteRoom.h"
#include "UserTable.h"

#include "Player.h"
#include "AutoNoteManager.h"


CAutoNoteRoom::CAutoNoteRoom(void)
{
}

CAutoNoteRoom::~CAutoNoteRoom(void)
{
}

void CAutoNoteRoom::CreateRoom( CPlayer* pAskPlayer, CPlayer* pAutoPlayer, DWORD dwQuestion )
{
	m_dwAskCharacter = pAskPlayer->GetID();
	m_dwAutoCharacter = pAutoPlayer->GetID();
	m_dwAskUserIdx = pAskPlayer->GetUserID();		
	m_dwAutoUserIdx = pAutoPlayer->GetUserID();	


	SafeStrCpy( m_AskCharacterName, pAskPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
	SafeStrCpy( m_AutoCharacterName, pAutoPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );

	pAutoPlayer->SetAutoAskPlayerIdx( pAskPlayer->GetID() );

	m_nANRState = eANRS_WAITANSWER;
	m_dwCreateTime = gCurTime;
	m_dwLastAnswerTime = gCurTime;
	m_nChance = AUTONOTEMGR->GetReplyChance();

	for( int i = 0; i < 4; ++i )
	{
		m_dwAskColor[i] = rand()%4;	
	}
}

void CAutoNoteRoom::AskToAuto()
{
	CPlayer* pAskPlayer = (CPlayer*)g_pUserTable->FindUser(m_dwAskCharacter);

	CPlayer* pAutoPlayer = (CPlayer*)g_pUserTable->FindUser(m_dwAutoCharacter);

	if( pAskPlayer == NULL || pAutoPlayer == NULL ) return;

	AUTONOTEMGR->MakeSendRaster( m_dwAskColor );

	MSG_AUTONOTE_IMAGE msg;
	msg.Category = MP_AUTONOTE;

	msg.Protocol = MP_AUTONOTE_ASKTOAUTO_IMAGE;
	msg.dwReplyTime = AUTONOTEMGR->GetReplyTime();				
	memcpy( msg.Image, AUTONOTEMGR->GetSendRaster(), 128*32*3 );
	pAutoPlayer->SendMsg( &msg, sizeof(msg) );

	MSGBASE msg2;
	msg2.Category = MP_AUTONOTE;
	msg2.Protocol = MP_AUTONOTE_ASKTOAUTO_ACK;
	pAskPlayer->SendMsg( &msg2, sizeof(msg2) );
}

BOOL CAutoNoteRoom::CheckAnswerFromAuto( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 )
{
	DWORD data[4];
	data[0] = dwData1;
	data[1] = dwData2;
	data[2] = dwData3;
	data[3] = dwData4;


	for( int i = 0; i < 4; ++i )
	{
		if( data[i] > 3 || m_dwAskColor[i] != data[i] )
		{
			--m_nChance;

			m_dwLastAnswerTime = gCurTime-500;	
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAutoNoteRoom::IsTimeOver()
{
	if( m_nANRState == eANRS_FASTANSWER )
		return TRUE;

	if( gCurTime > m_dwCreateTime + AUTONOTEMGR->GetReplyTime()*1000 + 5*1000)

		return TRUE;

	return FALSE;
}

void CAutoNoteRoom::FastAnswer()
{
	m_nANRState = eANRS_FASTANSWER;
	m_dwCreateTime += ( rand() % 60 - 50 );	

	m_nChance = 0;

}






