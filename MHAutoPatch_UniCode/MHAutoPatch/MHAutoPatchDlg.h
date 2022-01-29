
// MHAutoPatchDlg.h : 头文件
//

#pragma once
//#include <atlimage.h>
#define  UPDATAURL ".\\Image\\Autopatch\\AutoPatchURL.bin"
#include "HtmlViewNotice.h"
#include "CheckResourceFile.h"
#include "cTextParsing.h"
#include "GlobalStructDef.h"
#include "afxwin.h"

// CMHAutoPatchDlg 对话框
class CMHAutoPatchDlg : public CDialogEx
{
	TextColor sTextColor[eMaxColor];//[文字颜色][BY:十里坡剑神][QQ:112582793][2019-5-2][20:53:05]
	DlgRect sDlgRect[eMaxRect];		//[控件大小信息][BY:十里坡剑神][QQ:112582793][2019-5-2][20:53:13]
	FontSet sFontSet;				//[字体信息][BY:十里坡剑神][QQ:112582793][2019-5-2][20:53:33]
	CFont m_font;					//[字体][BY:十里坡剑神][QQ:112582793][2019-5-2][20:52:08]
	HBRUSH hbr;						//[背景笔刷][BY:十里坡剑神][QQ:112582793][2019-5-2][20:52:16]
	HBRUSH hbr1;					//[背景笔刷][BY:十里坡剑神][QQ:112582793][2019-5-2][20:52:16]
	HBRUSH hbr2;					//[背景笔刷][BY:十里坡剑神][QQ:112582793][2019-5-2][20:52:16]

	CBitmapButton m_BtnStart;	//[开始按钮][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CBitmapButton m_BtnHomePage;//[主页按钮][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CBitmapButton m_BtnClose;	//[关闭按钮][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	Image* m_bgImag;			//[背景图片][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CButton m_CheckWndMode;		//[窗口模式][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CComboBox m_ComboBox;		//[分辨率选择][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]

	CStatic m_StTip;			//[提示信息][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CStatic m_StDwnlTip;		//[下载信息][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CStatic m_AutoPatchVer;		//[版本号][BY:十里坡剑神][QQ:112582793][2019-5-3][1:18:23]

	bool m_bIsRunClient;		//[更新完成后是否直接进入游戏][BY:十里坡剑神][QQ:112582793][2019-5-2][21:48:33]
	BOOL StopTheard;			//[进程退出控制变量][BY:十里坡剑神][QQ:112582793][2019-5-2][21:36:50]
	size_t m_FileCount;			//[需下载文件数量][BY:十里坡剑神][QQ:112582793][2019-5-2][21:37:32]
	CHtmlViewNotice* mHtmlViewNotice;	//[公告窗口][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CRect	m_HtmlRect;					//[公告窗口大小][BY:十里坡剑神][QQ:112582793][2019-4-30][21:47:17]
	CProgressCtrl m_ProgressDone;		//[总进度条][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]
	CProgressCtrl m_Progress;			//[下载进度条][BY:十里坡剑神][QQ:112582793][2019-4-30][21:45:44]

	ServerInfo m_ServerInfo;				//[更新配置信息][BY:十里坡剑神][QQ:112582793][2019-4-30][21:54:48]
	TCHAR m_CurPath[MAX_PATH];				//[当前运行路径][BY:十里坡剑神][QQ:112582793][2019-4-30][23:29:29]
	CheckResourceFile m_FileCheck;			//[文件校验][BY:十里坡剑神][QQ:112582793][2019-5-1][0:00:38]
	PathFile m_WaitDownlaodFileList;		//[需下载文件列表][BY:十里坡剑神][QQ:112582793][2019-5-1][0:02:03]
	cTextParsing m_TextPars;				//[字符串解析][BY:十里坡剑神][QQ:112582793][2019-5-1][0:02:56]
	NOTIFYICONDATA NotifyIcon; 
	HANDLE hMapFile;
	LPVOID lpBase;
public:
	CMHAutoPatchDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMHAutoPatchDlg();
	//[初始化相关][BY:十里坡剑神][QQ:112582793][2019-5-2][21:22:45]
	bool Init();			//[初始化登录器][BY:十里坡剑神][QQ:112582793][2019-4-30][22:22:40]
	bool LoadServerInfo();	//[加载更新配置文件][BY:十里坡剑神][QQ:112582793][2019-4-30][21:53:29]
	bool LoadDlgInfo();		//[加载界面信息][BY:十里坡剑神][QQ:112582793][2019-4-30][22:23:57]
	void DrawDialog();		//[绘制窗口][BY:十里坡剑神][QQ:112582793][2019-4-30][22:28:11]
	void InitComBox();		//[初始化分辨率选择][BY:十里坡剑神][QQ:112582793][2019-4-30][23:04:11]
	void StartWork();		//[启动登录器校验][BY:十里坡剑神][QQ:112582793][2019-5-2][22:10:15]
	void RunGame(std::wstring path);
							//[补丁下载函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:23:16]						
	std::wstring  MakePath(std::wstring& str);			//[文件保存路径生成][BY:十里坡剑神][QQ:112582793][2019-5-2][21:23:30]
	std::wstring  MakeUrl(std::wstring& str);				//[补丁下载地址生成][BY:十里坡剑神][QQ:112582793][2019-5-2][21:23:42]
	void MakeDownloadInfo(std::wstring &str);			//[下载提示信息生成][BY:十里坡剑神][QQ:112582793][2019-5-2][21:23:51]
	std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);	//[编码转换][BY:十里坡剑神][QQ:112582793][2019-5-2][21:24:00]
	std::wstring  GetFileName(std::wstring& str);			//[获取下载的文件名][BY:十里坡剑神][QQ:112582793][2019-5-2][21:24:34]

	void UpDataLunch();						//[登录器更新函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:42:21]
	void DownloadFile(DownLoadInfo info);	//[下载线程函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:25:17]
	void StartDownload();					//[下载线程管理函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:25:31]
	void StartCheckFile();					//[文件校验函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:26:43]
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MHAUTOPATCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	ULONG_PTR m_pGdiToken;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	//[分辨率选择事件][BY:十里坡剑神][QQ:112582793][2019-4-30][23:24:04]
	afx_msg void OnCbnSelchangeComboScreen();
	//[开始按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:08]
	afx_msg void OnBnClickedButtonStart();
	//[关闭按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:41]
	afx_msg void OnBnClickedButtonClose();
	//[主页按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:47]
	afx_msg void OnBnClickedButtonHomepage();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
