/***�˺Ź���������	by:OpU_Eye QQ:418914674***/
#pragma once
#include "afxcmn.h"


// IDManager �Ի���

class IDManager : public CDialogEx
{
	DECLARE_DYNAMIC(IDManager)
public:
	/**�Ի�����ر���	by:OpU_Eye QQ:418914674**/
	CListCtrl* m_IDList;	//�˺�List�ؼ�����
	CListCtrl* m_CharList;	//��ɫList�ؼ�����
	CBrush m_brush;			//��ˢ�޸ı�����ɫ	
	COLORREF m_color;		//�޸ľ�̬������ɫ
	COLORREF m_ResultColor;
	CStatic* m_pStaticCheck;
	CStatic* m_pStaticTmp;
	CStatic* m_pStatic;
	/**���ݿ���ر���	by:OpU_Eye QQ:418914674**/
	CGMManagerToolApp *App;	//��Ի���ȫ�ֱ�����ʵ��
	CString m_DBUser;		//���ݿ��˺�
	CString m_DBPassWord;	//���ݿ�����
	CString m_DBip;			//���ݿ��ַ
	
	/**�˺���ر���	by:OpU_Eye QQ:418914674**/
	int   m_propid;			//�˺�propid
	CString  m_UserId;		//��Ϸ�˺�
	CString  m_UserPassWord;//��Ϸ����
	CString  m_IP;			//ע��IP
	CString  m_Email;		//�ܱ�����
	CString  m_Tel;			//�ܱ��ֻ�
	int   m_UserLevel;		//�˺�Ȩ��
	long  m_GoldMoney;		//Ԫ��	
	long  m_MallMoney;		//�ݵ�	
	int   m_Id_Idx;			//ID����

	/**�˺Ž�ɫ��ر���	by:OpU_Eye QQ:418914674**/
	CString m_CharacterName;	//��ɫ��
	int m_CharacterLevel;		//��ɫ�ȼ�
	int m_CharacterRest;		//��ɫת����
	int m_CharacterMoney;		//��ɫ��Ϸ��
	int m_CharacterIdx;			//��ɫID

	/**��ѯ������ر���	by:OpU_Eye QQ:418914674**/
	long long m_Gold;		//Ԫ��
	long long m_Mall;		//�ݵ�
	CString m_IPSel;	//ע��IP
	int m_Level;	//�˺�Ȩ��

public:
	IDManager(CWnd* pParent = NULL);   // Ĭ�Ϲ���	by:OpU_Eye QQ:418914674
	virtual ~IDManager();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif
	/**����ʵ�ֺ���		by:OpU_Eye QQ:418914674**/
	void InitIDInfo();
	void GetUserInfoByID();
	void GetUserInfoByGold();
	void GetUserInfoByMall();
	void GetUserInfoByLevel();
	void GetUserInfoByIP();
	void GetChDBInfo();
	void SetEdit();
	void SetIdList(int i);
	BOOL ConnDB();
	BOOL GetInfo();
	void InitEdit();
	void SetTextColor(int cl);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	/**��Ϣӳ�亯��		by:OpU_Eye QQ:418914674**/
	afx_msg void OnBnClickedButtonConndb();
	afx_msg void OnBnClickedButtonIdSelect();
	afx_msg void OnBnClickedButtonGoldSel();
	afx_msg void OnBnClickedButtonMallSel();
	afx_msg void OnBnClickedButtonLevelSel();
	afx_msg void OnBnClickedButtonIpSel();
	afx_msg void OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonNewId();
	afx_msg void OnBnClickedButtonGetinfo();
	afx_msg void OnBnClickedButtonAddgold();
	afx_msg void OnBnClickedButtonClgold();
	afx_msg void OnBnClickedButtonDisableid();
	afx_msg void OnBnClickedButtonNodisableid();
	afx_msg void OnBnClickedButtonGmpower();
	afx_msg void OnBnClickedButtonUserpower();
	afx_msg void OnBnClickedButtonClmall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
