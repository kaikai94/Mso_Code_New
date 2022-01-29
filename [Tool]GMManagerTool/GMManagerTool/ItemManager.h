#pragma once

//强化/绿化信息结构	by:OpU_Eye QQ:418914674
struct Option
{
	int O_GenGol;
	int O_MinChub;
	int O_Cheryuk;
	int O_Simmek;
	int O_Life;
	int O_NaeRyuk;
	int O_Shield;
	int O_FireAttr;
	int O_WaterAttr;
	int O_TreeAttr;
	int O_GoldAttr;
	int O_EarthAttr;
	int O_Phyattack;
	int O_Critical;
	int O_FireAttack;
	int O_WaterAttack;
	int O_TreeAttack;
	int O_GoldAttack;
	int O_EarthAttack;
	int O_Phydefense;
	Option()
	{
		O_GenGol = 0;
		O_MinChub = 0;
		O_Cheryuk = 0;
		O_Simmek = 0;
		O_Life = 0;
		O_NaeRyuk = 0;
		O_Shield = 0;
		O_FireAttr = 0;
		O_WaterAttr = 0;
		O_TreeAttr = 0;
		O_GoldAttr = 0;
		O_EarthAttr = 0;
		O_Phyattack = 0;
		O_Critical = 0;
		O_FireAttack = 0;
		O_WaterAttack = 0;
		O_TreeAttack = 0;
		O_GoldAttack = 0;
		O_EarthAttack = 0;
		O_Phydefense = 0;
	}
};
struct Rare
{
	int R_GenGol;
	int R_MinChub;
	int R_Cheryuk;
	int R_Simmek;
	int R_Life;
	int R_NaeRyuk;
	int R_Shield;
	int R_FireAttr;
	int R_WaterAttr;
	int R_TreeAttr;
	int R_GoldAttr;
	int R_EarthAttr;
	int R_Phyattack;
	int R_FireAttack;
	int R_WaterAttack;
	int R_TreeAttack;
	int R_GoldAttack;
	int R_EarthAttack;
	int R_Phydefense;
	Rare()
	{
		R_GenGol = 0;
		R_MinChub = 0;
		R_Cheryuk = 0;
		R_Simmek = 0;
		R_Life = 0;
		R_NaeRyuk = 0;
		R_Shield = 0;
		R_FireAttr = 0;
		R_WaterAttr = 0;
		R_TreeAttr = 0;
		R_GoldAttr = 0;
		R_EarthAttr = 0;
		R_Phyattack = 0;
		R_FireAttack = 0;
		R_WaterAttack = 0;
		R_TreeAttack = 0;
		R_GoldAttack = 0;
		R_EarthAttack = 0;
		R_Phydefense = 0;
	}
};
//物品基本信息结构	by:OpU_Eye QQ:418914674
struct BaseInfo
{
	int CharID;
	int DBID;
	DWORD ItemKind;
	int ItemID;
	int ItemPoint;
	CString ItemName;
	CString StaticName;
	int Static;
	long DBGrow;
	long ReadGrow;
	int ItemCount;
	Option m_Option;
	Rare	m_Rare;
	BaseInfo()
	{
		CharID = 0;
		DBID = 0;
		ItemID = 0;
		ItemCount = 0;
		ItemKind = 0;
		ItemName.Format(_T("%s"),_T("未知"));
		StaticName.Format(_T("%s"), _T("未知"));
		ItemPoint = 0;
		Static = 0;
		DBGrow = 0;
		ReadGrow = 0;
	}
};

class ItemManager : public CDialogEx
{
	DECLARE_DYNAMIC(ItemManager)
	/**对话框相关变量	by:OpU_Eye QQ:418914674**/
	CListCtrl*  m_BaseList;
	CGMManagerToolApp *App;
	CGMManagerToolApp *App2;
	CBrush m_brush;		
	COLORREF m_color;
	CStatic* m_pStaticR;
	CStatic* m_pStaticO; 
	CStatic* m_pStaticN;
	CStatic* m_pStaticT;
	/**物品查询相关变量		by:OpU_Eye QQ:418914674**/
	BaseInfo	m_BaseInfo;
	static	int	m_DBidx;
	static	int	m_ItemIdx;
public:
	ItemManager(CWnd* pParent = NULL);   // 默认构造函数	by:OpU_Eye QQ:418914674
	virtual ~ItemManager();
	/**功能实现函数声明	by:OpU_Eye QQ:418914674**/
	BOOL GetText();
	void GetItemName();
	void GetItemNum();
	void GetItemStatic();
	void GetCharId();
	void GetItemBaseInfo();
	void GetItemRareInfo();
	void GetItemOptionInfo();
	void SetItemRareInfo();
	void SetItemOptionInfo();
	void ReadRareInfo();
	void ReadOptionInfo();
	void Clear();
	void ClearOption();
	void ClearBaseInfo();
	void SetItemInfoList(int i);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	/**消息映射函数	by:OpU_Eye QQ:418914674**/
	afx_msg void OnBnClickedButtonItemName();
	afx_msg void OnBnClickedButtonItemNum();
	afx_msg void OnBnClickedButtonItemChr();
	afx_msg void OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRare();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnNMRclickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void On32774();
	afx_msg void On32773();
	afx_msg void On32772();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
