



// Item.h: interface for the CItem class.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)
#define AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./BaseItem.h"
#include "GameResourceManager.h"


class CExchangeItem;

class CItem : public CBaseItem
{
protected:

	ITEMBASE m_ItemBaseInfo;
	
	POSTYPE			m_LinkPos;	

	CExchangeItem*	m_pLinkItem;

	DWORD			m_Param;	

	cImage			m_DelayImage;

    cImage          m_LockImageEx; 

public:
	BOOL m_bAdded;

	CItem(ITEMBASE* pInfo);
	virtual ~CItem();

	virtual inline DWORD	GetDBIdx()					{	return m_ItemBaseInfo.dwDBIdx;	}
	virtual inline WORD		GetItemIdx()				{	return m_ItemBaseInfo.wIconIdx;	}
	virtual inline POSTYPE	GetPosition()				{	return m_ItemBaseInfo.Position;	}
	virtual inline void		SetPosition(POSTYPE pos)	{	m_ItemBaseInfo.Position = pos;	}


	virtual inline POSTYPE	GetQuickPosition()				{	return m_ItemBaseInfo.QuickPosition;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{	m_ItemBaseInfo.QuickPosition = pos;		}

	virtual inline DWORD	GetRareness()					{   return m_ItemBaseInfo.RareIdx; }
	virtual inline void		SetRareness(DWORD val)			{m_ItemBaseInfo.RareIdx=val;}

	virtual inline DURTYPE	GetDurability()					{	return m_ItemBaseInfo.Durability;	}
	virtual inline void		SetDurability(DURTYPE dur)		{	m_ItemBaseInfo.Durability = dur;		}

	inline POSTYPE GetLinkPosition() { return m_LinkPos; }
	inline void SetLinkPosition(POSTYPE pos)	{ m_LinkPos = pos; }
	void SetLinkItem( CExchangeItem* pItem )	{ m_pLinkItem = pItem; }
	CExchangeItem* GetLinkItem()				{ return m_pLinkItem; }

	const ITEMBASE*	GetItemBaseInfo() { return &m_ItemBaseInfo; }
	void SetItemBaseInfo( ITEMBASE * itemInfo );
	
	virtual void SetLock( BOOL bLock,BOOL bSend=FALSE );
	

	eITEM_KINDBIT GetItemKind();

	
	const ITEM_INFO* GetItemInfo();

	

	virtual void Render();
	
	virtual BOOL CanMoveToDialog( cDialog* ptargetdlg );
	virtual BOOL CanDelete();
	

	void SetItemParam(ITEMPARAM Param)		{ m_ItemBaseInfo.ItemParam = Param;	}

	DWORD GetItemParam()					{ return m_ItemBaseInfo.ItemParam;	}
	void SetUseParam( DWORD Param )			{ m_Param = Param;	}
	DWORD GetUseParam()						{ return m_Param;	}

	virtual inline	WORD	GetStatic()				{	return m_ItemBaseInfo.ItemStatic;	}
	virtual inline void		SetStatic(WORD Val)	{	m_ItemBaseInfo.ItemStatic =Val;	}

	virtual inline DWORD    GetGrow()  {return   m_ItemBaseInfo.ItemGrow;}
	virtual inline void     SetGrow(DWORD Val)  { m_ItemBaseInfo.ItemGrow=Val;}
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	virtual inline WORD		GetAbrasion(){return m_ItemBaseInfo.ItemAbrasion;}
	virtual inline void	SetAbrasion(WORD val){m_ItemBaseInfo.ItemAbrasion=val;}

	virtual inline WORD		GetMaxItemAbrasion(){return m_ItemBaseInfo.MaxItemAbrasion;}
	virtual inline void	SetMaxItemAbrasion(WORD val){m_ItemBaseInfo.MaxItemAbrasion=val;}

	virtual inline char * GetMaker(){return m_ItemBaseInfo.ItemMaker;}
	virtual inline void   SetMaker(char * maker){strcpy(m_ItemBaseInfo.ItemMaker,maker);}

	virtual inline char * GetMaker1(){return m_ItemBaseInfo.ItemMaker1;}
	virtual inline void   SetMaker1(char * maker){strcpy(m_ItemBaseInfo.ItemMaker1,maker);}


};

#endif // !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)


