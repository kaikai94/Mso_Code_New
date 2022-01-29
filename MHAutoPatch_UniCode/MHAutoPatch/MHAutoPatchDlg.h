
// MHAutoPatchDlg.h : ͷ�ļ�
//

#pragma once
//#include <atlimage.h>
#define  UPDATAURL ".\\Image\\Autopatch\\AutoPatchURL.bin"
#include "HtmlViewNotice.h"
#include "CheckResourceFile.h"
#include "cTextParsing.h"
#include "GlobalStructDef.h"
#include "afxwin.h"

// CMHAutoPatchDlg �Ի���
class CMHAutoPatchDlg : public CDialogEx
{
	TextColor sTextColor[eMaxColor];//[������ɫ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:53:05]
	DlgRect sDlgRect[eMaxRect];		//[�ؼ���С��Ϣ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:53:13]
	FontSet sFontSet;				//[������Ϣ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:53:33]
	CFont m_font;					//[����][BY:ʮ���½���][QQ:112582793][2019-5-2][20:52:08]
	HBRUSH hbr;						//[������ˢ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:52:16]
	HBRUSH hbr1;					//[������ˢ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:52:16]
	HBRUSH hbr2;					//[������ˢ][BY:ʮ���½���][QQ:112582793][2019-5-2][20:52:16]

	CBitmapButton m_BtnStart;	//[��ʼ��ť][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CBitmapButton m_BtnHomePage;//[��ҳ��ť][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CBitmapButton m_BtnClose;	//[�رհ�ť][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	Image* m_bgImag;			//[����ͼƬ][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CButton m_CheckWndMode;		//[����ģʽ][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CComboBox m_ComboBox;		//[�ֱ���ѡ��][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]

	CStatic m_StTip;			//[��ʾ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CStatic m_StDwnlTip;		//[������Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CStatic m_AutoPatchVer;		//[�汾��][BY:ʮ���½���][QQ:112582793][2019-5-3][1:18:23]

	bool m_bIsRunClient;		//[������ɺ��Ƿ�ֱ�ӽ�����Ϸ][BY:ʮ���½���][QQ:112582793][2019-5-2][21:48:33]
	BOOL StopTheard;			//[�����˳����Ʊ���][BY:ʮ���½���][QQ:112582793][2019-5-2][21:36:50]
	size_t m_FileCount;			//[�������ļ�����][BY:ʮ���½���][QQ:112582793][2019-5-2][21:37:32]
	CHtmlViewNotice* mHtmlViewNotice;	//[���洰��][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CRect	m_HtmlRect;					//[���洰�ڴ�С][BY:ʮ���½���][QQ:112582793][2019-4-30][21:47:17]
	CProgressCtrl m_ProgressDone;		//[�ܽ�����][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]
	CProgressCtrl m_Progress;			//[���ؽ�����][BY:ʮ���½���][QQ:112582793][2019-4-30][21:45:44]

	ServerInfo m_ServerInfo;				//[����������Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-30][21:54:48]
	TCHAR m_CurPath[MAX_PATH];				//[��ǰ����·��][BY:ʮ���½���][QQ:112582793][2019-4-30][23:29:29]
	CheckResourceFile m_FileCheck;			//[�ļ�У��][BY:ʮ���½���][QQ:112582793][2019-5-1][0:00:38]
	PathFile m_WaitDownlaodFileList;		//[�������ļ��б�][BY:ʮ���½���][QQ:112582793][2019-5-1][0:02:03]
	cTextParsing m_TextPars;				//[�ַ�������][BY:ʮ���½���][QQ:112582793][2019-5-1][0:02:56]
	NOTIFYICONDATA NotifyIcon; 
	HANDLE hMapFile;
	LPVOID lpBase;
public:
	CMHAutoPatchDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMHAutoPatchDlg();
	//[��ʼ�����][BY:ʮ���½���][QQ:112582793][2019-5-2][21:22:45]
	bool Init();			//[��ʼ����¼��][BY:ʮ���½���][QQ:112582793][2019-4-30][22:22:40]
	bool LoadServerInfo();	//[���ظ��������ļ�][BY:ʮ���½���][QQ:112582793][2019-4-30][21:53:29]
	bool LoadDlgInfo();		//[���ؽ�����Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-30][22:23:57]
	void DrawDialog();		//[���ƴ���][BY:ʮ���½���][QQ:112582793][2019-4-30][22:28:11]
	void InitComBox();		//[��ʼ���ֱ���ѡ��][BY:ʮ���½���][QQ:112582793][2019-4-30][23:04:11]
	void StartWork();		//[������¼��У��][BY:ʮ���½���][QQ:112582793][2019-5-2][22:10:15]
	void RunGame(std::wstring path);
							//[�������غ���][BY:ʮ���½���][QQ:112582793][2019-5-2][21:23:16]						
	std::wstring  MakePath(std::wstring& str);			//[�ļ�����·������][BY:ʮ���½���][QQ:112582793][2019-5-2][21:23:30]
	std::wstring  MakeUrl(std::wstring& str);				//[�������ص�ַ����][BY:ʮ���½���][QQ:112582793][2019-5-2][21:23:42]
	void MakeDownloadInfo(std::wstring &str);			//[������ʾ��Ϣ����][BY:ʮ���½���][QQ:112582793][2019-5-2][21:23:51]
	std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);	//[����ת��][BY:ʮ���½���][QQ:112582793][2019-5-2][21:24:00]
	std::wstring  GetFileName(std::wstring& str);			//[��ȡ���ص��ļ���][BY:ʮ���½���][QQ:112582793][2019-5-2][21:24:34]

	void UpDataLunch();						//[��¼�����º���][BY:ʮ���½���][QQ:112582793][2019-5-2][21:42:21]
	void DownloadFile(DownLoadInfo info);	//[�����̺߳���][BY:ʮ���½���][QQ:112582793][2019-5-2][21:25:17]
	void StartDownload();					//[�����̹߳�����][BY:ʮ���½���][QQ:112582793][2019-5-2][21:25:31]
	void StartCheckFile();					//[�ļ�У�麯��][BY:ʮ���½���][QQ:112582793][2019-5-2][21:26:43]
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MHAUTOPATCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	ULONG_PTR m_pGdiToken;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	//[�ֱ���ѡ���¼�][BY:ʮ���½���][QQ:112582793][2019-4-30][23:24:04]
	afx_msg void OnCbnSelchangeComboScreen();
	//[��ʼ��ť���][BY:ʮ���½���][QQ:112582793][2019-4-30][23:27:08]
	afx_msg void OnBnClickedButtonStart();
	//[�رհ�ť���][BY:ʮ���½���][QQ:112582793][2019-4-30][23:27:41]
	afx_msg void OnBnClickedButtonClose();
	//[��ҳ��ť���][BY:ʮ���½���][QQ:112582793][2019-4-30][23:27:47]
	afx_msg void OnBnClickedButtonHomepage();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
