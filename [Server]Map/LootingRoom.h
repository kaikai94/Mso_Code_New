



// LootingRoom.h: interface for the CLootingRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOOTINGROOM_H__6C44E6A9_62C7_441D_8140_03EBF8665E38__INCLUDED_)

#define AFX_LOOTINGROOM_H__6C44E6A9_62C7_441D_8140_03EBF8665E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000



enum eLOOTINGITEM

{
	eLI_NONE,
	eLI_ITEM,
	eLI_MONEY,

	eLI_EXP,
	eLI_SELECTED,
	eLI_MAX,
};

struct sLootingItem
{
	int		nKind;
	DWORD	dwData;		//item db idx, money, exp(Exp�?INT64형으�?바뀌었으나 어짜�?루팅 경험치는 전체경험치의 1%이고 DWORD�?넘지않아�?변경하지않았�?)

	sLootingItem() : nKind(eLI_NONE), dwData(0) {}
};


class CLootingRoom  
{
protected:
	DWORD	m_dwDiePlayer;		//»¯±â´Â À¯Àú			//key
	DWORD	m_dwAttacker;		//»¯´Â À¯Àú
	
	sLootingItem	m_LootingItemArray[PKLOOTING_ITEM_NUM];

	int		m_nItemLootCount;

	int		m_nChance;


	DWORD	m_dwLootingStartTime;
	

	int		m_nLootedItemCount;
	

public:
	CLootingRoom();
	virtual ~CLootingRoom();

	void SetLootingStart();
	int GetLootCount()		{ return m_nItemLootCount; }


	int GetChance()			{ return m_nChance; }

	void MakeLootingRoom( CPlayer* pDiePlayer, CPlayer* pAttackPlayer );
	BOOL Loot( int nArrayNum, BOOL bForce = FALSE );

	void EndLootingRoomMsg( BOOL bMsgToAttacker );
	void DiePlayerExitPanelty();

	DWORD GetAttackerID() { return m_dwAttacker; }
	DWORD GetDiePlayerID() { return m_dwDiePlayer; }

	BOOL IsTimeOut();


	int GetLootedItemCount() { return m_nLootedItemCount; }
};

#endif // !defined(AFX_LOOTINGROOM_H__6C44E6A9_62C7_441D_8140_03EBF8665E38__INCLUDED_)


