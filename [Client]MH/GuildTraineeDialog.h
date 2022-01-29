



#pragma once
#include "interface\cdialog.h"



//
#define	MAX_TRAINEEMUNPA_PERPAGE		4

class cCheckBox;
class cStatic;
class cButton;
class cPushupButton;
class cTextArea;

class CGuildMark;
//

class CGuildTraineeDialog :	public cDialog

{
	//cPtrList*		m_pMunpaList;

	cCheckBox*		m_pCheckBox[MAX_TRAINEEMUNPA_PERPAGE];

//	cStatic*		m_pRanking[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pMemberNum[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pMunpaName[MAX_TRAINEEMUNPA_PERPAGE];

	cStatic*		m_pMunpaMark[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pLockImage[MAX_TRAINEEMUNPA_PERPAGE];
    cTextArea*		m_pMunpaBook[MAX_TRAINEEMUNPA_PERPAGE];

	cPushupButton*	m_pPageListBtn[5];	// 1,2,3,4,5 ������ ��ư

	cButton*		m_pPageBtn[2];		// 0-�ڷ�, 1-������
	cButton*		m_pJoinBtn;			// �����ϱ� ��ư
	CGuildMark*		m_pGuildMark[MAX_TRAINEEMUNPA_PERPAGE];

	int				m_MaxCount;			// �� �Խñ�
	int				m_nPageIndex;		// �ڷ�, ������ ��ư�� �ε���
	int				m_backnumber;		// ���� ������ �������� �ȵǰ� �ϱ����ؼ�
	int				m_CheckIndex;		// üũ�� ���� �ε���
	MUNHA_INFO		m_MunHaInfo[MAX_TRAINEEMUNPA_PERPAGE];		// ���ļҰ��� ����

public:
	CGuildTraineeDialog(void);


	~CGuildTraineeDialog(void);	

	virtual void SetActive(BOOL val);

	void Linking();
	void Render();

	void OnActionEvent(LONG lId, void* p, DWORD we);
	void SetCheckBtn(int index);
	void SendPage(int Index, BOOL bFirst = FALSE);
	void SetPage(MSG_MUNHAINFO* pmsg);
	int GetBasePage(int count);			// ���� ������������ �ִ��� üũ
	void SetPageIndex(BOOL Flag);		// TRUE:front	FALSE:back

	void SetMunpaInfo(MUNHA_INFO temp[]);
};


