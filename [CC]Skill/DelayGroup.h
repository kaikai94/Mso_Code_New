// DelayGroup.h: interface for the CDelayGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
#define AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDelayGroup  
{
	struct CDelay
	{
		WORD Kind;
		WORD Idx;
		DWORD StartTime;
		DWORD DelayTime;
	};
	static CMemoryPoolTempl<CDelay>* m_pDelayPool;
	static DWORD m_PoolRefCount;

	CYHHashTable<CDelay> m_DelayedTable;

	DWORD MakeKey(WORD Kind,WORD Idx);

	CDelay* m_pTitanPotionDelay;
	CDelay* m_pYoungYakPotionDelay[20];  //[吃药延迟变量][By:十里坡剑神][QQ:112582793][2017/11/27]

public:
	enum eDelayKind
	{
		eDK_Skill = 1,
		eDK_Item = 2,
	};

	CDelayGroup();
	virtual ~CDelayGroup();

	void Init();
	void Release();

	void AddDelay(WORD Kind,WORD Idx,DWORD DelayTime,DWORD ElapsedTime=0);

	float CheckDelay(WORD Kind,WORD Idx,DWORD* pRemainTime = NULL );

	void AddTitanPotionDelay( DWORD DelayTime,DWORD ElapsedTime=0 );
	float CheckTitanPotionDelay( DWORD* pRemainTime = NULL );
	//增加吃药品延迟时间 by:十里坡剑神	QQ:112582793
	void AddYoungYakPotionDelay(DWORD DelayTime,WORD Type,DWORD ElapsedTime=0);
	float CheckYoungYakPotionDelay(WORD TYPE,DWORD* pRemainTime = NULL);
};

#endif // !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
