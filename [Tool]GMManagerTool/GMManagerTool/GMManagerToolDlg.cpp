/***���Ի�����ʵ�� by:OpU_Eye QQ:418914674***/

#include "stdafx.h"
#include "GMManagerTool.h"
#include "GMManagerToolDlg.h"
#include "afxdialogex.h"
#include "FileVersion.h"
#include "MD5Checksum.h"
#include "MHFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
virtual void DoDataExchange(CDataExchange* pDX);    

protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CGMManagerToolDlg::CGMManagerToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GMMANAGERTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGMManagerToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);  //TabControl�ؼ���������		by:OpU_Eye QQ:418914674
}

BEGIN_MESSAGE_MAP(CGMManagerToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CGMManagerToolDlg::OnTcnSelchangeTab) //TabControl�ؼ���Ϣӳ��		by:OpU_Eye QQ:418914674
END_MESSAGE_MAP()

BOOL CGMManagerToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����Tab��ǩ��	by:OpU_Eye QQ:418914674
	m_tabCtrl.InsertItem(0, _T("��ͼ����"));
	m_tabCtrl.InsertItem(1, _T("��������"));
	m_tabCtrl.InsertItem(2, _T("�˺Ź���"));
	m_tabCtrl.InsertItem(3, _T("��ɫ����"));
	m_tabCtrl.InsertItem(4, _T("��Ʒ����"));
	m_tabCtrl.InsertItem(5, _T("Ԫ����־��ѯ"));
	m_tabCtrl.InsertItem(6,	_T("������"));
	//����Tab��ǩ���ӶԻ���	by:OpU_Eye QQ:418914674
	m_MapManager.Create(IDD_DIALOG1, &m_tabCtrl);
	m_ConfigManager.Create(IDD_DIALOG2, &m_tabCtrl);
	m_IDManager.Create(IDD_DIALOG3, &m_tabCtrl);
	m_CharacterManager.Create(IDD_DIALOG4, &m_tabCtrl);
	m_ItemManager.Create(IDD_DIALOG5, &m_tabCtrl);
	m_GoldLogManager.Create(IDD_DIALOG6,&m_tabCtrl);
	m_DealManager.Create(IDD_DIALOG7, &m_tabCtrl);
	//��ȡ���Ի����Сby:OpU_Eye QQ:418914674
	CRect tabRect;
	m_tabCtrl.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.top += 22;
	tabRect.right -= 3;
	tabRect.bottom -= 2;
	//�����ӶԻ���λ�ô�С	by:OpU_Eye QQ:418914674
	m_MapManager.MoveWindow(&tabRect);
	m_ConfigManager.MoveWindow(&tabRect);
	m_IDManager.MoveWindow(&tabRect);
	m_CharacterManager.MoveWindow(&tabRect);
	m_ItemManager.MoveWindow(&tabRect);
	m_GoldLogManager.MoveWindow(&tabRect);
	m_DealManager.MoveWindow(&tabRect);
	//�����ӶԻ���Ĭ����ʾ״̬	by:OpU_Eye QQ:418914674
	m_MapManager.ShowWindow(false);
	m_ConfigManager.ShowWindow(false);
	m_IDManager.ShowWindow(true);
	m_CharacterManager.ShowWindow(false);
	m_ItemManager.ShowWindow(false);
	m_GoldLogManager.ShowWindow(false);
	m_DealManager.ShowWindow(false);
	m_tabCtrl.SetCurSel(2);
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  
}

void CGMManagerToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CGMManagerToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//����λͼ���Ʊ���	by:OpU_Eye QQ:418914674
		//CPaintDC dc(this);

		//CDC memdc;
		//memdc.CreateCompatibleDC(&dc);

		//CBitmap bkg;

		//bkg.LoadBitmapW(IDB_BITMAP_BG);

		//BITMAP bkginfo;
		//bkg.GetBitmap(&bkginfo);

		//memdc.SelectObject(&bkg);

		//RECT rect;

		//GetWindowRect(&rect);

		//dc.StretchBlt(0, 0, rect.right - rect.left, rect.bottom - rect.top, &memdc, 0, 0, bkginfo.bmWidth, bkginfo.bmHeight, SRCCOPY);
	}
}


HCURSOR CGMManagerToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CGMManagerToolDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//Tab��ǩѡ����Ӧ����	by:OpU_Eye QQ:418914674
	switch (m_tabCtrl.GetCurSel())
	{
		case 0:
			{
				m_MapManager.ShowWindow(true);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 1:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(true);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 2:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(true);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 3:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(true);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 4:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(true);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 5:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(true);
				m_DealManager.ShowWindow(false);
				break;
			}
		case 6:
			{
				m_MapManager.ShowWindow(false);
				m_ConfigManager.ShowWindow(false);
				m_IDManager.ShowWindow(false);
				m_CharacterManager.ShowWindow(false);
				m_ItemManager.ShowWindow(false);
				m_GoldLogManager.ShowWindow(false);
				m_DealManager.ShowWindow(true);
				break;
			}
	}
	*pResult = 0;
}




BOOL CGMManagerToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		// �����Ϣ�Ǽ��̰����¼�������Esc����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// �����Ϣ�Ǽ��̰����¼�������Entert����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
