



// RarenessManager.cpp: implementation of the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RarenessManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include <time.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


GLOBALTON(RarenessManager)

RarenessManager::RarenessManager()
{
	Init();
}

RarenessManager::~RarenessManager()
{
	Release();
}

void RarenessManager::Init()
{
	m_RareItemInfoTable.Initialize(64);
}

void RarenessManager::Release()
{
	ReleaseRareItemInfo();
}

bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

	sprintf(FileName, "Resource/Server/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;

	DWORD dwItemKindIdx = 0;
	while( !fp.IsEOF() )
	{
		if(dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line, 512);
			continue;
		}
		
		if( szBuf[0] == '#' )
		{
			m_RereItemInfo[dwItemKindIdx].wRareRate = fp.GetWord();//[新增几率控制][BY:十里坡剑神][QQ:112582793][2019-4-4][21:03:29]
			m_RereItemInfo[dwItemKindIdx].GenGol_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].GenGol = fp.GetWord();

			m_RereItemInfo[dwItemKindIdx].MinChub_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Life_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Life = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Shield_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Shield = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].PhyAttack_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense = fp.GetWord();
			for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
			{
			m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val( i, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val( i, fp.GetFloat() );
			}
			for(int j = ATTR_FIRE; j <= ATTR_MAX; ++j )
			{
			m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val( j, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val( j, fp.GetFloat() );
			}

			dwItemKindIdx++;
		}

	}

	return TRUE;
}

bool RarenessManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}

//		strupr(szBuf);

		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}

	return TRUE;
}

bool RarenessManager::LoadRareValueProbList()
{
	CMHFile fp;

	char FileName[256];
	char szBuf[256] = {0,};

	sprintf(FileName, "Resource/Server/Item_RareStatSetRate.bin");
	if(!fp.Init(FileName,"rb")) return FALSE;

	fp.GetLine(szBuf,256); 

	DWORD Index = 1;
	while( !fp.IsEOF() )
	{
		if(Index == RARE_VALUE_PROB_LIST_MAX) break;
		

		Index	= fp.GetDword();	
		m_dwRareValueProb[Index - 1] = fp.GetDword();
	}
    fp.Release();
	return TRUE;
}

void RarenessManager::ReleaseRareItemInfo()
{
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
}
BOOL RarenessManager::CanUseMaker(eITEM_KINDBIT eItemKind,ITEM_RARE_OPTION_INFO* NewInfo,ITEM_RARE_OPTION_INFO* OldInfo)
{
	if (!OldInfo)
		return TRUE;
	else if( eItemKind == eEQUIP_ITEM_WEAPON )
		return NewInfo->PhyAttack>OldInfo->PhyAttack? TRUE:FALSE;
	else if( eItemKind == eEQUIP_ITEM_DRESS||eItemKind == eEQUIP_ITEM_HAT || eEQUIP_ITEM_SHOES)
		return NewInfo->Life>OldInfo->Life?  TRUE:FALSE;
	else if( eItemKind == eEQUIP_ITEM_RING||eItemKind == eEQUIP_ITEM_CAPE||
		 eItemKind == eEQUIP_ITEM_NECKLACE||eItemKind == eEQUIP_ITEM_ARMLET||
		 eItemKind == eEQUIP_ITEM_BELT)
		return NewInfo->GenGol>OldInfo->GenGol? 
		TRUE:NewInfo->MinChub>OldInfo->MinChub? 
		TRUE:NewInfo->CheRyuk>OldInfo->CheRyuk? 
		TRUE:NewInfo->SimMek>OldInfo->SimMek? TRUE:FALSE;
}
bool RarenessManager::GetRare( WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, DWORD& bSignature,BOOL bRare,ITEM_RARE_OPTION_INFO* OldRareOption)
{//[装备祝福几率计算][By:十里坡剑神][QQ:112582793][2017/12/13]
	if(ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ObtainItemIdx );
	if( !pInfo ) return FALSE;

	eITEM_KINDBIT eItemKind = GetItemKind( ObtainItemIdx );	
	if( !(eItemKind & eEQUIP_ITEM) ) return FALSE;	

	DWORD ItemIdx = ObtainItemIdx;
	if( eItemKind == eEQUIP_ITEM_WEAPON 
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES )
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}	

	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);

	if( !RareInfo ) return FALSE;

	BOOL baa = FALSE;

	if( FALSE == bRare )
	{
		float addPob = 1.0f;	
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = ( ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) )*addPob <= (RareInfo->RareProb) );
	}

	if( baa || bRare )
	{
		DWORD RareItemKind = 0;	
		if( eItemKind == eEQUIP_ITEM_WEAPON )
		{
			RareItemKind = pInfo->WeaponType - 1;
		}
		else if( eItemKind == eEQUIP_ITEM_DRESS )			RareItemKind = eRI_DRESS;
		else if( eItemKind == eEQUIP_ITEM_HAT )				RareItemKind = eRI_HAT;
		else if( eItemKind == eEQUIP_ITEM_SHOES )			RareItemKind = eRI_SHOES;
		else if( eItemKind == eEQUIP_ITEM_RING )			RareItemKind = eRI_RING;
		else if( eItemKind == eEQUIP_ITEM_CAPE )			RareItemKind = eRI_CAPE;
		else if( eItemKind == eEQUIP_ITEM_NECKLACE )		RareItemKind = eRI_NECKLACE;
		else if( eItemKind == eEQUIP_ITEM_ARMLET )			RareItemKind = eRI_ARMLET;
		else if( eItemKind == eEQUIP_ITEM_BELT )			RareItemKind = eRI_BELT;

		if( m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek )	//泅犁 厘脚备 利侩
		{
			switch( rand()%StatesKindMAX )
			{
			case eRI_GENGOL:
				pRareOptionInfo->GenGol = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol,m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_MINCHUB:
				pRareOptionInfo->MinChub = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub,m_RereItemInfo[RareItemKind].wRareRate);
				break;

			case eRI_CHERYUK:
				pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk,m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_SIMMEK:
				pRareOptionInfo->SimMek = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek ,m_RereItemInfo[RareItemKind].wRareRate);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{	

			pRareOptionInfo->GenGol = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol,m_RereItemInfo[RareItemKind].wRareRate );
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub,m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk,m_RereItemInfo[RareItemKind].wRareRate );
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek,m_RereItemInfo[RareItemKind].wRareRate);
		}

		if( eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES )
		{
			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life,m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk,m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield,m_RereItemInfo[RareItemKind].wRareRate);
		}
		else if( m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield )
		{
			switch( rand()%ValueKindMAX )
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life,m_RereItemInfo[RareItemKind].wRareRate );
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk,m_RereItemInfo[RareItemKind].wRareRate );
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield,m_RereItemInfo[RareItemKind].wRareRate );
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life,m_RereItemInfo[RareItemKind].wRareRate );
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk,m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield,m_RereItemInfo[RareItemKind].wRareRate );
		}
		
		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyAttack,m_RereItemInfo[RareItemKind].wRareRate );
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyDefense,m_RereItemInfo[RareItemKind].wRareRate );
		WORD i = rand()%ATTR_MAX + 1;
		
		if(rand()%2)	
		{
			pRareOptionInfo->AttrAttack.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i)),m_RereItemInfo[RareItemKind].wRareRate ) / 100.f);
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i)),m_RereItemInfo[RareItemKind].wRareRate) / 100.f );
		}
		bSignature=	CanUseMaker(eItemKind,pRareOptionInfo,OldRareOption);
		return TRUE;
	}
	else
		return FALSE;
}

DWORD RarenessManager::GetRareItemValue(DWORD RndMin, DWORD RndMax,WORD wRareRate)
{
	if( !RndMin || !RndMax ) return 0; 

	if( RndMin == RndMax ) return RndMax;	

	ASSERTMSG(RndMax>RndMin, "Check RareOptionInfo File!");
	DWORD RndValue = ((rand()%10000) * 10000 + (rand()%10000 + 1));

	DWORD Index = 0;
	while(RndValue < m_dwRareValueProb[Index])
	{
		Index++;
	}
	
	DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / wRareRate ) + RndMin;
	if(XRndValue>RndMax) 
		XRndValue =(RndMin+rand()%(RndMax-RndMin+1));
	return XRndValue;
}


