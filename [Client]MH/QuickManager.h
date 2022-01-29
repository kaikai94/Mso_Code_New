
// QuickManager.h: interface for the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
#define AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndexGenerator.h"

#define QUICKMGR USINGTON(CQuickManager)


class cIcon;
class CItem;
class cImage;
class CQuickItem;
class CBaseItem;
class CQuickManager  
{

	CYHHashTable<CQuickItem> m_QuickHashForID;
public:
	CQuickManager();
	virtual ~CQuickManager();

	static void AddQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos);
	static void DeleteQuickPosition(POSTYPE& QuickPos, WORD SheetNum);

	static WORD GetQuickPosition(POSTYPE QuickPos, WORD SheetNum);
	static void AnalyzeQuickPosition(POSTYPE QuickPos, WORD* QuickPosList);

	static void UpdateQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos);
	static POSTYPE MergeQuickPosition(WORD* QuickPosList);

	static POSTYPE GetAbilityQuickPosition(BYTE Kind, BYTE Pos, ABILITY_TOTALINFO* pOutAbilityTotalInfo);
	static void AddAbilityQuickPosition(BYTE Kind, BYTE Pos, POSTYPE QuickPos, ABILITY_TOTALINFO* pOutAbilityTotalInfo);

	BOOL CanEquip(cIcon * pIcon);
	void Release();
	CQuickItem * GetQuickItem(DWORD dwDBIdx);

	void UseQuickItem(POSTYPE pos);

	void MoveQuickItemReal( POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos, bool DeleteToQuickPos);
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	BOOL AddQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx, POSTYPE OldSrcPos=0, WORD OldSrcItemIdx=0);

	BOOL SetQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx);

	void RemoveQuickItemReal(POSTYPE SrcPos, WORD wSrcItemIdx, POSTYPE QuickPos);
	void OverlapCheck(POSTYPE SrcPos);

	void RefreshQickItem();
	void RemQuickItem(POSTYPE QuickPos);


	BOOL CheckQPosForItemIdx( WORD ItemIdx );
	void ReleaseQuickItem(CQuickItem * quick);

	void SetQuickItem(POSTYPE QuickPos, CItem * pItem);
	DWORD GetUserSkillTime(){return m_UseSkillTime;}
	void UseQuickItem(POSTYPE pos,int Page);  //[内挂双技能释放重载][By:十里坡剑神][QQ:112582793][2017/11/27]

	void RemAllQuickItem();
private:
	CQuickItem * NewQuickItem(CBaseItem * pSrcIcon);
	CIndexGenerator m_IconIndexCreator;

	void GetImage( CQuickItem * pQuickItem );
	DWORD m_UseSkillTime;
};
EXTERNGLOBALTON(CQuickManager);
#endif // !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)


