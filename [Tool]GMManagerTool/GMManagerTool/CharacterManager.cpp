/***角色管理类实现	by:OpU_Eye QQ:418914674***/
#include "stdafx.h"
#include "GMManagerTool.h"
#include "CharacterManager.h"
#include "afxdialogex.h"
/*配置文件路径（实现物品名称<->物品ID转换）	by:OpU_Eye QQ:418914674*/
IMPLEMENT_DYNAMIC(CharacterManager, CDialogEx)

CharacterManager::CharacterManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_pStaticTip = (CStatic*)GetDlgItem(IDC_STATIC_CHRTIP);
	m_pStaticTip->RedrawWindow();
	/*转换变量初始化	by:OpU_Eye QQ:418914674*/
	m_CharacterID = -1;
	m_Map = 0;
	m_ItemNum = 0;
	m_ItemName.Format(_T("%s"), _T("未知物品"));
	/*窗口背景颜色初始化	by:OpU_Eye QQ:418914674*/
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	/*全局变量初始化	by:OpU_Eye QQ:418914674*/
	App = (CGMManagerToolApp *)AfxGetApp();
	/*List控件初始化	by:OpU_Eye QQ:418914674*/
	m_ItemList = (CListCtrl*)GetDlgItem(IDC_LIST_CHR_ITEM);
	m_SelChrList = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	CRect rect1, rect2;
	m_SelChrList->GetClientRect(&rect1);
	m_ItemList->GetClientRect(&rect2);

	m_SelChrList->SetExtendedStyle(m_ItemList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ItemList->SetExtendedStyle(m_SelChrList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_SelChrList->InsertColumn(0, _T("角色ID"), LVCFMT_CENTER, rect1.Width() / 13, 0);
	m_SelChrList->InsertColumn(1, _T("角色名"), LVCFMT_CENTER, rect1.Width() / 10, 1);
	m_SelChrList->InsertColumn(2, _T("当前等级"), LVCFMT_CENTER, rect1.Width() / 10, 2);
	m_SelChrList->InsertColumn(3, _T("最大等级"), LVCFMT_CENTER, rect1.Width() / 10, 3);
	m_SelChrList->InsertColumn(4, _T("力量"), LVCFMT_CENTER, rect1.Width() / 13, 4);
	m_SelChrList->InsertColumn(5, _T("敏捷"), LVCFMT_CENTER, rect1.Width() / 13, 5);
	m_SelChrList->InsertColumn(6, _T("体质"), LVCFMT_CENTER, rect1.Width() / 13, 6);
	m_SelChrList->InsertColumn(7, _T("心脉"), LVCFMT_CENTER, rect1.Width() / 13, 7);
	m_SelChrList->InsertColumn(8, _T("游戏币"), LVCFMT_CENTER, rect1.Width() / 8, 8);
	m_SelChrList->InsertColumn(9, _T("转数"), LVCFMT_CENTER, rect1.Width() / 14, 9);
	m_SelChrList->InsertColumn(10, _T("剩余属性点"), LVCFMT_CENTER, rect1.Width() / 8, 10);

	m_ItemList->InsertColumn(0, _T("物品编号"), LVCFMT_CENTER, rect1.Width() / 4, 0);
	m_ItemList->InsertColumn(1, _T("物品名称"), LVCFMT_CENTER, rect1.Width() / 4, 1);
	m_ItemList->InsertColumn(2, _T("拥有数量"), LVCFMT_CENTER, rect1.Width() / 4, 2);
	m_ItemList->InsertColumn(3, _T("数据库编号"), LVCFMT_CENTER, rect1.Width() / 4, 3);

}

/*消息映射表	by:OpU_Eye QQ:418914674*/
BEGIN_MESSAGE_MAP(CharacterManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_USERID, &CharacterManager::OnBnClickedButtonUserid)
	ON_BN_CLICKED(IDC_BUTTON_ID, &CharacterManager::OnBnClickedButtonId)
	ON_BN_CLICKED(IDC_BUTTON_NAME, &CharacterManager::OnBnClickedButtonName)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RESULT, &CharacterManager::OnLvnItemchangedListId)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULT, &CharacterManager::OnNMRclickListChrItem)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CHR_ITEM, &CharacterManager::OnNMRclickListItem)
	ON_COMMAND(32775, &CharacterManager::On32775)
	ON_COMMAND(32771, &CharacterManager::On32771)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_RESULT, &CharacterManager::OnLvnKeydownListResult)
END_MESSAGE_MAP()

/*物品编号->物品名转换函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetItemName()
{
	std::map<int, ITEMINFO>::iterator iter;
	if (m_equipIdx.m_Armlet != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Armlet);
		if(iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Armlet = iter->second.Name.c_str();
		m_equipIdx.m_Armlet = 0;
	}
	if (m_equipIdx.m_Belt != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Belt);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Belt = iter->second.Name.c_str();
		m_equipIdx.m_Belt = 0;
	}
	if (m_equipIdx.m_Cape != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Cape);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Cape = iter->second.Name.c_str();
		m_equipIdx.m_Cape = 0;
	}
	if (m_equipIdx.m_Dress != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Dress);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Dress = iter->second.Name.c_str();
		m_equipIdx.m_Dress = 0;
	}
	if (m_equipIdx.m_Hat != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Hat);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Hat = iter->second.Name.c_str();
		m_equipIdx.m_Hat = 0;
	}
	if (m_equipIdx.m_Necklace != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Necklace);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Necklace = iter->second.Name.c_str();
		m_equipIdx.m_Necklace = 0;
	}
	if (m_equipIdx.m_Ring1 != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Ring1);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Ring1 = iter->second.Name.c_str();
		m_equipIdx.m_Ring1 = 0;
	}
	if (m_equipIdx.m_Ring2 != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Ring2);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Ring2 = iter->second.Name.c_str();
		m_equipIdx.m_Ring2 = 0;
	}
	if (m_equipIdx.m_Shoes != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Shoes);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Shoes = iter->second.Name.c_str();
		m_equipIdx.m_Shoes = 0;
	}
	if (m_equipIdx.m_Weapon != 0)
	{

		iter = App->m_ChangeUseConfig.find(m_equipIdx.m_Weapon);
		if (iter != App->m_ChangeUseConfig.end())
			m_EquipName.m_Weapon = iter->second.Name.c_str();
		m_equipIdx.m_Weapon = 0;
	}
	if (m_ItemNum != 0)
	{
		iter = App->m_ChangeUseConfig.find(m_ItemNum);
		if (iter != App->m_ChangeUseConfig.end())
			m_ItemName = iter->second.Name.c_str();
		//m_ItemNum = 0;
	}
}
/*物品名->物品编号转换函数	by:OpU_Eye QQ:418914674*/
//void CharacterManager::GetItemNum()
//{
//	if (m_EquipName.m_Armlet != "")
//	{
//		m_equipIdx.m_Armlet = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Armlet, 0, _T(_path_));
//		m_EquipName.m_Armlet.Format(_T(""));
//	}
//	if (m_EquipName.m_Belt != "")
//	{
//		m_equipIdx.m_Belt = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Belt, 0, _T(_path_));
//		m_EquipName.m_Belt.Format(_T(""));
//	}
//	if (m_EquipName.m_Cape != "")
//	{
//		m_equipIdx.m_Cape = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Cape, 0, _T(_path_));
//		m_EquipName.m_Cape.Format(_T(""));
//	}
//	if (m_EquipName.m_Dress != "")
//	{
//		m_equipIdx.m_Dress = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Dress, 0, _T(_path_));
//		m_EquipName.m_Dress.Format(_T(""));
//	}
//	if (m_EquipName.m_Hat != "")
//	{
//		m_equipIdx.m_Hat = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Hat, 0, _T(_path_));
//		m_EquipName.m_Hat.Format(_T(""));
//	}
//	if (m_EquipName.m_Belt != "")
//	{
//		m_equipIdx.m_Necklace = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Necklace, 0, _T(_path_));
//		m_EquipName.m_Belt.Format(_T(""));
//	}
//	if (m_EquipName.m_Ring1 != "")
//	{
//		m_equipIdx.m_Ring1 = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Ring1, 0, _T(_path_));
//		m_EquipName.m_Ring1.Format(_T(""));
//	}
//	if (m_EquipName.m_Ring2 != "")
//	{
//		m_equipIdx.m_Ring2 = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Ring2, 0, _T(_path_));
//		m_EquipName.m_Ring2.Format(_T(""));
//	}
//	if (m_EquipName.m_Shoes != "")
//	{
//		m_equipIdx.m_Shoes = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Shoes, 0, _T(_path_));
//		m_EquipName.m_Shoes.Format(_T(""));
//	}
//	if (m_EquipName.m_Weapon != "")
//	{
//		m_equipIdx.m_Weapon = ::GetPrivateProfileInt(_T("ItemToIdx"), m_EquipName.m_Weapon, 0, _T(_path_));
//		m_EquipName.m_Weapon.Format(_T(""));
//	}
//}
/*转职编号->名称转换函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetStateName()
{
	std::map<int, std::wstring>::iterator iter;
	if (m_State == 64 || m_State == 65 || m_State == 128 || m_State == 129)
	{
		iter= App->m_StateChange.find(m_State);
		if (iter != App->m_StateChange.end())
		{
			m_StateName = iter->second.c_str();
		}
		m_State = 0;
		return;
	}
	else
	{
		m_StateName.Format(_T("一般"));
		return;
	}
}
/*转职名称->编号转换函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetStateNum()
{
	std::map<int, std::wstring>::iterator iter;
	if (m_StateName.Find(_T("一般")) == -1)
	{
		for (iter = App->m_StateChange.begin(); iter != App->m_StateChange.end(); ++iter)
		{
			if (m_StateName == iter->second.c_str())
			{
				m_State = iter->first;
				m_StateName.Format(_T("一般"));
				break;
			}
		}
		return;	
	}
	else
	{
		m_State = 0;
	}
}
/*地图编号->地图名转换函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetMapName()
{
	
	if (m_Map != 0)
	{
		std::map<std::wstring, int>::iterator iter;
		for (iter = App->m_MapInfo.begin(); iter != App->m_MapInfo.end(); ++iter)
		{
			if (iter->second == m_Map)
				m_MapName = iter->first.c_str();
		}
		m_Map = 0;
		return;
	}
	else
	{
		m_MapName.Format(_T("宇宙边缘"));
		return;
	}
}
/*地图名->编号转换函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetMapNum()
{
	if (m_MapName.Find(_T("宇宙边缘")) == -1)
	{
		std::map<std::wstring, int>::iterator iter;
		iter = App->m_MapInfo.find(m_MapName.GetBuffer());
		if (iter != App->m_MapInfo.end())
		{
			m_Map = iter->second;
			m_MapName.Format(_T("宇宙边缘"));
		}
		return;
	}
	else
	{
		m_Map = 0;
		return;
	}
}
/*获取查询条件及查询判断处理函数	by:OpU_Eye QQ:418914674*/
BOOL CharacterManager::GetText()
{
	CString TempStr;
	BOOL IsOk = false;
	GetDlgItemText(IDC_EDIT_USERID_SEL, m_UserID);
	GetDlgItemText(IDC_EDIT_IDX_SEL, m_CharacterIDX);
	GetDlgItemText(IDC_EDIT_NAME_SEL, m_CharacterName);
	if (!m_UserID.IsEmpty() && !m_CharacterIDX.IsEmpty() ||
		!m_UserID.IsEmpty() && !m_CharacterName.IsEmpty() ||
		!m_CharacterIDX.IsEmpty() && !m_CharacterName.IsEmpty())
	{
		::MessageBox(this->m_hWnd, _T("每次只能输入一个查询条件"), _T("查询条件过多"), MB_OK);
		return IsOk;
	}
	if (!m_UserID.IsEmpty())
	{
		int count = 0;
		TempStr.Format(_T("SELECT COUNT(id_idx) AS IDCount FROM chr_log_info WHERE id_loginid = '%s'"), m_UserID);
		if(!App->m_IDDB.SelectSQL(TempStr)) return false;
		App->m_IDDB.CollectMsg("IDCount", count);
		App->m_IDDB.CloseRecord();
		if (count == 0)
		{
			::MessageBox(this->m_hWnd,_T("不存在该账号，请重新输入!"),_T("账号查询角色"),MB_OK);
			return IsOk;
		}
		if (!App->SelectLoginIdByID(m_UserID))
			return IsOk;
		App->m_IDDB.CollectMsg("propid", m_CharacterID);
		App->m_IDDB.CloseRecord();
		IsOk = true;
	}
	if (!m_CharacterIDX.IsEmpty())
	{
		m_CharacterID = _ttoi(m_CharacterIDX);
		IsOk = true;
	}
	if (!m_CharacterName.IsEmpty())
	{
		IsOk = true;
	}
	return IsOk;
}
/*账号查询按钮消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnBnClickedButtonUserid()
{
	CString TempStr;
	TempStr.Format(_T("%s"), "");
	if (!GetText())return;
	if (m_CharacterID == -1)
	{
		::MessageBox(this->m_hWnd, _T("请输入要查询的角色账号"), _T("账号查询"), MB_OK);
		return;
	}
	m_SelChrList->DeleteAllItems();
	App->SelectCharacterByUserId(m_CharacterID);
	GetCharacterInfoForList();
	m_CharacterID = -1;
}
/*ID查询按钮消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnBnClickedButtonId()
{
	CString TempStr;
	if (!GetText())return;
	m_SelChrList->DeleteAllItems();

	int count = 0;
	TempStr.Format(_T("SELECT COUNT(CHARACTER_NAME) AS IDCount FROM TB_CHARACTER WHERE CHARACTER_IDX =%d"), m_CharacterID);
	if(!App->m_ChrDB.SelectSQL(TempStr))return;
	App->m_ChrDB.CollectMsg("IDCount", count);
	App->m_ChrDB.CloseRecord();
	if (count == 0)
	{
		TempStr.Format(_T("不存在角色ID为[%d]的角色"), m_CharacterID);
		::MessageBox(this->m_hWnd, TempStr, _T("角色ID查询角色"), MB_OK);
		SetDlgItemText(IDC_EDIT_IDX_SEL, TempStr);
		return;
	}
	if (m_CharacterID == -1)
	{
		::MessageBox(this->m_hWnd, _T("请输入要查询的角色ID"), _T("ID查询"), MB_OK);
		return;
	}
	m_SelChrList->DeleteAllItems();
	if(!App->SelectChrByChrID(m_CharacterID))return;
	GetCharacterInfoForList();
	SetEdit();
	if(App->SelectChrItemForChr(m_CharacterID))return;
	GetItemInfo();
	SetDlgItemText(IDC_EDIT_IDX_SEL, TempStr);
	m_CharacterID = -1;
}
/*角色名查询按钮消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnBnClickedButtonName()
{
	CString TempStr;
	if (!GetText())return;
	m_SelChrList->DeleteAllItems();

	int count = 0;
	TempStr.Format(_T("SELECT COUNT(CHARACTER_NAME) AS IDCount FROM TB_CHARACTER WHERE CHARACTER_NAME = '%s'"), m_CharacterName);
	if(!App->m_ChrDB.SelectSQL(TempStr))return;
	App->m_ChrDB.CollectMsg("IDCount", count);
	App->m_ChrDB.CloseRecord();
	if (count == 0)
	{
		TempStr.Format(_T("不存在角色名为[%s]的角色"), m_CharacterName);
		::MessageBox(this->m_hWnd, TempStr, _T("角色名查询角色"), MB_OK);
		TempStr.Format(_T("%s"), "");
		SetDlgItemText(IDC_EDIT_NAME_SEL, TempStr);
		return;
	}
	if (m_CharacterName.IsEmpty())
	{
		::MessageBox(this->m_hWnd, _T("请输入要查询的角色名"), _T("角色名查询"), MB_OK);
		return;
	}
	if(!App->SelectCharacterByName(m_CharacterName))return;
	GetCharacterInfoForList();
	if(!App->SelectChrItemForChr(m_CharaIdx))return;
	GetItemInfo();
	SetEdit();
}
/*获取数据库结果集到List控件显示	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetCharacterInfoForList()
{
	int i = 0;
	while (!App->m_ChrDB.IsRecordEOF())
	{
		App->m_ChrDB.CollectMsg("USER_IDX", m_USER_IDX);
		App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_CharaIdx);
		App->m_ChrDB.CollectMsg("CHARACTER_GENGOAL", m_GenGoal);
		App->m_ChrDB.CollectMsg("CHARACTER_DEX", m_Dex);
		App->m_ChrDB.CollectMsg("CHARACTER_STA", m_Sta);
		App->m_ChrDB.CollectMsg("CHARACTER_SIMMAK", m_Simmak);
		App->m_ChrDB.CollectMsg("CHARACTER_FAME", m_Fame);
		App->m_ChrDB.CollectMsg("CHARACTER_NAME", m_Name);
		App->m_ChrDB.CollectMsg("CHARACTER_GRADE", m_Grade);
		App->m_ChrDB.CollectMsg("CHARACTER_MAXGRADE", m_MaxGrade);
		App->m_ChrDB.CollectMsg("CHARACTER_GRADEUPPOINT", m_GradePoint);
		App->m_ChrDB.CollectMsg("CHARACTER_STAGE", m_State);
		App->m_ChrDB.CollectMsg("CHARACTER_BADFAME", m_BadFame);
		App->m_ChrDB.CollectMsg("CHARACTER_RESET", m_Reset);
		App->m_ChrDB.CollectMsg("CHARACTER_MONEY", m_Money);
		App->m_ChrDB.CollectMsg("CHARACTER_MAP", m_Map);
		App->m_ChrDB.CollectMsg("CHARACTER_HAT", m_equipIdx.m_Hat);
		App->m_ChrDB.CollectMsg("CHARACTER_WEAPON", m_equipIdx.m_Weapon);
		App->m_ChrDB.CollectMsg("CHARACTER_DRESS", m_equipIdx.m_Dress);
		App->m_ChrDB.CollectMsg("CHARACTER_SHOES", m_equipIdx.m_Shoes);
		App->m_ChrDB.CollectMsg("CHARACTER_RING1", m_equipIdx.m_Ring1);
		App->m_ChrDB.CollectMsg("CHARACTER_RING2", m_equipIdx.m_Ring2);
		App->m_ChrDB.CollectMsg("CHARACTER_CAPE", m_equipIdx.m_Cape);
		App->m_ChrDB.CollectMsg("CHARACTER_NECKLACE", m_equipIdx.m_Necklace);
		App->m_ChrDB.CollectMsg("CHARACTER_ARMLET", m_equipIdx.m_Armlet);
		App->m_ChrDB.CollectMsg("CHARACTER_BELT", m_equipIdx.m_Belt);
		App->m_ChrDB.CollectMsg("CHARACTER_ABILITYEXP", m_AbilityExp);
		App->m_ChrDB.CollectMsg("CHARACTER_NICKNAME", m_NickName);
		App->m_ChrDB.CollectMsg("CHARACTER_MARRYNAME", m_MarryName);
		SetChrList(i);
		App->m_ChrDB.RecordMoveNext();
		i++;
	}
	App->m_ChrDB.CloseRecord();
	GetItemName();
	GetMapName();
	GetStateName();
}
/*设置角色list空间项目显示	by:OpU_Eye QQ:418914674*/
void CharacterManager::SetChrList(int i)
{
	CString TempStr;
	TempStr.Format(_T("%d"), i);
	TempStr.Format(_T("%d"), m_CharaIdx);
	m_SelChrList->InsertItem(i, TempStr);
	m_SelChrList->SetItemText(i, 1, m_Name);
	TempStr.Format(_T("%d"), m_Grade);
	m_SelChrList->SetItemText(i, 2, TempStr);
	TempStr.Format(_T("%d"), m_MaxGrade);
	m_SelChrList->SetItemText(i, 3, TempStr);
	TempStr.Format(_T("%d"), m_GenGoal);
	m_SelChrList->SetItemText(i, 4, TempStr);
	TempStr.Format(_T("%d"), m_Dex);
	m_SelChrList->SetItemText(i, 5, TempStr);
	TempStr.Format(_T("%d"), m_Sta);
	m_SelChrList->SetItemText(i, 6, TempStr);
	TempStr.Format(_T("%d"), m_Simmak);
	m_SelChrList->SetItemText(i, 7, TempStr);
	TempStr.Format(_T("%d"), m_Money);
	m_SelChrList->SetItemText(i, 8, TempStr);
	TempStr.Format(_T("%d"), m_Reset);
	m_SelChrList->SetItemText(i, 9, TempStr);
	TempStr.Format(_T("%d"), m_GradePoint);
	m_SelChrList->SetItemText(i, 10, TempStr);
}
/*设置角色信息到编辑框	by:OpU_Eye QQ:418914674*/
void CharacterManager::SetEdit()
{
	CString TempStr;
	TempStr.Format(_T("%d"), m_CharaIdx);
	SetDlgItemText(IDC_EDIT_CHR_ID, TempStr);
	TempStr.Format(_T("%d"), m_GenGoal);
	SetDlgItemText(IDC_EDIT_CHR_GEN, TempStr);
	TempStr.Format(_T("%d"), m_Dex);
	SetDlgItemText(IDC_EDIT_CHR_DEX, TempStr);
	TempStr.Format(_T("%d"), m_Sta);
	SetDlgItemText(IDC_EDIT_CHR_STA, TempStr);
	TempStr.Format(_T("%d"), m_Simmak);
	SetDlgItemText(IDC_EDIT_CHR_SIM, TempStr);
	TempStr.Format(_T("%d"), m_Fame);
	SetDlgItemText(IDC_EDIT_CHR_FAME, TempStr);
	SetDlgItemText(IDC_EDIT_CHR_NAME, m_Name);
	TempStr.Format(_T("%d"), m_Grade);
	SetDlgItemText(IDC_EDIT_CHR_GRADE, TempStr);
	TempStr.Format(_T("%d"), m_MaxGrade);
	SetDlgItemText(IDC_EDIT_CHR_MAXGRADE, TempStr);
	TempStr.Format(_T("%d"), m_GradePoint);
	SetDlgItemText(IDC_EDIT_CHR_LEVELPOINT, TempStr);
	SetDlgItemText(IDC_EDIT_ZHANZHI, m_StateName);
	TempStr.Format(_T("%d"), m_BadFame);
	SetDlgItemText(IDC_EDIT_CHR_BADFAME, TempStr);
	TempStr.Format(_T("%d"), m_Reset);
	SetDlgItemText(IDC_EDIT_CHR_RESET, TempStr);
	TempStr.Format(_T("%d"), m_Money);
	SetDlgItemText(IDC_EDIT_MONEY, TempStr);
	SetDlgItemText(IDC_EDIT_MAP, m_MapName);



	SetDlgItemText(IDC_EDIT_HAT, m_EquipName.m_Hat);

	SetDlgItemText(IDC_EDIT_CHR_WEAPON, m_EquipName.m_Weapon);

	SetDlgItemText(IDC_EDIT_DRESS, m_EquipName.m_Dress);

	SetDlgItemText(IDC_EDIT_SHOES, m_EquipName.m_Shoes);

	SetDlgItemText(IDC_EDIT_RING1, m_EquipName.m_Ring1);

	SetDlgItemText(IDC_EDIT_RING2, m_EquipName.m_Ring2);

	SetDlgItemText(IDC_EDIT_CAPE, m_EquipName.m_Cape);

	SetDlgItemText(IDC_EDIT_NECKLACE, m_EquipName.m_Necklace);

	SetDlgItemText(IDC_EDIT_ARMLET, m_EquipName.m_Armlet);

	SetDlgItemText(IDC_EDIT_BELT, m_EquipName.m_Belt);

	TempStr.Format(_T("%d"), m_AbilityExp);
	SetDlgItemText(IDC_EDIT_ABILITYEXP, TempStr);

	SetDlgItemText(IDC_EDIT_CHR_NICKENAME, m_NickName);

	SetDlgItemText(IDC_EDIT_CHR_MARRYNAME, m_MarryName);
}
/*设置角色物品到物品List控件显示	by:OpU_Eye QQ:418914674*/
void CharacterManager::SetItemList(int i)
{
	CString TempStr;
	if (m_ItemCount == 0 || m_ItemPoint >= 80 && m_ItemPoint <= 89)
		m_ItemCount = 1;

	TempStr.Format(_T("%d"), i);
	TempStr.Format(_T("%d"), m_ItemNum);
	m_ItemList->InsertItem(i, TempStr);
	m_ItemList->SetItemText(i, 1, m_ItemName);
	TempStr.Format(_T("%d"), m_ItemCount);
	m_ItemList->SetItemText(i, 2, TempStr);
	TempStr.Format(_T("%d"), m_ItemDBID);
	m_ItemList->SetItemText(i, 3, TempStr);
}
/*获取数据库结果集数据到物品结构	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetItemInfo()
{
	int i = 0;
	m_ItemList->DeleteAllItems();
	while (!App->m_ChrDB.IsRecordEOF())
	{
		App->m_ChrDB.CollectMsg("ITEM_IDX", m_ItemNum);
		App->m_ChrDB.CollectMsg("ITEM_DURABILITY", m_ItemCount);
		App->m_ChrDB.CollectMsg("ITEM_POSITION", m_ItemPoint);
		App->m_ChrDB.CollectMsg("ITEM_DBIDX", m_ItemDBID);
		GetItemName();
		SetItemList(i);
		App->m_ChrDB.RecordMoveNext();
		i++;
	}
	App->m_ChrDB.CloseRecord();

}
/*获取数据库结果集数据到角色信息结构	by:OpU_Eye QQ:418914674*/
void CharacterManager::GetChrInfoWithList()
{
	App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_CharaIdx);
	App->m_ChrDB.CollectMsg("USER_IDX",m_USER_IDX);
	App->m_ChrDB.CollectMsg("CHARACTER_GENGOAL", m_GenGoal);
	App->m_ChrDB.CollectMsg("CHARACTER_DEX", m_Dex);
	App->m_ChrDB.CollectMsg("CHARACTER_STA", m_Sta);
	App->m_ChrDB.CollectMsg("CHARACTER_SIMMAK", m_Simmak);
	App->m_ChrDB.CollectMsg("CHARACTER_FAME", m_Fame);
	App->m_ChrDB.CollectMsg("CHARACTER_NAME", m_Name);
	App->m_ChrDB.CollectMsg("CHARACTER_GRADE", m_Grade);
	App->m_ChrDB.CollectMsg("CHARACTER_MAXGRADE", m_MaxGrade);
	App->m_ChrDB.CollectMsg("CHARACTER_GRADEUPPOINT", m_GradePoint);
	App->m_ChrDB.CollectMsg("CHARACTER_STAGE", m_State);
	App->m_ChrDB.CollectMsg("CHARACTER_BADFAME", m_BadFame);
	App->m_ChrDB.CollectMsg("CHARACTER_RESET", m_Reset);
	App->m_ChrDB.CollectMsg("CHARACTER_MONEY", m_Money);
	App->m_ChrDB.CollectMsg("CHARACTER_MAP", m_Map);
	App->m_ChrDB.CollectMsg("CHARACTER_HAT", m_equipIdx.m_Hat);
	App->m_ChrDB.CollectMsg("CHARACTER_WEAPON", m_equipIdx.m_Weapon);
	App->m_ChrDB.CollectMsg("CHARACTER_DRESS", m_equipIdx.m_Dress);
	App->m_ChrDB.CollectMsg("CHARACTER_SHOES", m_equipIdx.m_Shoes);
	App->m_ChrDB.CollectMsg("CHARACTER_RING1", m_equipIdx.m_Ring1);
	App->m_ChrDB.CollectMsg("CHARACTER_RING2", m_equipIdx.m_Ring2);
	App->m_ChrDB.CollectMsg("CHARACTER_CAPE", m_equipIdx.m_Cape);
	App->m_ChrDB.CollectMsg("CHARACTER_NECKLACE", m_equipIdx.m_Necklace);
	App->m_ChrDB.CollectMsg("CHARACTER_ARMLET", m_equipIdx.m_Armlet);
	App->m_ChrDB.CollectMsg("CHARACTER_BELT", m_equipIdx.m_Belt);
	App->m_ChrDB.CollectMsg("CHARACTER_ABILITYEXP", m_AbilityExp);
	App->m_ChrDB.CollectMsg("CHARACTER_NICKNAME", m_NickName);
	App->m_ChrDB.CollectMsg("CHARACTER_MARRYNAME", m_MarryName);
	App->m_ChrDB.CloseRecord();
	GetItemName();
	GetMapName();
	GetStateName();
}
/*角色List控件鼠标单击消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString TempStr;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	POSITION pos = m_SelChrList->GetFirstSelectedItemPosition();
	if (NULL != pos)
	{

		int nItem = m_SelChrList->GetNextSelectedItem(pos);
		TempStr = m_SelChrList->GetItemText(nItem, 0);
		m_CharaIdx = _ttoi(TempStr);
		if(!App->SelectChrByChrID(m_CharaIdx))return;
		GetChrInfoWithList();
		SetEdit();
		App->m_ChrDB.CloseRecord();
		if(!App->SelectChrItemForChr(m_CharaIdx))return;
		GetItemInfo();
		App->m_ChrDB.CloseRecord();
		
		TempStr.Format(_T("SELECT * FROM chr_log_info WHERE propid = %d"), m_USER_IDX);
		if(!App->m_IDDB.SelectSQL(TempStr))return;
		m_color = RGB(0,0,255);
		
		App->m_IDDB.CollectMsg("id_loginid",TempStr);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_CHRUSERIDX);
		m_pStaticTmp->SetWindowTextW(TempStr);
		
		App->m_IDDB.CollectMsg("GoldMoney", m_GoldMoney);
		TempStr.Format(_T("%d"), m_GoldMoney);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_CHRGOLD);
		m_pStaticTmp->SetWindowTextW(TempStr);
		
		App->m_IDDB.CollectMsg("MallMoney", m_GoldMoney);
		TempStr.Format(_T("%d"), m_GoldMoney);
		m_pStaticTmp = (CStatic*)GetDlgItem(IDC_STATIC_CHRMALL);
		m_pStaticTmp->SetWindowTextW(TempStr);
		
		m_pStaticTmp->RedrawWindow();
		App->m_IDDB.CloseRecord();
	}
	*pResult = 0;
}

/*角色List鼠标右键消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnNMRclickListChrItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_ITEMOPTION);
	CMenu *pContextMenu = menu.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}
/*物品List鼠标右键消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnNMRclickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menuB;
	menuB.LoadMenu(IDR_MENU_DEL_CHRITEM);
	CMenu *pContextMenu = menuB.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}
/*物品删除菜单消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::On32775()
{
	CString TempStr;
	if (IDOK == ::MessageBox(AfxGetMainWnd()->m_hWnd, _T("删除操作无法恢复！确定删除该物品吗？"), _T("角色物品删除"), MB_OKCANCEL))
	{
		POSITION pos = m_ItemList->GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			int nItem = m_ItemList->GetNextSelectedItem(pos);
			TempStr = m_ItemList->GetItemText(nItem, 3);
			m_ItemDBID = _ttoi(TempStr);
			if(!App->DeleteItem(m_ItemDBID))return;
			m_ItemList->DeleteItem(nItem);
		}
	}
}

/*角色删除菜单消息响应函数	by:OpU_Eye QQ:418914674*/
void CharacterManager::On32771()
{
	CString TempStr;
	if (IDOK == ::MessageBox(NULL, _T("删除操作无法恢复！确定删除该角色吗？"), _T("角色删除"), MB_OKCANCEL))
	{
		POSITION pos = m_SelChrList->GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			int nItem = m_SelChrList->GetNextSelectedItem(pos);
			TempStr = m_SelChrList->GetItemText(nItem, 0);
			m_CharaIdx = _ttoi(TempStr);
			if(!App->DeleteChrByID(m_CharaIdx))return;
			m_ItemList->DeleteAllItems();
			m_SelChrList->DeleteItem(nItem);
		}
	}
}
/*角色信息编辑框数据获取到结构	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnBnClickedButtonXg()
{
	CString TempStr;
	GetDlgItemText(IDC_EDIT_CHR_ID, TempStr);
	m_CharaIdx = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_GEN, TempStr);
	m_GenGoal = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_DEX, TempStr);
	m_Dex = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_STA, TempStr);
	m_Sta = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_SIM, TempStr);
	m_Simmak = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_FAME, TempStr);
	m_Fame = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_GRADE, TempStr);
	m_Grade = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_MAXGRADE, TempStr);
	m_MaxGrade = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_LEVELPOINT, TempStr);
	m_GradePoint = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_ZHANZHI, m_StateName);
	GetStateNum();

	GetDlgItemText(IDC_EDIT_CHR_BADFAME, TempStr);
	m_BadFame = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_RESET, TempStr);
	m_Reset = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_MONEY, TempStr);
	m_Money = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_MAP, m_MapName);
	GetMapNum();

	GetDlgItemText(IDC_EDIT_ABILITYEXP, TempStr);
	m_AbilityExp = _ttoi(TempStr);

	GetDlgItemText(IDC_EDIT_CHR_NICKENAME, m_NickName);

	GetDlgItemText(IDC_EDIT_CHR_MARRYNAME, m_MarryName);
	if(!App->UpdateCharacterInfo(m_CharaIdx, m_GenGoal, m_Dex, m_Sta, m_Simmak, m_Fame,
		m_Grade, m_MaxGrade, m_GradePoint, m_State, m_BadFame, m_Reset, m_Money, 
		m_Map, m_AbilityExp, m_NickName, m_MarryName))
		return;
	App->m_ChrDB.CloseRecord();
}
/*F5快捷键消息响应函数（修改角色信息）	by:OpU_Eye QQ:418914674*/
void CharacterManager::OnLvnKeydownListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if (pLVKeyDow->wVKey == 116)
	{
		if (::MessageBox(NULL, _T("请确认要修改的角色信息是否正确！"), _T("角色信息修改"), MB_OKCANCEL) == 1)
		{
			OnBnClickedButtonXg();
		}
	}
	*pResult = 0;
}
HBRUSH CharacterManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_CHRUSERIDX:
			case IDC_STATIC_CHRGOLD:
			case IDC_STATIC_CHRMALL:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_color);
				break;
			}
			case IDC_STATIC_CHRTIP:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255,0,0));
				break;
			}
			default:
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0,0,0));
		}

	}
	return (HBRUSH)m_brush.GetSafeHandle();
}

BOOL CharacterManager::PreTranslateMessage(MSG* pMsg)
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
