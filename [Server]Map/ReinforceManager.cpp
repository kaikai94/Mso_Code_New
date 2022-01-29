



// ReinforceManager.cpp: implementation of the CReinforceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReinforceManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReinforceManager::CReinforceManager()
{
	m_ReinforceItemInfoList.Initialize( eRJP_MAX );
	m_RareReinforceItemInfoList.Initialize( eRJP_MAX );
	m_dwRareMaterialTotalGravity = 0;
	//LoadItemLevelLimitInfo();
}

CReinforceManager::~CReinforceManager()
{
	Release();
}

BOOL CReinforceManager::Init()
{
	srand( GetTickCount() );
	if( m_ReinforceItemInfoList.GetDataNum() != 0  )
		Release();

	ZeroMemory( m_fTableAbility, sizeof(float)*MAX_RTL_ABILITY*MAX_REINFORCE );
	ZeroMemory( m_fTableMaterial, sizeof(float)*MAX_RTL_MATERIAL*MAX_REINFORCE );
	ZeroMemory( m_fTableItemLevel, sizeof(float)*MAX_RTL_ITEMLEVEL*MAX_REINFORCE );
	ZeroMemory( m_fTableAdjust, sizeof(float)*MAX_REINFORCE );

	if( !LoadReinforceInfo() )
	{

		ASSERTMSG( 0, "LoadReinforceInfo(): ScriptError" );
		return FALSE;
	}
	if( !LoadRareReinforceInfo() )
	{
		ASSERTMSG ( 0, "LoadRareinforceInfo(): ScriptError" );
		return FALSE;
	}
//	if( !LoadReinforceTable() )
//	{
//		ASSERTMSG( 0, "LoadReinforceTable(): ScriptError" );
//		return FALSE;
//	}

	return TRUE;
}

void CReinforceManager::Release()
{
	sITEM_REINFORCE_INFO* pInfo = NULL;

	m_ReinforceItemInfoList.SetPositionHead();
	while( pInfo = m_ReinforceItemInfoList.GetData() )
	{
		delete pInfo;
	}

	m_ReinforceItemInfoList.RemoveAll();


	sITEM_RAREREINFORCE_INFO* pRInfo = NULL;

	m_RareReinforceItemInfoList.SetPositionHead();
	while( pRInfo = m_RareReinforceItemInfoList.GetData() )
	{
		delete pRInfo;
		pRInfo = NULL;
	}

	m_RareReinforceItemInfoList.RemoveAll();
}

BOOL CReinforceManager::LoadReinforceInfo()
{
	CMHFile file;

	if( !file.Init( "./Resource/itemReinforceList.bin", "rb" ) )		return FALSE;


	while(1)
	{
		if(file.IsEOF())
			break;

		WORD tempItemIdx		= file.GetWord();
		WORD tempReinforceType	= file.GetWord();
		float tempMaxReinforce	= file.GetFloat();	
		float tempW				= file.GetFloat();	

		if( tempItemIdx != 0 && tempReinforceType != 0 && tempMaxReinforce > 0.0f && tempW > 0.0f
			&& m_ReinforceItemInfoList.GetData( tempItemIdx ) == NULL )
		{
			sITEM_REINFORCE_INFO* pInfo = new sITEM_REINFORCE_INFO;
			pInfo->wItemIdx			= tempItemIdx;
			pInfo->ReinforceType	= tempReinforceType;
			pInfo->fMaxReinforce	= tempMaxReinforce;
			pInfo->fw				= tempW;
			m_ReinforceItemInfoList.Add( pInfo, pInfo->wItemIdx );
		}
		else	//error
		{
			if( file.IsEOF())
				break;
			file.Release();
			return FALSE;
		}
	}

	file.Release();
	return TRUE;
}
void CReinforceManager::ReLoadRareReinforceInfo()
{
	sITEM_RAREREINFORCE_INFO* pRInfo = NULL;

	m_RareReinforceItemInfoList.SetPositionHead();
	while( pRInfo = m_RareReinforceItemInfoList.GetData() )
	{
		delete pRInfo;
		pRInfo = NULL;
	}

	m_RareReinforceItemInfoList.RemoveAll();

	CMHFile	file;

	if(!file.Init( "./Resource/item_RareReinforceList.bin", "rb" ))
		return ;

	if( m_RareReinforceItemInfoList.GetDataNum() > 0 )
	{
		Release();
		ASSERT(0);
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		sITEM_RAREREINFORCE_INFO temp;
		temp.wItemIdx		= file.GetWord();
		temp.ReinforceType	= file.GetWord();
		temp.fMaxReinforce	= file.GetFloat();
		temp.fw				= file.GetFloat();
		temp.wMaterialGravity	= file.GetWord();

		if( temp.wItemIdx && temp.ReinforceType && temp.fMaxReinforce > 0.0f && temp.fw > 0.0f && temp.wMaterialGravity 
			&& m_RareReinforceItemInfoList.GetData( temp.wItemIdx ) == NULL )
		{
			sITEM_RAREREINFORCE_INFO* pRInfo = new sITEM_RAREREINFORCE_INFO(temp);

			m_RareReinforceItemInfoList.Add( pRInfo, pRInfo->wItemIdx );
		}
		else
		{
			file.Release();
			return ;
		}
	}

	file.Release();

	return ;
}
BOOL CReinforceManager::LoadRareReinforceInfo()
{
	CMHFile	file;

	if(!file.Init( "./Resource/item_RareReinforceList.bin", "rb" ))
		return FALSE;

	if( m_RareReinforceItemInfoList.GetDataNum() > 0 )
	{
		Release();
		ASSERT(0);
	}
	
	while(1)
	{
		if(file.IsEOF())
			break;

		sITEM_RAREREINFORCE_INFO temp;
		temp.wItemIdx		= file.GetWord();
		temp.ReinforceType	= file.GetWord();
		temp.fMaxReinforce	= file.GetFloat();
		temp.fw				= file.GetFloat();
		temp.wMaterialGravity	= file.GetWord();

		if( temp.wItemIdx && temp.ReinforceType && temp.fMaxReinforce > 0.0f && temp.fw > 0.0f && temp.wMaterialGravity 
			&& m_RareReinforceItemInfoList.GetData( temp.wItemIdx ) == NULL )
		{
			sITEM_RAREREINFORCE_INFO* pRInfo = new sITEM_RAREREINFORCE_INFO(temp);

			m_RareReinforceItemInfoList.Add( pRInfo, pRInfo->wItemIdx );
		}
		else
		{
			file.Release();
			return FALSE;
		}
	}

	file.Release();

	return TRUE;
	
}
//WORD CReinforceManager::GetItemLevelLimit(WORD level)
//{
//	stdext::hash_map<WORD,WORD>::iterator iter;
//	iter = m_ItemLevelLimitInfo.find(level);
//	if(iter!=m_ItemLevelLimitInfo.end())
//		return iter->second;
//	else
//		return 100;
//}
//BOOL CReinforceManager::RLoadItemLevelLimitInfo()
//{
//	if(m_ItemLevelLimitInfo.size()>0)
//		m_ItemLevelLimitInfo.clear();
//	return LoadItemLevelLimitInfo();
//}
//BOOL CReinforceManager::LoadItemLevelLimitInfo()
//{
//	CMHFile file;
//
//	if( !file.Init( "./Resource/item_powerup.bin", "rb" ) )		return FALSE;
//	while(1)
//	{
//		if(file.IsEOF())
//			break;
//		m_ItemLevelLimitInfo[file.GetWord()]= file.GetWord();
//	}
//	file.Release();
//	return TRUE;
//}
BOOL CReinforceManager::LoadReinforceTable()	
{
	CMHFile file;

	if( !file.Init( "./Resource/item_powerup.bin", "rb" ) )		return FALSE;

	int i, j;
	float fCheckSum;	

	for( i = 0 ; i < MAX_RTL_ABILITY ; ++i )
	{
		fCheckSum = 0.0f;
		for( j = 0 ; j < MAX_REINFORCE ; ++j )
		{
			if( file.IsEOF() )
			{
				file.Release();
				return FALSE;
			}

			m_fTableAbility[i][j] = file.GetFloat();
			fCheckSum += m_fTableAbility[i][j];
		}

		if( fCheckSum >= 100.1f )
		{
			ASSERTMSG( 0, "LoadReinforceTable: Over 100%" );
		}
	}

	for( i = 0 ; i < MAX_RTL_MATERIAL ; ++i )
	{
		fCheckSum = 0.0f;
		for( j = 0 ; j < MAX_REINFORCE ; ++j )
		{
			if( file.IsEOF() )
			{
				file.Release();
				return FALSE;
			}

			m_fTableMaterial[i][j] = file.GetFloat();
			fCheckSum += m_fTableMaterial[i][j];
		}

		if( fCheckSum >= 100.1f )
		{
			ASSERTMSG(0,"LoadReinforceTable: Over 100%");
		}
	}

	for( i = 0 ; i < MAX_RTL_ITEMLEVEL ; ++i )
	{
		fCheckSum = 0.0f;
		for( j = 0 ; j < MAX_REINFORCE ; ++j )
		{
			if( file.IsEOF() )
			{
				file.Release();
				return FALSE;
			}

			m_fTableItemLevel[i][j] = file.GetFloat();
			fCheckSum += m_fTableItemLevel[i][j];
		}

		if( fCheckSum >= 100.1f )
		{
			ASSERTMSG(0,"LoadReinforceTable: Over 100%");
		}
	}

	{
		fCheckSum = 0.0f;
		for( j = 0 ; j < MAX_REINFORCE ; ++j )
		{
			if( file.IsEOF() )
			{
				file.Release();
				return FALSE;
			}

			m_fTableAdjust[j] = file.GetFloat();
			fCheckSum += m_fTableAdjust[j];
		}

		if( fCheckSum >= 100.1f )
		{

			ASSERTMSG(0,"LoadReinforceTable: Over 100%");
		}
	}

	file.Release();		
	return TRUE;
}

sITEM_REINFORCE_INFO* CReinforceManager::GetReinforceInfo( WORD wItemIdx )
{
	return m_ReinforceItemInfoList.GetData( wItemIdx );
}

sITEM_RAREREINFORCE_INFO* CReinforceManager::GetRareReinforceInfo( WORD wItemIdx )

{
	return m_RareReinforceItemInfoList.GetData( wItemIdx );
}


float CReinforceManager::GetReinforceWithMetariel( ITEM_OPTION_INFO* pOption, DWORD MaterialItemIdx, WORD eItemOption )
{
	eITEM_KINDBIT eKindMaterial = GetItemKind( (WORD)MaterialItemIdx );
	if( !(eKindMaterial & eEXTRA_ITEM_JEWEL) )	 return 0;

	sITEM_REINFORCE_INFO* pInfo = NULL;
	if( eItemOption == eIOK_Normal )

	{
		pInfo = GetReinforceInfo( (WORD)MaterialItemIdx );
	}
	else if( eItemOption == eIOK_Rare )
	{
		pInfo = (sITEM_REINFORCE_INFO*)GetRareReinforceInfo( (WORD)MaterialItemIdx );		
	}
	if( !pInfo ) return 0;
	
	if( pInfo->ReinforceType == pOption->GetOptionType() )	

		return pOption->GetOptionValue( pInfo->ReinforceType );	

	return 0;
}


BOOL CReinforceManager::CheckValidMaterial( WORD wReinforceItemIdx, WORD wMaterialItemIdx, WORD eItemOption )
{
	eITEM_KINDBIT eKindItem		= GetItemKind( wReinforceItemIdx );
	eITEM_KINDBIT eKindMaterial = GetItemKind( wMaterialItemIdx );

	if( !(eKindItem & eEQUIP_ITEM) ) return FALSE;	
	if( !(eKindMaterial & eEXTRA_ITEM_JEWEL) ) return FALSE;	

	sITEM_REINFORCE_INFO* pInfo = NULL;
	if( eItemOption == eIOK_Normal )
	{
		pInfo = GetReinforceInfo(wMaterialItemIdx);
	}

	else if( eItemOption == eIOK_Rare )
	{
		pInfo = (sITEM_REINFORCE_INFO*)GetRareReinforceInfo(wMaterialItemIdx);
	}
	if( pInfo == NULL ) return FALSE;

	WORD ReinforceType = pInfo->ReinforceType;

	switch( eKindItem )	
	{
	case eEQUIP_ITEM_RING:
	case eEQUIP_ITEM_NECKLACE:
		if( ReinforceType == eRJP_FIREATTACK ||
			ReinforceType == eRJP_WATERATTACK ||
			ReinforceType == eRJP_TREEATTACK ||
			ReinforceType == eRJP_GOLDATTACK ||
			ReinforceType == eRJP_EARTHATTACK )
			return TRUE;
		break;

	case eEQUIP_ITEM_ARMLET:
		if( ReinforceType == eRJP_FIREREGIST ||
			ReinforceType == eRJP_WATERREGIST ||
			ReinforceType == eRJP_TREEREGIST ||
			ReinforceType == eRJP_GOLDREGIST ||
			ReinforceType == eRJP_EARTHREGIST ||
			ReinforceType == eRJP_PHYATTACK ||	
			ReinforceType == eRJP_CRITICAL )
			return TRUE;
		break;
	case eEQUIP_ITEM_CAPE:
	case eEQUIP_ITEM_BELT:
		if( ReinforceType == eRJP_FIREREGIST ||
			ReinforceType == eRJP_WATERREGIST ||
			ReinforceType == eRJP_TREEREGIST ||
			ReinforceType == eRJP_GOLDREGIST ||
			ReinforceType == eRJP_EARTHREGIST )
			return TRUE;
		break;

	case eEQUIP_ITEM_SHOES:
		if( ReinforceType == eRJP_GENGOL ||
			ReinforceType == eRJP_MINCHUB ||
			ReinforceType == eRJP_CHERYUK ||
			ReinforceType == eRJP_SIMMEK )
			return TRUE;
		break;

	case eEQUIP_ITEM_DRESS:
		if( ReinforceType == eRJP_LIFE ||
			ReinforceType == eRJP_NAERYUK ||
			ReinforceType == eRJP_SHIELD )
			return TRUE;
		break;

	case eEQUIP_ITEM_WEAPON:
	case eEQUIP_ITEM_UNIQUE:
		if( ReinforceType == eRJP_PHYATTACK ||
			ReinforceType == eRJP_CRITICAL )
			return TRUE;
		break;

	case eEQUIP_ITEM_HAT:
		if( ReinforceType == eRJP_PHYDEFENSE )
			return TRUE;
		break;
	}	

	return FALSE;
}

WORD CReinforceManager::GetValueForAbility( WORD wAbilGrade )
{
	float fSeed = (float)(rand()%10000 + 1) / 100.0f;

	float fSum = 0.0f;

	for( int i = 0 ; i < MAX_REINFORCE ; ++i )
	{
		fSum += m_fTableAbility[wAbilGrade-1][i];
		if( fSeed < fSum )
		{
			return i+1;
		}		
	}

	return MAX_REINFORCE;
}

WORD CReinforceManager::GetValueForMaterialNum( WORD wMaterialNum )
{
	float fSeed = (float)(rand()%10000 + 1) / 100.0f;

	float fSum = 0.0f;

	for( int i = 0 ; i < MAX_REINFORCE ; ++i )
	{
		fSum += m_fTableMaterial[wMaterialNum-1][i];
		if( fSeed < fSum )
		{
			return i+1;
		}		
	}

	return MAX_REINFORCE;
}

WORD CReinforceManager::GetValueForItemLevel( WORD wItemLevel )
{
	float fSeed = (float)(rand()%10000 + 1) / 100.0f;

	float fSum = 0.0f;

	for( int i = 0 ; i < MAX_REINFORCE ; ++i )
	{
		fSum += m_fTableItemLevel[wItemLevel-1][i];
		if( fSeed < fSum )
		{
			return i+1;
		}		
	}

	return MAX_REINFORCE;
}

WORD CReinforceManager::GetAdjustValue()
{
	float fSeed = (float)(rand()%10000 + 1) / 100.0f;

	float fSum = 0.0f;

	for( int i = 0 ; i < MAX_REINFORCE ; ++i )
	{
		fSum += m_fTableAdjust[i];
		if( fSeed < fSum )
		{
			return i+1;
		}		
	}

	return MAX_REINFORCE;
}

BOOL CReinforceManager::CheckMaterialTotalGravity(WORD wMaterialItemIdx, WORD wMaterialCount, WORD eItemOption)
{
	ASSERT(wMaterialCount);

	if( eItemOption == eIOK_Normal ) return TRUE;

	if( eItemOption == eIOK_Rare )
	{
		sITEM_RAREREINFORCE_INFO* pRInfo = m_RareReinforceItemInfoList.GetData(wMaterialItemIdx);

		DWORD MaterialGravity = pRInfo->wMaterialGravity * wMaterialCount;

		int OverGravity = m_dwRareMaterialTotalGravity + MaterialGravity - MATERIAL_TOTALGRAVITY_MAX;
		if( 0 < OverGravity )
		{
			ASSERTMSG(0, "RareMaterial OverGravity");
			return FALSE;
		}

		m_dwRareMaterialTotalGravity += MaterialGravity;
	}

	return TRUE;
}


