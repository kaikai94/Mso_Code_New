






// BaseItem.h: interface for the CBaseItem class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_)
#define AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cIcon.h"

class CBaseItem : public cIcon  
{
public:
	CBaseItem();

	virtual ~CBaseItem();

	virtual inline DWORD	GetDBIdx(){ return 0; }
	virtual inline WORD		GetItemIdx(){ return 0; }
	virtual inline POSTYPE	GetPosition(){ return 0; }
	virtual inline void		SetPosition(POSTYPE pos){}



	virtual inline POSTYPE	GetQuickPosition()				{	return 0;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{}
	
	virtual inline DWORD	GetRareness()					{   return 0;	}
	virtual inline DURTYPE	GetDurability()					{	return 0;	}
	virtual inline void		SetDurability(DURTYPE dur)		{}

	
	virtual inline void		SetExpPoint(DWORD point){}
	virtual inline DWORD	GetExpPoint()			{	return 0;	}
	virtual inline void		SetSung(BYTE sung)		{}
	virtual inline BYTE		GetSung()				{	return 0;	}

	virtual inline DWORD    GetGrow()  {return  0;}
	virtual inline void     SetGrow(DWORD Val)  {}

	virtual inline WORD GetStatic(){return 0;}
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	virtual inline void SetAbrasion(WORD val){}
	virtual inline WORD GetAbrasion(){return 100;}
	
	virtual inline void SetMaxItemAbrasion(WORD val){}
	virtual inline WORD GetMaxItemAbrasion(){return 100;}

	virtual inline void SetMaker(char* maker){}
	virtual inline char * GetMaker(){return "无";}

	virtual inline void SetMaker1(char* maker){}
	virtual inline char * GetMaker1(){return "无";}
};

#endif // !defined(AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_)


