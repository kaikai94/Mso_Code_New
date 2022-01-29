



// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
#define AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"


class cStreetStallTitleTip; // LBS 03.09.23

class CPet;
//class CPetBase;
class CTitan;
class CMapObject;

class CPlayer : public CObject  
{
	void InitPlayer(CHARACTER_TOTALINFO* pTotalInfo);

	int m_StandardMotion;
	int m_Move_Start_Motion;
	int m_Move_Ing_Motion;
	int m_Move_End_Motion;

	ABILITY_STATS m_Ability;
	
	WANTEDTYPE			m_WantedIDX;
	
	int	m_ItemUseMotion;
	int m_ItemUseEffect;

	BOOL m_bInTitan;
	float m_TitanMoveSpeed;
	BOOL m_bTitanPreView;

	DWORD m_dwSetItemEffectID;
protected:
	CHARACTER_TOTALINFO m_CharacterInfo;	
	SHOPITEMOPTION		m_ShopItemOption;	
	AVATARITEMOPTION	m_AvatarOption;

	SKILLSTATSOPTION	m_SkillStatsOption;

	TITAN_APPEARANCEINFO	m_TitanAppearanceInfo;
	TITAN_APPEARANCEINFO	m_TitanPreViewAppInfo;

	cStreetStallTitleTip* m_pSSTitle;

	CPet*				m_pPet;

	CTitan*				m_pTitan;

	CMapObject*			m_DecorationAddress;
	DWORD				m_dwDecorationStartTime;
	BOOL				m_bDecorationFlag;	

	DWORD m_dwSkinDelayTime;
	BOOL m_bSkinDelayResult;

public:
	CPlayer();
	virtual ~CPlayer();


	inline ABILITY_STATS* GetAbilityStats()		{	return &m_Ability;	}
	
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	CHARACTER_TOTALINFO* GetCharacterTotalInfo()	{	return &m_CharacterInfo;	}

	void Process();

	friend class CObjectManager;
	friend class CAppearanceManager;
	friend void CS_BtnFuncEnter(LONG lId, void* p, DWORD we);

	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	WORD GetWearedItemIdx(DWORD WearedPosition)					{	return m_CharacterInfo.WearedItemIdx[WearedPosition];	}
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();
	WORD GetTitanWearedItemIdx(DWORD WearedPosition);


	void SetMunpaID(DWORD MunpaID)	{ m_CharacterInfo.MunpaID = MunpaID; 	}
	DWORD GetMunpaIdx()		{ return m_CharacterInfo.MunpaID; 	}
	
	void SetMunpaMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} 
	BYTE GetMunpaMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} 

	void SetGuildIdxRank(DWORD GuildIdx, BYTE Rank); 
	DWORD GetGuildIdx()		{ return m_CharacterInfo.MunpaID; 	}
	void SetGuildIdx(DWORD GuildIdx);
	void SetGuildMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} 
	BYTE GetGuildMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} 
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	void SetNickName(char* pNickName);
	void SetGuildName(char* GuildName);

	char* GetGuildName();

	
	MARKNAMETYPE GetGuildMarkName();
	virtual void ClearGuildInfo();
	
	char* GetNickName();

	void SetStreetStallBalloonImage( DWORD eKind, BOOL bFlag );	
	
	void SetPeaceMode(bool bPeace)		{	m_CharacterInfo.bPeace = bPeace;	}
	
	void SetBaseMotion();
	int GetStandardMotion()		{return m_StandardMotion;	}
	
	virtual void SetMotionInState(BYTE State);

	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );

	virtual void OnStartObjectState(BYTE State);
	virtual void OnEndObjectState(BYTE State);
	//////////////////////////////////////////////////////////////////////////
	void ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetStallTitle( char* strTitle );
	void ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetBuyStallTitle( char* strTitle );
	/////////////////////////////////////////////////
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);
	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal);
	
	virtual DWORD GetLife(){	return m_CharacterInfo.Life; }
	virtual void SetMaxLife(DWORD maxlife){ m_CharacterInfo.MaxLife = maxlife; }
	virtual void SetLife(DWORD life, BYTE type = 1);
	virtual DWORD DoGetMaxLife();

	virtual DWORD GetShield(){	return m_CharacterInfo.Shield; }
	virtual void SetMaxShield(DWORD maxShield){ m_CharacterInfo.MaxShield = maxShield; }
	virtual void SetShield(DWORD Shield, BYTE type = 1);
	virtual DWORD DoGetMaxShield();

	// 2014-08-12 获取魔法值 为属性!
	virtual DWORD GetNaeRyuk(){return m_CharacterInfo.naeryuk;}
	virtual float DoGetMoveSpeed();

	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	
	virtual void SetLevel(LEVELTYPE level) { m_CharacterInfo.Level = level; }
	LEVELTYPE GetLevel() { return m_CharacterInfo.Level; }
	
	BYTE GetGender() { return m_CharacterInfo.Gender; }
	//////////////////////////////////////////////////////////////////////////
	BOOL OnEndEffectProc(HEFFPROC hEff);
	//////////////////////////////////////////////////////////////////////////
	void SetPKMode( BOOL bPKMode );
	BOOL IsPKMode()		{ return m_CharacterInfo.bPKMode; }
	virtual void SetBadFame(FAMETYPE val);
	FAMETYPE GetBadFame()			{ return m_CharacterInfo.BadFame;	}	

	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
	
	void SetAvatarInfo(WORD* Avatar)			{	memcpy( m_ShopItemOption.Avatar, Avatar, sizeof(WORD)*eAvatar_Max);	}
	void SetShopItemOptionInfo(SHOPITEMOPTION* pInfo);
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	void SetItemUseMotionIdx( int Idx )			{	m_ItemUseMotion = Idx;	}
	void SetItemUseEffectIdx( int Idx )			{	m_ItemUseEffect = Idx;	}

	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}

	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	void SetPet(CPet* pet);
	CPet* GetPet()					{	return m_pPet;	}

	virtual void	SetStage( BYTE Stage );
	BYTE			GetStage()		{ return m_CharacterInfo.Stage; }

	void	SetGuildUnionIdx( DWORD dwUnionIdx )	{ m_CharacterInfo.dwGuildUnionIdx = dwUnionIdx; }
	void	SetGuildUnionName( char* pName )		{ strncpy( m_CharacterInfo.sGuildUnionName, pName, MAX_GUILD_NAME+1 ); }

	void	SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_CharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }
	DWORD	GetGuildUnionIdx()		{ return m_CharacterInfo.dwGuildUnionIdx; }
	char*	GetGuildUnionName()		{ return m_CharacterInfo.sGuildUnionName; }
	DWORD	GetGuildUnionMarkIdx()	{ return m_CharacterInfo.dwGuildUnionMarkIdx; }
	
	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );

	void SetSiegeName( DWORD NameType );
	void SetRestraintMode( bool val )			{	m_CharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_CharacterInfo.bRestraint;	}

	void CalcAvatarOption();

//#ifdef _JAPAN_LOCAL_
//	void	SetMainCharAttr( int nMainAttr )	{ m_CharacterInfo.nMainAttr = nMainAttr; }
//	void	SetSubCharAttr( int nSubAttr )		{ m_CharacterInfo.nSubAttr = nSubAttr; }
//	int 	GetMainCharAttr()					{ return m_CharacterInfo.nMainAttr; }		// 林己	
//	int		GetSubCharAttr()					{ return m_CharacterInfo.nSubAttr; }		// 何己
//	int		WhatIsAttrRelation( int nOtherMainAttr );
//#endif
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);
	//////////////////////////////////////////////////////////////////////////
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 

	BOOL IsDetect() 
	{ 
		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////
	void SetFullMoonEventIndex( BYTE Index )		{	m_CharacterInfo.EventIndex = Index; }
	BYTE GetFullMoonEventIndex()					{	return m_CharacterInfo.EventIndex;	}

	void SetTitanAppearanceInfo(TITAN_APPEARANCEINFO* pInfo);
	void SetTitanMoveSpeed();
	void RidingTitan(BOOL bVal)		{	m_bInTitan = bVal;	}
	BOOL InTitan()					{	return m_bInTitan;	}
	
	void SetTitanPreView( TITAN_APPEARANCEINFO* pInfo );
	void ReleaseTitanPreView();
	BOOL IsTitanPreView()			{	return m_bTitanPreView;	}
	TITAN_APPEARANCEINFO*	GetTitanPreViewInfo()		{	return &m_TitanPreViewAppInfo;	}
	

	void	SetCurTitan(CTitan* pTitan);
	CTitan* GetCurTitan()			{	return m_pTitan;	}

	TITAN_APPEARANCEINFO* GetTitanAppearInfo()		{	return	&m_TitanAppearanceInfo;	}
	float GetTitanMoveSpeed()		{	return m_TitanMoveSpeed;	}

	bool GetAvatarView()							{	return m_CharacterInfo.bNoAvatarView;	}
	void SetAvatarView( bool bNoAvatarView )		{	m_CharacterInfo.bNoAvatarView = bNoAvatarView;	}

	DWORD GetSetItemEffectID();
	void SetSetItemEffectID(DWORD dwEffectID);

	CMapObject* GetDecorationAddress()	{	return m_DecorationAddress;	}
	void SetDecorationAddress(CMapObject* pMapObject)	{	m_DecorationAddress = pMapObject;	}
	DWORD GetDecorationStartTime()	{	return m_dwDecorationStartTime;	}
	BOOL GetDecorationFlag()	{	return m_bDecorationFlag;	}
	void SetDecorationInfo(DWORD dwTime, BOOL bFlag)	{	m_dwDecorationStartTime = dwTime;	m_bDecorationFlag = bFlag;	}

	void InitSkinDelayTime();
	void StartSkinDelayTime();
	BOOL CheckSkinDelay();
	/////////////////////////////////////////////
	void	SetMarryName(char* pName);
	char*	GetMarryName();
	bool	IsMarry(){ return strcmp(GetMarryName(), "0") != 0; }

	void    ClearShiFu(DWORD ShiFuId);
	char    * GetShiFuName();
	bool    IsFlashNameFlag(){return  m_CharacterInfo.FlashNameFlag!=0;}
	void    SetFlashNameFlag(WORD Flag){ m_CharacterInfo.FlashNameFlag=Flag;}
	WORD    GetFlashNameFlag(){return m_CharacterInfo.FlashNameFlag;}
	//[图片称号函数定义][By:十里坡剑神][QQ:112582793][2017/11/29]
	bool	IsImageName(){return m_CharacterInfo.ImageName!=0;}
	void	SetImageName(WORD val){m_CharacterInfo.ImageName = val;}
	WORD	GetImageName(){return m_CharacterInfo.ImageName;}
};

#endif // !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)


