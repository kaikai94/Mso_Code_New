




// StatusIconDlg.h: interface for the CStatusIconDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)
#define AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cImage.h"
#include "./Interface/cMultiLineText.h"
class CObject;
class CMouse;
#define STATUSICONDLG USINGTON(CStatusIconDlg)



struct ICONRENDERINFO
{
	DWORD	ItemIndex;

	BOOL	bPlus;

	BOOL	bAlpha;
	int		Alpha;
};



class CStatusIconDlg  
{
	int m_MaxDesc;
	DWORD DelItemIdx;   //右键删除buff 状态删除物品编号	by:十里坡剑神	QQ:112582793
	BOOL  CanDelBuf;	//右键删除buff 是否可删除状态	by:十里坡剑神	QQ:112582793
	BOOL	m_Show;
	StaticString* m_pDescriptionArray;


	CObject* m_pObject;
	WORD m_IconCount[eStatusIcon_Max];
	cImage m_StatusIcon[eStatusIcon_Max];
	ICONRENDERINFO m_IconInfo[eStatusIcon_Max];
	
	DWORD m_dwRemainTime[eStatusIcon_Max];

	DWORD m_dwStartTime[eStatusIcon_Max];

	VECTOR2 m_DrawPosition;		
	int m_MaxIconPerLine;		

	cMultiLineText m_toolTip;
	int m_CurIconNum;

	int		m_nQuestIconCount;
	
	void LoadDescription();
public:

	CStatusIconDlg();
	virtual ~CStatusIconDlg();

	void AddIcon(CObject* pObject,WORD StatusIconNum,WORD ItemIdx=0, DWORD dwRemainTime = 0);
	void AddQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveIcon(CObject* pObject,WORD StatusIconNum, WORD ItemIdx=0);
	void RemoveQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveAllQuestTimeIcon();

	void Render();


	void Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine);
	void Release();

	void SetOneMinuteToShopItem(DWORD ItemIdx);


	void AddQuestIconCount()	{ ++m_nQuestIconCount; }

	BOOL CheckSkillState(WORD StateIcon);

	void SetDelItemIdx(DWORD idx){DelItemIdx=idx;}  //右键删除buff 设置删除状态物品编号  by:十里坡剑神 QQ:112582793
	DWORD GetDelItemIdx(){return DelItemIdx;}		//右键删除buff 获取删除状态物品编号  by:十里坡剑神 QQ:112582793
	void SetCanDelBuf(BOOL val){CanDelBuf=val;}		//右键删除buff 设置可删除状态标志	by:十里坡剑神 QQ:112582793};
	//[buff图标隐藏设置][By:十里坡剑神][QQ:112582793][2017/11/22]
	void SetShow(BOOL val){m_Show=val;}
	BOOL IsShow(){return m_Show;}
};

EXTERNGLOBALTON(CStatusIconDlg)

#endif // !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)


