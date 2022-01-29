#if !defined(AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_)
#define AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_


#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

class MONSTEREX_LIST;

class cRand_Big
{
private:
	int mRangeVal;
public:
	DWORD RandomVal();
	cRand_Big(){mRangeVal = 0;}
	~cRand_Big(){}
};

class CDistributeWay 
{
protected:
	cRand_Big mBigRandObj;
	BOOL ChkGetItemLevel(LEVELTYPE PlayerLvl, LEVELTYPE MonsterLvl);
	
public:
	void ItemChangeAtLv(WORD& DropItemId); 
	CDistributeWay();
	virtual ~CDistributeWay();
	virtual void CalcAbilandSend(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel);
	virtual void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel) {};
	void SendToPersonalExp(CPlayer* pReceivePlayer, DWORD Exp);
	void SendToPersonalAbil(CPlayer* pReceivePlayer, LEVELTYPE MonsterLevel);
	void SendToPersonalMoney(CPlayer* pPlayer, MONEYTYPE Money, WORD MonsterKind);
	void SendToPersonalItem(CPlayer* pPlayer, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind);	
	DWORD CalcObtainAbilityExp(LEVELTYPE KillerLevel,LEVELTYPE MonsterLevel);
};


#endif // !defined(AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_)


