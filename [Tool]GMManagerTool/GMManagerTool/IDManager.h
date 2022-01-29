/***账号管理类声明	by:OpU_Eye QQ:418914674***/
#pragma once
#include "afxcmn.h"


// IDManager 对话框

class IDManager : public CDialogEx
{
	DECLARE_DYNAMIC(IDManager)
public:
	/**对话框相关变量	by:OpU_Eye QQ:418914674**/
	CListCtrl* m_IDList;	//账号List控件变量
	CListCtrl* m_CharList;	//角色List控件变量
	CBrush m_brush;			//画刷修改背景颜色	
	COLORREF m_color;		//修改静态字体颜色
	COLORREF m_ResultColor;
	CStatic* m_pStaticCheck;
	CStatic* m_pStaticTmp;
	CStatic* m_pStatic;
	/**数据库相关变量	by:OpU_Eye QQ:418914674**/
	CGMManagerToolApp *App;	//多对话框全局变量简单实现
	CString m_DBUser;		//数据库账号
	CString m_DBPassWord;	//数据库密码
	CString m_DBip;			//数据库地址
	
	/**账号相关变量	by:OpU_Eye QQ:418914674**/
	int   m_propid;			//账号propid
	CString  m_UserId;		//游戏账号
	CString  m_UserPassWord;//游戏密码
	CString  m_IP;			//注册IP
	CString  m_Email;		//密保邮箱
	CString  m_Tel;			//密保手机
	int   m_UserLevel;		//账号权限
	long  m_GoldMoney;		//元宝	
	long  m_MallMoney;		//泡点	
	int   m_Id_Idx;			//ID索引

	/**账号角色相关变量	by:OpU_Eye QQ:418914674**/
	CString m_CharacterName;	//角色名
	int m_CharacterLevel;		//角色等级
	int m_CharacterRest;		//角色转生数
	int m_CharacterMoney;		//角色游戏币
	int m_CharacterIdx;			//角色ID

	/**查询条件相关变量	by:OpU_Eye QQ:418914674**/
	long long m_Gold;		//元宝
	long long m_Mall;		//泡点
	CString m_IPSel;	//注册IP
	int m_Level;	//账号权限

public:
	IDManager(CWnd* pParent = NULL);   // 默认构造	by:OpU_Eye QQ:418914674
	virtual ~IDManager();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif
	/**功能实现函数		by:OpU_Eye QQ:418914674**/
	void InitIDInfo();
	void GetUserInfoByID();
	void GetUserInfoByGold();
	void GetUserInfoByMall();
	void GetUserInfoByLevel();
	void GetUserInfoByIP();
	void GetChDBInfo();
	void SetEdit();
	void SetIdList(int i);
	BOOL ConnDB();
	BOOL GetInfo();
	void InitEdit();
	void SetTextColor(int cl);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	/**消息映射函数		by:OpU_Eye QQ:418914674**/
	afx_msg void OnBnClickedButtonConndb();
	afx_msg void OnBnClickedButtonIdSelect();
	afx_msg void OnBnClickedButtonGoldSel();
	afx_msg void OnBnClickedButtonMallSel();
	afx_msg void OnBnClickedButtonLevelSel();
	afx_msg void OnBnClickedButtonIpSel();
	afx_msg void OnLvnItemchangedListId(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonNewId();
	afx_msg void OnBnClickedButtonGetinfo();
	afx_msg void OnBnClickedButtonAddgold();
	afx_msg void OnBnClickedButtonClgold();
	afx_msg void OnBnClickedButtonDisableid();
	afx_msg void OnBnClickedButtonNodisableid();
	afx_msg void OnBnClickedButtonGmpower();
	afx_msg void OnBnClickedButtonUserpower();
	afx_msg void OnBnClickedButtonClmall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
