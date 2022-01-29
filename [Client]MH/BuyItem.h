




// BuyItem.h: interface for the CBuyItem class.
// 备概侩 啊惑 酒捞袍
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
#define AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "./Interface/cIcon.h"


class CItem;

class CBuyItem : public cIcon  
{

	WORD		m_ItemIdx;
	WORD		m_nVolume;
	MONEYTYPE		m_nMoney;

	ITEMPARAM	m_dwParam;
	BOOL		m_bVolume;
	DWORD       m_bGrow;  
	DWORD		m_RareIdx;
	DWORD		m_OptionIdx;
	 //[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	WORD        m_Abrasion;
	WORD		m_MaxItemAbrasion;
	char		m_Maker[MAX_NAME_LENGTH+1];
	char		m_Maker1[MAX_NAME_LENGTH+1];

public:
	CBuyItem();
	virtual ~CBuyItem();



	void InitItem(ITEMBASE ItemInfo, WORD Volume, MONEYTYPE Money);
	
	WORD GetVolume() { return m_nVolume; }
	void SetVolume(WORD Volume) { m_nVolume = Volume; }
	
	MONEYTYPE GetMoney() { return m_nMoney; }
	void SetMoney(MONEYTYPE Money) { m_nMoney = Money; }



	WORD GetItemIdx() { return m_ItemIdx; }

	virtual DWORD   GetGrow()  {return  m_bGrow;}          
    virtual void    SetGrow(DWORD Val)  {m_bGrow=Val;}
	virtual void Render();
	virtual WORD GetStatic(){return 0;}
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	virtual WORD GetAbrasion(){return m_Abrasion;}
	virtual void SetAbrasion(WORD val){m_Abrasion = val;}

	virtual WORD GetMaxItemAbrasion(){return m_MaxItemAbrasion;}
	virtual void SetMaxItemAbrasion(WORD val){m_MaxItemAbrasion = val;}

	virtual char * GetMaker(){return m_Maker;}
	virtual void SetMaker(char * maker){strcpy(m_Maker,maker);}

	virtual char * GetMaker1(){return m_Maker;}
	virtual void SetMaker1(char * maker){strcpy(m_Maker1,maker);}
};

#endif // !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)


