#pragma once
#include "afxwin.h"

// DealManager �Ի���

class DealManager : public CDialogEx
{
	DECLARE_DYNAMIC(DealManager)
	CGMManagerToolApp *App;
	CBrush m_brush;
	COLORREF m_color;
public:
	DealManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DealManager();
	int GetMapNum(CString MapName);
	void KillProcess(int port);
	int execmd(char * cmd, int &result);
	int pskill(int id);   //���ݽ���IDɱ����
	void EnableDebugPriv();

	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//������
	GUILDINFO m_GuildInfo;
	CString m_GuildName;
	CString m_MasterName;
	int	    m_MasterIdx;
	int m_MapNum;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_EditMapName;
	CEdit m_EditMapNum;
	afx_msg void OnBnClickedButtonFind();
	CStatic m_TipText;
	afx_msg void OnBnClickedButton1();
	void GetGuildInfoFromDB();
	void SetEditText();
	void GetMapName();
	void ClearDlg();
	afx_msg void OnBnClickedButtonUpdataGuild();
	afx_msg void OnBnClickedButtonUpdataPoint();
	CStatic m_MemberCount;
};
