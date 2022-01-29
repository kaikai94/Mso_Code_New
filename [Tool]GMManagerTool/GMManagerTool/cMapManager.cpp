// cMapManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "cMapManager.h"
#include "afxdialogex.h"
#include "MHFile.h"
#include <vector>
#include <algorithm>
#include "cMyTime.h"
int TimeCount;
UINT StartMap(LPVOID pParam);
// cMapManager �Ի���

IMPLEMENT_DYNAMIC(cMapManager, CDialogEx)
QSTATETYPE GetCurTime();
cMapManager::cMapManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

cMapManager::~cMapManager()
{
	App->m_MapInfo.clear();
}

void cMapManager::LoadMapInfo()
{
	std::vector<PAIR> Map_info_vec(App->m_MapInfo.begin(), App->m_MapInfo.end());
	std::sort(Map_info_vec.begin(), Map_info_vec.end(), CmpByValue());
	CString str;
	std::vector<PAIR>::iterator iter;
	int i = 0;
	for (iter = Map_info_vec.begin(); iter != Map_info_vec.end(); ++iter)
	{
		
		m_MapList.InsertItem(i, iter->first.c_str());
		str.Format(_T("[%d]"),iter->second);
		m_MapList.SetItemText(i,1,str);
		i++;
	}
}

void cMapManager::DoDataExchange(CDataExchange* pDX)
{
	/*���ڱ�����ɫ��ʼ��	by:OpU_Eye QQ:418914674*/
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	CDialogEx::DoDataExchange(pDX);
	TimeCount = 1000;
	App = (CGMManagerToolApp *)AfxGetApp();
	DDX_Control(pDX, IDC_LIST1, m_MapList);
}


BEGIN_MESSAGE_MAP(cMapManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_MAPSTART, &cMapManager::OnBnClickedButtonMapstart)
	ON_EN_CHANGE(IDC_EDIT_MAP_STARTTIME, &cMapManager::OnEnChangeEditMapStarttime)
	ON_BN_CLICKED(IDC_BUTTON2, &cMapManager::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &cMapManager::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEALLMAP, &cMapManager::OnBnClickedButtonCloseallmap)
	ON_BN_CLICKED(IDC_BUTTON_CLOSESERVER, &cMapManager::OnBnClickedButtonCloseserver)
	ON_BN_CLICKED(IDC_BUTTON7, &cMapManager::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &cMapManager::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_AG0, &cMapManager::OnBnClickedButtonAg0)
	ON_BN_CLICKED(IDC_BUTTON_AG1, &cMapManager::OnBnClickedButtonAg1)
	ON_BN_CLICKED(IDC_BUTTON_AG2, &cMapManager::OnBnClickedButtonAg2)
	ON_BN_CLICKED(IDC_BUTTON_AG3, &cMapManager::OnBnClickedButtonAg3)
	ON_BN_CLICKED(IDC_BUTTON_AG4, &cMapManager::OnBnClickedButtonAg4)
	ON_BN_CLICKED(IDC_BUTTON_AG5, &cMapManager::OnBnClickedButtonAg5)
	ON_BN_CLICKED(IDC_BUTTON_DIS0, &cMapManager::OnBnClickedButtonDis0)
	ON_BN_CLICKED(IDC_BUTTON_DIS1, &cMapManager::OnBnClickedButtonDis1)
	ON_BN_CLICKED(IDC_BUTTON_DIS2, &cMapManager::OnBnClickedButtonDis2)
	ON_BN_CLICKED(IDC_BUTTON_DIS3, &cMapManager::OnBnClickedButtonDis3)
	ON_BN_CLICKED(IDC_BUTTON_DIS4, &cMapManager::OnBnClickedButtonDis4)
	ON_BN_CLICKED(IDC_BUTTON_DIS5, &cMapManager::OnBnClickedButtonDis5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &cMapManager::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_GETSIEGEWARTIME, &cMapManager::OnBnClickedButtonGetsiegewartime)
	ON_BN_CLICKED(IDC_BUTTON_SETSIEGEWARTIME, &cMapManager::OnBnClickedButtonSetsiegewartime)
END_MESSAGE_MAP()


// cMapManager ��Ϣ�������


BOOL cMapManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect1;
	m_MapList.GetClientRect(&rect1);
	m_MapList.InsertColumn(0, _T("��ͼ"), LVCFMT_CENTER, rect1.Width() / 1.5, 0);
	m_MapList.InsertColumn(1, _T("���"), LVCFMT_CENTER, rect1.Width() / 4, 1);
	m_MapList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	LoadMapInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


/**������ɫ�ı���Ϣ��Ӧ����	by:OpU_Eye QQ:418914674**/
HBRUSH cMapManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_SIEGEWARTIME:
			case IDC_STATIC_ACCEPT:
			case IDC_STATIC_NOTICE:
			{
				CFont m_font;
				m_font.CreatePointFont(120, _T("����"));
				pDC->SelectObject(&m_font);
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 0,0));
				break;
			}
			default:
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_color);
		}
	}
	return (HBRUSH)m_brush.GetSafeHandle();
}

void cMapManager::OnBnClickedButtonMapstart()
{
	CWinThread* pThread = AfxBeginThread(StartMap, this, THREAD_PRIORITY_NORMAL);
}
UINT StartMap(LPVOID pParam)
{
	cMapManager* map = (cMapManager*)pParam;
	std::map<std::wstring, int>::iterator iter;
	for (int i = 0; i < map->m_MapList.GetItemCount(); i++)
	{
		if (map->m_MapList.GetCheck(i))
		{
			iter = map->App->m_MapInfo.find(map->m_MapList.GetItemText(i, 0).GetBuffer());
			if (iter != map->App->m_MapInfo.end())
			{
				CString buff;
				buff.Format(_T("%d"), iter->second);
				ShellExecute(NULL, _T("open"), _T("MapServer.exe"), buff, NULL, SW_SHOWNORMAL);
				Sleep(TimeCount);
			}
		}
	}
	return 0;
}

void cMapManager::OnEnChangeEditMapStarttime()
{
	int i = GetDlgItemInt(IDC_EDIT_MAP_STARTTIME);
	TimeCount = i * 1000;
}


void cMapManager::OnBnClickedButton2()
{
	for (int i = 0; i < m_MapList.GetItemCount(); i++)
	{
		m_MapList.SetCheck(i);
	}
}


void cMapManager::OnBnClickedButton3()
{
	for (int i = 0; i < m_MapList.GetItemCount(); i++)
	{
		m_MapList.SetCheck(i,false);
	}
}


void cMapManager::OnBnClickedButtonCloseallmap()
{
	CString str;
	str.Format(_T("/c TaskKill /f /im MapServer.exe"));
	ShellExecute(NULL, _T("runas"), _T("cmd.exe"), str, NULL, SW_HIDE);
}


void cMapManager::OnBnClickedButtonCloseserver()
{
	CString str, str1, str2;
	str.Format(_T("/c TaskKill /f /im MapServer.exe"));
	str1.Format(_T("/c TaskKill /f /im AgentServer.exe"));
	str2.Format(_T("/c TaskKill /f /im DistributeServer.exe"));
	ShellExecute(NULL, _T("runas"), _T("cmd.exe"), str, NULL, SW_HIDE);
	ShellExecute(NULL, _T("runas"), _T("cmd.exe"), str1, NULL, SW_HIDE);
	ShellExecute(NULL, _T("runas"), _T("cmd.exe"), str2, NULL, SW_HIDE);
}
void cMapManager::KillProcess(int port)
{
	CString str;
	int PID;
	int code = 0;
	char cmdcommond[128];
	sprintf_s(cmdcommond, "netstat -ano|findstr \"%d\"", port);
	code = execmd(cmdcommond, PID);
	if (code == 1)
	{
		if (::MessageBox(this->m_hWnd, _T("���̲��ҳɹ��Ƿ��������"), _T("��ʾ"), MB_YESNO) == IDYES)
		{
			EnableDebugPriv();
			if (pskill(PID) == 1)
				::MessageBox(this->m_hWnd, _T("�ɹ���������"), _T("��ʾ"), MB_OK);
			else
			{
				str.Format(_T("��������ʧ�ܣ����ֶ��������̡�����PID%d"), PID);
				::MessageBox(this->m_hWnd, str, _T("��ʾ"), MB_OK);
			}
		}
		return;
	}
	else if (code == 2)
		::MessageBox(this->m_hWnd, _T("δ���ҵ���ؽ���"), _T("��ʾ"), MB_OK);

}
int cMapManager::execmd(char * cmd, int &result)
{
	char buffer[128];
	char buf[64] = { '\0' };
	int i = 71;
	int j = 0;
	FILE* pipe = _popen(cmd, "r");
	if (!pipe)
		return 0;
	while (!feof(pipe))
		if (fgets(buffer, 128, pipe))
		{
			while (buffer[i] != '\n')
			{
				buf[j] = buffer[i];
				i++;
				j++;
			}
			result = atoi(buf);
			return 1;
		}
	_pclose(pipe);
	return 2;
}
int cMapManager::pskill(int id)   //���ݽ���IDɱ����  
{
	HANDLE hProcess = NULL;
	//��Ŀ�����  
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, id);
	if (hProcess == NULL) {
		wprintf(L"\nOpen Process fAiled:%d\n", GetLastError());
		return -1;
	}
	//����Ŀ�����  
	DWORD ret = TerminateProcess(hProcess, 0);
	if (ret == 0) {
		wprintf(L"%d", GetLastError());
		return -1;
	}
	return 1;
}
void cMapManager::EnableDebugPriv()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, NULL);
	CloseHandle(hToken);
}

void cMapManager::OnBnClickedButton7()
{
	int port = GetDlgItemInt(IDC_EDIT2);
	KillProcess(port);
}


void cMapManager::OnBnClickedButton8()
{
	CString str;
	GetDlgItemTextW(IDC_EDIT3, str);
	std::wstring MapName = str.GetBuffer();
	std::map<std::wstring, int>::iterator iter;
	iter = App->m_MapInfo.find(MapName);
	if (iter != App->m_MapInfo.end())
	{
		KillProcess(iter->second);
	}
}
void cMapManager::OnBnClickedButtonAg0()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), NULL, NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonAg1()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), _T("1"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonAg2()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), _T("2"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonAg3()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), _T("3"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonAg4()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), _T("4"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonAg5()
{
	ShellExecute(NULL, _T("open"), _T("AgentServer.exe"), _T("5"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis0()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), NULL, NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis1()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), _T("1"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis2()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), _T("2"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis3()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), _T("3"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis4()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), _T("4"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnBnClickedButtonDis5()
{
	ShellExecute(NULL, _T("open"), _T("DistributeServer.exe"), _T("5"), NULL, SW_SHOWNORMAL);
}


void cMapManager::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;
	std::map<std::wstring, int>::iterator iter;
	iter = App->m_MapInfo.find((m_MapList.GetItemText(temp->iItem,0)).GetBuffer());
	if (iter != App->m_MapInfo.end())
	{
		CString buff;
		buff.Format(_T("%d"), iter->second);
		ShellExecute(NULL, _T("open"), _T("MapServer.exe"), buff, NULL, SW_SHOWNORMAL);
		m_MapList.SetCheck(temp->iItem);
	}
	*pResult = 0;
}
void cMapManager::ShowSiegeWarTime(DWORD dwTime0, DWORD dwTime1)
{
	stTIME time;
	time.SetTime(0, 0, 2, 0, 0, 0);

	stTIME time0, time1;
	time0.value = dwTime0;
	time1.value = dwTime1;

	CString str,str1;
	str.Format(_T("%02d �� %02d �� %02d ʱ"), time1.GetMonth(), time1.GetDay(), time1.GetHour());
	SetDlgItemText(IDC_STATIC_SIEGEWARTIME,str);

	str.Format(_T("%02d �� %02d �� %02d ʱ ��ʼ"),time0.GetMonth(), time0.GetDay(), time0.GetHour());

	time0 += time;

	DWORD dayEcp = time0.GetDay();
	DWORD hourEcp = time0.GetHour();

	if (1 != dayEcp)
	{
		dayEcp -= 1;
		hourEcp += 24;
	}
	str1.Format(_T("%02d �� %02d �� %02d ʱ Ϊֹ"), time0.GetMonth(), time0.GetDay() - 1, time0.GetHour() + 24);
	str += _T(" ~ ");
	str += str1;
	SetDlgItemText(IDC_STATIC_NOTICE, str);

	str.Format(_T("%02d �� %02d �� %02d ʱ ��ʼ"), time0.GetMonth(), time0.GetDay(), time0.GetHour());

	time.SetTime(0, 0, 3, 0, 0, 0);
	time0 += time;

	dayEcp = time0.GetDay();
	hourEcp = time0.GetHour();

	if (1 != dayEcp)
	{
		dayEcp -= 1;
		hourEcp += 24;
	}
	str1.Format(_T("%02d �� %02d �� %02d ʱ Ϊֹ"), time0.GetMonth(), time0.GetDay() - 1, time0.GetHour() + 24);
	str += _T(" ~ ");
	str += str1;
	SetDlgItemText(IDC_STATIC_ACCEPT, str);
}
QSTATETYPE GetCurTime()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	stTIME time;
	time.SetTime(systime.wYear-2015, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

	return time.value;
}

void cMapManager::OnBnClickedButtonGetsiegewartime()
{
	CString str;
	count = 0;
	str.Format(_T("select REGIST_TIME from TB_SIEGEWAR_INFO where SIEGEWAR_IDX=9999 and MAPNUM=5"));
	if (!App->m_ChrDB.SelectSQL(str))
		return;
	App->m_ChrDB.CollectMsg("REGIST_TIME", count);
	App->m_ChrDB.CloseRecord();
	str.Format(_T("select * from TB_SIEGEWAR_INFO where SIEGEWAR_IDX=%d and MAPNUM=5"), count);
	if (!App->m_ChrDB.SelectSQL(str))
		return;
	DWORD RegTime, SiegeWarTime;
	App->m_ChrDB.CollectMsg("REGIST_TIME", RegTime);
	App->m_ChrDB.CollectMsg("SIEGEWAR_TIME", SiegeWarTime);
	ShowSiegeWarTime(RegTime, SiegeWarTime);
}


void cMapManager::OnBnClickedButtonSetsiegewartime()
{
	stTIME time0, time1,time;
	DWORD Month, Day, Hour;
	time.SetTime(0, 0, 2, 0, 0, 0);
	Month = Day = Hour = 0;
	Month = GetDlgItemInt(IDC_EDIT_ACCEPT_START_M);
	Day = GetDlgItemInt(IDC_EDIT_ACCEPT_START_D);
	Hour = GetDlgItemInt(IDC_EDIT_ACCEPT_START_H);
	if (Month == 0 || Day == 0)
	{ 
		MessageBox(_T("����������д����"), _T("��ʾ"), MB_OK);
		Month = Day = Hour = 0;
		return;
	}
	time0.SetTime(0, Month, Day, Hour, 0, 0);
	time0 -= time;
	Month = Day = Hour = 0;
	Month = GetDlgItemInt(IDC_EDIT_SIEGEWAR_START_M);
	Day = GetDlgItemInt(IDC_EDIT_SIEGEWAR_START_D);
	Hour = GetDlgItemInt(IDC_EDIT_SIEGEWAR_START_H);
	if (Month == 0 || Day == 0||Hour>20||Hour<2)
	{
		MessageBox(_T("����������д����"), _T("��ʾ"), MB_OK);
		Month = Day = Hour = 0;
		return;
	}
	time1.SetTime(0, Month, Day, Hour, 0, 0);
	CString str;
	str.Format(_T("UPDATE TB_SIEGEWAR_INFO SET REGIST_TIME = %d,SIEGEWAR_TIME=%d WHERE SIEGEWAR_IDX=%d and MAPNUM=5"),  time0, time1, count);
	long count;
	if (!App->m_ChrDB.ExecuteSQL(str,count))
		MessageBox(_T("����ʧ��"), _T("��ʾ"),MB_OK);
	MessageBox(_T("���³ɹ��������������������ǵ�ͼ�鿴"), _T("��ʾ"), MB_OK);
}
