/***物品管理对话框类功能实现   by:OpU_Eye  QQ:418914674***/
#include "stdafx.h"
#include "GMManagerTool.h"
#include "ItemManager.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(ItemManager, CDialogEx)
//默认构造   by:OpU_Eye  QQ:418914674
ItemManager::ItemManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}
//默认析构   by:OpU_Eye  QQ:418914674
ItemManager::~ItemManager()
{

}
//初始化对话框   by:OpU_Eye  QQ:418914674
void ItemManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_pStaticT = (CStatic*)GetDlgItem(IDC_STATIC_TIP);
	m_pStaticT->RedrawWindow();
	//背景颜色初始化 	by:OpU_Eye QQ:418914674
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	//list控件初始化	by:OpU_Eye QQ:418914674
	m_BaseList = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	CRect rect1;
	m_BaseList->GetClientRect(&rect1);

	m_BaseList->SetExtendedStyle(m_BaseList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_BaseList->InsertColumn(0, _T("角色ID"), LVCFMT_CENTER, rect1.Width() / 5, 0);
	m_BaseList->InsertColumn(1, _T("物品名称"), LVCFMT_CENTER, rect1.Width() / 4, 1);
	m_BaseList->InsertColumn(2, _T("数据库编号"), LVCFMT_CENTER, rect1.Width() / 3, 2);
	m_BaseList->InsertColumn(3, _T("物品数量"), LVCFMT_CENTER, rect1.Width() / 4, 3);
	m_BaseList->InsertColumn(4, _T("锁定状态"), LVCFMT_CENTER, rect1.Width() / 4, 4);
	m_BaseList->InsertColumn(5, _T("装备成长值"), LVCFMT_CENTER, rect1.Width() / 3, 5);
	//全局变量初始化	by:OpU_Eye QQ:418914674
	App = (CGMManagerToolApp *)AfxGetApp();
	//App2 = (CGMManagerToolApp *)AfxGetApp();

}

//消息映射表   by:OpU_Eye  QQ:418914674
BEGIN_MESSAGE_MAP(ItemManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ITEM_NAME, &ItemManager::OnBnClickedButtonItemName)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_NUM, &ItemManager::OnBnClickedButtonItemNum)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_CHR, &ItemManager::OnBnClickedButtonItemChr)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, &ItemManager::OnNMClickListResult)
	ON_BN_CLICKED(IDC_BUTTON_RARE, &ItemManager::OnBnClickedButtonRare)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &ItemManager::OnBnClickedButtonOption)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULT, &ItemManager::OnNMRclickListResult)
	ON_COMMAND(32774, &ItemManager::On32774)
	ON_COMMAND(32773, &ItemManager::On32773)
	ON_COMMAND(32772, &ItemManager::On32772)
END_MESSAGE_MAP()
//获取查询信息，处理查询逻辑   by:OpU_Eye  QQ:418914674
BOOL ItemManager::GetText()
{
	CString ItemName, CharName, ItemIdx;
	BOOL IsOk = false;
	GetDlgItemText(IDC_EDIT_ITEM_NAME, ItemName);
	GetDlgItemText(IDC_EDIT_ITEM_NUM, ItemIdx);
	GetDlgItemText(IDC_EDIT_ITEM_CHR, CharName);
	if (!ItemName.IsEmpty() && !ItemIdx.IsEmpty() ||
		!ItemName.IsEmpty() && !CharName.IsEmpty() ||
		!ItemIdx.IsEmpty() && !CharName.IsEmpty() ||
		!ItemName.IsEmpty() && !ItemIdx.IsEmpty() && !CharName.IsEmpty())
	{
		::MessageBox(NULL, _T("每次只能输入一个查询条件"), _T("查询条件过多"), MB_OK);
		return IsOk;
	}
	if (!ItemName.IsEmpty())
	{
		m_BaseInfo.ItemName.Format(_T("%s"), ItemName);
		GetItemNum();
	}
	else if (!ItemIdx.IsEmpty())
	{
		m_BaseInfo.ItemID = _ttoi(ItemIdx);
	}
	else if (!CharName.IsEmpty())
	{
		int count = 0;
		App->TempStr.Format(_T("SELECT COUNT(CHARACTER_NAME) AS IDCount FROM TB_CHARACTER WHERE CHARACTER_NAME = '%s'"), CharName);
		if(!App->m_ChrDB.SelectSQL(App->TempStr)) return false;
		App->m_ChrDB.CollectMsg("IDCount", count);
		App->m_ChrDB.CloseRecord();
		if (count == 0)
		{
			App->TempStr.Format(_T("不存在角色名为[%s]的角色"), CharName);
			::MessageBox(this->m_hWnd, App->TempStr, _T("角色名查询物品"), MB_OK);
			App->TempStr.Format(_T("%s"), "");
			SetDlgItemText(IDC_EDIT_ITEM_CHR, App->TempStr);
			return IsOk;
		}
		if (!App->SelectCharacterByName(CharName))
			return IsOk;
		GetCharId();
	}
	else
	{
		::MessageBox(NULL, _T("请输入查询信息"), _T("查询条件不足"), MB_OK);
		return IsOk;
	}

	if (!CharName.IsEmpty())
	{
		if (!App->SelectItemByCharID(m_BaseInfo.CharID))
			return IsOk;
		IsOk = true;
	}
	if (!ItemIdx.IsEmpty() || !ItemName.IsEmpty())
	{
		int count = 0;
		App->TempStr.Format(_T("SELECT COUNT(ITEM_IDX) AS IDCount FROM TB_ITEM WHERE ITEM_IDX = %d"), m_BaseInfo.ItemID);
		if (!App->m_ChrDB.SelectSQL(App->TempStr)) return false;
		App->m_ChrDB.CollectMsg("IDCount", count);
		App->m_ChrDB.CloseRecord();
		if (count == 0)
		{
			App->TempStr.Format(_T("不存在的物品"));
			::MessageBox(this->m_hWnd, App->TempStr, _T("查询物品"), MB_OK);
			App->TempStr.Format(_T("%s"), "");
			SetDlgItemText(IDC_EDIT_ITEM_NAME, App->TempStr);
			SetDlgItemText(IDC_EDIT_ITEM_NUM, App->TempStr);
			return IsOk;
		}
		if (!App->SelectItemByItemID(m_BaseInfo.ItemID))
			return IsOk;
		IsOk = true;
	}
	return IsOk;
}

//物品ID转换物品名称   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemName()
{
	CString str;
	ITEMINFO * ItemInfo = NULL;
	if (m_BaseInfo.ItemID != 0)
	{
		std::map<int, ITEMINFO>::iterator iter;
		iter = App->m_ChangeUseConfig.find(m_BaseInfo.ItemID);
		if (iter != App->m_ChangeUseConfig.end())
		{
			m_BaseInfo.ItemName = iter->second.Name.c_str();
			m_BaseInfo.ItemKind = iter->second.kindnum;
		}
		m_BaseInfo.ItemID = 0;
	}
}
//物品锁定状态编号->名称转换   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemStatic()
{
	m_BaseInfo.StaticName = L"未定义";
}
//物品名称转换为物品编号   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemNum()
{
	if (m_BaseInfo.ItemName != "未知")
	{
		std::map<int, ITEMINFO>::iterator iter;
		for (iter = App->m_ChangeUseConfig.begin(); iter != App->m_ChangeUseConfig.end(); ++iter)
		{
			if (m_BaseInfo.ItemName == iter->second.Name.c_str())
			{
				m_BaseInfo.ItemID = iter->second.Num;
				m_BaseInfo.ItemName.Format(_T("未知"));
				break;
			}
		}
	}
}
//获取数据库结果集for角色ID   by:OpU_Eye  QQ:418914674
void ItemManager::GetCharId()
{
	App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_BaseInfo.CharID);
	App->m_ChrDB.CloseRecord();
}
//获取数据库结果集到物品基本信息结构   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemBaseInfo()
{
	int i = 0,count= 0;
	do
	{
		App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_BaseInfo.CharID);
		App->m_ChrDB.CollectMsg("ITEM_DBIDX", m_BaseInfo.DBID);
		App->m_ChrDB.CollectMsg("ITEM_POSITION", m_BaseInfo.ItemPoint);
		App->m_ChrDB.CollectMsg("ITEM_IDX", m_BaseInfo.ItemID);
		GetItemName();
		App->m_ChrDB.CollectMsg("ITEM_DURABILITY", m_BaseInfo.ItemCount);
		App->m_ChrDB.CollectMsg("ITEM_STATIC", m_BaseInfo.Static);
		GetItemStatic();
		App->m_ChrDB.CollectMsg("ITEM_Grow", m_BaseInfo.DBGrow);
		SetItemInfoList(i);
		App->m_ChrDB.RecordMoveNext();
		i++;
		count += m_BaseInfo.ItemCount;
	} while (!App->m_ChrDB.IsRecordEOF());
	App->TempStr.Format(_T("%d"), count);
	SetDlgItemText(IDC_STATIC_ITEMNUM, App->TempStr);
	App->TempStr.Empty();
	App->m_ChrDB.CloseRecord();
}
//获取数据库查询结果集数据到物品强化信息结构   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemRareInfo()
{
	while (!App->m_ChrDB.IsRecordEOF())
	{
		App->m_ChrDB.CollectMsg("GENGOL", m_BaseInfo.m_Rare.R_GenGol);
		App->m_ChrDB.CollectMsg("MINCHUB", m_BaseInfo.m_Rare.R_MinChub);
		App->m_ChrDB.CollectMsg("CHERYUK", m_BaseInfo.m_Rare.R_Cheryuk);
		App->m_ChrDB.CollectMsg("SIMMEK", m_BaseInfo.m_Rare.R_Simmek);
		App->m_ChrDB.CollectMsg("LIFE", m_BaseInfo.m_Rare.R_Life);
		App->m_ChrDB.CollectMsg("NAERYUK", m_BaseInfo.m_Rare.R_NaeRyuk);
		App->m_ChrDB.CollectMsg("SHIELD", m_BaseInfo.m_Rare.R_Shield);
		App->m_ChrDB.CollectMsg("FREGIST", m_BaseInfo.m_Rare.R_FireAttr);
		App->m_ChrDB.CollectMsg("WREGIST", m_BaseInfo.m_Rare.R_WaterAttr);
		App->m_ChrDB.CollectMsg("TREGIST", m_BaseInfo.m_Rare.R_TreeAttr);
		App->m_ChrDB.CollectMsg("GREGIST", m_BaseInfo.m_Rare.R_GoldAttr);
		App->m_ChrDB.CollectMsg("EREGIST", m_BaseInfo.m_Rare.R_EarthAttr);
		App->m_ChrDB.CollectMsg("PHYATTACK", m_BaseInfo.m_Rare.R_Phyattack);
		App->m_ChrDB.CollectMsg("FATTACK", m_BaseInfo.m_Rare.R_FireAttack);
		App->m_ChrDB.CollectMsg("WATTACK", m_BaseInfo.m_Rare.R_WaterAttack);
		App->m_ChrDB.CollectMsg("TATTACK", m_BaseInfo.m_Rare.R_TreeAttack);
		App->m_ChrDB.CollectMsg("GATTACK", m_BaseInfo.m_Rare.R_GoldAttack);
		App->m_ChrDB.CollectMsg("EATTACK", m_BaseInfo.m_Rare.R_EarthAttack);
		App->m_ChrDB.CollectMsg("PHYDEFENSE", m_BaseInfo.m_Rare.R_Phydefense);
		App->m_ChrDB.RecordMoveNext();
	}
	App->m_ChrDB.CloseRecord();
}
void ItemManager::GetItemOptionInfo()
{
	while (!App->m_ChrDB.IsRecordEOF())
	{
		App->m_ChrDB.CollectMsg("GENGOL", m_BaseInfo.m_Option.O_GenGol);
		App->m_ChrDB.CollectMsg("MINCHUB", m_BaseInfo.m_Option.O_MinChub);
		App->m_ChrDB.CollectMsg("CHERYUK", m_BaseInfo.m_Option.O_Cheryuk);
		App->m_ChrDB.CollectMsg("SIMMEK", m_BaseInfo.m_Option.O_Simmek);
		App->m_ChrDB.CollectMsg("LIFE", m_BaseInfo.m_Option.O_Life);
		App->m_ChrDB.CollectMsg("NAERYUK", m_BaseInfo.m_Option.O_NaeRyuk);
		App->m_ChrDB.CollectMsg("SHIELD", m_BaseInfo.m_Option.O_Shield);
		App->m_ChrDB.CollectMsg("FIREATTRREGIST", m_BaseInfo.m_Option.O_FireAttr);
		App->m_ChrDB.CollectMsg("WATERATTRREGIST", m_BaseInfo.m_Option.O_WaterAttr);
		App->m_ChrDB.CollectMsg("TREEATTRREGIST", m_BaseInfo.m_Option.O_TreeAttr);
		App->m_ChrDB.CollectMsg("GOLDATTRREGIST", m_BaseInfo.m_Option.O_GoldAttr);
		App->m_ChrDB.CollectMsg("EARTHATTRREGIST", m_BaseInfo.m_Option.O_EarthAttr);
		App->m_ChrDB.CollectMsg("PHYATTACK", m_BaseInfo.m_Option.O_Phyattack);
		App->m_ChrDB.CollectMsg("CRITICAL", m_BaseInfo.m_Option.O_Critical);
		App->m_ChrDB.CollectMsg("FIREATTRATTACK", m_BaseInfo.m_Option.O_FireAttack);
		App->m_ChrDB.CollectMsg("WATERATTRATTACK", m_BaseInfo.m_Option.O_WaterAttack);
		App->m_ChrDB.CollectMsg("TREEATTRATTACK", m_BaseInfo.m_Option.O_TreeAttack);
		App->m_ChrDB.CollectMsg("GOLDATTRATTACK", m_BaseInfo.m_Option.O_GoldAttack);
		App->m_ChrDB.CollectMsg("EARTHATTRATTACK", m_BaseInfo.m_Option.O_EarthAttack);
		App->m_ChrDB.CollectMsg("PHYDEFENSE", m_BaseInfo.m_Option.O_Phydefense);
		App->m_ChrDB.RecordMoveNext();
	}
	App->m_ChrDB.CloseRecord();
}
//将结构数据写入绿化编辑框   by:OpU_Eye  QQ:418914674
void ItemManager::SetItemRareInfo()
{
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_GenGol);
	SetDlgItemText(IDC_EDIT_GENGOL_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_MinChub);
	SetDlgItemText(IDC_EDIT_MINCHUB_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Cheryuk);
	SetDlgItemText(IDC_EDIT_CHERYUK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Simmek);
	SetDlgItemText(IDC_EDIT_SIMMEK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Life);
	SetDlgItemText(IDC_EDIT_LIFE_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Shield);
	SetDlgItemText(IDC_EDIT_NAERYUK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_NaeRyuk);
	SetDlgItemText(IDC_EDIT_SHIELD_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_FireAttr);
	SetDlgItemText(IDC_EDIT_FREGIST_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_WaterAttr);
	SetDlgItemText(IDC_EDIT_WREGIST_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_TreeAttr);
	SetDlgItemText(IDC_EDIT_TREGIST_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_GoldAttr);
	SetDlgItemText(IDC_EDIT_GREGIST_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_EarthAttr);
	SetDlgItemText(IDC_EDIT_EREGIST_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Phyattack);
	SetDlgItemText(IDC_EDIT_PHYATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_FireAttack);
	SetDlgItemText(IDC_EDIT_FATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_WaterAttack);
	SetDlgItemText(IDC_EDIT_WATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_TreeAttack);
	SetDlgItemText(IDC_EDIT_TATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_GoldAttack);
	SetDlgItemText(IDC_EDIT_GATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_EarthAttack);
	SetDlgItemText(IDC_EDIT_EATTACK_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Rare.R_Phydefense);
	SetDlgItemText(IDC_EDIT_PHYDEFENSE_RARE, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.DBGrow);
	SetDlgItemText(IDC_EDIT_GROW, App->TempStr);
}
//将结构数据写入强化编辑框   by:OpU_Eye  QQ:418914674
void ItemManager::SetItemOptionInfo()
{
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_GenGol);
	SetDlgItemText(IDC_EDIT_GENGOL_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_MinChub);
	SetDlgItemText(IDC_EDIT_MINCHUB_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Cheryuk);
	SetDlgItemText(IDC_EDIT_CHERYUK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Simmek);
	SetDlgItemText(IDC_EDIT_SIMMEK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Life);
	SetDlgItemText(IDC_EDIT_LIFE_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Shield);
	SetDlgItemText(IDC_EDIT_NAERYUK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_NaeRyuk);
	SetDlgItemText(IDC_EDIT_SHIELD_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_FireAttr);
	SetDlgItemText(IDC_EDIT_FREGIST_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_WaterAttr);
	SetDlgItemText(IDC_EDIT_WREGIST_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_TreeAttr);
	SetDlgItemText(IDC_EDIT_TREGIST_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_GoldAttr);
	SetDlgItemText(IDC_EDIT_GREGIST_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_EarthAttr);
	SetDlgItemText(IDC_EDIT_EREGIST_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Phyattack);
	SetDlgItemText(IDC_EDIT_PHYATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Critical);
	SetDlgItemText(IDC_EDIT_CRITICAL_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_FireAttack);
	SetDlgItemText(IDC_EDIT_FATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_WaterAttack);
	SetDlgItemText(IDC_EDIT_WATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_TreeAttack);
	SetDlgItemText(IDC_EDIT_TATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_GoldAttack);
	SetDlgItemText(IDC_EDIT_GATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_EarthAttack);
	SetDlgItemText(IDC_EDIT_EATTACK_OPTION, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.m_Option.O_Phydefense);
	SetDlgItemText(IDC_EDIT_PHYDEFENSE_OPTION, App->TempStr);
}
//获取物品绿化编辑框内容到结构   by:OpU_Eye  QQ:418914674
void ItemManager::ReadRareInfo()
{
	GetDlgItemText(IDC_EDIT_GENGOL_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_GenGol = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_MINCHUB_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_MinChub = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_CHERYUK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Cheryuk = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_SIMMEK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Simmek = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_LIFE_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Life = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_NAERYUK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Shield = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_SHIELD_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_NaeRyuk = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_FREGIST_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_FireAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_WREGIST_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_WaterAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_TREGIST_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_TreeAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_GREGIST_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_GoldAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_EREGIST_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_EarthAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_PHYATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Phyattack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_FATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_FireAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_WATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_WaterAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_TATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_TreeAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_GATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_GoldAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_EATTACK_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_EarthAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_PHYDEFENSE_RARE, App->TempStr);
	m_BaseInfo.m_Rare.R_Phydefense = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_GROW, App->TempStr);
	m_BaseInfo.ReadGrow = _ttoi(App->TempStr);
}
//获取物品强化编辑框内容到结构   by:OpU_Eye  QQ:418914674
void ItemManager::ReadOptionInfo()
{
	GetDlgItemText(IDC_EDIT_GENGOL_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_GenGol = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_MINCHUB_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_MinChub = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_CHERYUK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Cheryuk = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_SIMMEK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Simmek = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_LIFE_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Life = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_NAERYUK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_NaeRyuk = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_SHIELD_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Shield = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_FREGIST_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_FireAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_WREGIST_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_WaterAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_TREGIST_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_TreeAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_GREGIST_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_GoldAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_EREGIST_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_EarthAttr = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_PHYATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Phyattack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_CRITICAL_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Critical = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_FATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_FireAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_WATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_WaterAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_TATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_TreeAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_GATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_GoldAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_EATTACK_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_EarthAttack = _ttoi(App->TempStr);

	GetDlgItemText(IDC_EDIT_PHYDEFENSE_OPTION, App->TempStr);
	m_BaseInfo.m_Option.O_Phydefense = _ttoi(App->TempStr);
}
//清空编辑框内容   by:OpU_Eye  QQ:418914674
void ItemManager::Clear()
{
	App->TempStr.Format(_T("%s"), _T(""));
	SetDlgItemText(IDC_EDIT_GENGOL_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_MINCHUB_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_CHERYUK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_SIMMEK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_LIFE_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_NAERYUK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_SHIELD_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_FREGIST_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_WREGIST_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_TREGIST_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_GREGIST_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_EREGIST_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_PHYATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_CRITICAL_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_FATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_WATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_TATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_GATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_EATTACK_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_PHYDEFENSE_OPTION, App->TempStr);
	SetDlgItemText(IDC_EDIT_GENGOL_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_MINCHUB_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_CHERYUK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_SIMMEK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_LIFE_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_NAERYUK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_SHIELD_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_FREGIST_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_WREGIST_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_TREGIST_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_GREGIST_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_EREGIST_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_PHYATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_FATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_WATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_TATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_GATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_EATTACK_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_PHYDEFENSE_RARE, App->TempStr);
	SetDlgItemText(IDC_EDIT_GROW, App->TempStr);
}
//初始化物品强化信息结构(强化和绿化公用一个结构，所以每次查询完都要清理)   by:OpU_Eye  QQ:418914674
void ItemManager::ClearOption()
{
	m_BaseInfo.m_Option.O_GenGol = 0;
	m_BaseInfo.m_Option.O_MinChub = 0;
	m_BaseInfo.m_Option.O_Cheryuk = 0;
	m_BaseInfo.m_Option.O_Simmek = 0;
	m_BaseInfo.m_Option.O_Life = 0;
	m_BaseInfo.m_Option.O_Shield = 0;
	m_BaseInfo.m_Option.O_NaeRyuk = 0;
	m_BaseInfo.m_Option.O_FireAttr = 0;
	m_BaseInfo.m_Option.O_WaterAttr = 0;
	m_BaseInfo.m_Option.O_TreeAttr = 0;
	m_BaseInfo.m_Option.O_GoldAttr = 0;
	m_BaseInfo.m_Option.O_EarthAttr = 0;
	m_BaseInfo.m_Option.O_Phyattack = 0;
	m_BaseInfo.m_Option.O_Critical = 0;
	m_BaseInfo.m_Option.O_FireAttack = 0;
	m_BaseInfo.m_Option.O_WaterAttack = 0;
	m_BaseInfo.m_Option.O_TreeAttack = 0;
	m_BaseInfo.m_Option.O_GoldAttack = 0;
	m_BaseInfo.m_Option.O_EarthAttack = 0;
	m_BaseInfo.m_Option.O_Phydefense = 0;
	m_BaseInfo.m_Rare.R_GenGol = 0;
	m_BaseInfo.m_Rare.R_MinChub = 0;
	m_BaseInfo.m_Rare.R_Cheryuk = 0;
	m_BaseInfo.m_Rare.R_Simmek = 0;
	m_BaseInfo.m_Rare.R_Life = 0;
	m_BaseInfo.m_Rare.R_Shield = 0;
	m_BaseInfo.m_Rare.R_NaeRyuk = 0;
	m_BaseInfo.m_Rare.R_FireAttr = 0;
	m_BaseInfo.m_Rare.R_WaterAttr = 0;
	m_BaseInfo.m_Rare.R_TreeAttr = 0;
	m_BaseInfo.m_Rare.R_GoldAttr = 0;
	m_BaseInfo.m_Rare.R_EarthAttr = 0;
	m_BaseInfo.m_Rare.R_Phyattack = 0;
	m_BaseInfo.m_Rare.R_FireAttack = 0;
	m_BaseInfo.m_Rare.R_WaterAttack = 0;
	m_BaseInfo.m_Rare.R_TreeAttack = 0;
	m_BaseInfo.m_Rare.R_GoldAttack = 0;
	m_BaseInfo.m_Rare.R_EarthAttack = 0;
	m_BaseInfo.m_Rare.R_Phydefense = 0;
}
//初始化物品基本信息结构(强化和绿化公用一个结构，所以每次查询完都要清理)   by:OpU_Eye  QQ:418914674
void ItemManager::ClearBaseInfo()
{
	m_BaseInfo.CharID = 0;
	m_BaseInfo.DBID = 0;
	m_BaseInfo.DBGrow = 0;
	m_BaseInfo.ReadGrow = 0;
	m_BaseInfo.ItemCount = 0;
	m_BaseInfo.ItemID = 0;
	m_BaseInfo.Static = 0;
	m_BaseInfo.ItemPoint = 0;
	m_BaseInfo.StaticName.Format(_T("%s"), _T("未知"));
	m_BaseInfo.ItemName.Format(_T("%s"), _T("未知"));
}
//将查询结果添加到列表   by:OpU_Eye  QQ:418914674
void ItemManager::SetItemInfoList(int i)
{
	if (m_BaseInfo.ItemPoint >= 80 && m_BaseInfo.ItemPoint <= 89 ||
		m_BaseInfo.ItemCount == 0|| m_BaseInfo.ItemCount>100||
		m_BaseInfo.ItemKind>= 2049&& m_BaseInfo.ItemKind<= 2062)
	{
		m_BaseInfo.ItemCount = 1;
	}
	App->TempStr.Format(_T("%d"), i);
	App->TempStr.Format(_T("%d"), m_BaseInfo.CharID);
	m_BaseList->InsertItem(i, App->TempStr);
	m_BaseList->SetItemText(i, 1, m_BaseInfo.ItemName);
	App->TempStr.Format(_T("%d"), m_BaseInfo.DBID);
	m_BaseList->SetItemText(i, 2, App->TempStr);
	App->TempStr.Format(_T("%d"), m_BaseInfo.ItemCount);
	m_BaseList->SetItemText(i, 3, App->TempStr);
	m_BaseList->SetItemText(i, 4, m_BaseInfo.StaticName);
	App->TempStr.Format(_T("%d"), m_BaseInfo.DBGrow);
	m_BaseList->SetItemText(i, 5, App->TempStr);
}
//物品名查询按钮消息响应函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemName()
{
	if (!GetText())return;
	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//物品编号查询按钮消息响应函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemNum()
{
	if (!GetText())return;
	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//角色名查询按钮消息响应函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemChr()
{
	if (!GetText())return;

	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//当用户点击列表内物品执行物品信息获取   by:OpU_Eye  QQ:418914674
void ItemManager::OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_BaseList->GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		int count = 0,count1=0;
		int nItem = m_BaseList->GetNextSelectedItem(pos);
		//选中物品获取物品数量  by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 3);
		SetDlgItemText(IDC_STATIC_ITEMNUM, App->TempStr);
		//获取物品数据库编号  by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 2);
		m_BaseInfo.DBID = _ttoi(App->TempStr);
		//物品名显示   by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 1);
		m_color = RGB(0, 255, 0);
		m_pStaticN = (CStatic*)GetDlgItem(IDC_STATIC_ITEMNAME);
		m_pStaticN->SetWindowTextW(App->TempStr);
		m_pStaticN->RedrawWindow();
		//查询是否存在绿化选项，如果存在则取出绿化信息  by:OpU_Eye  QQ:418914674
		App->TempStr.Format(_T("SELECT COUNT(ITEM_DBIDX) AS RCount FROM TB_ITEM_RARE WHERE ITEM_DBIDX=%d"), m_BaseInfo.DBID);
		if (App->m_ChrDB.SelectSQL(App->TempStr))
		{
			App->m_ChrDB.CollectMsg("RCount", count);
			if (count > 0)
			{
				App->TempStr.Format(_T("是"));
				m_pStaticR = (CStatic*)GetDlgItem(IDC_STATIC_ISRARE);
				m_pStaticR->SetWindowTextW(App->TempStr);
				m_pStaticR->RedrawWindow();
				App->SelectItemRareInfo(m_BaseInfo.DBID);
				GetItemRareInfo();
				App->TempStr.Format(_T("SELECT ITEM_Grow FROM TB_ITEM WHERE ITEM_DBIDX=%d"), m_BaseInfo.DBID);
				App->m_ChrDB.SelectSQL(App->TempStr);
				App->m_ChrDB.CollectMsg("ITEM_Grow", m_BaseInfo.DBGrow);
				App->m_ChrDB.CloseRecord();
				SetItemRareInfo();
				ClearOption();
			}
			else
			{
				App->TempStr.Format(_T("否"));
				m_color = RGB(255, 0, 0);
				m_pStaticR = (CStatic*)GetDlgItem(IDC_STATIC_ISRARE);
				m_pStaticR->SetWindowTextW(App->TempStr);
				m_pStaticR->RedrawWindow();
				ClearOption();
				SetItemRareInfo();
			}
			App->m_ChrDB.CloseRecord();
		}
		//查询是否存在强化选项，如果存在则取出强化信息  by:OpU_Eye  QQ:418914674
		App->TempStr.Format(_T("SELECT COUNT(ITEM_DBIDX) AS OCount FROM TB_ITEM_OPTION WHERE ITEM_DBIDX=%d"), m_BaseInfo.DBID);
		if (App->m_ChrDB.SelectSQL(App->TempStr))
		{
			App->m_ChrDB.CollectMsg("OCount", count1);
			if (count1 > 0)
			{
				App->TempStr.Format(_T("是"));
				m_pStaticO = (CStatic*)GetDlgItem(IDC_STATIC_ISOPTION);
				m_pStaticO->SetWindowTextW(App->TempStr);
				m_pStaticO->RedrawWindow();
				App->SelectItemOptionInfo(m_BaseInfo.DBID);
				GetItemOptionInfo();
				SetItemOptionInfo();
				ClearOption();
			}
			else
			{
				App->TempStr.Format(_T("否"));
				m_color = RGB(255, 0, 0);
				m_pStaticO = (CStatic*)GetDlgItem(IDC_STATIC_ISOPTION);
				m_pStaticO->SetWindowTextW(App->TempStr);
				m_pStaticO->RedrawWindow();
				ClearOption();
				SetItemOptionInfo();
			}
			App->m_ChrDB.CloseRecord();
		}

	}
	*pResult = 0;
}
//修改绿化信息按钮单击消息响应函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonRare()
{
	ReadRareInfo();
	if (m_BaseInfo.ReadGrow != m_BaseInfo.DBID)
	{
		long count;
		App->TempStr.Format(_T("UPDATE TB_ITEM SET ITEM_Grow =%d WHERE ITEM_DBIDX = %d"), m_BaseInfo.ReadGrow, m_BaseInfo.DBID);
		if (!App->m_ChrDB.ExecuteSQL(App->TempStr, count))
		{
			::MessageBox(this->m_hWnd,_T("物品成长值修改失败"),_T("物品成长修改"),MB_OK);
		}
	}
	App->UpdataItemRareInfo(m_BaseInfo.DBID, m_BaseInfo.m_Rare.R_GenGol, m_BaseInfo.m_Rare.R_MinChub, m_BaseInfo.m_Rare.R_Cheryuk, m_BaseInfo.m_Rare.R_Simmek, m_BaseInfo.m_Rare.R_Life, m_BaseInfo.m_Rare.R_NaeRyuk, m_BaseInfo.m_Rare.R_Shield, m_BaseInfo.m_Rare.R_FireAttr, m_BaseInfo.m_Rare.R_WaterAttr, m_BaseInfo.m_Rare.R_TreeAttr, m_BaseInfo.m_Rare.R_GoldAttr, m_BaseInfo.m_Rare.R_EarthAttr, m_BaseInfo.m_Rare.R_Phyattack, m_BaseInfo.m_Rare.R_FireAttack, m_BaseInfo.m_Rare.R_WaterAttack, m_BaseInfo.m_Rare.R_TreeAttack, m_BaseInfo.m_Rare.R_GoldAttack, m_BaseInfo.m_Rare.R_EarthAttack, m_BaseInfo.m_Rare.R_Phydefense);
	Clear();
	ClearOption();
}
//修改强化信息按钮单击消息响应函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonOption()
{
	ReadOptionInfo();
	App->UpdataItemOptionInfo(m_BaseInfo.DBID, m_BaseInfo.m_Option.O_GenGol, m_BaseInfo.m_Option.O_MinChub, m_BaseInfo.m_Option.O_Cheryuk, m_BaseInfo.m_Option.O_Simmek, m_BaseInfo.m_Option.O_Life, m_BaseInfo.m_Option.O_NaeRyuk, m_BaseInfo.m_Option.O_Shield, m_BaseInfo.m_Option.O_FireAttr, m_BaseInfo.m_Option.O_WaterAttr, m_BaseInfo.m_Option.O_TreeAttr, m_BaseInfo.m_Option.O_GoldAttr, m_BaseInfo.m_Option.O_EarthAttr, m_BaseInfo.m_Option.O_Phyattack, m_BaseInfo.m_Option.O_Critical, m_BaseInfo.m_Option.O_FireAttack, m_BaseInfo.m_Option.O_WaterAttack, m_BaseInfo.m_Option.O_TreeAttack, m_BaseInfo.m_Option.O_GoldAttack, m_BaseInfo.m_Option.O_EarthAttack, m_BaseInfo.m_Option.O_Phydefense);
	Clear();
	ClearOption();
}
//列表右键单击消息响应弹出菜单函数   by:OpU_Eye  QQ:418914674
void ItemManager::OnNMRclickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_DELETE_ITEM);
	CMenu *pContextMenu = menu.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}
//设置字体和背景颜色消息响应   by:OpU_Eye  QQ:418914674
HBRUSH ItemManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC_TIP:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,0,0));
			break;
		}
		case IDC_STATIC_ITEMNAME:
		case IDC_STATIC_ISRARE:
		case IDC_STATIC_ISOPTION:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_color);
			break;
		}
		case IDC_STATIC_ITEMNUM:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,0,0));
			break;
		}
		default:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0));
			break;
		}
	}
	return (HBRUSH)m_brush.GetSafeHandle();
}
//右键菜单删除物品   by:OpU_Eye  QQ:418914674
void ItemManager::On32774()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("删除操作无法恢复！确定删除吗？"), _T("物品删除"), MB_OKCANCEL))
	{
		POSITION pos = m_BaseList->GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			int nItem = m_BaseList->GetNextSelectedItem(pos);
			App->TempStr = m_BaseList->GetItemText(nItem, 2);
			m_BaseInfo.DBID = _ttoi(App->TempStr);
			App->DeleteItem(m_BaseInfo.DBID);
			Clear();
			ClearOption();
			m_BaseList->DeleteItem(nItem);
		}
	}
}
//右键菜单删除物品强化信息   by:OpU_Eye  QQ:418914674
void ItemManager::On32773()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("删除操作无法恢复！确定删除吗？"), _T("强化信息删除"), MB_OKCANCEL))
	{
		POSITION pos = m_BaseList->GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			int nItem = m_BaseList->GetNextSelectedItem(pos);
			App->TempStr = m_BaseList->GetItemText(nItem, 2);
			m_BaseInfo.DBID = _ttoi(App->TempStr);
			App->DeleteItemOptionInfo(m_BaseInfo.DBID);
			Clear();
			ClearOption();
		}
	}
}
//右键菜单删除物品绿化信息   by:OpU_Eye  QQ:418914674
void ItemManager::On32772()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("删除操作无法恢复！确定删除吗？"), _T("绿化信息删除"), MB_OKCANCEL))
	{
		POSITION pos = m_BaseList->GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			int nItem = m_BaseList->GetNextSelectedItem(pos);
			App->TempStr = m_BaseList->GetItemText(nItem, 2);
			m_BaseInfo.DBID = _ttoi(App->TempStr);
			App->DeleteItemRareInfo(m_BaseInfo.DBID);
			Clear();
			ClearOption();
		}
	}
}


BOOL ItemManager::PreTranslateMessage(MSG* pMsg)
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
