




#pragma once

#include "INTERFACE\cDialog.h"


class cListDialog;
class cPtrList;


class CGuildMunhaDialog : public cDialog

{

	cListDialog*	m_pListDlg[2];		// [0]:������	[1]:�ѹ��ϻ�
	cPtrList		m_MunHaList;		// ������
	cButton*		m_pNumHaBtn[3];
	int				m_MunHaIndex;		// ������ ���ϻ� �ε���
	BOOL			m_ResetFlag;		// ���ϸʿ����� �����͸� �ѹ��� Recv (���̵��ϸ� �ٽ� Recv)

public:
	CGuildMunhaDialog(void);
	~CGuildMunhaDialog(void);



	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void OnActionEvent(LONG lId, void* p, DWORD we);

	void Linking();
	void SetMunHaInfo(char name[]);

	void DeleteMunHaInfo(char name[]);
	void RecvMunHaJoinInfo(MSG_MUNHACATALOG* pTemp);
	void RecvMunHaJoinInfo(MSG_NAME_DWORD2* pTemp);
	void RecvUpdateMunHaJoin(MSG_DWORD2* pTemp);
};



