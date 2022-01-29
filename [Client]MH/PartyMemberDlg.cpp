








// PartyMemberDlg.cpp: implementation of the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PartyMemberDlg.h"
#include "PartyManager.h"
#include "GameIn.h"


#include "WindowIDEnum.h"
#include "./Interface/cPushupButton.h"

#include "./ObjectGuagen.h"
#include "./Interface/cStatic.h"
#include "PartyBtnDlg.h"
#include "./interface/cScriptManager.h"
#include "ChatManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD PMD_LOGIN_BASICCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_OVERCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

DWORD PMD_LOGOUT_BASICCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_OVERCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

CPartyMemberDlg::CPartyMemberDlg()
{
	m_MemberID = 0;
	m_bRealActive = FALSE;
	
	m_bSetTopOnActive = FALSE;

	m_pPartyBtnDlg = NULL;
	m_nIndex = -1;
	m_bOption = TRUE;
	m_bMember = TRUE;
#ifdef _XDYY_
	m_pVoiceBtn = NULL;
	m_dwTime = 0;
#endif
}


CPartyMemberDlg::~CPartyMemberDlg()
{


}

void CPartyMemberDlg::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	if( m_bMember == FALSE )
		val = FALSE;	
	m_bRealActive = val;	
	if(m_MemberID == 0)
		val = FALSE;
#ifdef _XDYY_
	if(!val)
		m_pVoiceBtn->SetActive(val);
#endif
	cDialog::SetActive(val);
}

void CPartyMemberDlg::Linking(int i)
{
	m_pName = (cPushupButton *)GetWindowForID(PA_MEMBER1NAME+i);

	m_pLife = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1LIFE+i);
	m_pNaeryuk = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1NAERYUK+i);
	m_pLevel = (cStatic *)GetWindowForID(PA_MEMBER1LEVEL+i);
	m_nIndex = i;
#ifdef _XDYY_
	m_pVoiceBtn = (cPushupButton *)GetWindowForID(OV_TEAMVOICE_MEMBER1+i);
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_pVoiceBtn->SetToolTip(CHATMGR->GetChatMsg(2336),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	m_pVoiceBtn->SetActive(FALSE);
#endif
}


void CPartyMemberDlg::SetMemberData(PARTY_MEMBER* pInfo)
{
	if(pInfo == NULL)
	{

		m_MemberID = 0;
	}
	else
	{
		char buf[10];
		sprintf(buf, "Lv.%d", pInfo->Level);
		m_MemberID = pInfo->dwMemberID;
		if(pInfo->bLogged)
		{
			m_pName->SetText(pInfo->Name, PMD_LOGIN_BASICCOLOR, PMD_LOGIN_OVERCOLOR, PMD_LOGIN_PRESSCOLOR);

			m_pLife->SetValue(pInfo->LifePercent*0.01f, 0);
			m_pNaeryuk->SetValue(pInfo->NaeRyukPercent*0.01f, 0);

			m_pLevel->SetStaticText(buf);
		}

		else
		{
			m_pName->SetText(pInfo->Name, PMD_LOGOUT_BASICCOLOR, PMD_LOGOUT_OVERCOLOR, PMD_LOGOUT_PRESSCOLOR);

			m_pLife->SetValue(0, 0);
			m_pNaeryuk->SetValue(0, 0);
			m_pLevel->SetStaticText("");
		}		
	}


	SetActive(m_bRealActive);
}

DWORD CPartyMemberDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_LBTNCLICK)
	{
		GAMEIN->GetPartyDialog()->SetClickedMemberID(m_MemberID);
	}
	return we;
}

void CPartyMemberDlg::SetNameBtnPushUp(BOOL val)
{
	m_pName->SetPush(val);
}

void CPartyMemberDlg::Render()
{
	if( m_pPartyBtnDlg && m_nIndex != -1 )
	{

		float x = m_pPartyBtnDlg->GetAbsX();

		float y = m_pPartyBtnDlg->GetAbsY();


		if( m_bOption )
		{
			this->SetAbsXY( (LONG)x, (LONG)(y+144 + ( 58 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y+168 + ( 58 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y+188 + ( 58 * m_nIndex )) );
			m_pNaeryuk->SetAbsXY( (LONG)(x+6), (LONG)(y+194 + ( 58 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y+155 + ( 58 * m_nIndex )) );
#ifdef _XDYY_
			m_pVoiceBtn->SetAbsXY((LONG)(x+130), (LONG)(y+170 + ( 58 * m_nIndex )) );
#endif
		}
		else
		{
			this->SetAbsXY( (LONG)x, (LONG)(y + 74 + ( 58 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y + 88 + ( 58 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y + 108 + ( 58 * m_nIndex )) );
			m_pNaeryuk->SetAbsXY( (LONG)(x+6), (LONG)(y + 114 + ( 58 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y + 75 + ( 58 * m_nIndex )) );
#ifdef _XDYY_
			m_pVoiceBtn->SetAbsXY( (LONG)(x+130), (LONG)(y + 90 + ( 58 * m_nIndex )) );
#endif
		}
	}
#ifdef _XDYY_
	if(gCurTime-m_dwTime>1000)
	{//[С������_������ɫ][By:ʮ���½���][QQ:112582793][2019/3/21][17:03:59]
		m_pVoiceBtn->SetImageRGB(0xffffffff);
		m_dwTime = gCurTime;
	}
#endif
	cDialog::Render();
}
#ifdef _XDYY_
//[С������][By:ʮ���½���][QQ:112582793][2019/3/21][17:03:52]

BOOL CPartyMemberDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_PUSHUP)
	{
		if(lId==m_pVoiceBtn->GetID())
		{
			PARTYMGR->OpenMemberVoice(m_MemberID);
			return TRUE;
		}
	}
	else if(we & WE_PUSHDOWN)
	{
		if(lId==m_pVoiceBtn->GetID())
		{
			PARTYMGR->DisableMemberVoice(m_MemberID);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CPartyMemberDlg::UpdateVoiceBtn(DWORD memberID,BOOL bShow)
{
	if(m_MemberID==memberID)
	{
		m_pVoiceBtn->SetActive(bShow);
		return TRUE;
	}
	return FALSE;
}
BOOL CPartyMemberDlg::UpdateVoiceBtnRGB(DWORD memberID,DWORD FullColor)
{
	if(m_MemberID==memberID)
	{
		if(!m_pVoiceBtn->IsActive())
			m_pVoiceBtn->SetActive(TRUE);
		m_pVoiceBtn->SetImageRGB(FullColor);
		return TRUE;
	}
	return FALSE;
}

#endif