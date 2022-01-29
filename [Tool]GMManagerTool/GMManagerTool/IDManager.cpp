/***账号管理类声明	by:OpU_Eye QQ:418914674***/
#include "stdafx.h"
#include "GMManagerTool.h"
#include "IDManager.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(IDManager, CDialogEx)

IDManager::IDManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

IDManager::~IDManager()
{

}

void IDManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_pStatic = (CStatic*)GetDlgItem(IDC_STATIC_IDSELRESULT);
	SetTextColor(1);
	//背景颜色修改	by:OpU_Eye QQ:418914674
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	//默认连接数据库信息	by:OpU_Eye QQ:418914674
	CString str;
	::GetPrivateProfileStringW(_T("IP"), _T("ServerIp"), _T("192.168.145.128"), (LPWSTR)(LPCTSTR)str,16,_T(_path_));
	SetDlgItemTextW(IDC_EDIT_SERVERADRR, str);
	::GetPrivateProfileStringW(_T("ID"), _T("SqlID"), _T("sa"), (LPWSTR)(LPCTSTR)str, 26, _T(_path_));
	SetDlgItemTextW(IDC_EDIT_SQLUSER, str);
	::GetPrivateProfileStringW(_T("PWD"), _T("SqlPwd"), _T("sa"), (LPWSTR)(LPCTSTR)str, 50, _T(_path_));
	SetDlgItemTextW(IDC_EDIT_SQLPASSWORD,str);
	m_pStaticCheck=(CStatic*)GetDlgItem(IDC_STATIC_CONNCHECK);
	//全局变量初始化	by:OpU_Eye QQ:418914674
	App = (CGMManagerToolApp *)AfxGetApp();
	//List控件初始化	by:OpU_Eye QQ:418914674
	m_IDList = (CListCtrl*)GetDlgItem(IDC_LIST_ID);
	m_CharList = (CListCtrl*)GetDlgItem(IDC_LIST_ID_CHARACTER);
	CRect rect1, rect2;

	m_IDList->GetClientRect(&rect1);
	m_CharList->GetClientRect(&rect2);

	m_IDList->SetExtendedStyle(m_IDList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_CharList->SetExtendedStyle(m_CharList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_IDList->InsertColumn(0, _T("账号IDX"), LVCFMT_CENTER, rect1.Width() / 8, 0);
	m_IDList->InsertColumn(1, _T("账号"), LVCFMT_CENTER, rect1.Width() / 8, 1);
	m_IDList->InsertColumn(2, _T("密码"), LVCFMT_CENTER, rect1.Width() / 8, 2);
	m_IDList->InsertColumn(3, _T("元宝数量"), LVCFMT_CENTER, rect1.Width() / 8, 4);
	m_IDList->InsertColumn(4, _T("泡点数量"), LVCFMT_CENTER, rect1.Width() / 8, 5);
	m_IDList->InsertColumn(5, _T("账号权限"), LVCFMT_CENTER, rect1.Width() / 8, 6);
	m_IDList->InsertColumn(6, _T("注册IP"), LVCFMT_CENTER, rect1.Width() / 8, 7);
	m_IDList->InsertColumn(7, _T("密保邮箱"), LVCFMT_CENTER, rect1.Width() / 8, 8);
	m_IDList->InsertColumn(8, _T("密保手机"), LVCFMT_CENTER, rect1.Width() / 8, 10);

	m_CharList->InsertColumn(0, _T("角色ID"), LVCFMT_CENTER, rect2.Width() /5, 0);
	m_CharList->InsertColumn(1, _T("角色名"), LVCFMT_CENTER, rect2.Width() / 5, 1);
	m_CharList->InsertColumn(2, _T("等级"), LVCFMT_CENTER, rect2.Width() / 5, 2);
	m_CharList->InsertColumn(3, _T("转生数"), LVCFMT_CENTER, rect2.Width() / 5, 3);
	m_CharList->InsertColumn(4, _T("游戏币"), LVCFMT_CENTER, rect2.Width() / 5, 4);
}
//消息映射表	by:OpU_Eye QQ:418914674
BEGIN_MESSAGE_MAP(IDManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CONNDB, &IDManager::OnBnClickedButtonConndb)
	ON_BN_CLICKED(IDC_BUTTON_ID_SELECT, &IDManager::OnBnClickedButtonIdSelect)
	ON_BN_CLICKED(IDC_BUTTON_GOLD_SEL, &IDManager::OnBnClickedButtonGoldSel)
	ON_BN_CLICKED(IDC_BUTTON_MALL_SEL, &IDManager::OnBnClickedButtonMallSel)
	ON_BN_CLICKED(IDC_BUTTON_LEVEL_SEL, &IDManager::OnBnClickedButtonLevelSel)
	ON_BN_CLICKED(IDC_BUTTON_IP_SEL, &IDManager::OnBnClickedButtonIpSel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ID, &IDManager::OnLvnItemchangedListId)
	ON_BN_CLICKED(IDC_BUTTON4, &IDManager::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ID, &IDManager::OnBnClickedButtonNewId)
	ON_BN_CLICKED(IDC_BUTTON_GETINFO, &IDManager::OnBnClickedButtonGetinfo)
	ON_BN_CLICKED(IDC_BUTTON_ADDGOLD, &IDManager::OnBnClickedButtonAddgold)
	ON_BN_CLICKED(IDC_BUTTON_CLGOLD, &IDManager::OnBnClickedButtonClgold)
	ON_BN_CLICKED(IDC_BUTTON_DISABLEID, &IDManager::OnBnClickedButtonDisableid)
	ON_BN_CLICKED(IDC_BUTTON_NODISABLEID, &IDManager::OnBnClickedButtonNodisableid)
	ON_BN_CLICKED(IDC_BUTTON_GMPOWER, &IDManager::OnBnClickedButtonGmpower)
	ON_BN_CLICKED(IDC_BUTTON_USERPOWER, &IDManager::OnBnClickedButtonUserpower)
	ON_BN_CLICKED(IDC_BUTTON_CLMALL, &IDManager::OnBnClickedButtonClmall)
END_MESSAGE_MAP()


//数据库连接函数	by:OpU_Eye QQ:418914674
BOOL IDManager::ConnDB()
{
	CString TempStr;
	GetDlgItemText(IDC_STATIC_CONNCHECK, TempStr);
	if (TempStr == "链接成功")
	{
		App->m_IDDB.CloseDB();
		App->m_ChrDB.CloseDB();
		App->m_LogDB.CloseDB();
	}
	BOOL IsConn = false;

	GetDlgItemText(IDC_EDIT_SERVERADRR, m_DBip);
	GetDlgItemText(IDC_EDIT_SQLUSER, m_DBUser);
	GetDlgItemText(IDC_EDIT_SQLPASSWORD, m_DBPassWord);
	if (m_DBip.IsEmpty() || m_DBUser.IsEmpty() || m_DBPassWord.IsEmpty())
	{
		TempStr.Format(_T("【数 据 库 连 接】请 输 入 数 据 库 连 接 信 息！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return IsConn;
	}
	m_color = RGB(0, 0, 255);
	TempStr.Format(_T("%s"), _T("正在连接..."));
	m_pStaticCheck->SetWindowTextW(TempStr);
	m_pStaticCheck->RedrawWindow();
	if (!App->SetIDDBInfo(m_DBUser, m_DBPassWord, m_DBip))
	{
		return false;
	}
	if (!App->SetChrDBInfo(m_DBUser, m_DBPassWord, m_DBip))
	{
		return false;
	}
	if (!App->SetLogDBInfo(m_DBUser, m_DBPassWord, m_DBip))
	{
		return false;
	}

	if (App->m_IDDB.m_pConn == NULL || App->m_ChrDB.m_pConn == NULL || App->m_LogDB.m_pConn == NULL)
	{
		TempStr.Format(_T("%s"), _T("链接失败"));
		m_color = RGB(255, 0, 0);
		m_pStaticCheck->SetWindowTextW(TempStr);
		m_pStaticCheck->RedrawWindow();
		IsConn = false;
		return IsConn;
	}
	else
	{
		TempStr.Format(_T("%s"), _T("加载信息中..."));
		m_pStaticCheck->SetWindowTextW(TempStr);
		m_pStaticCheck->RedrawWindow();
		IsConn = true;
	}
	return IsConn;
}
//获取账号信息，元宝信息，泡点信息，在线玩家信息	by:OpU_Eye QQ:418914674
BOOL IDManager::GetInfo()
{
	CString TempStr;
	long IDCount=0,GoldCount=0,MallCount=0,LoginUserCount=0;
	
	TempStr.Format(_T("select count(id_loginid) as IDCount,sum(isnull(GoldMoney,0)) as GoldCount,sum(isnull(MallMoney,0)) as Mallcount from chr_log_info"));
	if (App->m_IDDB.SelectSQL(TempStr))
	{
		App->m_IDDB.CollectMsg("IDCount", IDCount);
		App->m_IDDB.CollectMsg("GoldCount", GoldCount);
		App->m_IDDB.CollectMsg("Mallcount", MallCount);
		App->m_IDDB.CloseRecord();
		TempStr.Format(_T("select COUNT(user_id) as LoginUserCount from LoginTable;"));
		App->m_IDDB.SelectSQL(TempStr);
		App->m_IDDB.CollectMsg("LoginUserCount", LoginUserCount);
		App->m_IDDB.CloseRecord();

		//账号总数查询
		TempStr.Format(_T("%d"), IDCount);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_ID_COUNT);
		m_pStaticTmp->SetWindowTextW(TempStr);
		//元宝总数查询
		TempStr.Format(_T("%d"), GoldCount);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_GOLD);
		m_pStaticTmp->SetWindowTextW(TempStr);
		//泡点总数查询
		TempStr.Format(_T("%d"), MallCount);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_MALL);
		m_pStaticTmp->SetWindowTextW(TempStr);
		//在线玩家查询
		TempStr.Format(_T("%d"), LoginUserCount);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_ONLINECOUNT);
		m_pStaticTmp->SetWindowTextW(TempStr);
		m_pStaticTmp->RedrawWindow();
		return TRUE;
	}
	return false;
}

//清空编辑框内容	by:OpU_Eye QQ:418914674
void IDManager::InitEdit()
{
	CString TempStr;
	TempStr.Format(_T("%s"), _T(""));
	SetDlgItemText(IDC_EDIT_ID, TempStr);

	SetDlgItemText(IDC_EDIT_PASSWORD, TempStr);

	SetDlgItemText(IDC_EDIT_IP, TempStr);

	SetDlgItemText(IDC_EDIT_EMAIL, TempStr);

	SetDlgItemText(IDC_EDIT_TEL, TempStr);

	SetDlgItemText(IDC_EDIT_USERLEVEL, TempStr);

	SetDlgItemText(IDC_EDIT_GOLD,TempStr);

	SetDlgItemText(IDC_EDIT_MALL, TempStr);
}
void IDManager::SetTextColor(int cl)
{
	switch (cl)
	{
	case 1:
		m_ResultColor = RGB(255, 0, 0);
		break;
	case 2:
		m_ResultColor = RGB(0, 255, 0);
		break;
	}
	m_pStatic->RedrawWindow();
}
//数据库连接	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonConndb()
{
	CString TempStr;
	if (!ConnDB())
	{
		TempStr.Format(_T("%s"), _T("数据库未连接"));
		m_color = RGB(255, 0, 0);
		m_pStaticCheck->SetWindowTextW(TempStr);
		m_pStaticCheck->RedrawWindow();
		return;
	}
	if (!GetInfo())
	{
		TempStr.Format(_T("%s"), _T("获取信息失败!"));
		m_color = RGB(255, 0, 0);
		m_pStaticCheck->SetWindowTextW(TempStr);
		m_pStaticCheck->RedrawWindow();
		return;
	}
	TempStr.Format(_T("%s"), _T("链接成功"));
	m_color = RGB(0, 255, 0);
	m_pStaticCheck->SetWindowTextW(TempStr);
	m_pStaticCheck->RedrawWindow();
}
//设置账号数据到List控件	by:OpU_Eye QQ:418914674
void IDManager::SetIdList(int i)
{
	CString TempStr;
	TempStr.Format(_T("%d"), i);
	TempStr.Format(_T("%d"), m_Id_Idx);
	m_IDList->InsertItem(i, TempStr);
	TempStr.Format(_T("%s"), m_UserId);
	m_IDList->SetItemText(i, 1, TempStr);
	TempStr.Format(_T("%s"), m_UserPassWord);
	m_IDList->SetItemText(i, 2, TempStr);
	TempStr.Format(_T("%d"), m_GoldMoney);
	m_IDList->SetItemText(i, 3, TempStr);
	TempStr.Format(_T("%d"), m_MallMoney);
	m_IDList->SetItemText(i, 4, TempStr);
	TempStr.Format(_T("%d"), m_UserLevel);
	m_IDList->SetItemText(i, 5, TempStr);
	TempStr.Format(_T("%s"), m_IP);
	m_IDList->SetItemText(i, 6, TempStr);
	TempStr.Format(_T("%s"), m_Email);
	m_IDList->SetItemText(i, 7, TempStr);
	TempStr.Format(_T("%s"), m_Tel);
	m_IDList->SetItemText(i, 8, TempStr);
}
//初始化账号信息变量	by:OpU_Eye QQ:418914674
void IDManager::InitIDInfo()
{
	m_UserId.Format(_T("%s"),_T("无"));
	m_UserPassWord.Format(_T("%s"), _T("无"));
	m_IP.Format(_T("%s"), _T("无"));
	m_Email.Format(_T("%s"), _T("无"));
	m_UserLevel = 0;
	m_GoldMoney = 0;
	m_MallMoney = 0;
	m_Id_Idx = 0;

	m_CharacterName.Format(_T("%s"), _T("无"));
	m_CharacterLevel = 0;
	m_CharacterRest = 0;
	m_CharacterMoney = 0;
}
//设置账号信息到编辑框	by:OpU_Eye QQ:418914674
void IDManager::SetEdit()
{
	CString TempStr;
	TempStr.Format(_T("%s"), m_UserId);
	SetDlgItemText(IDC_EDIT_ID, TempStr);

	TempStr.Format(_T("%s"), m_UserPassWord);
	SetDlgItemText(IDC_EDIT_PASSWORD, TempStr);

	TempStr.Format(_T("%s"), m_IP);
	SetDlgItemText(IDC_EDIT_IP, TempStr);

	TempStr.Format(_T("%s"), m_Email);
	SetDlgItemText(IDC_EDIT_EMAIL, TempStr);

	TempStr.Format(_T("%s"), m_Tel);
	SetDlgItemText(IDC_EDIT_TEL, TempStr);

	TempStr.Format(_T("%d"), m_UserLevel);
	SetDlgItemText(IDC_EDIT_USERLEVEL, TempStr);

	TempStr.Format(_T("%d"), m_GoldMoney);
	SetDlgItemText(IDC_EDIT_GOLD, TempStr);

	TempStr.Format(_T("%d"), m_MallMoney);
	SetDlgItemText(IDC_EDIT_MALL, TempStr);
}
//获取数据库结果集到角色变量并添加到List控件显示	by:OpU_Eye QQ:418914674
void IDManager::GetChDBInfo()
{
	CString TempStr;
	m_CharList->DeleteAllItems();
	int i = m_CharList->GetItemCount();
	if (!App->SelectCharacterByUserId(m_Id_Idx)) return;
	while (!App->m_ChrDB.IsRecordEOF())
	{
		App->m_ChrDB.CollectMsg("CHARACTER_NAME", m_CharacterName);
		App->m_ChrDB.CollectMsg("CHARACTER_MONEY", m_CharacterMoney);
		App->m_ChrDB.CollectMsg("CHARACTER_GRADE", m_CharacterLevel);
		App->m_ChrDB.CollectMsg("CHARACTER_RESET", m_CharacterRest);
		App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_CharacterIdx);

		TempStr.Format(_T("%d"), i);
		TempStr.Format(_T("%d"), m_CharacterIdx);
		m_CharList->InsertItem(i, TempStr);
		TempStr.Format(_T("%s"), m_CharacterName);
		m_CharList->SetItemText(i, 1, TempStr);
		TempStr.Format(_T("%d"), m_CharacterLevel);
		m_CharList->SetItemText(i, 2,TempStr);
		TempStr.Format(_T("%d"), m_CharacterRest);
		m_CharList->SetItemText(i, 3, TempStr);
		TempStr.Format(_T("%d"), m_CharacterMoney);
		m_CharList->SetItemText(i, 4, TempStr);
		App->m_ChrDB.RecordMoveNext();
		i++;
	}
	App->m_ChrDB.CloseRecord();
}
//通过账号查询账号信息	by:OpU_Eye QQ:418914674
void IDManager::GetUserInfoByID()
{
	CString id_loginid,TempStr;
	GetDlgItemText(IDC_EDIT_ID_SELECT, id_loginid);
	if (id_loginid.IsEmpty())
	{
		SetTextColor(1);
		TempStr.Format(_T("【账 号 查 询】请 输 入 查 询 信 息！"), id_loginid);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	int count = 0;
	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE id_loginid = '%s'"), id_loginid);
	if(!App->m_IDDB.SelectSQL(TempStr)) return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count == 0)
	{
		SetTextColor(1);
		TempStr.Format(_T("【账 号 查 询】账 号【%s】不 存 在"), id_loginid);
		SetDlgItemText(IDC_STATIC_IDSELRESULT,TempStr);
		return ;
	}
	InitIDInfo();
	if (!App->SelectLoginIdByID(id_loginid)) return;
	App->m_IDDB.CollectMsg("id_loginid", m_UserId);
	App->m_IDDB.CollectMsg("id_passwd", m_UserPassWord);
	App->m_IDDB.CollectMsg("UserLevel", m_UserLevel);
	App->m_IDDB.CollectMsg("Register_Ip", m_IP);
	App->m_IDDB.CollectMsg("MallMoney", m_MallMoney);
	App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
	App->m_IDDB.CollectMsg("Register_Email", m_Email);
	App->m_IDDB.CollectMsg("Register_Tel", m_Tel);
	App->m_IDDB.CollectMsg("propid", m_Id_Idx);

	m_IDList->DeleteAllItems();
	int i = m_IDList->GetItemCount();
	SetIdList(i);
	GetChDBInfo();
	SetEdit();
	SetTextColor(2);
	TempStr.Format(_T("账 号【%s】信 息 查 询 成 功!"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
	App->m_IDDB.CloseRecord();
}
//通过注册IP查询账号信息	by:OpU_Eye QQ:418914674
void IDManager::GetUserInfoByIP()
{
	CString TempStr;
	int count=0;
	GetDlgItemText(IDC_EDIT_IP_SELECT, m_IPSel);
	if (m_IPSel.IsEmpty())
	{
		SetTextColor(1);
		TempStr.Format(_T(" 【IP 查 询】请 输 入 查 询 信 息！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE Register_Ip = '%s'"), m_IPSel);
	if(!App->m_IDDB.SelectSQL(TempStr))return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count == 0)
	{
		SetTextColor(1);
		TempStr.Format(_T("未 查 询 到 注 册 IP 为【%s】的 账 号！"), m_IPSel);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	m_IDList->DeleteAllItems();
	int i = m_IDList->GetItemCount();
	if (!App->SelectLonginIdByIp(m_IPSel)) return;
	while (!App->m_IDDB.IsRecordEOF())
	{
		InitIDInfo();
		App->m_IDDB.CollectMsg("id_loginid", m_UserId);
		App->m_IDDB.CollectMsg("id_passwd", m_UserPassWord);
		App->m_IDDB.CollectMsg("UserLevel", m_UserLevel);
		App->m_IDDB.CollectMsg("Register_Ip", m_IP);
		App->m_IDDB.CollectMsg("MallMoney", m_MallMoney);
		App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
		App->m_IDDB.CollectMsg("Register_Email", m_Email);
		App->m_IDDB.CollectMsg("Register_Tel", m_Tel);
		App->m_IDDB.CollectMsg("propid", m_Id_Idx);
		SetIdList(i);
		App->m_IDDB.RecordMoveNext();
		i++;
	}
	SetTextColor(2);
	TempStr.Format(_T("注 册 IP 为【%s】的 用 户 共 查 询 到【%d】个！"), m_IPSel,i);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//通过账号权限查询账号信息	by:OpU_Eye QQ:418914674
void IDManager::GetUserInfoByLevel()
{
	CString TempStr;
	int count = 0;
	m_Level = GetDlgItemInt(IDC_EDIT_USERLEVE_SELECT);
	if (m_Level==0)
	{
		TempStr.Format(_T(" 【账 号 权 限 查 询】请 输 入 查 询 信 息！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE UserLevel = %d"), m_Level);
	if(!App->m_IDDB.SelectSQL(TempStr))return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count == 0)
	{
		TempStr.Format(_T(" 未 查 询 到 权 限 等 于【%d】的 账 号！"), m_Level);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	m_IDList->DeleteAllItems();
	int i = m_IDList->GetItemCount();
	if (!App->SelectLonginIdByLevel(m_Level)) return;
	while (!App->m_IDDB.IsRecordEOF())
	{
		InitIDInfo();
		App->m_IDDB.CollectMsg("id_loginid", m_UserId);
		App->m_IDDB.CollectMsg("id_passwd", m_UserPassWord);
		App->m_IDDB.CollectMsg("UserLevel", m_UserLevel);
		App->m_IDDB.CollectMsg("Register_Ip", m_IP);
		App->m_IDDB.CollectMsg("MallMoney", m_MallMoney);
		App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
		App->m_IDDB.CollectMsg("Register_Email", m_Email);
		App->m_IDDB.CollectMsg("Register_Tel", m_Tel);
		App->m_IDDB.CollectMsg("propid", m_Id_Idx);
		SetIdList(i);
		App->m_IDDB.RecordMoveNext();
		i++;
	}
	SetTextColor(2);
	TempStr.Format(_T("权 限 为 【%d】 的 账 号 共 有 【%d】 个"), m_Level,i);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//通过泡点数量查询账号信息	by:OpU_Eye QQ:418914674
void IDManager::GetUserInfoByMall()
{
	CString TempStr;
	int count = 0;
	m_Mall=GetDlgItemInt(IDC_EDIT_MALL_SELECT);
	if (m_Mall==0)
	{
		TempStr.Format(_T("【泡 点 查 询】请 输 入 查 询 信 息!"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE MallMoney >= %d"), m_Mall);
	if(!App->m_IDDB.SelectSQL(TempStr))return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count == 0)
	{
		TempStr.Format(_T("未 查 询 到 泡 点 数 量 大 于 等 于【%lld】的 账 号"), m_Gold);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	m_IDList->DeleteAllItems();
	int i = m_IDList->GetItemCount();
	if (!App->SelectLonginIdByMall(m_Mall)) return;
	while (!App->m_IDDB.IsRecordEOF())
	{
		InitIDInfo();
		App->m_IDDB.CollectMsg("id_loginid", m_UserId);
		App->m_IDDB.CollectMsg("id_passwd", m_UserPassWord);
		App->m_IDDB.CollectMsg("UserLevel", m_UserLevel);
		App->m_IDDB.CollectMsg("Register_Ip", m_IP);
		App->m_IDDB.CollectMsg("MallMoney", m_MallMoney);
		App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
		App->m_IDDB.CollectMsg("Register_Email", m_Email);
		App->m_IDDB.CollectMsg("Register_Tel", m_Tel);
		App->m_IDDB.CollectMsg("propid", m_Id_Idx);
		SetIdList(i);
		App->m_IDDB.RecordMoveNext();
		i++;
	}
	SetTextColor(2);
	TempStr.Format(_T("泡 点 数 量 大 于 等 于【%lld】的 账 号 共 有【%d】个 !"), m_Mall, i);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//通过元宝数量查询账号信息	by:OpU_Eye QQ:418914674
void IDManager::GetUserInfoByGold()
{
	CString TempStr;
	int count = 0;
	m_Gold=GetDlgItemInt(IDC_EDIT_GOLD_SELECT);
	if (m_Gold==0)
	{
		TempStr.Format(_T("【元 宝 查 询】请 输 入 查 询 信 息!"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);		return;
	}
	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE GoldMoney >= %d"), m_Gold);
	if(!App->m_IDDB.SelectSQL(TempStr))return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count == 0)
	{
		TempStr.Format(_T("未 查 询 到 元 宝 数 量 大 于 等 于【%lld】的 账 号"), m_Gold);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return ;
	}

	m_IDList->DeleteAllItems();
	int i = m_IDList->GetItemCount();
	if (!App->SelectLonginIdByGold(m_Gold)) return;
	while (!App->m_IDDB.IsRecordEOF())
	{
		InitIDInfo();
		App->m_IDDB.CollectMsg("id_loginid", m_UserId);
		App->m_IDDB.CollectMsg("id_passwd", m_UserPassWord);
		App->m_IDDB.CollectMsg("UserLevel", m_UserLevel);
		App->m_IDDB.CollectMsg("Register_Ip", m_IP);
		App->m_IDDB.CollectMsg("MallMoney", m_MallMoney);
		App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
		App->m_IDDB.CollectMsg("Register_Email", m_Email);
		App->m_IDDB.CollectMsg("Register_Tel", m_Tel);
		App->m_IDDB.CollectMsg("propid", m_Id_Idx);
		SetIdList(i);
		App->m_IDDB.RecordMoveNext();
		i++;
	}
	SetTextColor(2);
	TempStr.Format(_T("元 宝 数 量 大 于 等 于【%lld】的 账 号 共 有【%d】个 !"), m_Gold, i);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//ID查询按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonIdSelect()
{
	SetTextColor(1);
	GetUserInfoByID();
}
//元宝查询按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonGoldSel()
{
	SetTextColor(1);
	GetUserInfoByGold();
	InitEdit();
	m_CharList->DeleteAllItems();
}
//泡点查询按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonMallSel()
{
	SetTextColor(1);
	GetUserInfoByMall();
	InitEdit();
	m_CharList->DeleteAllItems();
}
//账号权限查询按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonLevelSel()
{
	SetTextColor(1);
	GetUserInfoByLevel();
	InitEdit();
	m_CharList->DeleteAllItems();
}
//IP查询按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonIpSel()
{
	SetTextColor(1);
	GetUserInfoByIP();
	InitEdit();
	m_CharList->DeleteAllItems();
}
//ID List控件鼠标单击消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString TempStr;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	POSITION pos = m_IDList->GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		
		int nItem = m_IDList->GetNextSelectedItem(pos);
		TempStr = m_IDList->GetItemText(nItem, 0);
		m_Id_Idx = _ttoi(TempStr);
		m_UserId = m_IDList->GetItemText(nItem, 1);
		m_UserPassWord = m_IDList->GetItemText(nItem, 2);
		TempStr = m_IDList->GetItemText(nItem, 3);
		m_GoldMoney = _ttoi(TempStr);
		TempStr = m_IDList->GetItemText(nItem, 4);
		m_MallMoney = _ttoi(TempStr);
		TempStr = m_IDList->GetItemText(nItem, 5);
		m_UserLevel = _ttoi(TempStr);
		m_IP = m_IDList->GetItemText(nItem, 6);
		m_Email = m_IDList->GetItemText(nItem, 7);
		m_Tel = m_IDList->GetItemText(nItem, 8);

		GetChDBInfo();
		SetEdit();
	}
	*pResult = 0;
}
//账号修改按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButton4()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);


	GetDlgItemText(IDC_EDIT_PASSWORD, m_UserPassWord);

	GetDlgItemText(IDC_EDIT_IP, m_IP);
	
	GetDlgItemText(IDC_EDIT_EMAIL, m_Email);

	GetDlgItemText(IDC_EDIT_TEL, m_Tel);
	if (m_UserId.IsEmpty() || m_UserPassWord.IsEmpty() ||
		m_IP.IsEmpty() || m_Email.IsEmpty() || m_Tel.IsEmpty())
	{
		TempStr.Format(_T("【账 号 修 改】账 号 信 息 不 完 整！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}

	if(!App->UpdateUerID(m_UserId, m_UserPassWord, m_IP, m_Email, m_Tel))return;
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【账 号 修 改】账 号【%s】 修 改 成 功！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//账号注册按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonNewId()
{
	CString TempStr;
	SetTextColor(1);
	int count = 0;
	GetDlgItemText(IDC_EDIT_ID, m_UserId);

	GetDlgItemText(IDC_EDIT_PASSWORD, m_UserPassWord);

	GetDlgItemText(IDC_EDIT_IP, m_IP);

	GetDlgItemText(IDC_EDIT_EMAIL, m_Email);

	GetDlgItemText(IDC_EDIT_TEL, m_Tel);
	if (m_UserId.IsEmpty() || m_UserPassWord.IsEmpty() ||
		m_IP.IsEmpty() || m_Email.IsEmpty() || m_Tel.IsEmpty())
	{
		TempStr.Format(_T("【账 号 注 册】注 册 信 息 不 完 整！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE id_loginid = %d"), m_UserId);
	if (!App->m_IDDB.SelectSQL(TempStr))return;
	App->m_IDDB.CollectMsg("IDCount", count);
	if (count != 0)
	{
		TempStr.Format(_T(" 【账 号 注 册】账 号 %s 已 存 在"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	if (!App->GetMax())
	{
		TempStr.Format(_T("【账 号 注 册】获 取【propid】失 败！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	App->m_IDDB.CollectMsg((ULONG)0, m_propid);
	m_propid += 1;
	if(!App->InsertNewID(m_propid, m_UserId, m_UserPassWord, m_IP, m_Email, m_Tel))return;
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【账 号 注 册】账 号【%s】 注 册 成 功！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//信息刷新按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonGetinfo()
{
	CString TempStr;
	if (!GetInfo())
	{
		SetTextColor(1);
		TempStr.Format(_T("【信 息 刷 新】信 息 刷 新 失 败！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	SetTextColor(2);
	TempStr.Format(_T("【信 息 刷 新】信 息 刷 新 成 功！"));
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}

//元宝充值按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonAddgold()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID,m_UserId);
	m_GoldMoney=GetDlgItemInt(IDC_EDIT_GOLD);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【元 宝 充 值】请 输 入 要 充 值 的 账 号！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	if(!App->AddGoldMoney(m_UserId, m_GoldMoney))return;
	InitEdit();
	m_IDList->DeleteAllItems();
	App->m_IDDB.CloseRecord();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【元 宝 充 值】账 号【%s】 元 宝 充 值 成 功！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}

//元宝清理按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonClgold()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【元 宝 清 除】请 输 入 要 清 除 元 宝 的 账 号！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	if (!App->CLGoldMoney(m_UserId))	return;
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【元 宝 清 除】账 号【%s】 元 宝 被 成 功 清 除！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//账号封禁按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonDisableid()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【账 号 封 禁】请 输 入 需 封 禁 账 号！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_DisableUser '%s'"), m_UserId);
	if (!App->m_IDDB.ExecuteSQL(TempStr, count))
	{
		TempStr.Format(_T("【账 号 封 禁】账 号【%s】封 号 失 败！"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【账 号 封 禁】账 号【%s】 已 被 封 禁！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//账号解封按钮消息响应函数
void IDManager::OnBnClickedButtonNodisableid()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【账 号 解 封】请 输 入 需 解 封 账 号"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_NoDisableUser '%s'"), m_UserId);
	if (!App->m_IDDB.ExecuteSQL(TempStr, count))
	{
		TempStr.Format(_T("【账 号 解 封】账 号【%s】 解 封 失 败！"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【账 号 解 封】账 号【%s】 解 封 成 功！"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//GM权限提升按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonGmpower()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【GM 权 限 提 升】请 输 入 需 提 升 权 限 的 账 号！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	m_UserLevel = GetDlgItemInt(IDC_EDIT_USERLEVEL);
	if (m_UserLevel < 6)
	{
		TempStr.Format(_T("【GM 权 限 提 升】该 账 号 已 经 是 GM 权 限 账 号！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	if (m_UserLevel >= 10)
	{
		TempStr.Format(_T("【GM 权 限 提 升】被 封 禁 的 账 号 无 法 提 升 为 GM 权 限，请 先 解 除 封 禁！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT,TempStr);
		return;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_Modfy_ID_Info_To_GM '%s'"), m_UserId);
	if (!App->m_IDDB.ExecuteSQL(TempStr, count))
	{
		TempStr.Format(_T("【GM 权 限 提 升】GM 权 限 提 升 失 败！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【GM 权 限 提 升】账 号【%s】已 成 功 提 升 为 GM 权 限 账 号!"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//GM权限消除按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonUserpower()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【GM 权 限 消 除】请 输 入 需 消 除 权 限 的 账 号!"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	m_UserLevel = GetDlgItemInt(IDC_EDIT_USERLEVEL);
	if (m_UserLevel == 6)
	{
		TempStr.Format(_T("【GM 权 限 消 除】该 账 号 已 经 是 普 通 权 限 账 号!"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT,TempStr);
		return;
	}
	if (m_UserLevel >= 10)
	{
		TempStr.Format(_T("【GM 权 限 消 除】被 封 禁 的 账 号 无 修 改 权 限，请 先 解 除 封 禁!"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_Modfy_ID_Info_To_NOM '%s'"), m_UserId);
	if (!App->m_IDDB.ExecuteSQL(TempStr, count))
	{
		TempStr.Format(_T("【GM 权 限 消 除】权 限 修 改 失 败!"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【GM 权 限 消 除】账 号【%s】已 成 功 变 更 为 普 通 权 限 账 号!"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}
//泡点清理按钮消息响应函数	by:OpU_Eye QQ:418914674
void IDManager::OnBnClickedButtonClmall()
{
	CString TempStr;
	SetTextColor(1);
	GetDlgItemText(IDC_EDIT_ID, m_UserId);
	if (m_UserId.IsEmpty())
	{
		TempStr.Format(_T("【泡 点 清 理】请 输 入 需 消 除 泡 点 的 账 号!"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT,TempStr);
		return;
	}
	m_MallMoney = GetDlgItemInt(IDC_EDIT_MALL);
	if (m_MallMoney == 0)
	{
		TempStr.Format(_T("【泡 点 清 理】账 号【%s】 没 有 泡 点!"), m_UserId);
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
		return;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_Clear_MallMoney '%s'"), m_UserId);
	if (!App->m_IDDB.ExecuteSQL(TempStr, count))
	{
		TempStr.Format(_T("【泡 点 清 理】泡 点 清 除 失 败！"));
		SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);

		return;
	}
	InitEdit();
	m_IDList->DeleteAllItems();
	SetDlgItemText(IDC_EDIT_ID_SELECT, m_UserId);
	GetUserInfoByID();
	App->m_IDDB.CloseRecord();
	SetTextColor(2);
	TempStr.Format(_T("【泡 点 清 理】账 号【%s】已 成 功 清 除 所 有 泡 点!"), m_UserId);
	SetDlgItemText(IDC_STATIC_IDSELRESULT, TempStr);
}

//字体颜色改变消息响应函数	by:OpU_Eye QQ:418914674
HBRUSH IDManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC_CONNCHECK:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_color);
			break;
		}
		case IDC_STATIC_ID_COUNT:
		case IDC_STATIC_ONLINECOUNT:
		case IDC_STATIC_GOLD:
		case IDC_STATIC_MALL:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,0,0));
			break;
		}
		case IDC_STATIC_IDSELRESULT:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_ResultColor);
			break;
		}
		default:
			break;
		}

	}
	return (HBRUSH)m_brush.GetSafeHandle();
}

BOOL IDManager::PreTranslateMessage(MSG* pMsg)
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
