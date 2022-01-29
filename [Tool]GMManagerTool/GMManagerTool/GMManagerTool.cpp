
// GMManagerTool.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "GMManagerToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGMManagerToolApp

BEGIN_MESSAGE_MAP(CGMManagerToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGMManagerToolApp 构造
std::wstring stringToWstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}
CGMManagerToolApp::CGMManagerToolApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	LoadItemInfo();
	LoadMapInfo();
	m_StateChange[64] = L"化境";
	m_StateChange[65] = L"玄境";
	m_StateChange[128] = L"玄境";
	m_StateChange[129] = L"脱魔";
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
void CGMManagerToolApp::LoadItemInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/ItemList.bin"))
	{
		return;
	}
	int buf1, buf2;
	char buf3[512];
	while (1)
	{
		if (file.IsEOF())
			break;
		ITEMINFO info;
		info.Num = file.GetWord();
		info.Name=file.GetString();
		buf1 = file.GetWord();
		buf2 = file.GetWord();
		info.kindnum = file.GetWord();
		file.GetLine(buf3,512);
		m_ChangeUseConfig[info.Num]= info;
	}
	file.Release();
}

void CGMManagerToolApp::LoadMapInfo()
{
	int MapNum;
	std::wstring MapName;
	std::string tmp;
	char buf[512];
	CMHFile file;
	file.Init("./Resource/MapKindInfo.bin");
	while (1)
	{
		if (file.IsEOF())
			break;
		MapNum = file.GetWord();
		tmp = file.GetString();
		MapName = stringToWstring(tmp);
		m_MapInfo[MapName] = MapNum;
		file.GetLine(buf, 512);
	}
	file.Release();
}

CGMManagerToolApp::~CGMManagerToolApp()
{
	m_ChangeUseConfig.clear();
	m_StateChange.clear();
	m_MapInfo.clear();
}

CGMManagerToolApp theApp;

BOOL CGMManagerToolApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CGMManagerToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}
	else if (nResponse == -1)
	{

		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

BOOL CGMManagerToolApp::SetIDDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip)
{
	CString DBInfo;
	DBInfo.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;Password = %s;Initial Catalog=MHCMember;Data Source=%s"), m_DBUser, m_DBPassWord, m_DBip);
	if (!m_IDDB.SetConnStr(DBInfo))
	{
		return false;
	}
	if (!m_IDDB.ConnDB())
	{
		return false;
	}
	return true;
}
BOOL CGMManagerToolApp::SetChrDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip)
{
	CString DBInfo;
	DBInfo.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;Password = %s;Initial Catalog=MHGame;Data Source=%s"), m_DBUser, m_DBPassWord, m_DBip);
	if (!m_ChrDB.SetConnStr(DBInfo))
	{
		return false;
	}
	if(!m_ChrDB.ConnDB())
	{
		return false;
	}
	return true;
}
BOOL CGMManagerToolApp::SetLogDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip)
{
	CString DBInfo;
	DBInfo.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;Password = %s;Initial Catalog=MHLog;Data Source=%s"), m_DBUser, m_DBPassWord, m_DBip);

	if (!m_LogDB.SetConnStr(DBInfo))
	{
		return false;
	}
	if(!m_LogDB.ConnDB())
	{
		return false;
	}
	return true;
}
BOOL CGMManagerToolApp::SelectChrByChrID(int m_UserId)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("角色ID查询角色"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_CHARACTER where CHARACTER_IDX = %d"), m_UserId);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectCharacterByUserId(int m_Id_Idx)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("账号查询角色"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_CHARACTER where USER_IDX = %d"), m_Id_Idx);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectCharacterByName(CString m_CharacterName)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("角色名查询角色"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_CHARACTER where CHARACTER_NAME = '%s'"), m_CharacterName);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectChrItemForChr(int m_CharaIdx)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("角色ID查询物品"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM where CHARACTER_IDX = %d"), m_CharaIdx);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectLoginIdByID(CString id_loginid)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("账号查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select * from chr_log_info where id_loginid = '%s'"), id_loginid);
	return m_IDDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectSafeCodeByID(CString id_loginid)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("保安码查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from chr_log_info_sec_pass where id_loginid = '%s'"), id_loginid);
	return m_IDDB.SelectSQL(TempStr);
}

BOOL CGMManagerToolApp::SelectLonginIdByIp(CString m_IPSel)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("IP查询账号"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from chr_log_info where Register_Ip = '%s'"), m_IPSel);
	return m_IDDB.SelectSQL(TempStr);
}

BOOL CGMManagerToolApp::SelectLonginIdByLevel(int m_Level)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("权限查询账号"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from chr_log_info where UserLevel = %d"), m_Level);
	return m_IDDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectLonginIdByMall(long long m_Mall)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("泡点查询账号"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from chr_log_info where MallMoney > %d"), m_Mall);
	return m_IDDB.SelectSQL(TempStr);
}

BOOL CGMManagerToolApp::SelectLonginIdByGold(long long m_Gold)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("元宝查询账号"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from chr_log_info where GoldMoney > %d"), m_Gold);
	return m_IDDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::UpdateUerID(CString m_UserId, CString m_UserPassWord, CString m_IP, CString m_Email, CString tel)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("账号修改"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_Update_loginInfo '%s','%s','%s','%s','%s'"), m_UserId, m_UserPassWord,m_IP, m_Email,tel);
	if (!m_IDDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("修改失败"), _T("账号修改"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功修改%d条记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("账号修改"), MB_OK);
	return true;
}

BOOL CGMManagerToolApp::UpdateCharacterInfo(int m_CharaIdx, int m_GenGoal, int m_Dex, int m_Sta, int m_Simmak, int m_Fame, int m_Grade, int m_MaxGrade, int m_GradePoint, int m_State, int m_BadFame, int m_Reset, long m_Money, int m_Map, long m_AbilityExp, CString m_NickName, CString m_MarryName)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("角色修改"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("update TB_CHARACTER set CHARACTER_GENGOAL = %d,CHARACTER_DEX = %d,CHARACTER_STA = %d,CHARACTER_SIMMAK = %d,CHARACTER_FAME = %d,CHARACTER_GRADE = %d,CHARACTER_MAXGRADE = %d,CHARACTER_GRADEUPPOINT = %d,CHARACTER_STAGE=%d,CHARACTER_BADFAME=%d,CHARACTER_RESET=%d,CHARACTER_MONEY=%d,CHARACTER_MAP=%d,CHARACTER_ABILITYEXP=%d,CHARACTER_NICKNAME='%s',CHARACTER_MARRYNAME='%s' where CHARACTER_IDX=%d"),
		m_GenGoal, m_Dex, m_Sta, m_Simmak, m_Fame, m_Grade, m_MaxGrade, m_GradePoint, m_State, m_BadFame, m_Reset, m_Money, m_Map, m_AbilityExp, m_NickName, m_MarryName, m_CharaIdx);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("修改失败"), _T("角色修改"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功修改%d条记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("角色修改"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::DeleteChrByID(int m_CharaIdx)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("角色删除"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T( "EXEC MHGame.dbo.GMTOOL_CHARACTER_DeleteCharacter %d"), m_CharaIdx);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("删除失败"), _T("角色删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功删除%d个角色"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("角色删除"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::InsertNewID(int m_propid, CString m_UserId, CString m_UserPassWord, CString m_IP, CString m_Email,  CString tel)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("账号注册"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T( "EXEC Reg_ID '%s','%s','%s','%s',%s,'%s'"),
		m_UserId, m_UserPassWord, m_Email,tel,_T("GMd54727fbGM"),m_IP);
	if (!m_IDDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("注册失败"), _T("账号注册"), MB_OK);
		return false;
	}
	TempStr.Format(_T("账号注册成功"));
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("账号注册"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::AddGoldMoney(CString UserId, long GoldMoney)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("元宝充值"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("EXEC chr_GoldLog_Create '%s',%d"), UserId, GoldMoney);
	if (!m_IDDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("元宝充值失败"), _T("元宝充值"), MB_OK);
		return false;
	}
	TempStr.Format(_T("本次成功充值[%d]元宝"), GoldMoney);
	::MessageBox(AfxGetMainWnd()->m_hWnd, _T("元宝充值成功"), _T("元宝充值"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::CLGoldMoney(CString UserID)
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("元宝清除"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("EXEC GMTool_Clear_GoldMoney '%s'"), UserID);
	if (!m_IDDB.ExecuteSQL(TempStr, count))
	{
		return false;
	}
	::MessageBox(AfxGetMainWnd()->m_hWnd, _T("元宝清除成功"), _T("元宝清除"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::GetMax()
{
	if (m_IDDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("账号注册"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select max(propid) from chr_log_info "));
	return m_IDDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectItemByItemID(int ItemID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("物品查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM where ITEM_IDX = %d"), ItemID);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectItemByCharID(int CharID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("物品查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM where CHARACTER_IDX = %d"), CharID);
	return m_ChrDB.SelectSQL(TempStr);
}

BOOL CGMManagerToolApp::SelectItemRareInfo(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("绿化查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM_RARE where ITEM_DBIDX = %d"), DBID);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::SelectItemOptionInfo(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("强化查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM_OPTION where ITEM_DBIDX = %d"), DBID);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::DeleteItem(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("物品删除"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T( "exec MHGame.dbo.GMTool_DeleteItemOptionByItemDB_IDX %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("物品强化信息删除失败"), _T("物品删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("exec MHGame.dbo.GMTool_DeleteItemRareByItemDB_IDX %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("物品绿化信息删除失败"), _T("物品删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("DELETE TB_ITEM WHERE ITEM_DBIDX = %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("删除失败"), _T("物品删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功删除%d条物品记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("物品删除"), MB_OK);
	return true;
}

BOOL CGMManagerToolApp::DeleteItemRareInfo(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("绿化删除"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T( "Delete from TB_ITEM_RARE where ITEM_DBIDX = %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("删除失败"), _T("绿化删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功删除%d条绿化记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("绿化删除"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::DeleteItemOptionInfo(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("强化删除"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("Delete from TB_ITEM_OPTION where ITEM_DBIDX = %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("强化信息删除失败"), _T("强化删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("UPDATE TB_ITEM SET  ITEM_DURABILITY = 0 where ITEM_DBIDX = %d"), DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("强化信息删除成功，物品信息更新失败"), _T("强化删除"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功删除%d条强化记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("强化删除"), MB_OK);
	return true;

}
BOOL CGMManagerToolApp::SelectItemByDBID(int DBID)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("select *from TB_ITEM where ITEM_DBIDX = %d"), DBID);
	return m_ChrDB.SelectSQL(TempStr);
}
BOOL CGMManagerToolApp::UpdataItemRareInfo(int DBID, int GenGol, int MinChub, int Cheryuk, int Simmek, int Life, int NaeRyuk, int Shield, int FireAttr, int WaterAttr, int TreeAttr, int GoldAttr, int EarthAttr, int Phyattack, int FireAttack, int WaterAttack, int TreeAttack, int GoldAttack, int EarthAttack, int Phydefense)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("绿化修改"), MB_OK);
		return false;
	}

	long count;
	TempStr.Format(_T("update TB_ITEM_RARE set GENGOL=%d,MINCHUB=%d,CHERYUK=%d,SIMMEK=%d,LIFE=%d,NAERYUK=%d,SHIELD=%d,FREGIST=%d,WREGIST=%d,TREGIST=%d,GREGIST=%d,EREGIST=%d,PHYATTACK=%d,FATTACK=%d,WATTACK=%d,TATTACK=%d,GATTACK=%d,EATTACK=%d,PHYDEFENSE=%d where ITEM_DBIDX = %d "),
		GenGol, MinChub, Cheryuk, Simmek, Life, NaeRyuk, Shield, FireAttr, WaterAttr, TreeAttr, GoldAttr, EarthAttr, Phyattack, FireAttack, WaterAttack, TreeAttack, GoldAttack, EarthAttack, Phydefense, DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("修改失败"), _T("绿化修改"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功修改%d条绿化信息记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("绿化修改"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::UpdataItemOptionInfo(int DBID, int GenGol, int MinChub, int Cheryuk, int Simmek, int Life, int NaeRyuk, int Shield, int FireAttr, int WaterAttr, int TreeAttr, int GoldAttr, int EarthAttr, int Phyattack, int Critical, int FireAttack, int WaterAttack, int TreeAttack, int GoldAttack, int EarthAttack, int Phydefense)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("强化修改"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("update TB_ITEM_OPTION set GENGOL=%d,MINCHUB=%d,CHERYUK=%d,SIMMEK=%d,LIFE=%d,NAERYUK=%d,SHIELD=%d,FIREATTRREGIST=%d,WATERATTRREGIST	=%d,TREEATTRREGIST=%d,GOLDATTRREGIST=%d,EARTHATTRREGIST=%d,PHYATTACK=%d,CRITICAL=%d,FIREATTRATTACK=%d,WATERATTRATTACK=%d,TREEATTRATTACK=%d,GOLDATTRATTACK=%d,EARTHATTRATTACK=%d,PHYDEFENSE=%d where ITEM_DBIDX = %d "),
		GenGol, MinChub, Cheryuk, Simmek, Life, NaeRyuk, Shield, FireAttr, WaterAttr, TreeAttr, GoldAttr, EarthAttr, Phyattack, Critical, FireAttack, WaterAttack, TreeAttack, GoldAttack, EarthAttack, Phydefense, DBID);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("修改失败"), _T("强化修改"), MB_OK);
		return false;
	}
	TempStr.Format(_T("成功修改%d条强化信息记录"), count);
	::MessageBox(AfxGetMainWnd()->m_hWnd, TempStr, _T("强化修改"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::SelectGuildInfoForGuildName(CString GuildName)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("帮会信息查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("EXEC GMTOOL_SearchGuildInfo '%s',0,'0'"), GuildName);
	long count;
	if (m_ChrDB.ExecuteSQL(TempStr, count))
	{
		m_ChrDB.CloseRecord();
		GetGuildData();
		return TRUE;
	}
	return FALSE;
}
BOOL CGMManagerToolApp::GetGuildData()
{
	TempStr.Format(_T("SELECT * FROM TempTable"));
	if (m_ChrDB.SelectSQL(TempStr))
		return TRUE;
	else
		return FALSE;
}
BOOL CGMManagerToolApp::SelectGuildInfoForGuildMasterName(CString GuildMasterName)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("帮会信息查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("EXEC GMTOOL_SearchGuildInfo '0',0,'%s'"), GuildMasterName);
	long count;
	if (m_ChrDB.ExecuteSQL(TempStr, count))
	{
		m_ChrDB.CloseRecord();
		GetGuildData();
		return TRUE;
	}
	return FALSE;
}
BOOL CGMManagerToolApp::SelectGuildInfoForMasterIdx(int MasterIdx)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("帮会信息查询"), MB_OK);
		return false;
	}
	TempStr.Format(_T("EXEC GMTOOL_SearchGuildInfo '0',%d,'0'"),MasterIdx);
	long count;
	if (m_ChrDB.ExecuteSQL(TempStr, count))
	{
		m_ChrDB.CloseRecord();
		GetGuildData();
		return TRUE;
	}
	return FALSE;
}

BOOL CGMManagerToolApp::UpdataGuildInfo(GUILDINFO* GuildInfo)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("帮会信息修改"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("UPDATE TB_GUILD SET GuildLevel=%d,GuildMoney=%d WHERE GuildIdx=%d"),  GuildInfo->GuildLevel, GuildInfo->GuildMoney, GuildInfo->GuildIdx);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("帮会信息修改失败"), _T("帮会信息修改"), MB_OK);
		return false;
	}
	::MessageBox(AfxGetMainWnd()->m_hWnd, _T("帮会信息修改成功"), _T("帮会信息修改"), MB_OK);
	return true;
}
BOOL CGMManagerToolApp::UpddataGUildPointInfo(GUILDINFO* GuildInfo)
{
	if (m_ChrDB.m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("请先连接数据库"), _T("帮经信息修改"), MB_OK);
		return false;
	}
	long count;
	TempStr.Format(_T("UPDATE TB_GUILD_POINTINFO SET GuildPoint=%d,GuildKillMonsterCount=%d WHERE GuildIdx=%d"), GuildInfo->GuildPoint, GuildInfo->GuildKillMonster, GuildInfo->GuildIdx);
	if (!m_ChrDB.ExecuteSQL(TempStr, count))
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("帮经信息修改失败"), _T("帮经信息修改"), MB_OK);
		return false;
	}
	::MessageBox(AfxGetMainWnd()->m_hWnd, _T("帮经信息修改成功"), _T("帮经信息修改"), MB_OK);
	return true;
}