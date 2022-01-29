// CryptStringToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CryptStringTool.h"
#include "CryptStringToolDlg.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct KEY
{
	WORD OpenEncryptNum;
	char OpenEncryptStr[33];
	char SaveEncryptStr[33];
	WORD SaveEncryptNum;
};
KEY  g_Key;
std::string BmpPath;
std::string InBmpPath;
void Writef(unsigned char a[],int c_l,unsigned char tu[],int tu_l)
{
	FILE * fp = NULL;
	unsigned char t;
	int j,k;
	for(j = 64;j<(64+c_l*8);j++)
		tu[j] = tu[j]&0xFE;

	for(j =0;j<c_l;j++)
	{
		for(k = 0;k<8;k++)
		{
			t=a[j]&0x01;
			tu[64+j*8+k] |=t;
			a[j]=a[j]>>1;
		}
	}
	fopen_s(&fp,"./ok.bmp","wb");
	if(fp)
	{
		fwrite(tu,tu_l,1,fp);
		fclose(fp);
	}
}
void Readf(int n,unsigned char tu[],int tu_l)
{
	FILE* fp = NULL;
	unsigned char t,c;
	unsigned char a[100]={0,};


	int j,k;

	for (j = 0;j<n;j++)
	{
		a[j]&= 0x00;

		for(k = 0;k<8;k++)
		{
			t = tu[64+j*8+k];
			t = t&0x01;
			a[j]|=t;
			c = a[j];
			a[j] = (c>>1)|(c<<7);
		}
	}
	a[n]='\0';
	memcpy(&g_Key,a,sizeof(KEY));
}

void Writein()
{
	FILE *fp = NULL;
	fopen_s(&fp,BmpPath.c_str(),"rb");
	long L;
	unsigned char * f;
	if(fp)
	{
		fseek(fp,0,2);
		L = ftell(fp);
		f = new unsigned char[L+1];
		memset(f,0,L+1);
		rewind(fp);
		fread(f,L,1,fp);
		fclose(fp);
	}
	Writef((unsigned char*)&g_Key,sizeof(g_Key),f,L);
	delete [] f;
}

void Readout()
{
	FILE * fp = NULL;
	long L;
	unsigned char * f;
	int n;
	fopen_s(&fp,InBmpPath.c_str(),"rb");
	if(fp)
	{
		fseek(fp,0,2);
		L = ftell(fp);
		f = new unsigned char[L+1];
		memset(f,0,L+1);
		rewind(fp);
		fread(f,L,1,fp);
		fclose(fp);
	}
	Readf(sizeof(g_Key),f,L);
}




CCryptStringToolDlg::CCryptStringToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCryptStringToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptStringToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCryptStringToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCryptStringToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCryptStringToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCryptStringToolDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCryptStringToolDlg 消息处理程序

BOOL CCryptStringToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCryptStringToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCryptStringToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCryptStringToolDlg::OnBnClickedButton1()
{
	CString str;
	memset(&g_Key,0,sizeof(KEY));
	GetDlgItemText(IDC_EDIT1,str);
	strcpy(g_Key.OpenEncryptStr,str.GetBuffer());
	GetDlgItemText(IDC_EDIT2,str);
	strcpy(g_Key.SaveEncryptStr,str.GetBuffer());

	g_Key.OpenEncryptNum = GetDlgItemInt(IDC_EDIT3);
	g_Key.SaveEncryptNum = GetDlgItemInt(IDC_EDIT4);
	if(BmpPath.length()==0)
	{
		MessageBox("请先选择保存密钥的图片","提示",MB_OK);
		return;
	}
	Writein();
	BmpPath.empty();
	MessageBox("加密成功文件名<Ok.bmp>请自行修改","提示",MB_OK);
}

void CCryptStringToolDlg::OnBnClickedButton2()
{
	CString strFile = "";
	CFileDialog dlg( TRUE, NULL, "*.bmp", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, 
		"bmp 格式(*.bmp)|*.bmp", NULL );
	if(dlg.DoModal())
	{
		strFile = dlg.GetPathName();
		BmpPath = strFile.GetBuffer();
		MessageBox(BmpPath.c_str(),"使用的图片:",MB_OK);
	}
}

void CCryptStringToolDlg::OnBnClickedButton3()
{
	CString strFile = "";
	CFileDialog dlg( TRUE, NULL, "*.bmp", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, 
		"bmp 格式(*.bmp)|*.bmp", NULL );
	if(dlg.DoModal())
	{
		strFile = dlg.GetPathName();
		InBmpPath = strFile.GetBuffer();
		MessageBox(InBmpPath.c_str(),"读取密钥图片:",MB_OK);
	}
	Readout();
	SetDlgItemText(IDC_EDIT5,g_Key.OpenEncryptStr);
	SetDlgItemText(IDC_EDIT6,g_Key.SaveEncryptStr);
	SetDlgItemInt(IDC_EDIT7,g_Key.OpenEncryptNum);
	SetDlgItemInt(IDC_EDIT8,g_Key.SaveEncryptNum);
}
