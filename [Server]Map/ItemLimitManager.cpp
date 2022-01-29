




#include "stdafx.h"

#include "ItemLimitManager.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"


GLOBALTON(CItemLimitManager)

CItemLimitManager::CItemLimitManager(void)
{
	Init();
}

CItemLimitManager::~CItemLimitManager(void)
{
	Release();
}

void CItemLimitManager::Init()
{
	m_ItemLimitTable.Initialize( 20 );
}

void CItemLimitManager::Release()
{
	ITEMLIMITINFO* pInfo = NULL;
	m_ItemLimitTable.SetPositionHead();
	while( pInfo = m_ItemLimitTable.GetData() )
	{
		delete pInfo;
		pInfo = NULL;
	}
	m_ItemLimitTable.RemoveAll();
}

BOOL CItemLimitManager::LoadItemLimitInfo()
{
	CMHFile file;

	char filename[256] = {0,};
	char buf[256] = {0,};
	char line[512] = {0,};

	sprintf(filename, "Resource/Server/ItemLimitInfo.bin");
	if( !file.Init(filename,"rb") ) return FALSE;

	DWORD dwItemIdx = 0;
	ITEMLIMITINFO* pInfo = NULL;
	while( !file.IsEOF() )
	{
		dwItemIdx = file.GetDword();
		if( 0 == dwItemIdx )
		{
			file.GetLineX(line,512);
			continue;
		}

		pInfo = m_ItemLimitTable.GetData(dwItemIdx);
		if( pInfo == NULL )
		{
            ITEMLIMITINFO* pInfo = new ITEMLIMITINFO;
            pInfo->dwItmeIdx = dwItemIdx;
			file.GetString();
            pInfo->nItemLimitCount = file.GetInt();			
            pInfo->nItemCurrentCount = 0;
			m_ItemLimitTable.Add( pInfo, pInfo->dwItmeIdx );
		}
		else
		{
			pInfo->dwItmeIdx = dwItemIdx;
			file.GetString();
			pInfo->nItemLimitCount = file.GetInt();			
			pInfo->nItemCurrentCount = 0;
		}
	}

	return TRUE;
}

int CItemLimitManager::CheckItemLimitInfo( DWORD dwItemIdx )
{
	ITEMLIMITINFO* pInfo = m_ItemLimitTable.GetData( dwItemIdx );
	if( pInfo )
	{
		if( pInfo->nItemLimitCount > pInfo->nItemCurrentCount )

			return (pInfo->nItemLimitCount - pInfo->nItemCurrentCount);
		else
			return 0;
	}
	return 1;
}

void CItemLimitManager::SetItemLimitInfoFromDB( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount )
{
    ITEMLIMITINFO* pInfo = m_ItemLimitTable.GetData( dwItemIdx );
	if( NULL != pInfo )
	{
		pInfo->nItemLimitCount = nItemLimitCount;
		pInfo->nItemCurrentCount = nItemCurrentCount;
	}
}


void CItemLimitManager::AddCurrentItemCount( DWORD dwItemIdx, int nItemCount )
{
	ITEMLIMITINFO* pInfo = m_ItemLimitTable.GetData( dwItemIdx );
	if( pInfo )
	{
		pInfo->nItemCurrentCount += nItemCount;
		ItemLimitInfoUpdate( pInfo->dwItmeIdx, pInfo->nItemLimitCount, pInfo->nItemCurrentCount );
	}

}

void CItemLimitManager::NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_ITEMLIMIT_ADDCOUNT_TO_MAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			ITEMLIMITINFO* pInfo = m_ItemLimitTable.GetData( pmsg->dwData1 );
			if( NULL != pInfo )
			{
				pInfo->nItemCurrentCount = (int)pmsg->dwData2;
			}
		}
		break;
	}
}

BOOL CItemLimitManager::SetItemLimitCount( DWORD dwItemIdx, int nItemLimitCount )
{

	ITEMLIMITINFO* pInfo = m_ItemLimitTable.GetData( dwItemIdx );
	if( pInfo )
	{
		pInfo->nItemLimitCount = nItemLimitCount;
		return TRUE;
	}
	return FALSE;
}














