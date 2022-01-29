



// PartyMemberDlg.h: interface for the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)
#define AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_


#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"


class CObjectGuagen;
class cPushupButton;

class cStatic;

class CPartyBtnDlg;

class CPartyMemberDlg  : public cDialog
{
	cPushupButton * m_pName;
	CObjectGuagen * m_pLife;
	CObjectGuagen * m_pNaeryuk;

	cStatic		  * m_pLevel;

	CPartyBtnDlg*	m_pPartyBtnDlg;


	BOOL			m_bRealActive;
	int				m_nIndex;
	BOOL			m_bOption;

	BOOL			m_bMember;
#ifdef _XDYY_
	//[小队语音_喇叭按钮][By:十里坡剑神][QQ:112582793][2019/3/21][17:04:31]
	cPushupButton * m_pVoiceBtn;
	//[小队语音_喇叭颜色更新时间][By:十里坡剑神][QQ:112582793][2019/3/21][17:04:20]
	DWORD			m_dwTime;
#endif
public:

	DWORD m_MemberID;
	
	CPartyMemberDlg();

	virtual ~CPartyMemberDlg();
	
	void SetActive(BOOL val);
	void Linking(int i);
	void SetMemberData(PARTY_MEMBER* pInfo);
	void SetNameBtnPushUp(BOOL val);
	void SetPartyBtnDlg( CPartyBtnDlg* pDlg )	{ m_pPartyBtnDlg = pDlg; }
	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void	ShowOption( BOOL bOption )			{ m_bOption = bOption; }	
	void	ShowMember( BOOL bMember )			{ m_bMember = bMember; }
#ifdef _XDYY_
	//[小队语音][By:十里坡剑神][QQ:112582793][2019/3/21][17:04:45]
	BOOL UpdateVoiceBtn(DWORD memberID,BOOL bShow);
	BOOL UpdateVoiceBtnRGB(DWORD memberID,DWORD FullColor);
#endif
};


#endif // !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)


