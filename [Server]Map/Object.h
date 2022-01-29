



// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)
#define AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridUnit.h"
#include "Status.h"
#include "PtrList.h"

class CBattle;
class CStunState;
class CAmplifiedPower;



enum eSpecialState
{
	eSpecialState_Stun,
	eSpecialState_AmplifiedPowerPhy,
	eSpecialState_AmplifiedPowerAtt,
	eSpecialState_DetectItem,
	eSpecialState_Max
};
class CAction;
class CSpecialState;

class CObject : public CGridUnit
{
public:  
	
	DWORD m_KillTime;
	DWORD m_KillTimeout;

private:
	CSpecialState* m_pSpecialState[eSpecialState_Max];

	TILEINDEX m_dwPreoccupiedTileIndex;		

	BOOL m_bInited;
	friend class CObjectFactory;
	friend class CCharMove;
	friend class CObjectStateManager;

	
protected:
	CObject();
	virtual ~CObject();

	DWORD m_AgentNum;
	BYTE m_ObjectKind;

	BASEOBJECT_INFO m_BaseObjectInfo;

	MOVE_INFO m_MoveInfo;
	STATE_INFO	m_ObjectState;

	RECOVER_TIME m_LifeRecoverTime;
	YYRECOVER_TIME m_YYLifeRecoverTime;

	RECOVER_TIME m_ShieldRecoverTime;
	YYRECOVER_TIME m_YYShieldRecoverTime;

	cPtrList m_StatusList;
	
	DWORD m_KyungGongNaeRyuk_LastCheckTime;
	
	CBattle* m_pBattle;

	virtual void DoSetInited()	{}
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)	{}

	friend class CCharacterCalcManager;

	virtual void SetState(BYTE state);

public:

	BYTE GetBattleTeam()						{	return m_BaseObjectInfo.BattleTeam;	}
	void SetBattleTeam( DWORD BattleTeam )		{	m_BaseObjectInfo.BattleTeam = (BYTE)BattleTeam;	}
	DWORD GetBattleID()				{	return m_BaseObjectInfo.BattleID;	}

	void SetBattleID(DWORD BattleID){ m_BaseObjectInfo.BattleID = BattleID;	}
	CBattle* GetBattle()			{	return m_pBattle;	}
	void SetBattle( DWORD BattleID, BYTE Team, DWORD Param=0 );
	
	virtual void SendMsg(MSGBASE* pMsg,int MsgLen);
	void SendMsgToAllAgent(MSGBASE* pMsg,int MsgLen);
	inline DWORD GetAgentNum()	
	{
		return m_AgentNum;	
	}

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	inline BYTE GetObjectKind()						{	return m_ObjectKind;	}
	inline void SetObjectKind(EObjectKind kind)		{	m_ObjectKind = kind;	}


	inline char* GetObjectName()					{return m_BaseObjectInfo.ObjectName;	}
	
	inline BYTE GetState()					{return m_BaseObjectInfo.ObjectState;	}


	void GetPosition(VECTOR3* pos);
	BASEMOVE_INFO * GetMoveInfo() { return &m_MoveInfo; }

	void SetInited();
	void SetNotInited();
	BOOL GetInited()	{	return m_bInited;	}


	inline DWORD GetID()	{	return m_BaseObjectInfo.dwObjectID;	}
	inline DWORD GetUserID()	{	return m_BaseObjectInfo.dwUserID;	}

	void GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo);

	virtual void GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList,BOOL bSetDir = FALSE);
	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin) = 0;
	virtual void SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

	virtual float GetRadius()	{	return 0;	}
	
	//////////////////////////////////////////////////////////////////////////
	virtual void OnStartObjectState(BYTE State,DWORD dwParam)	{}
	virtual void OnEndObjectState(BYTE State) {}
	virtual void StateProcess()		{};	

	TILEINDEX GetPreoccupiedTileIndex()		{	return m_dwPreoccupiedTileIndex;	}
	void SetPreoccupiedTileIndex(TILEINDEX tileindex)		{	m_dwPreoccupiedTileIndex = tileindex;	}
	virtual int GetObjectTileSize();
	
	void Die(CObject* pAttacker);
	virtual void DoDie(CObject* pAttacker)	{}
	virtual void CalcRealDamage(CObject* pOperator,DWORD PhyDamage,DWORD AttrDamage,RESULTINFO* pDamageInfo);
	virtual void CalcRealAttack(CObject* pTarget, DWORD PhyDamage, DWORD AttrDamage, RESULTINFO* pDamageInfo, BOOL bIsContinueAttack = FALSE );

//#ifdef _JAPAN_LOCAL_
//	virtual DWORD CalcShieldDamage(DWORD ShieldDamage);
//#else
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage);
//#endif

	virtual void CalcCounterDamage(WORD Attrib,DWORD AttrDamage,DWORD PhyDamage,RESULTINFO* pDamageInfo);	
	virtual void CalcVampiric(RESULTINFO* pDamageInfo);
	virtual void CalcReverseVampiric(RESULTINFO* pDamageInfo);
	virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);	
	

	virtual void AddStatus(CStatus* pStatus);
	virtual void RemoveStatus(CStatus* pStatus);

	virtual LEVELTYPE GetLevel()	{	return 0;	};
	virtual void SetLevel(LEVELTYPE level)	{};	
	virtual DWORD GetLife() {	return 0;	}
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE)	{};	
	virtual DWORD GetShield() {	return 0;	}
	virtual void SetShield(DWORD Life,BOOL bSendMsg = TRUE)	{};	
	virtual DWORD GetNaeRyuk(){ return 0; }
	virtual void SetNaeRyuk(DWORD val,BOOL bSendMsg = TRUE)	{};	

	virtual DWORD GetRecoverLife();
	virtual DWORD GetRecoverNaeryuk();
	virtual DWORD GetRecoverShield();

//////////////////////////////////////////////////////////////////////////
// 
#define GET_STATUS(var_type,func)						\
var_type	func ## ()								\
{														\
	var_type Ori = Do ## func();						\
	var_type Up = 0,Down = 0;							\
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)		\
		pSL->func(Ori,Up,Down);							\
	PTRLISTSEARCHEND;									\
	return STATUSCALC(Ori,Up,Down);						\
};														\
virtual var_type	Do ## func ## ()	{	return 0; }		
//	


	GET_STATUS(DWORD,GetMaxLife);
	GET_STATUS(DWORD,GetMaxShield);
	GET_STATUS(DWORD,GetMaxNaeRyuk);
	GET_STATUS(DWORD,GetPhyDefense);
	GET_STATUS(DWORD,GetPhyAttackPowerMin);
	GET_STATUS(DWORD,GetPhyAttackPowerMax);
	GET_STATUS(DWORD,GetCritical);
	GET_STATUS(DWORD,GetDecisive);
	GET_STATUS(float,GetMoveSpeed);
	GET_STATUS(float,GetUngiPlusRate);
	GET_STATUS(float,GetAddAttackRange);
	GET_STATUS(float,GetDodgeRate);
	
	float GetAttDefense(WORD Attrib);
	virtual float DoGetAttDefense(WORD Attrib)	{return 0;}
//////////////////////////////////////////////////////////////////////////

	virtual void SetMaxLife(WORD life)	{};	
	virtual void SetMaxNaeRyuk(WORD val)	{};	
	virtual float GetAttribPlusPercent(WORD Attrib) { return 0;	}	
	
	void AddLife(DWORD val,DWORD* realAddLife,BOOL bSendMsg = TRUE);
	void AddShield(DWORD val);
	void AddNaeRyuk(DWORD val,DWORD* realAddNaeRyuk);
	void ReduceNaeRyuk(DWORD val);
	
	void CheckLifeValid();
	void CheckNaeRyukValid();
	void CheckShieldValid();

	virtual DWORD GetGravity(){ return 0;	}
	virtual void SetGravity(DWORD gravity){ }
	//////////////////////////////////////////////////////////////////////////	

	virtual void StartSpecialState(DWORD SpecialStateKind,DWORD Time,
						WORD wParam1,WORD wParam2,float fParam3,
						WORD EffectNum,WORD StateIcon);
	void EndSpecialState(DWORD SpecialStateKind);
	BOOL IsInSpecialState(DWORD SpecialStateKind);
	void SpecialStateProcess(DWORD TickTime);	
	WORD GetSpecialStateParam1(DWORD SpecialStateKind);
	WORD GetSpecialStateParam2(DWORD SpecialStateKind);

	float GetSpecialStateParam3(DWORD SpecialStateKind);
	//////////////////////////////////////////////////////////////////////////
private:
	DWORD	m_SingleSpecialStateUsedTime[eSingleSpecialState_Max];

public:
	void SetSingleSpecialState(WORD State, BOOL bVal) { m_BaseObjectInfo.SingleSpecialState[State] = bVal ? true : false; }
	BOOL GetSingleSpecialState(WORD State) { return m_BaseObjectInfo.SingleSpecialState[State]; }

	DWORD m_StateNaeRyukCountTime[eSingleSpecialState_Max];

	void SetSingleSpecialStateUsedTime(WORD State, DWORD Time) 

	{ 
		m_SingleSpecialStateUsedTime[State] = Time; 
	}
	DWORD GetSingleSpecialStateUsedTime(WORD State) 
	{ 
		return m_SingleSpecialStateUsedTime[State]; 
	}

	void SendSingleSpeicalStateAck(WORD OldState, bool bVal, bool Forced = 0);
	void SendSingleSpeicalStateNack(WORD OldState, WORD Err);
};

#endif // !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)



