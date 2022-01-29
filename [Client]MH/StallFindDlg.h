



//----------------------------------------------------------------------------------------------------
//  StallFindDlg   version:  1.0   ·  date: 03/31/2008
//
//  Copyright (C) 2008 - All Rights Reserved
//----------------------------------------------------------------------------------------------------
///	Index	Stiner(8)
///	@file	StallFindDlg.h
///	@author	이성민
///	@brief	노점 검색을 위한 다이얼로그
//----------------------------------------------------------------------------------------------------
#pragma once

#include "./Interface/cDialog.h"
#include "GameResourceStruct.h"

#define SEARCH_DELAY		3000	///< 검색 레이 시간
#define	ITEMVIEW_DELAY		1000	///< 아이템 뷰 레이
#define MAX_RESULT_PAGE		5		///< 최대 페이지
#define MAX_LINE_PER_PAGE	6		///< 최대 줄 수

class cListDialog;
class cEditBox;
class cButton;
class cPushupButton;
class cComboBox;
class cStatic;

class CMouse;
class CExchangeItem;

class CStallFindDlg : public cDialog
{
	struct TItemInfo
	{
		int		Type;		
		int		DetailType;
		DWORD	ItemIdx;
	};


	enum ITEM_TYPE
	{
		WEAPON,				
		CLOTHES,			
		ACCESSORY,			
		POTION,				
		MATERIAL,			

		ETC,			
		ITEM_MALL,			
		TITAN_ITEM,			

		ITEM_TYPE_COUNT,	
	};

	BOOL		m_bSearchedAll;			
	DWORD		m_dwSearchType;			

	cPtrList	m_ptrItemInfo;			

	int						m_nStallCount;						
	STREETSTALL_PRICE_INFO	m_arrStallInfo[MAX_STALLITEM_NUM];

	cComboBox*		m_pItemTypeCombo;							
	cComboBox*		m_arrItemDetailTypeCombo[ITEM_TYPE_COUNT];	


	cListDialog*	m_pItemList;		
	cListDialog*	m_pClassList;		

	cPushupButton*	m_pSellModeRadioBtn;	
	cPushupButton*	m_pBuyModeRadioBtn;	

	cStatic*		m_pNameStatic;		
	cStatic*		m_pPriceStatic;		

	cListDialog*	m_pStallList;					
	cPushupButton*	m_parrPageBtn[MAX_RESULT_PAGE];	
	cButton*		m_parrPageUpDownBtn[2];			

	int	m_nBasePage;			
	int	m_nMaxPage;				
	int	m_nCurrentPage;			

	int m_nItemType;				
	int m_nItemDetailType;		

	int	m_nSelectedItemListIdx;		
	int	m_nSelectedClassListIdx;	
	int	m_nSelectedStallListIdx;	

	DWORD	m_dwSelectedObjectIndex;
	DWORD	m_dwPrevSelectedType;	

	void	LoadItemList();
	void	UpdateItemList();

	void	UpdateStallList();

	void	SortStallList(BOOL flag);

	void	SetPage(int index);

	void	SetBasePage(BOOL bNext);

	BOOL	CheckDelay(DWORD dwDelayTime, int nID);


public:
	CStallFindDlg(void);
	~CStallFindDlg(void);


	void	Linking();

	void	SetSearchType(DWORD val);

	void	SetStallPriceInfo(SEND_STREETSTALL_INFO * pStallInfo);

	void	SendItemViewMsg();

	inline DWORD	GetSelectedObjectIndex()	{ return m_dwSelectedObjectIndex; };

	inline DWORD	GetPrevSelectedType()		{ return m_dwPrevSelectedType; };

	inline void		SetSelectedObjectIndex(DWORD dwIndex)	{ m_dwSelectedObjectIndex = dwIndex; };

	inline void		SetPrevSelectedType(DWORD dwType)		{ m_dwPrevSelectedType = dwType; };

	void	OnActionEvent(LONG lId, void * p, DWORD we);

	virtual void	SetActive(BOOL val);

	virtual DWORD	ActionEvent(CMouse * mouseInfo);

protected:
	void	OnClickSearchBtn();										
	void	OnClickSearchAllBtn();									
	void	OnClickFindTypeBtn(LONG lId, void * p, DWORD we);		
	void	OnEventTypeCombo(LONG lId, void * p, DWORD we);			
	void	OnEventDetailTypeCombo(LONG lId, void * p, DWORD we);	
	void	OnEventItemList(LONG lId, void * p, DWORD we);			
	void	OnEventClassList(LONG lId, void * p, DWORD we);			
	void	OnEventStallList(LONG lId, void * p, DWORD we);			
	void	OnClickPageBtn(LONG lId);								
	void	OnClickPageUpDonwBtn(LONG lId);							
	void	OnClose();												
};


