
#pragma once
#include "CharacterDialog.h"
#include "WearedExDialog.h"
#include "cTabDialog.h"
#include "Item.h"
#include "Hero.h"
class CPlayer;
class CGoldShopSubDialog;
class CQuipInfoDlg: public cTabDialog
{
	CHARSTATICCTRL		m_ppStatic;	
	cStatic * CharShopInfo;
	cStatic * m_pAttDef[5];
	CGoldShopSubDialog *SubDlg;
	DWORD m_dwTime;
public:
	CQuipInfoDlg(void);
	virtual ~CQuipInfoDlg(void);
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive( BOOL val );
	virtual void Add(cWindow * window);
	void Linking();
	BOOL AddItem(CItem* pItem);
	void AddShopItem(MSG_QUERY_EQUIPINF * msg);
	void ClearShopItem();
	BOOL DeleteItem(CItem** ppItem);
	void SetLevel(LEVELTYPE level);
	void SetLife(CPlayer* pPlayer);
	void SetShield(DWORD Shield);
	void SetNaeRyuk(DWORD naeryuk);
	void SetGenGol(WORD resVal);
	void SetMinChub(WORD resVal);
	void SetCheRyuk(WORD resVal);
	void SetSimMek(WORD resVal);
	void SetReset(WORD resVal);
	void SetAttackRate();
	void SetDefenseRate(WORD value);

	void SetCritical(DWORD resVal1);
	void SetAttackRange(WORD val);
	void SetAttDef(MSG_QUERY_EQUIPINF* msg);
	void SendGetInfoMsg();
private:
	CWearedExDialog* m_pHeroWearDlg;
};