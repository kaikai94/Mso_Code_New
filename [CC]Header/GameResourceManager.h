



// GameResourceManager.h: interface for the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#include "GameResourceStruct.h"
class CMHFile;
#include "ptrlist.h"
#include <vector>
#include <map>
#define LOWRSLTN_W  800
#define LOWRSLTN_H	600   
#define MIDRSLTN_W	1024
#define MIDRSLTN_H	768


#define GAMERESRCMNGR	USINGTON(CGameResourceManager)

#define GET_MAINWIN_W	GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth
#define GET_MAINWIN_H	GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight
struct  WIN_DPI
{
	int Index;
	int x;
	int y;
};

///////////////////////////////////////////////////////////////////////////////
struct sPRELOAD_EFFECT_INFO{
	int MapNum;

	int Count;

	cPtrList Effect;
};
///////////////////////////////////////////////////////////////////////////////

struct DROPINFO_TIMAP
{
	DWORD	wExpPoint;
	DWORD	wMoneyMin;
	DWORD	wMoneyMax;
};
struct MONSTERDROP_TIMAP
{
	DROPINFO_TIMAP	stDropInfo[MAX_PARTY_LISTNUM];
};

class CGameResourceManager  
{
	BOOL m_bMonsterRegenLoad;
	BOOL m_bNpcRegenLoad;
	BOOL m_bMonsterListLoad;
	BOOL m_bNpcListLoad;
	BOOL m_bMonsterDropItem;
	BOOL m_bLowResolution;
    DWORD m_PaneltyTime;
	CMemoryPoolTempl<LEV_PENALTY>*		m_LevPenaltyPool;
	CYHHashTable<LEV_PENALTY>			m_LevPenaltyTable;
	////////////////////////////////////////////////////////////////////////////////////
	CYHHashTable<UNIQUE_ITEM_OPTION_INFO>	m_UniqueItemListTable;
	//////////////////////////////////////////////////////////////////////////
	CYHHashTable<UNIQUE_ITEM_MIX_INFO>		m_UniqueItemMixTable;
	
	DWORD m_MaxNpcChxList;
	CHXLIST* m_NpcChxList;

	CYHHashTable<MONSTERDROP_TIMAP>		m_TitanMapMosterDropTable;

	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_NomalClothesSkinTable;
	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_CotumeSkinTable;		

	CYHHashTable<stMAPKINDINFO>	m_MapKindStateInfoTable;


	DWORD m_MaxItemChxCount;
	CHXLIST* m_ItemChxList;

#ifdef _MAPSERVER_
	CYHHashTable<FB_MAP_INFO> m_FBMapInfo;  //[副本地图信息列表定][By:十里坡剑神][QQ:112582793][2019/3/10][16:25:47]
	std::map<WORD,std::string>  m_CanUseAAKLifeList;
#endif //_MAPSERVER_
private:
	struct MonsterInfoInMap
	{
		unsigned int MapNumber;
		unsigned int MinLevel;
		unsigned int MaxLevel;
	};

	struct ItemChangeRateofLv
	{
		unsigned int Level;  

		unsigned int Rate[10]; 
	};

	struct ItemChangeRateofAttrib
	{
		

		struct _attribInfo
		{
			WORD changeIndex;
			char Name[MAX_NAME_LENGTH+1];
			unsigned int changeRate;
		};
		
		_attribInfo attribinfo[5]; 
	};

	std::vector<MonsterInfoInMap> m_MonsterInfoInMapList; 
	std::vector<ItemChangeRateofLv> m_ItemChangeRateofLvList;
	std::vector<ItemChangeRateofAttrib> m_ItemChangeRateAttribList; 
	
	std::vector<std::string> m_UserCommendList; 
public:
	//[药品回气时间链表][By:十里坡剑神][QQ:112582793][2019/3/10][16:25:21]
	CYHHashTable<YOUNGYAK_ITEM_DELAY_TYPE> m_DelayType;
	void LoadYoungYakDelay();
	CYHHashTable<YOUNGYAK_ITEM_DELAY_TYPE>* GetYoungYakDelay(){return &m_DelayType;}

	void LoadUserCommendList();

	void LoadItemChangeRateofLv(); 
	void LoadItemChangeRateofAttrib();
	
	void LoadMonsterInfoInMapList(); 

	std::vector<std::string>& GetUserCommendList() { return m_UserCommendList; }
	std::vector<MonsterInfoInMap>& GetMonsterInfoInMapList() { return m_MonsterInfoInMapList; }
	int CheckAttribItemIndex(WORD idx);
	std::vector<ItemChangeRateofAttrib>& GetItemChangeRateAttribList() { return m_ItemChangeRateAttribList; }
	std::vector<ItemChangeRateofLv>& GetItemChangeRateOfLvList() { return m_ItemChangeRateofLvList; }
	unsigned int GetMinMonsterLvInMap(unsigned int MapNum);
	
	
	//------------------------------------------------------------------------------------------------
public:

	WORD m_TotalNpcGroupCount;
	WORD m_MaxNpcRegenCount;

	CGameResourceManager();
	virtual ~CGameResourceManager();

	CharBasicStats				m_BasicStatsForJob[MAX_JOB];

	DWORD m_PLAYERxMONSTER_POINT[MAX_PLAYERLEVEL_NUM][MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1];

	EXPTYPE m_CharacterExpPoint[MAX_CHARACTER_LEVEL_NUM];
	BOOL LoadExpPoint();
	EXPTYPE GetMaxExpPoint(LEVELTYPE level)
	{ 
		if( level > 0 )			return m_CharacterExpPoint[level-1];
		else					return m_CharacterExpPoint[0];
	}
	BOOL LoadPlayerxMonsterPoint();
	DWORD GetPLAYERxMONSTER_POINT(LEVELTYPE level,int levelgap);

	CYHHashTable<BASE_PET_LIST> m_PetListTable;
	BOOL LoadPetList();
	BASE_PET_LIST*  GetPetListInfo( int idx );
	BASE_PET_LIST*  GetPetListInfoByName( char* PetName );
	void ResetPetList();

	PET_RULE m_PetRule;
	BOOL LoadPetRule();
	PET_RULE* GetPetRule()	{	return &m_PetRule;	}

	PET_BUFF_LIST* m_pBuffList;
	BOOL LoadPetBuffList();
	PET_BUFF_LIST* GetPetBuffInfo(int idx);
	void ResetPetBuffList();

	DWORD m_TitanSpellCostInfo[MAX_MAP_ENUMCOUNT+1];	
	BOOL LoadTitanSpellCostInfo();
	DWORD GetTitanSpellCost(WORD MapNum);

	CYHHashTable<BASE_TITAN_LIST> m_TitanListTable;
	BOOL LoadTitanList();
	BASE_TITAN_LIST* GetTitanListInfo(DWORD idx);
	TITANINFO_GRADE* GetTitanGradeInfo(DWORD idx, DWORD grade);
	void RemoveTitanList();

	TITAN_RULE	m_TitanRule;
	BOOL	LoadTitanRule();
	TITAN_RULE*	GetTitanRule()	{	return &m_TitanRule;	}
	
	enum DayOfWeek{Sun,Mon,Tue,Wed,Thu,Fri,Sat,Week,};
	BYTE FlagNpcSet[eFlgKind][Week];
	BOOL LoadFlagNpcInfo();
	BYTE GetFlagFromDate(int flgKind, int day);

	BOOL LoadMonsterList();

	CYHHashTable<BASE_MONSTER_LIST> m_SMonsterListTable;
	void LoadSMonsterList();
	void LoadSMonsterInfo( int idx, char * fileName );
	BASE_MONSTER_LIST * GetMonsterListInfo(int idx);
	int GetMonsterIndexForName( char* pStrName );
	void ResetMonsterList();

	void GetMonsterStats(WORD wMonsterKind, monster_stats * stats);	
	BOOL LoadNpcChxList();
	char* GetMonsterChxName(DWORD index);
	char* GetNpcChxName(DWORD index);
 
	BOOL LoadTacticStartInfoList();
	BOOL LoadTaticAbilityInfo();	
	
	NPC_LIST m_NpcList[MAX_NPC_NUM];
	BOOL LoadNpcList(NPC_LIST* pNpcList);

	NPC_LIST* GetNpcInfo(WORD kind){return &m_NpcList[kind-1];}

	STATIC_NPCINFO*	GetStaticNpcInfo( WORD wNpcUniqueIdx );

#define BOBUSANG_POSNUM_MAX	4
	BobusangPosPerMap	m_PosPerMap[MAX_MAP_ENUMCOUNT][BOBUSANG_POSNUM_MAX];	
	BOOL LoadBobusangNpcInfo();
	BobusangPosPerMap* GetBobusangPos(DWORD mapNum, DWORD posNum);

	BOOL LoadMapChangeArea();

	//BOOL LoadMapChangeArea(MAPCHANGE_INFO* pMapChangeArea);
	BOOL LoadLoginPoint(LOGINPOINT_INFO* pLogin);
	//MAPCHANGE_INFO m_MapChangeArea[MAX_POINT_NUM];
	CYHHashTable<MAPCHANGE_INFO> m_MapChangeArea;
	LOGINPOINT_INFO m_LoginPoint[MAX_POINT_NUM];
	void GetRegenPoint(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex);
	void GetRegenPointFromEventMap(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex,DWORD dwTeam);
	MAPCHANGE_INFO* GetMapChangeInfo(DWORD Kind)
	{
		MAPCHANGE_INFO* pInfo = NULL;
		pInfo = m_MapChangeArea.GetData(Kind);
		/*for(int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_MapChangeArea[i].Kind == Kind)
				return &m_MapChangeArea[i];
		}*/
		//return NULL;
		return pInfo;
	}
	MAPCHANGE_INFO* GetMapChangeInfoForDestMapNum( WORD wDestMapNum )
	{
		MAPCHANGE_INFO* pInfo = NULL;
		m_MapChangeArea.SetPositionHead();
		while (pInfo = m_MapChangeArea.GetData())
		{
			if (pInfo->MoveMapNum == wDestMapNum)
				return pInfo;
		}
		return NULL;
		//for(int i = 0; i < MAX_POINT_NUM; i++)
		//{
		//	if(m_MapChangeArea[i].MoveMapNum == wDestMapNum)
		//	{
		//		return &m_MapChangeArea[i];
		//	}
		//}
		//return NULL;
	}

	MAPCHANGE_INFO* GetMapChangeInfoFromToMapNum( WORD wFromMapNum, WORD wToMapNum )
	{
		MAPCHANGE_INFO* pInfo = NULL;
		m_MapChangeArea.SetPositionHead();
		while (pInfo = m_MapChangeArea.GetData())
		{
			if (pInfo->CurMapNum==wFromMapNum&&pInfo->MoveMapNum==wToMapNum)
				return pInfo;
		}
		return NULL;
		//for(int i = 0; i < MAX_POINT_NUM; ++i)
		//{
		//	if(m_MapChangeArea[i].CurMapNum == wFromMapNum && m_MapChangeArea[i].MoveMapNum == wToMapNum)
		//		return &m_MapChangeArea[i];
		//}
		//return NULL;
	}
	
	LOGINPOINT_INFO* GetLoginPointInfo(DWORD Kind)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].Kind == Kind)
				return &m_LoginPoint[i];
		}
		return NULL;
	}
	LOGINPOINT_INFO* GetLoginPointInfoFromMapNum(WORD MapNum)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].MapNum == MapNum)
				return &m_LoginPoint[i];
		}
		ASSERT(0);
		return &m_LoginPoint[0];
	}
	BOOL CheckCorrectLoginPoint(VECTOR3* pos);

	BOOL LoadDeveloperInfo();

	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;
	BOOL LoadAvatarEquipList();

	CYHHashTable<SHOPITEMDUP>		m_ShopItemDupOptionTable;
	BOOL LoadShopItemDupList();

	BOOL	LoadUniqueItemOptionList();
	UNIQUE_ITEM_OPTION_INFO*	GetUniqueItemOptionList( DWORD dwItemIdx );

	BOOL	LoadUniqueItemMixList();
	UNIQUE_ITEM_MIX_INFO*	GetUniqueItemMixList( DWORD dwItemIdx );

	CYHHashTable<stMAPKINDINFO>* GetMapKindInfoTable() { return &m_MapKindStateInfoTable; }	
	void	LoadMapKindInfo();
	stMAPKINDINFO* GetMapKindInfo(DWORD dwMapNum);

	BOOL LoadItemChxList();
	char* GetItemChxName(DWORD index);

	BOOL LoadNomalClothesSkinList();	
	SKIN_SELECT_ITEM_INFO* GetNomalClothesSkinList( DWORD dwKeyIndex );
	DWORD GetNomalClothesSkinListCountNum();
	void RemoveNomalClothesSkinList();
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* GetNomalClothesSkinTable();

	BOOL LoadCostumeSkinList();	
	SKIN_SELECT_ITEM_INFO* GetCostumeSkinList( DWORD dwKeyIndex );
	DWORD GetCostumeSkinListCountNum();
	void RemoveCostumeSkinList();
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* GetCostumeSkinTable();
	//////////////////////////////////////////////////////////////////
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

	MOD_LIST			m_ModFileList[GENDER_MAX];
	MOD_LIST			m_HairModList[GENDER_MAX];
	MOD_LIST			m_FaceModList[GENDER_MAX];
	BOOL LoadModFileList(MOD_LIST pModList[GENDER_MAX]);
	BOOL LoadHairModFileList(MOD_LIST pHairModList[GENDER_MAX]);
	BOOL LoadFaceModFileList(MOD_LIST pFaceModList[GENDER_MAX]); //pjscode
	BOOL LoadBodyModFileList(MOD_LIST pBodyModList[GENDER_MAX]);

	char*				m_TitanChxList[TITANKIND_MAX][MAX_TITAN_GRADE];
	MOD_LIST			m_TitanModFileList;
    MOD_LIST			m_TitanAttachModList[TITANKIND_MAX];	
	BOOL LoadTitanModFileList(MOD_LIST* pModList);

	BOOL LoadTitanAttachList(MOD_LIST pAttachModList[TITANKIND_MAX]);

	GAMEDESC_INI		m_GameDesc;
	BOOL LoadGameDesc();
	
	void LoadPreDataTable();
	void DeletePreLoadData();
	void LoadPreMonsterData(sPRELOAD_INFO* pPreLoadInfo, MAPTYPE MapNum);	
	CYHHashTable<sPRELOAD_INFO>		m_PreLoadDataTable;
	sPRELOAD_INFO*	GetPreDataInfo(int MapNum)		{	return m_PreLoadDataTable.GetData(MapNum);	}
	cPtrList m_PreLoadEfflist;
	cPtrList* GetPreLoadEfflist()	{	return &m_PreLoadEfflist;	}
	cPtrList m_PreLoadModlist;
	cPtrList* GetPreLoadModlist()	{	return &m_PreLoadModlist;	}
	WORD* m_PreLoadItemlist;
	WORD* GetPreLoadItemlist()	{	return m_PreLoadItemlist;	}

	CYHHashTable<sPRELOAD_EFFECT_INFO>		m_PreLoadEffectTable;
	sPRELOAD_EFFECT_INFO*	GetPreEffectInfo(int MapNum)		{	return m_PreLoadEffectTable.GetData(MapNum);	}
	///////////////////////////////////////////////////////////////////////////////
	CYHHashTable<PLUSITEM_EFFECT_INFO> m_PlusItemEffectInfoTable;	

	CYHHashTable<WIN_DPI>  m_WindowDpiList;  
	BOOL LoadPlusItemEffect();
	void RemovePlusItemEffectTable();
	PLUSITEM_EFFECT_INFO* GetPlusItemEffectInfo(WORD wItemType);
	////////////////////////////////////////////////////////////////////////////////
	void PreLoadData();	
	
	SEVERLIST	m_ServerList[3][50];
	int			m_nMaxServerList1;
	int			m_nMaxServerList2;
	int			m_nMaxServerList3;
	BOOL		LoadServerList();
	char*		GetServerSetName();	
	void GetConnectSp();
	SEVERLIST * GetServerList(WORD Type,WORD Index); 
	//[分辨率相关][By:十里坡剑神][QQ:112582793][2019/3/10][16:25:08]
	int  m_bPrintScreenIndex;      
	int	GetPrintScreenIndex()	{	return m_bPrintScreenIndex;	}
	void SetPrintScreenIndex(int Index){m_bPrintScreenIndex=Index;}
	WIN_DPI * GetPrintScressInfo(int PrintScreenIndex){return m_WindowDpiList.GetData(PrintScreenIndex);}

//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
	MAPTYPE m_LoadingMapNum;
	MAPTYPE GetLoadMapNum()
	{
		return m_LoadingMapNum;
	}
	void SetLoadMapNum(MAPTYPE Mapnum);
	

	BOOL LoadNpcRegen(NPC_REGEN pNpcRegen[MAX_NPC_GROUPNUM][MAX_NPC_REGEN_NUM]);

	//MAPCHANGE_INFO* GetMapChangeAreaValue(){return m_MapChangeArea;}
	LOGINPOINT_INFO* GetLoginPointValue(){return m_LoginPoint;}
	
	void SetNpcRegenLoadFlag(BOOL flag){m_bNpcRegenLoad = flag;}
	void SetNpcListLoadFlag(BOOL flag){m_bNpcListLoad = flag;}
	BOOL GetNpcRegenLoadFlag(){return m_bNpcRegenLoad;}
	BOOL GetNpcListLoadFlag(){return m_bNpcListLoad;}
	

	CYHHashTable<STATIC_NPCINFO>	m_StaticNpcTable;
	BOOL	LoadStaticNpc();
	void	ReleaseStaticNpc();
	NPC_LIST* GetNpcList(WORD JobKind);
	
	CYHHashTable<STATIC_NPCINFO>	m_WareNpcTable;
	BOOL	IsInWareNpcPosition( VECTOR3* pvPos );

	CYHHashTable<HIDE_NPC_INFO>		m_HideNpcTable;
	BOOL	LoadHideNpcList();
	HIDE_NPC_INFO* GetHideNpcInfo(WORD UniqueIdx) { return m_HideNpcTable.GetData(UniqueIdx); }
	CYHHashTable<HIDE_NPC_INFO>* GetHideNpcTable() { return &m_HideNpcTable; }

	int		m_FullMoonEventRate[eFULLMOONEVENT_MAX];
	BOOL	LoadFullMoonEventRate();
	int		GetFullMoonEventIdx( int Rate );

	BOOL	LoadLevPenalty();
	LEV_PENALTY*	GetLevPenalty( LEVELTYPE Lev );

	BOOL	LoadPaneltyTime();
	DWORD	GetPaneltyTime()			{	return m_PaneltyTime;	}

	void LoadTitanMapMonsterDropInfo();
	
	MONSTERDROP_TIMAP*	GetTitanMapMonsterDropInfo( WORD wLevel )		{	return m_TitanMapMosterDropTable.GetData( wLevel );	}
#ifdef _MAPSERVER_ //[副本地图信息相关函数][By:十里坡剑神][QQ:112582793][2019/3/10][16:26:08]
	BOOL LoadFBMapInfo();
	BOOL IsFBMap(WORD MapNum);
	BYTE GetFBMapKind(WORD MapNum);
	void LoadCanUseAAKLifeInfo();
	BOOL IsCanUseAAKLife(WORD monster);
#endif  //_MAPSERVER_
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
};
EXTERNGLOBALTON(CGameResourceManager);
#endif // !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)


