// ChannelSystem.h: interface for the CChannelSystem class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)
#define AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Channel.h"
#define CHANNELSYSTEM CChannelSystem::GetInstance()

class CChannelSystem  

{
	CChannel*	m_Channel[MAX_CHANNEL_NUM];
	char		m_ChannelName[MAX_CHANNEL_NAME+1];
	DWORD		m_dwCount;
	
public:
	CChannelSystem();
	virtual ~CChannelSystem();
	GETINSTANCE(CChannelSystem);

	void	LoadChannelInfo();
	void	CreateChannel(BYTE Kind, BYTE num);
	void	IncreasePlayerNum(DWORD ChannelID);
	void	DecreasePlayerNum(DWORD ChannelID);
	void	SendChannelInfo(MSG_DWORD3* pInfo, DWORD dwConnectionIndex);
	DWORD	GetChannelID(int num);
	DWORD	GetChannelCount()			{ return m_dwCount;	}
	void    SendMapChangeChannelInfo(MSG_DWORD4* pInfo, DWORD dwConnectionIndex);
	float	GetRegenRate(DWORD ChannelID);
	float	GetDropRate(DWORD ChannelID);
	BOOL	IsBattleChannel(DWORD ChannelID);
	void	GetChannelInfo( MSG_CHANNEL_INFO* pInfo );
protected:
	CChannel	m_QMChannel[1000];
	BOOL		m_bQuestMap;

public:
	void	InitQuestMapChannel();
	DWORD	CreateQuestMapChannel();
	void	DestroyQuestMapChannel( DWORD dwChannel );
	BOOL	IsQuestMap()		{ return m_bQuestMap; }
	
	void	Init( WORD wMapNum );
	DWORD	GetRandomChannelID();
	DWORD	GetLastChannelID();
	void	SendChannelInfoToMS( DWORD dwConnectionIndex );
	BOOL    IsPeaceChannel(WORD wChannel )
	{
		if(wChannel<1)
			wChannel=1;
		return m_Channel[wChannel-1]->IsPeaceChannel();
	}
	float  GetExpRate(WORD wChannel)
	{
		if(wChannel<1)
			wChannel=1;
		return m_Channel[wChannel-1]->GetExpRate();
	}
	float GetAbilityRate(WORD wChannel)
	{
		if(wChannel<1)
			wChannel=1;
		return m_Channel[wChannel-1]->GetAbilityRate();
	}
	float GetMoneyRate(WORD wChannel)
	{
		if(wChannel<1)
			wChannel=1;
		return m_Channel[wChannel-1]->GetMoneyRate();
	}
};
#endif // !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)



