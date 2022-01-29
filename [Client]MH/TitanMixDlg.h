




// TitanMixDlg.h: interface for the CTitanMixDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANMIXDLG_H
#define _TITANMIXDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "./interface/cStatic.h"
#include "./interface/cGuagen.h"
#include "VirtualItem.h"
#include "TitanMixProgressBarDlg.h"

enum eTitanMixPos
{
	eTitanMixPos_Head,
	eTitanMixPos_Body,
	eTitanMixPos_Hands,
	eTitanMixPos_Legs,

	eTitanMixPos_Max,
};

enum eTitanMixReleaseOpt
{
	eTMixHeadRelease,
	eTMixBodyRelease,
	eTMixHandsRelease,
	eTMixLegsRelease,



	eTMixAllRelease,
};

struct TITAN_ATTRDEF

{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib,WORD value,DWORD color=0)

	{
		char temp[32];
		sprintf(temp,"%d%%",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		pGuage.GetElement_Val(attrib)->SetValue(value*0.01f);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};

struct TITANMIX_CTRL
{
	cStatic*		titanType;			// ����
	cStatic*		titanAtt;			// ��������
	cStatic*		titanCritical;		// �ϰ�
	cStatic*		titanLongAtt;		// ���Ÿ�����

	cStatic*		titanDistance;		// �����Ÿ�
	cStatic*		titanLife;			// �����
	cStatic*		titanDef;			// ����
	//cStatic*		titanMana;			// ����
	cStatic*		titanMagicAttack;	// ��������
	TITAN_ATTRDEF	TitanAttrDef;		// ���� ���׷�(ȭ,��,��,��,��)

};

class cIconDialog;
class CItem;

class CTitanMixDlg : public cDialog
{
	cIconDialog * m_pTitanMixDlg;
	cGuageBar* m_pTitanSizeBar;
	TITANMIX_CTRL m_TitanMixCtrl;
	CTitanMixProgressBarDlg* m_pTitanMixProgressBarDlg;


	CVirtualItem m_VirtualItem[eTitanMixPos_Max];
	float m_TitanSize;

public:
	CTitanMixDlg();
	virtual ~CTitanMixDlg();
	void Linking();
	virtual void Render();	//2007. 10. 23. CBH - ������ �� ó���� ���� ������
	void Release(eTitanMixReleaseOpt op=eTMixAllRelease, BOOL bFlag = TRUE);

	virtual void SetActive( BOOL val );

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void MixOkBtn();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void AddVirtualItem(POSTYPE pos, CItem * pItem);
	void UpdateData(DWORD titanIdx, BOOL bFlag = TRUE);
	void DieCheck();
	void TitanMix();
};

#endif // _TITANMIXDLG_H


