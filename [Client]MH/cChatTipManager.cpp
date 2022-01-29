// cChatTipManager.cpp: implementation of the cChatTipManager class.
//
//////////////////////////////////////////////////////////////////////

//#include <windows.h>	//*** GetTickCount	//�� �׽�Ʈ�ϰ� ����.
//#include "stdio.h"
#include <StdAfx.h>
#include "MHFile.h"
#include "MHTimeManager.h"	//
#include "CommonDefine.h"
#include "OptionManager.h"

#include "./Interface/cResourceDef.h"	//���ҽ� ���

#include "ChatManager.h"
#include "ObjectManager.h"
#include "cChatTipManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cChatTipManager)

cChatTipManager::cChatTipManager()
{
//	Init();			//GameIn 
}

cChatTipManager::~cChatTipManager()
{
	Release();
}

void cChatTipManager::Init()
{
	m_bActive			= TRUE;
	m_dwSize			= 0;	//�ε� ����ü ũ��	//�ǽð�
	m_dwMsgTimeLen		= 60000;
	m_dwLastMsgTime		= 0;	//���� �޽��� ��� �ð�
	m_dwCurHeroLevel	= 0;	//IsInfoCharged()���� �ʱ� TRUE ��ȯ//HEROINFO ����ü ��ü
	m_nCurIndex		= -1;	//���� �޽��� ��ȯ�� ���� ���� �ε��� ���//�ʱⰪ -1
	m_spChatTipMsg		= NULL;

	LoadChatTipMsg();
}

void cChatTipManager::Release()
{
	ReleaseMsgList();
}

void cChatTipManager::LoadChatTipMsg()	// ��ũ��Ʈ ���� ó�� �ȵ�. 
{
	char line[256] = {0,};

	CMHFile fp;
	int nIndex = 0;;
	int nLen = 0;

	if( !fp.Init( FILE_CHATTIP_MSG, "rb" ) )
		return;

	strcpy( line, strupr( fp.GetString() ) );
	if( strcmp( line, "#LISTCOUNT" ) == 0 )
	{
		m_dwSize = fp.GetDword();
		m_spChatTipMsg = new sChatTip[ m_dwSize ];
	}
	fp.GetLine( line, 256 );

	strcpy( line, strupr( fp.GetString() ) );
	if( strcmp( line, "#MSGTIMELENGTH") == 0 )
	{
		m_dwMsgTimeLen = fp.GetDword();
	}
	fp.GetLine( line, 256 );
//	m_dwSize = fp.GetInt();
//	m_spChatTipMsg = new sChatTip[ m_dwSize ];
//	fp.GetLine( line, 256 );
//	m_dwMsgTimeLen = fp.GetInt();
//	fp.GetLine( line, 256 );

	while( TRUE )
	{
		nIndex = fp.GetInt();
		
		if( fp.IsEOF() ) break;

		if( nIndex < 0 || nIndex >= (int)m_dwSize ) break;

		m_spChatTipMsg[nIndex].FromLv	= fp.GetInt();
		m_spChatTipMsg[nIndex].ToLv		= fp.GetInt();

		fp.GetStringInQuotation( line );
		nLen = strlen( line );
		if( nLen > 0 )
		{
			m_spChatTipMsg[nIndex].pTipStr = new char[ nLen + 1 ];
			memcpy( m_spChatTipMsg[nIndex].pTipStr, line, nLen + 1 );
			fp.GetLine( line, 256 );
		}
	}

	fp.Release();
}

char* cChatTipManager::GetChatTipMsg()
{

	m_dwCurHeroLevel = HERO->GetLevel();

		int i = m_nCurIndex;
		
		bool bIsFirst = FALSE;
		bool bLooped = FALSE;	
		
		if( m_nCurIndex == -1 )
		{
			bIsFirst = TRUE;
		}
				
		while(m_bActive)
		{
			if( ( i == m_nCurIndex || bIsFirst == TRUE ) && bLooped == TRUE )
			{
				m_bActive = FALSE;
				break;
			}
			
			i++;
			
			if( i == (int)m_dwSize)
			{
				i = 0;
			}
			
			if( i == (int)m_dwSize - 1 )
			{
				bLooped = TRUE;
			}
			if( m_dwCurHeroLevel >= m_spChatTipMsg[i].FromLv
				&& m_dwCurHeroLevel <= m_spChatTipMsg[i].ToLv )
			{
				m_nCurIndex = i;	
				break;
			}
		}
		
		if(m_bActive)
		{
			return m_spChatTipMsg[m_nCurIndex].pTipStr;
		}
		return NULL;
}

bool cChatTipManager::IsTimePassed()	
{
	if( gCurTime - m_dwLastMsgTime > m_dwMsgTimeLen )
	{
		m_dwLastMsgTime = gCurTime;
		return TRUE;
	}
	else
		return FALSE;
}

void cChatTipManager::ReleaseMsgList()
{
	if( m_spChatTipMsg != NULL )
	{
		for( int i = 0; i < (int)m_dwSize; i++ )
		{
			if( m_spChatTipMsg[i].pTipStr )
			{
				delete[] m_spChatTipMsg[i].pTipStr;
				m_spChatTipMsg[i].pTipStr = NULL;
			}
		}
		
		if(m_spChatTipMsg)
		{
			delete[] m_spChatTipMsg;
			m_spChatTipMsg = NULL;
		}
	}
}

void cChatTipManager::Process()
{
	if( !m_bActive || m_spChatTipMsg == NULL )
	{
		return;
	}

	if( !IsTimePassed() )
	{
		return;
	}

	char* str = GetChatTipMsg();
	if( str != NULL)
	{
		CHATMGR->AddMsg( CTC_TIPS, str );
	}
}
