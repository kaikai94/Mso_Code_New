// DealManager.cpp : 实现文件
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "DealManager.h"
#include "afxdialogex.h"
#include <windows.h>  
#include <winbase.h>  
#include <tlhelp32.h>  
#pragma comment(lib,"kernel32.lib")  
#pragma comment(lib,"advapi32.lib") 

// DealManager 对话框

IMPLEMENT_DYNAMIC(DealManager, CDialogEx)

DealManager::DealManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

DealManager::~DealManager()
{
}

void DealManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAPNAME, m_EditMapName);
	DDX_Control(pDX, IDC_EDIT_MAPNUM, m_EditMapNum);
	DDX_Control(pDX, IDC_STATIC_SELECTINFO, m_TipText);
	m_color = RGB(255, 0, 0);
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	m_TipText.RedrawWindow();
	/*全局变量初始化	by:OpU_Eye QQ:418914674*/
	App = (CGMManagerToolApp *)AfxGetApp();
	m_MasterIdx = 0;
	m_MapNum = 0;
	DDX_Control(pDX, IDC_STATIC_MEMBER_COUNT, m_MemberCount);
}


BEGIN_MESSAGE_MAP(DealManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &DealManager::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON1, &DealManager::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA_GUILD, &DealManager::OnBnClickedButtonUpdataGuild)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA_POINT, &DealManager::OnBnClickedButtonUpdataPoint)
END_MESSAGE_MAP()


// DealManager 消息处理程序


BOOL DealManager::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		// 如果消息是键盘按下事件，且是Esc键，执行以下代码（什么都不做，你可以自己添加想要的代码）
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// 如果消息是键盘按下事件，且是Entert键，执行以下代码（什么都不做，你可以自己添加想要的代码）
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/*地图名->编号转换函数	by:OpU_Eye QQ:418914674*/
int DealManager::GetMapNum(CString MapName)
{
	std::map<std::wstring, int>::iterator iter;

	if (!MapName.IsEmpty())
	{
		for (iter = App->m_MapInfo.begin(); iter != App->m_MapInfo.end(); ++iter)
		{
			if (MapName == iter->first.c_str())
				return iter->second;
		}
	}
	return 0;
}


void DealManager::KillProcess(int port)
{
	CString str;
	int PID;
	int code = 0;
	char cmdcommond[128];
	sprintf_s(cmdcommond, "netstat -ano|findstr \"%d\"", port);
	code = execmd(cmdcommond, PID);
	if (code == 1)
	{
		if (::MessageBox(this->m_hWnd, _T("进程查找成功是否结束进程"), _T("提示"), MB_YESNO) == IDYES)
		{
			EnableDebugPriv();
			if (pskill(PID) == 1)
				::MessageBox(this->m_hWnd, _T("成功结束进程"), _T("提示"), MB_OK);
			else
			{
				str.Format(_T("结束进程失败，请手动结束进程。进程PID%d"), PID);
				::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
			}
		}
		return;
	}
	else if (code == 2)
		::MessageBox(this->m_hWnd, _T("未能找到相关进程"), _T("提示"), MB_OK);

}
int DealManager::execmd(char * cmd, int &result)
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
int DealManager::pskill(int id)   //根据进程ID杀进程  
{
	HANDLE hProcess = NULL;
	//打开目标进程  
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, id);
	if (hProcess == NULL) {
		wprintf(L"\nOpen Process fAiled:%d\n", GetLastError());
		return -1;
	}
	//结束目标进程  
	DWORD ret = TerminateProcess(hProcess, 0);
	if (ret == 0) {
		wprintf(L"%d", GetLastError());
		return -1;
	}
	return 1;
}
void DealManager::EnableDebugPriv()
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
void DealManager::OnBnClickedButtonFind()
{
	CString MapName, Temp;
	int MapNum = 0;
	m_EditMapName.GetWindowText(MapName);
	m_EditMapNum.GetWindowText(Temp);
	MapNum = _ttoi(Temp);
	int MapPort = 18000;
	if (MapNum != 0 && !MapName.IsEmpty())
	{
		::MessageBox(this->m_hWnd, _T("只能使用一个条件查询"), _T("提示"), MB_OK);
		m_EditMapName.SetWindowText(_T(""));
		m_EditMapNum.SetWindowText(_T(""));
		return;
	}
	if (MapNum != 0 && MapName.IsEmpty())
	{
		MapPort += MapNum;
	}
	if (MapNum == 0 && !MapName.IsEmpty())
	{
		MapNum = GetMapNum(MapName);
		MapPort += MapNum;
	}
	KillProcess(MapPort);
}


HBRUSH DealManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC_SELECTINFO:
		case IDC_STATIC_MEMBER_COUNT:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_color);
			break;
		}
		default:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0, 0, 0));
		}

	}
	return (HBRUSH)m_brush.GetSafeHandle();
}


void DealManager::OnBnClickedButton1()
{
	ClearDlg();
	CString Temp1;
	int MemberCount = 0;
	GetDlgItem(IDC_EDIT_GUILDNAME)->GetWindowTextW(m_GuildName);
	GetDlgItem(IDC_EDIT_MASTERNAME)->GetWindowTextW(m_MasterName);
	GetDlgItem(IDC_EDIT_MASTERIDX)->GetWindowTextW(Temp1);
	m_MasterIdx = _ttoi(Temp1);
	if ((!m_GuildName.IsEmpty() && !m_MasterName.IsEmpty())||
		(!m_GuildName.IsEmpty() && m_MasterIdx!=0)||
		(!m_MasterName.IsEmpty()&& m_MasterIdx != 0))
	{
		m_color = RGB(255, 0, 0);
		Temp1.Format(_T("【查 询 条 件】每 次 只 能 输 入 一 个 查 询 条 件，请 重 新 输 入"));
		m_TipText.SetWindowTextW(Temp1);
		m_TipText.RedrawWindow();
		GetDlgItem(IDC_EDIT_GUILDNAME)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_MASTERNAME)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_MASTERIDX)->SetWindowTextW(_T(""));
		return;
	}
	if (!m_GuildName.IsEmpty())
	{
		Temp1.Format(_T("SELECT COUNT(GuildIdx) AS Count from TB_GUILD WHERE GuildName='%s'"), m_GuildName);
		if (!App->m_ChrDB.SelectSQL(Temp1))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】帮 会 信 息 查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		App->m_ChrDB.CollectMsg("Count", MemberCount);
		if (MemberCount == 0)
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】不 存 在 名 称 为【%s】的 帮 会"), m_GuildName);
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		if (!App->SelectGuildInfoForGuildName(m_GuildName))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}

	}
	else if (!m_MasterName.IsEmpty())
	{
		Temp1.Format(_T("SELECT COUNT(GuildIdx) AS Count from TB_GUILD WHERE MasterName='%s'"), m_MasterName);
		if (!App->m_ChrDB.SelectSQL(Temp1))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】帮 会 信 息 查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		App->m_ChrDB.CollectMsg("Count", MemberCount);
		if (MemberCount == 0)
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】角 色【%s】尚 未 创 建 帮 会"), m_MasterName);
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		if (!App->SelectGuildInfoForGuildName(m_GuildName))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
	}
	else if (m_MasterIdx != 0)
	{
		Temp1.Format(_T("SELECT COUNT(GuildIdx) AS Count from TB_GUILD WHERE MasterIdx='%d'"), m_MasterIdx);
		if (!App->m_ChrDB.SelectSQL(Temp1))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】帮 会 信 息 查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		App->m_ChrDB.CollectMsg("Count", MemberCount);
		if (MemberCount == 0)
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】I D 为【%d】的 角 色 尚 未 创 建 帮 会"), m_MasterIdx);
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
		if (!App->SelectGuildInfoForMasterIdx(m_MasterIdx))
		{
			m_color = RGB(255, 0, 0);
			Temp1.Format(_T("【信 息 查 询】查 询 失 败"));
			m_TipText.SetWindowTextW(Temp1);
			m_TipText.RedrawWindow();
			return;
		}
	}
	GetGuildInfoFromDB();
	SetEditText();
	m_color = RGB(0, 255, 0);
	Temp1.Format(_T("【信 息 查 询】查 询 成 功"));
	m_TipText.SetWindowTextW(Temp1);
	m_TipText.RedrawWindow();
	Temp1.Format(_T("SELECT COUNT(CHARACTER_IDX) AS MemberCount FROM TB_CHARACTER WHERE CHARACTER_MUNPAIDX=%d"), m_GuildInfo.GuildIdx);
	App->m_ChrDB.SelectSQL(Temp1);
	App->m_ChrDB.CollectMsg("MemberCount", MemberCount);
	Temp1.Format(_T("当 前 帮 会 人 数:%d 人"), MemberCount);
	m_MemberCount.SetWindowTextW(Temp1);
	m_MemberCount.RedrawWindow();
	return;
}
void DealManager::ClearDlg()
{
	GetDlgItem(IDC_EDIT_GUILD_IDX)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_MASTER_IDX)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_NAME)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_MASTER_NAME)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_MONEY)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_LEVEL)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_MAP)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_POINT)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_GUILD_KILLMONSTER)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_UNION_NAME)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_UNION_IDX)->SetWindowTextW(_T(""));
	m_MemberCount.SetWindowTextW(_T(""));
}
void DealManager::GetGuildInfoFromDB()
{
	App->m_ChrDB.CollectMsg("GuildIdx", m_GuildInfo.GuildIdx);
	App->m_ChrDB.CollectMsg("GuildName", m_GuildInfo.GuildName);
	App->m_ChrDB.CollectMsg("MasterIdx", m_GuildInfo.MasterIdx);
	App->m_ChrDB.CollectMsg("MasterName", m_GuildInfo.MasterName);
	App->m_ChrDB.CollectMsg("GuildLevel", m_GuildInfo.GuildLevel);
	App->m_ChrDB.CollectMsg("UnionIdx", m_GuildInfo.UnionIdx);
	App->m_ChrDB.CollectMsg("GuildMoney", m_GuildInfo.GuildMoney);
	App->m_ChrDB.CollectMsg("GuildMapNum", m_MapNum);
	App->m_ChrDB.CollectMsg("GuildPoint", m_GuildInfo.GuildPoint);
	App->m_ChrDB.CollectMsg("GuildKillMonster", m_GuildInfo.GuildKillMonster);
	App->m_ChrDB.CollectMsg("UnionName", m_GuildInfo.UnionIdxName);
}
void DealManager::SetEditText()
{
	CString str1;
	GetMapName();
	str1.Format(_T("%d"), m_GuildInfo.GuildIdx);
	GetDlgItem(IDC_EDIT_GUILD_IDX)->SetWindowTextW(str1);
	str1.Format(_T("%d"),m_GuildInfo.MasterIdx);
	GetDlgItem(IDC_EDIT_MASTER_IDX)->SetWindowTextW(str1);
	GetDlgItem(IDC_EDIT_GUILD_NAME)->SetWindowTextW(m_GuildInfo.GuildName);
	GetDlgItem(IDC_EDIT_MASTER_NAME)->SetWindowTextW(m_GuildInfo.MasterName);
	str1.Format(_T("%d"),m_GuildInfo.GuildMoney);
	GetDlgItem(IDC_EDIT_GUILD_MONEY)->SetWindowTextW(str1);
	str1.Format(_T("%d"), m_GuildInfo.GuildLevel);
	GetDlgItem(IDC_EDIT_GUILD_LEVEL)->SetWindowTextW(str1);
	GetDlgItem(IDC_EDIT_GUILD_MAP)->SetWindowTextW(m_GuildInfo.Location);
	str1.Format(_T("%d"),m_GuildInfo.GuildPoint);
	GetDlgItem(IDC_EDIT_GUILD_POINT)->SetWindowTextW(str1);
	str1.Format(_T("%d"),m_GuildInfo.GuildKillMonster);
	GetDlgItem(IDC_EDIT_GUILD_KILLMONSTER)->SetWindowTextW(str1);
	GetDlgItem(IDC_EDIT_UNION_NAME)->SetWindowTextW(m_GuildInfo.UnionIdxName);
	str1.Format(_T("%d"),m_GuildInfo.UnionIdx);
	GetDlgItem(IDC_EDIT_UNION_IDX)->SetWindowTextW(str1);
}
void DealManager::GetMapName()
{
	std::map<std::wstring, int>::iterator iter;
	if (m_MapNum != 0)
	{
		for (iter = App->m_MapInfo.begin(); iter != App->m_MapInfo.end(); ++iter)
		{
			if(iter->second==m_MapNum)
				m_GuildInfo.Location = iter->first.c_str();
		}
		m_MapNum = 0;
		return;
	}
	else
	{
		m_GuildInfo.Location.Format(_T("未知"));
		return;
	}
}


void DealManager::OnBnClickedButtonUpdataGuild()
{
	CString str2;
	GetDlgItem(IDC_EDIT_GUILD_MONEY)->GetWindowTextW(str2);
	m_GuildInfo.GuildMoney = _ttoi(str2);
	GetDlgItem(IDC_EDIT_GUILD_LEVEL)->GetWindowTextW(str2);
	m_GuildInfo.GuildLevel = _ttoi(str2);
	App->UpdataGuildInfo(&m_GuildInfo);
}


void DealManager::OnBnClickedButtonUpdataPoint()
{
	CString str3;
	GetDlgItem(IDC_EDIT_GUILD_POINT)->GetWindowTextW(str3);
	m_GuildInfo.GuildPoint = _ttoi(str3);
	GetDlgItem(IDC_EDIT_GUILD_KILLMONSTER)->GetWindowTextW(str3);
	m_GuildInfo.GuildKillMonster = _ttoi(str3);
	App->UpddataGUildPointInfo(&m_GuildInfo);
}
