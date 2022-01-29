#pragma once

#define	MAX_DROP_MAPITEM_PERCENT	1000000


#define MAPITEMDROP_OBJ	CMapItemDrop::GetInstance()



struct MAP_ITEM
{
#ifdef _DEBUG
	char	Name[MAX_ITEMNAME_LENGTH+1];	
#endif
	WORD	wItemIdx;					

	DWORD	dwDropPercent;		

	WORD	wDropCount;					
	WORD	wMaxDropCount;			
};

struct MAP_DROP_ITEM
{
	DWORD		wChannel;						
	WORD		wMaxDropNum;					
	WORD		wNumDropItem;					
	DWORD		dwTotalSumDropPercent;			
	MAP_ITEM	arrMapItems[MAX_DROPITEM_NUM];	
};


class CMapItemDrop
{
	BOOL	m_bLoadSucceed;
	BOOL	m_bInited;		

	WORD	m_wInitDay;		
	WORD	m_wInitHour;

	CYHHashTable<MAP_DROP_ITEM>	m_MapItemDropTable;

	SYSTEMTIME	m_PrevInitedTime;

protected:
	BOOL	LoadMapDropItemList();
	void	ClearMapDropItemList();
	BOOL	LoadInitedTimeFile();

	MAP_DROP_ITEM*	GetMapDropItem(DWORD wChannel);

	MAP_ITEM*	GetMapItem(WORD wChannel, WORD wItemIdx);


	void	DropItem(CPlayer* pPlayer, MAP_ITEM* pMapItem, WORD MonsterKind);


	void	SetPrevInitedTime(SYSTEMTIME* sTime);
	SYSTEMTIME*	GetPrevInitedTime();

	void	InitItemDropInfo();

	void	InitItemDropInfoToDB();
	void	UpdateItemDropInfoToDB( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount );
	void	LogMapDropItem(WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount);
	void	GetDropItemInfoFromDB();

public:
	GETINSTANCE(CMapItemDrop);

	CMapItemDrop(void);

	~CMapItemDrop(void);

	void	Init();
	void	Process();

	void	CalculateDropRate(CPlayer* pPlayer, WORD MonsterKind);

	void	SetItemDropInfoByDB(WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount);
};


