



// PartyCreateDlg.h: interface for the CPartyCreateDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_)
#define AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000


#include "INTERFACE\cDialog.h"
#include "interface\cEditBox.h"

class cButton;
class cComboBox;
class cCheckBox;
class cEditBox;


//2008. 5. 21. CBH - ���� ��Ī �ý��� ��� �߰��ϸ鼭 ��Ƽ ���� �κ� ����
class CPartyCreateDlg : public cDialog  
{
	cButton* m_pOKBtn;			//Ȯ�� ��ư
	cButton* m_pCancelBtn;		//��� ��ư
	cComboBox* m_pDistribute;	//�й�ɼ� �޺��ڽ�
	cEditBox* m_pThemeEdit;		//���� ����Ʈ�ڽ�
	cEditBox* m_pMinLevelEdit;	//�ּҷ��� ����Ʈ�ڽ�
	cEditBox* m_pMaxLevelEdit;	//�ִ뷹�� ����Ʈ�ڽ�
	cCheckBox* m_pPublicCheck;	//���� üũ�ڽ�
	cCheckBox* m_pPrivateCheck;	//����� üũ�ڽ�
	cComboBox* m_pMemberNumCombo;	//�ο� �޺��ڽ�	

public:
	CPartyCreateDlg();
	virtual ~CPartyCreateDlg();


	void Linking();
	void SetActive( BOOL val );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);

	DWORD ActionKeyboardEvent(CKeyboard * keyInfo);

	void InitOption();

	BOOL CreatePartySyn();


};

#endif // !defined(AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_)



