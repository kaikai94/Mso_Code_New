// SkillObjectAttachUnit_Immune.cpp: implementation of the CSkillObjectAttachUnit_Immune class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_Immune.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_Immune::CSkillObjectAttachUnit_Immune()
{

}

CSkillObjectAttachUnit_Immune::~CSkillObjectAttachUnit_Immune()
{

}

void CSkillObjectAttachUnit_Immune::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
#ifdef _MHCLIENT_
	//��ȿ��ǥ��	
	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
	m_StateIconNum[0] = eStatusIcon_Immune;
//	722	���� ����		���� ���  = 1					
#endif
}

void CSkillObjectAttachUnit_Immune::GetDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	CalcedDown = Original;
}
