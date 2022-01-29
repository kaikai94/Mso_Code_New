



// MoveManager.h: interface for the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEMANAGER_H__73815430_939A_4D3B_A757_779374317296__INCLUDED_)
#define AFX_MOVEMANAGER_H__73815430_939A_4D3B_A757_779374317296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//confirm //mousepointtest露贸....

#include "MacroManager.h"

#define MOVEMGR USINGTON(CMoveManager)


class CObject;
class CAction;
class CActionTarget;


class CMoveManager  
{
	//BOOL CalcPositionEx(CObject* pObject,DWORD CurTime);
	//BOOL CalcPosition(CObject* pObject,DWORD CurTime);
	BOOL CalcYPosition(CObject* pObject,DWORD CurTime);
	void CalcAngle(CObject* pObject,DWORD CurTime);
	BOOL m_bSetMove;
	BYTE m_bMoveCount;
public:
//	//MAKESINGLETON(CMoveManager)

	CMoveManager();
	virtual ~CMoveManager();
	BOOL CalcPositionEx(CObject* pObject, DWORD CurTime);
	void InitMove(CObject* pObject,BASEMOVE_INFO* pMoveInfo,float AngleDeg = 0);

	void SetPosition(CObject* pObject,VECTOR3* pPos);	

	void EndMove(CObject* pObject);
	void EndYMove(CObject* pObject);

	void StartMoveEx(CObject* pObject, DWORD CurTime, VECTOR3 * pTargetPos);
	void StartMoveEx(CObject* pObject, VECTOR3* pStartPos,DWORD CurTime);
	int TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos);

	void SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg = 0);

	void SetAngle(CObject* pObject,float AngleDeg,float TurnDuration);

	void MoveProcess(CObject* pObject);
	
	BOOL SetHeroTarget(VECTOR3* pPos,float biubiubiu); //[自动寻路-WORD参数只是用于区分自动寻路调用和普通调用][By:十里坡剑神][QQ:112582793][2019/3/22][13:03:33]
	void SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay = MACROMGR->IsVisibleCursor()); 

	void SetPetTarget(MOVE_INFO* pHeroMoveInfo, WORD wPosCount);
	void GetPetPath(MOVE_INFO* pHeroMoveInfo, WORD wPosCount);
	BOOL GetPetLastPos(VECTOR3* pSrcPos, VECTOR3* pFromPos, VECTOR3* pToPos);
	void SendPetMoveMsg();
	
	void SetHeroActionMove(CActionTarget* pTarget,CAction* pAction);
	
	void MoveStop(CObject* pObject,VECTOR3* pStopPos = NULL);
	void HeroMoveStop();
	void PetMoveStop();

	float CalcDistance(CObject* pObject1,CObject* pObject2);
	
	
	void ToggleHeroMoveMode();
	void ToggleHeroKyungGongMode();

	void SetWalkMode(CObject* pObject);

	void SetRunMode(CObject* pObject);
	BOOL SetKyungGong(CObject* pObject,WORD KGIdx);
	BOOL SetMonsterMoveType(CObject* pObject,WORD KGIdx);

	BOOL IsValidKyungGong(CObject* pObject,WORD KGIdx);

	float GetKyungGongSpeed(WORD KyungGongIdx);


	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	
	void EffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration);
	void EffectYMove(CObject* pObject,float StartY,float EndY,DWORD Duration);
	void HeroEffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration,CObject* pOperator);


	void EffectMoveInterpolation(CObject* pObject,VECTOR3* TargetPos);

	BYTE GetMoveMode(CObject* pObject);
	
	BOOL IsMoving(CObject* pObject);

	
	void RefreshHeroMove();
	void SetFindPath(){ m_bSetMove = FALSE; }
};
EXTERNGLOBALTON(CMoveManager)
#endif // !defined(AFX_MOVEMANAGER_H__73815430_939A_4D3B_A757_779374317296__INCLUDED_)


