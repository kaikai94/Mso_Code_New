/***角色管理类声明	by:OpU_Eye QQ:418914674***/
#pragma once

/*角色穿戴装备信息结构	by:OpU_Eye QQ:418914674*/
struct EquipIdx
{
	long m_Hat;
	long m_Weapon;
	long m_Dress;
	long m_Shoes;
	long m_Ring1;
	long m_Ring2;
	long m_Cape;
	long m_Necklace;
	long m_Armlet;
	long m_Belt;
	EquipIdx()
	{
		m_Armlet = 0;
		m_Belt = 0;
		m_Cape = 0;
		m_Dress = 0;
		m_Hat = 0;
		m_Necklace = 0;
		m_Ring1 = 0;
		m_Ring2 = 0;
		m_Shoes = 0;
		m_Weapon = 0;
	}
};
/*装备名结构	by:OpU_Eye QQ:418914674*/
struct EquipName
{
	CString m_Hat;
	CString m_Weapon;
	CString m_Dress;
	CString m_Shoes;
	CString m_Ring1;
	CString m_Ring2;
	CString m_Cape;
	CString m_Necklace;
	CString m_Armlet;
	CString m_Belt;
	EquipName()
	{
		m_Armlet.Format(_T("%s"),_T("无"));
		m_Belt.Format(_T("%s"),_T("无"));
		m_Cape.Format(_T("%s"),_T("无"));
		m_Dress.Format(_T("%s"),_T("无"));
		m_Hat.Format(_T("%s"),_T("无"));
		m_Necklace.Format(_T("%s"),_T("无"));
		m_Ring1.Format(_T("%s"),_T("无"));
		m_Ring2.Format(_T("%s"),_T("无"));
		m_Shoes.Format(_T("%s"),_T("无"));
		m_Weapon.Format(_T("%s"),_T("无"));
	}
};

class CharacterManager : public CDialogEx
{
	DECLARE_DYNAMIC(CharacterManager)
public:
	CGMManagerToolApp *App;
	/*角色信息变量	by:OpU_Eye QQ:418914674*/
	int			m_CharaIdx;
	int			m_USER_IDX;
	int			m_GoldMoney;
	int			m_GenGoal;
	int			m_Dex;
	int			m_Sta;
	int			m_Simmak;
	int			m_Fame;
	CString		m_Name;
	int			m_Grade;
	int			m_MaxGrade;
	int			m_GradePoint;
	long		m_Money;
	int			m_Map;
	CString		m_MapName;
	EquipIdx	m_equipIdx;
	int			m_State;
	CString		m_StateName;
	long		m_AbilityExp;
	int			m_BadFame;
	CString		m_NickName;
	int			m_Reset;
	CString		m_MarryName;
	EquipName	m_EquipName;
	CListCtrl*	m_ItemList;
	CListCtrl*	m_SelChrList;
	CStatic*	m_pStaticTmp;
	CStatic*	m_pStaticTip;
	
	int		m_CharacterID;
	CString	m_UserID;
	CString	m_CharacterIDX;
	CString	m_CharacterName;
	/*物品信息变量	by:OpU_Eye QQ:418914674*/
	int		m_ItemNum;
	CString m_ItemName;
	int		m_ItemCount;
	int		m_ItemPoint;
	int		m_ItemDBID;
	/*对话框变量	by:OpU_Eye QQ:418914674*/
	CBrush m_brush;
	COLORREF m_color;
public:
	/*功能实现函数声明	by:OpU_Eye QQ:418914674*/
	void GetItemName();
//	void GetItemNum();
	void GetStateName();
	void GetStateNum();
	void GetMapName();
	void GetMapNum();
	BOOL GetText();
	void GetCharacterInfoForList();
	void SetChrList(int i);
	void SetEdit();
	void SetItemList(int i);
	void GetItemInfo();
	void GetChrInfoWithList();
	CharacterManager(CWnd* pParent = NULL);   // 默认构造函数	by:OpU_Eye QQ:418914674
	virtual ~CharacterManager();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	/*消息映射处理函数声明	by:OpU_Eye QQ:418914674*/
	afx_msg void OnBnClickedButtonUserid();
	afx_msg void OnBnClickedButtonId();
	afx_msg void OnBnClickedButtonName();
	afx_msg void OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListChrItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32775();
	afx_msg void On32771();
	void OnBnClickedButtonXg();
	afx_msg void OnLvnKeydownListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
