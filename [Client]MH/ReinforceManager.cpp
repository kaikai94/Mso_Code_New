







// ReinforceManager.cpp: implementation of the CReinforceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReinforceManager.h"
#include "MHFile.h"
#include "GameResourceStruct.h"
#include "ItemManager.h"
#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////
GLOBALTON(CReinforceManager)
CReinforceManager::CReinforceManager()
{
	m_ReinforceItemInfoList.Initialize( eRJP_MAX );
	m_RareReinforceItemInfoList.Initialize( eRJP_MAX );

	m_dwRareMaterialTotalGravity = 0;
	m_dwTargetItemOptionKind = eIOK_Normal;
}

CReinforceManager::~CReinforceManager()
{
	Release();
}

void CReinforceManager::Release()
{
	sITEM_REINFORCE_INFO* pInfo = NULL;
	m_ReinforceItemInfoList.SetPositionHead();
	while( pInfo = m_ReinforceItemInfoList.GetData() )
		delete pInfo;


	m_ReinforceItemInfoList.RemoveAll();

	sITEM_RAREREINFORCE_INFO* pRInfo = NULL;
	m_RareReinforceItemInfoList.SetPositionHead();
	while( pRInfo = m_RareReinforceItemInfoList.GetData() )
		delete pRInfo;

	m_RareReinforceItemInfoList.RemoveAll();


}

sITEM_REINFORCE_INFO * CReinforceManager::GetReinforceItemInfo(WORD wItemIdx)
{
	return m_ReinforceItemInfoList.GetData(wItemIdx);
}

//SW051021
sITEM_RAREREINFORCE_INFO* CReinforceManager::GetRareReinforceItemInfo(WORD wItemIdx)
{
		return m_RareReinforceItemInfoList.GetData(wItemIdx);
}

void CReinforceManager::SetHeadPos()
{
	m_ReinforceItemInfoList.SetPositionHead();
}

sITEM_REINFORCE_INFO * CReinforceManager::GetData()
{
	return m_ReinforceItemInfoList.GetData();
}

BOOL CReinforceManager::LoadReinforceItemInfo()
{
	CMHFile file;

	if(!file.Init( "./Resource/ItemReinforceList.bin", "rb" ))
		return FALSE;

	if( m_ReinforceItemInfoList.GetDataNum() > 0 )
		Release();

	while(1)
	{
		if(file.IsEOF())
			break;



		WORD tempItemIdx		= file.GetWord();
		WORD tempReinforceType	= file.GetWord();
		float tempMaxReinforce	= file.GetFloat();
		float tempW				= file.GetFloat();

		if( tempItemIdx != 0 && tempReinforceType != 0 && tempMaxReinforce > 0.0f && tempW >0.0f
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

BOOL CReinforceManager::LoadRareReinnforceItemInfo()
{
	CMHFile	file;

	if(!file.Init( "./Resource/item_RareReinforceList.bin", "rb" ))
		return FALSE;

	if( m_RareReinforceItemInfoList.GetDataNum() > 0 )
		Release();


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
			if( file.IsEOF() )
				break;
			file.Release();
			return FALSE;
		}
	}

	file.Release();

	return TRUE;

}

 

BOOL CReinforceManager::CheckReinforceItemWithMeterial(	DWORD ItemPos, DWORD MaterialItemIdx, WORD eItemOption )
{
	// ��ȭ�� ���������� üũ
	const ITEMBASE* pItem = ITEMMGR->GetItemInfoAbsIn( HERO, (POSTYPE)ItemPos );
	if( !pItem )			return FALSE;
	ITEM_OPTION_INFO* pOption = ITEMMGR->GetItemOption( pItem->Durability );
	if( !pOption )			return FALSE;
	// Materialüũ
	eITEM_KINDBIT eKindMaterial = GetItemKind( (WORD)MaterialItemIdx );
	if( !(eKindMaterial & eEXTRA_ITEM_JEWEL) ) return FALSE;


	sITEM_REINFORCE_INFO* pInfo = NULL;
	if(eItemOption == eIOK_Normal)
	{
		pInfo = GetReinforceItemInfo( (WORD)MaterialItemIdx );
	}
	else if(eItemOption == eIOK_Rare)
	{
//		sITEM_RAREREINFORCE_INFO* pRInfo = GetRareReinforceItemInfo( MaterialItemIdx );
//		pInfo = (sITEM_REINFORCE_INFO*)pRInfo;
		pInfo = (sITEM_REINFORCE_INFO*)GetRareReinforceItemInfo( (WORD)MaterialItemIdx );

	}
	if( !pInfo ) return FALSE;

	//
	if( pOption->GetOptionType()  == pInfo->ReinforceType )
		return TRUE;

	return FALSE;
}

BOOL CReinforceManager::CheckValidMaterial( WORD wReinforceItemIdx, WORD wMaterialItemIdx, WORD eItemOption )
{
	eITEM_KINDBIT eKindItem		= GetItemKind( wReinforceItemIdx );
	eITEM_KINDBIT eKindMaterial = GetItemKind( wMaterialItemIdx );

	if( !(eKindItem & eEQUIP_ITEM) ) return FALSE;
	if( !(eKindMaterial & eEXTRA_ITEM_JEWEL) ) return FALSE;

	sITEM_REINFORCE_INFO* pInfo = NULL;
	if(eItemOption == eIOK_Normal)
	{
		pInfo = GetReinforceItemInfo(wMaterialItemIdx);
	}
	else if(eItemOption == eIOK_Rare)
	{
		pInfo = (sITEM_REINFORCE_INFO*)GetRareReinforceItemInfo(wMaterialItemIdx);
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
	case eEQUIP_ITEM_UNIQUE:	// magi82 - UniqueItem(071004) ����ũ�����۵� ��ȭ�ǵ���..

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

BOOL CReinforceManager::CheckMaterialTotalGravity( WORD wMaterialItemIdx, WORD wMaterialCount, WORD eItemOption )
{//��� �������� �߷��� ����� ���Ѽ�ġ�� ������ �ʰ���ġ�� ����. 0 => TRUE
	ASSERT(wMaterialCount);

	DWORD MaterialGravity = 0;

	if( eItemOption == eIOK_Normal )	//ĭ���� ���ѵȴ�. 100ĭ -_-.
	{
		MaterialGravity = wMaterialCount;	//�Ϲ� �������� ��� ���� ���� 1��.
	}
	else if( eItemOption == eIOK_Rare )
	{

		sITEM_RAREREINFORCE_INFO* pRInfo = GetRareReinforceItemInfo(wMaterialItemIdx);

		MaterialGravity = pRInfo->wMaterialGravity * wMaterialCount;

	}
	
	int OverGravity = m_dwRareMaterialTotalGravity + MaterialGravity - MATERIAL_TOTALGRAVITY_MAX;
	if( 0 < OverGravity )
		return OverGravity;	//�߷� �ʰ� ������ False / True ���� �ʰ��� ���� �˷����ڴ� ..
	
	m_dwRareMaterialTotalGravity += MaterialGravity;
	
	return 0;	//TRUE
}




