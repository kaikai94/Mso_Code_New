// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)
#define AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Object.h"
#include "AIParam.h"
#include "Distributer.h"

class CAIBase;
class CPlayer;

class CMonster : public CObject
{
	WORD				m_DropItemId;		

	DWORD				m_dwDropItemRatio;	

	DWORD				m_SubID;
	WORD				m_RegenNum;
	MONSTER_TOTALINFO	m_MonsterInfo;
	CDistributer		m_Distributer;
	VECTOR3*			m_TargetObjectPos;

	monster_stats m_mon_stats;
	
	BOOL m_bEventMob;
	int m_SuryunGroup;

	CPlayer* m_pLastAttackPlayer;
	
protected:
	DWORD m_KillerPlayer;		
	
	friend class CStateMachinen;
	friend class CBattle;
protected:
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife);
	BASE_MONSTER_LIST * m_pSInfo;
	CObject	*			m_pTObject;
public:
	void SetSuryunGroup(int group)		{	m_SuryunGroup = group;	}
	int GetSuryunGroup()				{	return m_SuryunGroup;	}
	void SetEventMob(BOOL bEventMob)	{	m_bEventMob = bEventMob;	}
	void SetDropItemID( WORD DropItemID, DWORD dwDropRatio = 100 )	{ m_DropItemId = DropItemID; m_dwDropItemRatio = dwDropRatio; }
	WORD GetDropItemID()				{ return m_DropItemId; }
	
	CMonster();
	virtual ~CMonster();

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();
	
	void InitMonster(MONSTER_TOTALINFO* pTotalInfo);
	void GetMonsterTotalInfo(MONSTER_TOTALINFO* pRtInfo);
	
	virtual void GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList);
	BASE_MONSTER_LIST * GetSMonsterList(){ return m_pSInfo;	}

	void SetSMonsterList(BASE_MONSTER_LIST * inf){ m_pSInfo = inf;	}

	inline WORD GetMonsterKind()	{	return m_MonsterInfo.MonsterKind;	}

	void AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage);
	void Drop();

	DWORD GetCurAttackIndex();

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

public:
	CAIParam			m_aiParam;
	CStateParam			m_stateParam;
	void SetSubID(DWORD id){ m_SubID = id;	}
	DWORD GetSubID() { return m_SubID; }

	BOOL SetTObject( CPlayer * pNewTPlayer );
	CObject *	GetTObject()						{	return m_pTObject;		}
	virtual DWORD GetGravity()
	{
		return m_pSInfo->Gravity;
	}

	void MoveStop();

	virtual void			OnStop();
	virtual void			OnMove( VECTOR3 * pPos );

	virtual void			DoStand();
	virtual void			DoRest(BOOL bStart);
	virtual BOOL			DoAttack( WORD attackNum );
	virtual CObject *		DoSearch();
	virtual BOOL			DoWalkAround();
	virtual void			DoRunaway();
	virtual BOOL			DoPursuit(CObject * pTObject=NULL);
	virtual CObject	*		OnCollisionObject();
	virtual CMonster *		DoFriendSearch(DWORD Range);
	
	void AddChat(char * str);

	void AddSpeech(DWORD SpeechType, DWORD SpeechIdx);

	monster_stats * GetMonsterStats() { return &m_mon_stats; }

	virtual WORD GetMonsterGroupNum()
	{
		return m_MonsterInfo.Group;
	}
		
	void OnStartObjectState(BYTE State,DWORD dwParam);
	virtual void OnEndObjectState(BYTE State);

	void SetRegenNum(WORD RegenNum){m_RegenNum = RegenNum;}
	WORD GetRegenNum(){return m_RegenNum;}

	float GetRadius();
	
	virtual void StateProcess();
	
	int GetObjectTileSize();
	
	void DistributePerDamage();

	void DistributeItemPerDamage();
	void DistributeDamageInit();
	void DistributeDeleteDamagedPlayer(DWORD CharacterID);
	
	virtual void AddStatus(CStatus* pStatus);
	virtual void RemoveStatus(CStatus* pStatus);

	virtual void DoDie(CObject* pAttacker);
	
	virtual LEVELTYPE GetLevel(){ return m_pSInfo->Level; }
	virtual void SetLevel(LEVELTYPE level)	{}

	virtual DWORD GetLife()									{ return m_MonsterInfo.Life; }
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);

	virtual DWORD GetShield()								{ return m_MonsterInfo.Shield; }
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);

	virtual DWORD DoGetMaxLife()							{ return m_pSInfo->Life; }
	virtual void SetMaxLife(DWORD life)						{}

	virtual DWORD DoGetMaxShield()							{ return m_pSInfo->Shield; }
	virtual void SetMaxShield(DWORD Shield)					{}

	virtual DWORD GetNaeRyuk(){ return 0; }
	virtual void SetNaeRyuk(DWORD val,BOOL bSendMsg = TRUE)	{}
	virtual DWORD DoGetMaxNaeRyuk(){ return 0; }
	virtual void SetMaxNaeRyuk(DWORD val)	{}
	virtual DWORD DoGetPhyDefense(){ return GetMonsterStats()->PhysicalDefense; }
	virtual float DoGetAttDefense(WORD Attrib){ return GetMonsterStats()->AttributeResist.GetElement_Val(Attrib); }
	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();

	virtual float DoGetMoveSpeed();

	CPlayer* GetLastAttackPlayer() { return m_pLastAttackPlayer; }
	void SetLastAttackPlayer( CPlayer* pLastAttackPlayer ) { m_pLastAttackPlayer = pLastAttackPlayer; }
};

#endif // !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)


