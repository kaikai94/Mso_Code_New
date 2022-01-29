



// RarenessManager.h: interface for the RarenessManager class.
//

//////////////////////////////////////////////////////////////////////



#if !defined(AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_)

#define AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"

#define RAREITEMMGR	USINGTON(RarenessManager)
#define BOUNDARY_INDEX_FOR_SHOPITEM	55101	//[物品列表普通物品和商城物品分界编号][By:十里坡剑神][QQ:112582793][2017/12/13]
#define RARE_VALUE_PROB_LIST_MAX	100

//enum WeaponKind{ GUM, DO, CHANG, AMGI, GOONG, WeaponKindMAX = 5 };

//enum ProtectorKind{ DRESS, HAT, SHOES, ProtectorkindMAX = 3 };
//enum AccessaryKind{ RING, CAPE, NECKLACE, ARMLET, BELT, AccessaryKindMAX = 5 };



class RarenessManager  
{
	CYHHashTable<sRareItemInfo>	m_RareItemInfoTable;

	sRareOptionInfo m_RereItemInfo[RareItemKindMAX];

	DWORD	m_dwRareValueProb[RARE_VALUE_PROB_LIST_MAX];
public:
	RarenessManager();
	virtual ~RarenessManager();
	
	void Init();
	void Release();

	bool LoadRareItemOptionInfo();

	bool LoadRareItemInfo();
	bool LoadRareValueProbList();
	void ReleaseRareItemInfo();	

	bool GetRare(WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, DWORD& bSignature,BOOL bRare=FALSE,ITEM_RARE_OPTION_INFO* OldRareOption=NULL);
	DWORD GetRareItemValue(DWORD RndMin, DWORD RndMax,WORD wRareRate);
	BOOL CanUseMaker(eITEM_KINDBIT eItemKind,ITEM_RARE_OPTION_INFO* NewInfo,ITEM_RARE_OPTION_INFO* OldInfo);
	BOOL IsRareItemAble( DWORD ItemIdx )
	{
		if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
		return FALSE;
	}
};

EXTERNGLOBALTON(RarenessManager)

#endif // !defined(AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_)


