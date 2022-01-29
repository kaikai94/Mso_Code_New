




// CharSelect.h: interface for the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)
#define AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "ChannelDialog.h"
#include "cImageSelf.h"
#define CHARSELECT USINGTON(CCharSelect)




class CPlayer;
class cDialog;
struct sCHARSELECTINIT
{
	WORD	wMapNum;
	VECTOR3	 vStandPos;
	WORD	 wStandHeight;
	VECTOR3	 vManStandPos[4];
	VECTOR3  vWomanStandPos[4];
	WORD	 wCount[2];
	VECTOR2	 vShadowPos;
	VECTOR2  vHeightPos1;
	VECTOR2	 vHeightPos2;
	float	 vAngle[4];
	WORD	wMontion;
	WORD	 wBaseMotion[2];
	VECTOR3	 vCameraPoint;
	WORD	 wCameraNum;
	VECTOR3	 fAngle;
	WORD	 wCameraMode;
	WORD	 wCurCamera;
};
class CCharSelect : public CGameState  
{
	sCHARSELECTINIT sCharSelect;
	int m_CurSelectedPlayer;
	CPlayer* m_pPlayer[MAX_CHARACTER_NUM];

	CPlayer* m_pDoublcClickedPlayer;

	void InitCameraInCharSelect();
	void LoadCharSelectInfo();

	void SetCurSelctedPlayer(int num);


	void PlaySelectedMotion(int num);

	void PlayDeselectedMotion(int num);

	BOOL m_bDiablePick;

	cDialog* m_pSelectDlg;
	CChannelDialog* m_pChannelDlg;
	
	BOOL m_bBackToMainTitle;
	
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	DWORD	m_ExtraCharacterCount;
	
	BOOL m_bEnterGame;

public:
	
	CCharSelect();
	virtual ~CCharSelect();
	
	////MAKESINGLETON(CCharSelect)

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();

	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	void SelectPlayer(int num);

	CPlayer* GetCurSelectedPlayer();
	int GetCurSelectedPlayerNum() { return m_CurSelectedPlayer; }

	BOOL IsFull();
	
	void SendMsgGetChannelInfo();
	BOOL EnterGame();
	void DeleteCharacter();

	void DisplayNotice(int MsgNum);
	
	void SetDisablePick( BOOL val );
	void BackToMainTitle();


	void OnDisconnect();

	DWORD CheckWeaponType(DWORD dwWeaponType);	
	
	CChannelDialog* GetChannelDialog() { return m_pChannelDlg; }

	//
//#ifdef _JAPAN_LOCAL_
//	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
//	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		}
//#endif
//#ifdef _HK_LOCAL_
//	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
//
//
//	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		} //
//#endif
//#ifdef _TL_LOCAL_
//	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
//	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		}
//#endif

};
EXTERNGLOBALTON(CCharSelect)
#endif // !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)


