// RecvFileServer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RecvFileServer.h"
#include "SkinMagicLib.h"
#include "cNetWork.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
HWND hWnd;
cNetWork gNETWORK;
//全局变量
HWND BtnStart;
HWND Edit_IP;
HWND Edit_Port;
HWND List_Msg;
HWND Progress;
HWND Progresscount;
HWND Progress1;
HWND g_hMainWnd;
HANDLE TheadHandl;
HINSTANCE g_hInstance;
TCHAR Port[8]={0,};
TCHAR TipMsg[256]={0,};
int port=0;
bool StopTheard=false;
struct ServerInfo
{
	TCHAR IP[32];
	int port;
};
ServerInfo Info;


void CreateSubDailog();
void ClearDialogMsg();
BOOL CALLBACK 	WndProc(HWND, UINT, WPARAM, LPARAM);
//线程函数
DWORD WINAPI ThreadProc (LPVOID pParam);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 
	InitSkinMagicLib(hInstance, NULL, NULL, NULL);
	LoadSkinFromResource(hInstance, (LPSTR)IDR_SKINMAGIC5, "SKINMAGIC");
	SetDialogSkin("Dialog");
	
	return (int) DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), NULL,WndProc);
}

BOOL CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	if (!g_hMainWnd)//创建时第一个消息到达,保存窗口句柄到全局变量
	{
		g_hMainWnd = hWnd;
	}
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDC_BUTTON_START:
			{
				SendMessage(Edit_IP,WM_GETTEXT,32,(LPARAM)Info.IP);
				SendMessage(Edit_Port,WM_GETTEXT,8,(LPARAM)Port);
				Info.port = _ttoi(Port);
				DWORD   dw1; 
				CloseHandle(CreateThread(NULL,0,ThreadProc,0,0,&dw1)); 
			}
			break;
		case IDC_BUTTON_CLEARMSG:
			{
				ClearDialogMsg();
			}
			break;
		default:
			return false;
		}
		return true;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		EndDialog(hWnd,true);
		//PostQuitMessage(0);
		break;
	case WM_INITDIALOG:
		CreateSubDailog();
	default:
		return false;
	}
	return true;
}

DWORD WINAPI ThreadProc (LPVOID pParam)
{

	if(!gNETWORK.start_listen(Info.IP,Info.port))
	{
		sprintf_s(TipMsg,"服务启动失败");
		SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
		return 0;
	}
	sprintf_s(TipMsg,"服务启动成功");
	SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
	while(true)
	{
		gNETWORK.accepeClinet();
	}
	return 0;
}

void CreateSubDailog()
{
	BtnStart = GetDlgItem(g_hMainWnd,IDC_BUTTON_START);
	Edit_IP	 = GetDlgItem(g_hMainWnd,IDC_EDIT_IP);
	Edit_Port= GetDlgItem(g_hMainWnd,IDC_EDIT_PORT);
	List_Msg = GetDlgItem(g_hMainWnd,IDC_LIST_MSG);
	Progress = GetDlgItem(g_hMainWnd,IDC_STATIC_RECV);
	Progresscount = GetDlgItem(g_hMainWnd,IDC_STATIC_FILECOUNT);
	Progress1 = GetDlgItem(g_hMainWnd,IDC_PROGRESS1);
}
void ClearDialogMsg()
{
	LRESULT count=0;
	count=SendMessage(List_Msg,LB_GETCOUNT,0,0);
	for (int i = 0;i<count;i++)
	{
		SendMessage(List_Msg,LB_DELETESTRING,i,0);
	}
	SendMessage(Progress,WM_SETTEXT,0,(LPARAM)"");
	SendMessage(Progresscount,WM_SETTEXT,0,(LPARAM)"");
	SendMessage(Progress1,PBM_SETPOS,0,0);
	gNETWORK.InitVal();
}