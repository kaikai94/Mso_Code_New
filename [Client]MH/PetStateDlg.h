



#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cTabDialog.h"
#include "./interface/cGuagen.h"
#include "PetManager.h"



class cStatic;
class cPushupButton;
class cImageSelf;

class cGuagen;
class cButton;


//enum {ePetRestBtn,ePetUseBtn};
class CPetStateDlg : public cTabDialog

{
	cStatic*		m_pNameDlg;		//�� �̸�
	cStatic*		m_pGradeDlg;	//�� ���
	cStatic*		m_pStateDlg;	//�� �޽�
	cStatic*		m_pFriend;		//�� ģ�е�
	cStatic*		m_pStamina;		//�� ���¹̳�

//	cImageSelf*		m_pPetFaceImg;
	cStatic*		m_pPetFaceImg;	//�� 2D �̹���

	cStatic*		m_pInvenNum;	//�� �κ� ����
	cStatic*		m_pSkill[MAX_PET_BUFF_NUM];	//���������

	cGuagen*		m_pFriendGuage;	//�� ģ�е� ������
	cGuagen*		m_pStaminaGuage;	//�� ���¹̳� ������
//	cGuagen*		m_pSkillGuage;	//�� ��ų ���� ������

	cButton*		m_pPetSealBtn;	//�� ���� ��ư
	cButton*		m_pPetUseRestBtn;	//�� Ȱ��/�޽� ��ư

	cButton*		m_pPetInvenBtn;	//�� �κ��丮

	cButton*		m_pDlgToggleBtn;

public:

	CPetStateDlg(void);
	virtual ~CPetStateDlg(void);

	virtual void Add(cWindow* window);

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);


	cStatic* GetFriendShipTextWin()	{	return m_pFriend;	}
	cStatic* GetStaminaTextWin()	{	return m_pStamina;	}

	cGuagen* GetFriendShipGuage()	{	return m_pFriendGuage;	}
	cGuagen* GetStaminaGuage()		{	return m_pStaminaGuage;	}

	cStatic* GetNameTextWin()		{	return m_pNameDlg;	}
	cStatic* GetGradeTextWin()		{	return m_pGradeDlg;	}
	cStatic* Get2DImageWin()		{	return m_pPetFaceImg;	}

	cStatic* GetInvenNumTextWin()	{	return m_pInvenNum;	}

	cStatic* GetUseRestTextWin()	{	return m_pStateDlg;	}

	cStatic** GetPetBuffTextWin()	{	return m_pSkill;	}
	//void SetGuageText(DWORD dwFriendship, DWORD dwStamina);

	

	void SetBtnClick(int btnKind);

};

//-----------!!!
//���� ���¸� �����ش�.
//���/�޽� ���ÿ� ���� �� ���°� ��ȭ�ǰ� ��ó���� �޶�����.
//��� ��ư�� ������ �̴ϻ���â���� �ٲ��. ��ư Ŭ���� ��ǥ�� �Ѱ��ش�.




