// MonitoringServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MSSystem.h"
#include "stdio.h"

//SLT
#include "../[CC]ServerModule/ServerListManager.h"

#define MAX_LOADSTRING 100

HWND gMainHwnd = NULL;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

BOOL g_bConsole = FALSE;
HANDLE g_hOutConsole;
void ReleaseConsole();
void InitConsole();
void LogConsole(char * buff,...);
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
CMSSystem * g_pMSSystem = NULL;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
//	/*
	// 복구툴 서버는 TYPE_RMS = 3
	// 복구툴 서버를 위해 번호 4번으로...
	
//	*/	

	DeleteFile( "MS_Debug.txt" );
	
	if(atoi(lpCmdLine) == 0)
	{
		LOG( "lpCmdLine == 0" );
		return 0;
	}

	char filename[256];
	FILE* fp = fopen("serverset.txt","r");
	if(fp == NULL)
	{
		LOG( "serverset.txt is not exist" );
		MessageBox(NULL,"serverset.txt is not exist. this server will be closed",0,0);
		exit(0);
		return 0;
	}
	int serversetnum;
	fscanf(fp,"%d",&serversetnum);
	fclose(fp);

	LOG( "Load serverset/%d/serverlist.msl", serversetnum );

	sprintf(filename,"serverset/%d/serverlist.msl",serversetnum);
	SERVERLISTMGR->Load(filename);

	LOG( "End Load serverset/%d/serverlist.msl", serversetnum );

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MONITORINGSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	int kind = atoi(lpCmdLine);
	char titles[256] = {0,};
	if( kind == 1 )
	{
		strcpy(titles,"MAS");
	}
	else if( kind == 2 )
	{
		strcpy(titles,"MS");
	}
	else if( kind == 3 )
	{
		strcpy(titles,"RMToolServer");
	}

	HANDLE hwnd = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, titles);
	if(hwnd != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		MessageBox( NULL, "Already Server Is running!!", 0, 0 );
		CloseHandle(hwnd); 
		hwnd = NULL;
		return FALSE;
	}

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MONITORINGSERVER);
	
	g_pMSSystem = new CMSSystem;
	g_pMSSystem->Start(atoi(lpCmdLine));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	g_pMSSystem->End();
	delete g_pMSSystem;
	ReleaseConsole();

	return msg.wParam;
}

void InitConsole()
{
	g_bConsole = AllocConsole();
	if(g_bConsole)
	{
		g_hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	
}
void ReleaseConsole()
{
	if(g_bConsole)
		FreeConsole();

}
void LogConsole(char * buff,...)
{
	if(g_bConsole)
	{
		static char msg2[255];
		va_list argList;
		va_start(argList, buff);
		vsprintf(msg2, buff, argList);
		va_end(argList);

		DWORD dw;
		WriteFile(g_hOutConsole, msg2,strlen(msg2),&dw,NULL);
		WriteFile(g_hOutConsole, "\n",strlen("\n"),&dw,NULL);
	}
	
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MONITORINGSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MONITORINGSERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   gMainHwnd = hWnd;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			if( g_pMSSystem )
			{
				if( g_pMSSystem->m_wServerType == 1 )
				{
					sprintf( szHello, "Monitoring Agent Server" );
				}
				else if( g_pMSSystem->m_wServerType == 2 )
				{
					sprintf( szHello, "Monitoring Server" );
				}
				else if( g_pMSSystem->m_wServerType == 3 )
				{
					sprintf( szHello, "RMTool Server" );
				}				
			}	
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else if(LOWORD(wParam) == IDC_BUTTON2)
			{
				InitConsole();
				return TRUE;
			}
			break;
	}
    return FALSE;
}
