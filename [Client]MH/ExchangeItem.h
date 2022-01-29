




#ifndef _EXCHANGEITEM_H
#define _EXCHANGEITEM_H



#include "./interface/cIcon.h"


class CItem;

class CExchangeItem : public cIcon
{


protected:

	POSTYPE			m_Position;
	POSTYPE			m_LinkPos;
	CItem*			m_pLinkItem;

	cImage*			m_pLinkImage;
	
	WORD			m_ItemIdx;
	DURTYPE			m_Durability; 
	DWORD			m_dwRareIdx;
	DWORD			m_dwDBId;
	DWORD			m_dwParam;
    DWORD           m_Grow;   
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	WORD			m_Abrasion;
	WORD			m_MaxItemAbrasion;
	char			m_Maker[MAX_NAME_LENGTH+1];
	char			m_Maker1[MAX_NAME_LENGTH+1];
public:

	CExchangeItem();
	~CExchangeItem();

	void	InitItem( CItem* pItem, POSTYPE Pos, LONG id );
	 
	void	SetPosition( POSTYPE Pos )				{ m_Position = Pos; }
	POSTYPE GetPosition()							{ return m_Position; }

	void	SetLinkPos( POSTYPE LinkPos )			{ m_LinkPos = LinkPos; }
	POSTYPE GetLinkPos()							{ return m_LinkPos; }


	void	SetLinkItem( CItem* pItem )				{ m_pLinkItem = pItem; }

	CItem*	GetLinkItem()							{ return m_pLinkItem; }
	virtual void SetLock( BOOL bLock );

	inline DWORD	GetRareness()					{ return m_dwRareIdx; }
	inline void		SetRareness( DWORD RareIdx)		{ m_dwRareIdx = RareIdx; }

	inline DURTYPE	GetDurability()					{ return m_Durability; }
	inline void		SetDurability( DURTYPE dura )	{ m_Durability = dura; }

    inline DWORD    GetGrow()  {return  m_Grow;}          
    inline void     SetGrow(DWORD Val)  {m_Grow=Val;}
	
	void SetDBId( DWORD id )						{ m_dwDBId = id; }
	DWORD GetDBId()									{ return m_dwDBId; }

	char* GetItemName();

	virtual void Render();

	void SetItemParam(ITEMPARAM Param) { m_dwParam = Param; }
	ITEMPARAM GetItemParam()			{ return m_dwParam; }
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	void SetAbrasion(WORD val){m_Abrasion = val;}
	WORD GetAbrasion(){return m_Abrasion;}
	void SetMaxItemAbrasion(WORD val){m_MaxItemAbrasion = val;}
	WORD GetMaxItemAbrasion(){return m_MaxItemAbrasion;}

	void SetMaker(char * maker){strcpy(m_Maker,maker);}
	char * GetMaker(){return m_Maker;}
	void SetMaker1(char * maker){strcpy(m_Maker1,maker);}
	char * GetMaker1(){return m_Maker1;}
	//[保存物品编号，获取物品信息。强化绿化图标添加][By:十里坡剑神][QQ:112582793][2018/2/18]
	void SetItemIdx(WORD val){m_ItemIdx = val;}
	WORD GetItemIdx(){return m_ItemIdx;}
};



#endif


