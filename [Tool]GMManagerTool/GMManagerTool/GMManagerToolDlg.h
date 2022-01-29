/***���Ի��������� by:OpU_Eye QQ:418914674***/
#pragma once
#include "ConfigManager.h"
#include "IDManager.h"
#include "CharacterManager.h"
#include "ItemManager.h"
#include "DealManager.h"
#include "GoldLogManager.h"
#include "cMapManager.h"
#include "afxcmn.h"


class CGMManagerToolDlg : public CDialogEx
{
	cMapManager         m_MapManager;		//��ͼ���� by:OpU_Eye QQ:418914674
	ConfigManager		m_ConfigManager;	//�������� by:OpU_Eye QQ:418914674
	IDManager			m_IDManager;		//�˺Ź��� by:OpU_Eye QQ:418914674
	CharacterManager	m_CharacterManager;	//��ɫ���� by:OpU_Eye QQ:418914674
	ItemManager			m_ItemManager;		//��Ʒ���� by:OpU_Eye QQ:418914674
	GoldLogManager		m_GoldLogManager;	//Ԫ����־ by:OpU_Eye QQ:418914674
	DealManager			m_DealManager;		//������־ by:OpU_Eye QQ:418914674
public:
	CGMManagerToolDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GMMANAGERTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);   //TabControl�ؼ���Ϣ��Ӧ���� by:OpU_Eye QQ:418914674
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
