



// DealItem.h: interface for the CDealItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)


#define AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "./Interface/cIcon.h"
#include "./BaseItem.h"


class CDealItem : public CBaseItem  
{
public:
	WORD m_wIdx;
	CDealItem();

	virtual ~CDealItem();

	void SetItemIdx(WORD itemIdx)
	{
		m_wItemIdx = itemIdx;
	}
	WORD GetItemIdx()

	{
		return m_wItemIdx;
	}
	DWORD GetSellPrice();
	DWORD GetBuyPrice();

	char* GetItemName();


	void SetCount(int count)
	{
		m_nItemCount = count;
	}
	int GetCount()	{	return m_nItemCount;	}
	void Render();
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	virtual inline void SetAbrasion(WORD val){}
	virtual inline WORD GetAbrasion(){return 100;}
	virtual inline void SetMaxItemAbrasion(WORD val){}
	virtual inline WORD GetMaxItemAbrasion(){return 100;}

	virtual inline void SetMaker(char* maker){}
	virtual inline char * GetMaker(){return "无";}

	virtual inline void SetMaker1(char* maker){}
	virtual inline char * GetMaker1(){return "无";}
private:
	WORD m_wItemIdx;

	//SW070626 焊何惑NPC
	int m_nItemCount;
};

#endif // !defined(AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)



