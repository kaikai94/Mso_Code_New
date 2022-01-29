/***��Ʒ����Ի����๦��ʵ��   by:OpU_Eye  QQ:418914674***/
#include "stdafx.h"
#include "GMManagerTool.h"
#include "ItemManager.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(ItemManager, CDialogEx)
//Ĭ�Ϲ���   by:OpU_Eye  QQ:418914674
ItemManager::ItemManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}
//Ĭ������   by:OpU_Eye  QQ:418914674
ItemManager::~ItemManager()
{

}
//��ʼ���Ի���   by:OpU_Eye  QQ:418914674
void ItemManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_pStaticT = (CStatic*)GetDlgItem(IDC_STATIC_TIP);
	m_pStaticT->RedrawWindow();
	//������ɫ��ʼ�� 	by:OpU_Eye QQ:418914674
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	//list�ؼ���ʼ��	by:OpU_Eye QQ:418914674
	m_BaseList = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	CRect rect1;
	m_BaseList->GetClientRect(&rect1);

	m_BaseList->SetExtendedStyle(m_BaseList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_BaseList->InsertColumn(0, _T("��ɫID"), LVCFMT_CENTER, rect1.Width() / 5, 0);
	m_BaseList->InsertColumn(1, _T("��Ʒ����"), LVCFMT_CENTER, rect1.Width() / 4, 1);
	m_BaseList->InsertColumn(2, _T("���ݿ���"), LVCFMT_CENTER, rect1.Width() / 3, 2);
	m_BaseList->InsertColumn(3, _T("��Ʒ����"), LVCFMT_CENTER, rect1.Width() / 4, 3);
	m_BaseList->InsertColumn(4, _T("����״̬"), LVCFMT_CENTER, rect1.Width() / 4, 4);
	m_BaseList->InsertColumn(5, _T("װ���ɳ�ֵ"), LVCFMT_CENTER, rect1.Width() / 3, 5);
	//ȫ�ֱ�����ʼ��	by:OpU_Eye QQ:418914674
	App = (CGMManagerToolApp *)AfxGetApp();
	//App2 = (CGMManagerToolApp *)AfxGetApp();

}

//��Ϣӳ���   by:OpU_Eye  QQ:418914674
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
//��ȡ��ѯ��Ϣ�������ѯ�߼�   by:OpU_Eye  QQ:418914674
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
		::MessageBox(NULL, _T("ÿ��ֻ������һ����ѯ����"), _T("��ѯ��������"), MB_OK);
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
			App->TempStr.Format(_T("�����ڽ�ɫ��Ϊ[%s]�Ľ�ɫ"), CharName);
			::MessageBox(this->m_hWnd, App->TempStr, _T("��ɫ����ѯ��Ʒ"), MB_OK);
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
		::MessageBox(NULL, _T("�������ѯ��Ϣ"), _T("��ѯ��������"), MB_OK);
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
			App->TempStr.Format(_T("�����ڵ���Ʒ"));
			::MessageBox(this->m_hWnd, App->TempStr, _T("��ѯ��Ʒ"), MB_OK);
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

//��ƷIDת����Ʒ����   by:OpU_Eye  QQ:418914674
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
//��Ʒ����״̬���->����ת��   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemStatic()
{
	m_BaseInfo.StaticName = L"δ����";
}
//��Ʒ����ת��Ϊ��Ʒ���   by:OpU_Eye  QQ:418914674
void ItemManager::GetItemNum()
{
	if (m_BaseInfo.ItemName != "δ֪")
	{
		std::map<int, ITEMINFO>::iterator iter;
		for (iter = App->m_ChangeUseConfig.begin(); iter != App->m_ChangeUseConfig.end(); ++iter)
		{
			if (m_BaseInfo.ItemName == iter->second.Name.c_str())
			{
				m_BaseInfo.ItemID = iter->second.Num;
				m_BaseInfo.ItemName.Format(_T("δ֪"));
				break;
			}
		}
	}
}
//��ȡ���ݿ�����for��ɫID   by:OpU_Eye  QQ:418914674
void ItemManager::GetCharId()
{
	App->m_ChrDB.CollectMsg("CHARACTER_IDX", m_BaseInfo.CharID);
	App->m_ChrDB.CloseRecord();
}
//��ȡ���ݿ���������Ʒ������Ϣ�ṹ   by:OpU_Eye  QQ:418914674
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
//��ȡ���ݿ��ѯ��������ݵ���Ʒǿ����Ϣ�ṹ   by:OpU_Eye  QQ:418914674
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
//���ṹ����д���̻��༭��   by:OpU_Eye  QQ:418914674
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
//���ṹ����д��ǿ���༭��   by:OpU_Eye  QQ:418914674
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
//��ȡ��Ʒ�̻��༭�����ݵ��ṹ   by:OpU_Eye  QQ:418914674
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
//��ȡ��Ʒǿ���༭�����ݵ��ṹ   by:OpU_Eye  QQ:418914674
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
//��ձ༭������   by:OpU_Eye  QQ:418914674
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
//��ʼ����Ʒǿ����Ϣ�ṹ(ǿ�����̻�����һ���ṹ������ÿ�β�ѯ�궼Ҫ����)   by:OpU_Eye  QQ:418914674
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
//��ʼ����Ʒ������Ϣ�ṹ(ǿ�����̻�����һ���ṹ������ÿ�β�ѯ�궼Ҫ����)   by:OpU_Eye  QQ:418914674
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
	m_BaseInfo.StaticName.Format(_T("%s"), _T("δ֪"));
	m_BaseInfo.ItemName.Format(_T("%s"), _T("δ֪"));
}
//����ѯ�����ӵ��б�   by:OpU_Eye  QQ:418914674
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
//��Ʒ����ѯ��ť��Ϣ��Ӧ����   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemName()
{
	if (!GetText())return;
	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//��Ʒ��Ų�ѯ��ť��Ϣ��Ӧ����   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemNum()
{
	if (!GetText())return;
	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//��ɫ����ѯ��ť��Ϣ��Ӧ����   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonItemChr()
{
	if (!GetText())return;

	m_BaseList->DeleteAllItems();
	Clear();
	ClearBaseInfo();
	GetItemBaseInfo();
}
//���û�����б�����Ʒִ����Ʒ��Ϣ��ȡ   by:OpU_Eye  QQ:418914674
void ItemManager::OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_BaseList->GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		int count = 0,count1=0;
		int nItem = m_BaseList->GetNextSelectedItem(pos);
		//ѡ����Ʒ��ȡ��Ʒ����  by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 3);
		SetDlgItemText(IDC_STATIC_ITEMNUM, App->TempStr);
		//��ȡ��Ʒ���ݿ���  by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 2);
		m_BaseInfo.DBID = _ttoi(App->TempStr);
		//��Ʒ����ʾ   by:OpU_Eye  QQ:418914674
		App->TempStr = m_BaseList->GetItemText(nItem, 1);
		m_color = RGB(0, 255, 0);
		m_pStaticN = (CStatic*)GetDlgItem(IDC_STATIC_ITEMNAME);
		m_pStaticN->SetWindowTextW(App->TempStr);
		m_pStaticN->RedrawWindow();
		//��ѯ�Ƿ�����̻�ѡ����������ȡ���̻���Ϣ  by:OpU_Eye  QQ:418914674
		App->TempStr.Format(_T("SELECT COUNT(ITEM_DBIDX) AS RCount FROM TB_ITEM_RARE WHERE ITEM_DBIDX=%d"), m_BaseInfo.DBID);
		if (App->m_ChrDB.SelectSQL(App->TempStr))
		{
			App->m_ChrDB.CollectMsg("RCount", count);
			if (count > 0)
			{
				App->TempStr.Format(_T("��"));
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
				App->TempStr.Format(_T("��"));
				m_color = RGB(255, 0, 0);
				m_pStaticR = (CStatic*)GetDlgItem(IDC_STATIC_ISRARE);
				m_pStaticR->SetWindowTextW(App->TempStr);
				m_pStaticR->RedrawWindow();
				ClearOption();
				SetItemRareInfo();
			}
			App->m_ChrDB.CloseRecord();
		}
		//��ѯ�Ƿ����ǿ��ѡ����������ȡ��ǿ����Ϣ  by:OpU_Eye  QQ:418914674
		App->TempStr.Format(_T("SELECT COUNT(ITEM_DBIDX) AS OCount FROM TB_ITEM_OPTION WHERE ITEM_DBIDX=%d"), m_BaseInfo.DBID);
		if (App->m_ChrDB.SelectSQL(App->TempStr))
		{
			App->m_ChrDB.CollectMsg("OCount", count1);
			if (count1 > 0)
			{
				App->TempStr.Format(_T("��"));
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
				App->TempStr.Format(_T("��"));
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
//�޸��̻���Ϣ��ť������Ϣ��Ӧ����   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonRare()
{
	ReadRareInfo();
	if (m_BaseInfo.ReadGrow != m_BaseInfo.DBID)
	{
		long count;
		App->TempStr.Format(_T("UPDATE TB_ITEM SET ITEM_Grow =%d WHERE ITEM_DBIDX = %d"), m_BaseInfo.ReadGrow, m_BaseInfo.DBID);
		if (!App->m_ChrDB.ExecuteSQL(App->TempStr, count))
		{
			::MessageBox(this->m_hWnd,_T("��Ʒ�ɳ�ֵ�޸�ʧ��"),_T("��Ʒ�ɳ��޸�"),MB_OK);
		}
	}
	App->UpdataItemRareInfo(m_BaseInfo.DBID, m_BaseInfo.m_Rare.R_GenGol, m_BaseInfo.m_Rare.R_MinChub, m_BaseInfo.m_Rare.R_Cheryuk, m_BaseInfo.m_Rare.R_Simmek, m_BaseInfo.m_Rare.R_Life, m_BaseInfo.m_Rare.R_NaeRyuk, m_BaseInfo.m_Rare.R_Shield, m_BaseInfo.m_Rare.R_FireAttr, m_BaseInfo.m_Rare.R_WaterAttr, m_BaseInfo.m_Rare.R_TreeAttr, m_BaseInfo.m_Rare.R_GoldAttr, m_BaseInfo.m_Rare.R_EarthAttr, m_BaseInfo.m_Rare.R_Phyattack, m_BaseInfo.m_Rare.R_FireAttack, m_BaseInfo.m_Rare.R_WaterAttack, m_BaseInfo.m_Rare.R_TreeAttack, m_BaseInfo.m_Rare.R_GoldAttack, m_BaseInfo.m_Rare.R_EarthAttack, m_BaseInfo.m_Rare.R_Phydefense);
	Clear();
	ClearOption();
}
//�޸�ǿ����Ϣ��ť������Ϣ��Ӧ����   by:OpU_Eye  QQ:418914674
void ItemManager::OnBnClickedButtonOption()
{
	ReadOptionInfo();
	App->UpdataItemOptionInfo(m_BaseInfo.DBID, m_BaseInfo.m_Option.O_GenGol, m_BaseInfo.m_Option.O_MinChub, m_BaseInfo.m_Option.O_Cheryuk, m_BaseInfo.m_Option.O_Simmek, m_BaseInfo.m_Option.O_Life, m_BaseInfo.m_Option.O_NaeRyuk, m_BaseInfo.m_Option.O_Shield, m_BaseInfo.m_Option.O_FireAttr, m_BaseInfo.m_Option.O_WaterAttr, m_BaseInfo.m_Option.O_TreeAttr, m_BaseInfo.m_Option.O_GoldAttr, m_BaseInfo.m_Option.O_EarthAttr, m_BaseInfo.m_Option.O_Phyattack, m_BaseInfo.m_Option.O_Critical, m_BaseInfo.m_Option.O_FireAttack, m_BaseInfo.m_Option.O_WaterAttack, m_BaseInfo.m_Option.O_TreeAttack, m_BaseInfo.m_Option.O_GoldAttack, m_BaseInfo.m_Option.O_EarthAttack, m_BaseInfo.m_Option.O_Phydefense);
	Clear();
	ClearOption();
}
//�б��Ҽ�������Ϣ��Ӧ�����˵�����   by:OpU_Eye  QQ:418914674
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
//��������ͱ�����ɫ��Ϣ��Ӧ   by:OpU_Eye  QQ:418914674
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
//�Ҽ��˵�ɾ����Ʒ   by:OpU_Eye  QQ:418914674
void ItemManager::On32774()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("ɾ�������޷��ָ���ȷ��ɾ����"), _T("��Ʒɾ��"), MB_OKCANCEL))
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
//�Ҽ��˵�ɾ����Ʒǿ����Ϣ   by:OpU_Eye  QQ:418914674
void ItemManager::On32773()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("ɾ�������޷��ָ���ȷ��ɾ����"), _T("ǿ����Ϣɾ��"), MB_OKCANCEL))
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
//�Ҽ��˵�ɾ����Ʒ�̻���Ϣ   by:OpU_Eye  QQ:418914674
void ItemManager::On32772()
{
	if (IDOK == ::MessageBox(this->m_hWnd, _T("ɾ�������޷��ָ���ȷ��ɾ����"), _T("�̻���Ϣɾ��"), MB_OKCANCEL))
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
