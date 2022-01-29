/***主对话框类声明 by:OpU_Eye QQ:418914674***/
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
	cMapManager         m_MapManager;		//地图管理 by:OpU_Eye QQ:418914674
	ConfigManager		m_ConfigManager;	//配置设置 by:OpU_Eye QQ:418914674
	IDManager			m_IDManager;		//账号管理 by:OpU_Eye QQ:418914674
	CharacterManager	m_CharacterManager;	//角色管理 by:OpU_Eye QQ:418914674
	ItemManager			m_ItemManager;		//物品管理 by:OpU_Eye QQ:418914674
	GoldLogManager		m_GoldLogManager;	//元宝日志 by:OpU_Eye QQ:418914674
	DealManager			m_DealManager;		//交易日志 by:OpU_Eye QQ:418914674
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
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);   //TabControl控件消息响应函数 by:OpU_Eye QQ:418914674
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
