////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改					  //
////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainGame.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cFont.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "./Input/cIMEWnd.h"
#include "ChatManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "MouseCursor.h"
#include "mhFile.h"
#include "OptionManager.h"
#include "MHAudioManager.h"
#include "ObjectManager.h"
#include "MHMap.h"
#include "ExitManager.h"
#include "QuestManager.h"
#include "ServerConnect.h"
#include <vfw.h>
#include "GameIn.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "CommCtrl.h"
#endif
#include "cScriptManager.h"
#include <fstream>


#ifdef _JF_
char g_CLIENTVERSION[33] = "MJF4201902191251";//[疾风][BY:十里坡剑神][2019/2/13][17:25:45]
#elif defined _QD_
char g_CLIENTVERSION[33] = "MQD4201906161306"; //[胖妞][BY:十里坡剑神][2019/2/19][12:34:48]
#elif defined _YQ_
//char g_CLIENTVERSION[33] = "MYQ4201908160430";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201908230512";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201908241556";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201908262034";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201909051616";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201909200438";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
//char g_CLIENTVERSION[33] = "MYQ4201909271033";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
char g_CLIENTVERSION[33] = "MYQ4201911010443";//[缘起][BY:十里坡剑神][2019/2/13][17:25:50]
#endif

#include "cChangeCharCode.h"

#include "AddFont.h"//[字体加载][By:十里坡剑神][QQ:112582793][2018/5/27]
#include "GameCheckManager.h"
#include "CheckResourceFile.h"
#include "cHwInfo.h"
#include "./lib/ClinkAPI.h"


#ifdef _CREATEDUMP_
#include "MiniDumper.h"
#endif // _CREATEDUMP_

#define MAX_LOADSTRING 512  // 20
BOOL bSiegeWarMasterID;
DWORD bSiegeWarMasterChar;
HINSTANCE g_hInst;							
HWND _g_hWnd;
char szTitle[MAX_LOADSTRING];								
char szWindowClass[MAX_LOADSTRING];								
BOOL g_bActiveApp = FALSE;
char g_AgentAddr[16];
WORD g_AgentPort;

char g_szHeroIDName[17];
int	g_nServerSetNum;
BOOL g_SafeLockIsOpen = FALSE;
HIMC g_hIMC = NULL;
HANDLE g_hMap = NULL;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void LeaveClientApplication();
BOOL EnterClientApplication();

DWORD IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep);
BOOL CheckVirtualPC();
BOOL CheckVirtualPC1();
BOOL IsInsideVMWare();				//[虚拟机检测][By:十里坡剑神][QQ:112582793][2017/12/6]
BOOL CheckRunParm(char* lpCmdLine);	//[检查启动参数][By:十里坡剑神][QQ:112582793][2017/12/6]
BOOL   g_IsExit;

BOOL g_HotKeyOK=FALSE;
BOOL g_ShowState=TRUE;
#define ID_H1 0x7777
#define ID_H2 0X8888
char g_FuncName[] = "[uPaqOinfxfmyg`~Ezaqtr";
char g_ModuleName[] = "ovghi(cde";
typedef DWORD(WINAPI *_SetInformationThread)
(HANDLE, DWORD, PVOID, ULONG);

void ClearDirectory( LPCTSTR pStrName )
{
	WIN32_FIND_DATA FindFileData;
	char buf[MAX_PATH];
	wsprintf( buf, "%s\\*", pStrName );

	HANDLE hFind = FindFirstFile( buf, &FindFileData);

	if( hFind == INVALID_HANDLE_VALUE ) return;

	while( 1 )
	{
		if( FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
		{
			if( strcmp( FindFileData.cFileName, "." ) != 0 &&
				strcmp( FindFileData.cFileName, ".." ) != 0 )
			{
				wsprintf( buf, "%s\\%s", pStrName, FindFileData.cFileName );

				ClearDirectory( buf );
				RemoveDirectory( buf );
			}
		}
		else
		{
			wsprintf( buf, "%s\\%s", pStrName, FindFileData.cFileName );
			DeleteFile( buf );
		}
		if( FindNextFile( hFind, &FindFileData ) == FALSE )
			break;
	}
	FindClose( hFind );	
}
static unsigned char g_DummyKey[32]={0x47,0x36,0xff,0xed,0x11,0x87,0xfa,0xcd,0xdd,0xcc,0xaf,0xfe,0xdd,0xca,0x69,0x32,
0xfe,0x87,0xdd,0x47,0x32,0xed,0x11,0xcd,0x36,0xdd,0xcc,0xaf,0xca,0xff,0xfa,0x69};
typedef int (APIENTRY *PMAIN)(HINSTANCE,HINSTANCE,LPSTR,int);
//#include "ReDisConnManager.h"
int APIENTRY WinMain2(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	/*if (*/FILECHECK->CheckLocal();//)//[地域屏蔽][BY:十里坡剑神][QQ:112582793][2019-9-14][12:40:30]
		//return 0;
	//FILE* fp = NULL;
	//fp=(FILE*)setlocale(LC_ALL, ".950");//chinese_china.936:gbk
#ifdef _CREATEDUMP_
	CMiniDumper md;	//[屏蔽生成dmp][By:十里坡剑神][QQ:112582793][2019/4/1][12:36:42]
#endif //_CREATEDUMP_
	//GAMEIN->GetInternetIP();//[获取公网IP][By:十里坡剑神][QQ:112582793][2019/4/1][12:36:42]
	if (!HWINFO->MakeHwID())
	{
		LOGFILE("硬件初始化失败!");
		return 0;
	}
#if (!defined _GMTOOL_) && (!defined _TESTMODE_)
	for(int i = 0;i<strlen(g_FuncName);i++)
	{
		g_FuncName[i]^=i+1;
	}
	for(int i = 0;i<strlen(g_ModuleName);i++)
	{
		g_ModuleName[i]^=i+1;
	}
	HMODULE hModule = GetModuleHandle(g_ModuleName);
	for(int i = 0;i<strlen(g_ModuleName);i++)
	{
		g_ModuleName[i] ^= i + 1;
	}
	if (!hModule)
	{
		return 0;
	}
	typedef DWORD(WINAPI *_SetInformationThread)
		(HANDLE, DWORD, PVOID, ULONG);
	_SetInformationThread MySetInformationThread = (_SetInformationThread)GetProcAddress(hModule, g_FuncName);
	for(int i = 0;i<strlen(g_FuncName);i++)
	{
		g_FuncName[i]^=i+1;
	}
	if (!MySetInformationThread)
	{
		return 0;
	}

	MySetInformationThread(GetCurrentThread(), 0x11, 0, 0);
	FreeLibrary(hModule);


	g_HotKeyOK=RegisterHotKey(_g_hWnd,ID_H2,MOD_CONTROL,'+');
	if(g_HotKeyOK!=TRUE)
		return FALSE;
	if(!EnterClientApplication()) 
		return 0;
#endif

	g_ShowState=TRUE;
	g_IsExit = FALSE;
	bSiegeWarMasterID = FALSE;
	bSiegeWarMasterChar = -1;

#ifdef _BIG5SYSTEM_
	CODEMGR->Init();
#endif //_BIG5SYSTEM_

#if (!defined _GMTOOL_) && (!defined _TESTMODE_)
	if (!CheckRunParm(lpCmdLine)) return 0;	//检查启动参数 by:十里坡剑神	QQ:418914674
	if(IsInsideVMWare()||CheckVirtualPC()||CheckVirtualPC1())	//虚拟机检测	by:十里坡剑神	QQ:112582793
	{
		return 0;
	}
#endif
	char path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	FILECHECK->DeleteFileForDmp(path);

	FILECHECK->LoadUrl();
	DeleteFile("./Log/Debug.txt");

	CreateDirectory("Log",NULL);

	LOGFILE("///////////////////////////////////////////////////////");
	LOGFILE("///////////////////  Client Start  ////////////////////");
	LOGFILE("///////////////////////////////////////////////////////");
	
	GAMERESRCMNGR->LoadGameDesc();
#ifdef _GMTOOL_
	sprintf(szTitle, "你不知道的天语_GMTOOL_");
	sprintf(szWindowClass, "你不知道的天语_GMTOOL_");

#else
	sprintf(szTitle, GAMERESRCMNGR->m_GameDesc.strWindowTitle);
	sprintf(szWindowClass, GAMERESRCMNGR->m_GameDesc.strWindowTitle);
#endif
	MyRegisterClass(hInstance);
	GAMERESRCMNGR->LoadDeveloperInfo();
	QUESTMGR->LoadQuestString( "./Resource/QuestScript/QuestString.bin" );
	QUESTMGR->LoadQuestScript( "./Resource/QuestScript/QuestScript.bin" );
	QUESTMGR->LoadNewbieGuide( "./Resource/QuestScript/NewbieGuide.bin" );
	QUESTMGR->LoadQuestNpc( "./Resource/QuestScript/QuestNpcList.bin" );
	SCRIPTMGR->InitScriptManager();
	if (!InitInstance (hInstance, nCmdShow)) 
	{
#if (!defined _GMTOOL_) && (!defined _TESTMODE_)
		LeaveClientApplication();
#endif
		return FALSE;
	}
	g_HotKeyOK=RegisterHotKey(_g_hWnd,ID_H1,MOD_CONTROL,'W');
	if(g_HotKeyOK!=TRUE)
		MessageBox(_g_hWnd,"隐藏窗口热键冲突","提示",MB_OK);
	MSG msg;
	while (1) 
    {
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
#ifdef _GMTOOL_
			if( !GMTOOLMGR->IsGMDialogMessage(&msg) )
#endif
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); 
			}
		}
		else
		{
			if(MAINGAME->Process() == -1)
			{
				break;
			}
		}		
	}
#if (!defined _GMTOOL_) && (!defined _TESTMODE_)
	LeaveClientApplication();
	UnregisterHotKey(_g_hWnd, ID_H1);
	clinkStop();
	LOGFILE("clinkStop()");
#endif

	UnregisterHotKey(_g_hWnd, ID_H2);
	return msg.wParam;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
		PMAIN pCodeEntry = NULL;
		DWORD *pProcAddr = reinterpret_cast<DWORD*>(&pCodeEntry);
		unsigned char a, b, c, d;
		__try
		{
			a = g_DummyKey[0] ^ g_DummyKey[1];
			b = g_DummyKey[8] ^ g_DummyKey[9];
			c = g_DummyKey[16] ^ g_DummyKey[17];
			d = g_DummyKey[24] ^ g_DummyKey[25];

			a = a^g_DummyKey[2];
			b = b^g_DummyKey[10];
			c = c^g_DummyKey[18];
			d = d^g_DummyKey[26];

			*pProcAddr = (a << 24) + (b << 16) + (c << 8) + d;//混淆

			b = b^g_DummyKey[3];
			c = c^g_DummyKey[11];
			a = a^g_DummyKey[19];
			d = d^g_DummyKey[27];

			*pProcAddr = *pProcAddr / a;//混淆

			d = d^g_DummyKey[4];
			c = c^g_DummyKey[12];
			a = a^g_DummyKey[20];
			b = b^g_DummyKey[28];

			*pProcAddr = *pProcAddr / b;//混淆

			b = b^g_DummyKey[5];
			c = c^g_DummyKey[13];
			a = a^g_DummyKey[21];
			d = d^g_DummyKey[29];

			*pProcAddr = *pProcAddr / b;//混淆

			c = c^g_DummyKey[6];
			a = a^g_DummyKey[14];
			d = d^a;
			b = b^c;

			*pProcAddr = *pProcAddr / c;//混淆

			d = d^g_DummyKey[22];
			b = b^g_DummyKey[30];

			*pProcAddr = *pProcAddr / d;//混淆

			d = d^g_DummyKey[7];
			b = b^g_DummyKey[15];

			*pProcAddr = *pProcAddr / a;//混淆

			d = d^g_DummyKey[23];
			b = b^g_DummyKey[31];
			b = b^d;
			*pProcAddr = *pProcAddr / b;//混淆. throw exception because b==0
			a = a^g_DummyKey[2];
			b = b^g_DummyKey[10];
			c = c^g_DummyKey[18];
			d = d^g_DummyKey[26];

			*pProcAddr = (a << 24) + (b << 16) + (c << 8) + d;//混淆

			b = b^g_DummyKey[3];
			c = c^g_DummyKey[11];
			a = a^g_DummyKey[19];
			d = d^g_DummyKey[27];

			*pProcAddr = *pProcAddr / a;//混淆

			d = d^g_DummyKey[4];
			c = c^g_DummyKey[12];
			a = a^g_DummyKey[20];
			b = b^g_DummyKey[28];

			*pProcAddr = *pProcAddr / b;//混淆

			b = b^g_DummyKey[5];
			c = c^g_DummyKey[13];
			a = a^g_DummyKey[21];
			d = d^g_DummyKey[29];

			*pProcAddr = *pProcAddr / b;//混淆

			c = c^g_DummyKey[6];
			a = a^g_DummyKey[14];
			d = d^a;
			b = b^c;

			*pProcAddr = *pProcAddr / c;//混淆

			d = d^g_DummyKey[22];
			b = b^g_DummyKey[30];

			*pProcAddr = *pProcAddr / d;//混淆

			d = d^g_DummyKey[7];
			b = b^g_DummyKey[15];

			*pProcAddr = *pProcAddr / a;//混淆
		}
		__except (GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			int nCount = strlen(g_CLIENTVERSION);
			for (int i = 0; i < nCount; i++)
			{
				g_CLIENTVERSION[i] += g_CLIENTVERSION[nCount - i];
			}
			pCodeEntry = WinMain2;
		}
		return pCodeEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_NOCLOSE; 

	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;

	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_GAMEICON) );


	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;

	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	CURSOR->Init( hInstance );

	g_hInst = hInstance; 
	int x = GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	int y = GetSystemMetrics(SM_CXFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION);


	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN |
		WS_SYSMENU |
		WS_MINIMIZEBOX;
	
    WIN_DPI * m_DpiSet=GAMERESRCMNGR->GetPrintScressInfo(GAMERESRCMNGR->GetPrintScreenIndex());
	if(m_DpiSet)
	{
		_g_hWnd  = CreateWindow(szWindowClass, szTitle, dwStyle, 
			CW_USEDEFAULT, 0, m_DpiSet->x+x, m_DpiSet->y+y, NULL, NULL, hInstance, NULL);	
	}
	else
	{
		_g_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, 
			CW_USEDEFAULT, 0, 1024+x, 768+y, NULL, NULL, hInstance, NULL);	
	}

	if (!_g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(_g_hWnd, nCmdShow);

	UpdateWindow(_g_hWnd);

	ShowCursor(TRUE);

	CoInitialize(NULL);

	MAINGAME->Init(_g_hWnd);
		
	//if( OPTIONMGR->GetGameOption()->bIntroFlag == FALSE )
        MAINGAME->SetGameState(eGAMESTATE_CONNECT);
	//else
	//	MAINGAME->SetGameState(eGAMESTATE_TITLE);               

#ifdef _GMTOOL_
		GMTOOLMGR->CreateGMDialog();
#endif
	return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MOUSE->GetWindowMessage(hWnd, message, wParam, lParam);
	CHATMGR->MsgProc( message, wParam );
	CIMEWND->OnMsg(hWnd, message, wParam, lParam);
	
	switch (message) 
	{
	case WM_CHAR:
	case WM_IME_CHAR:
		return 0;
	case WM_IME_STARTCOMPOSITION:
		{
			return 0;
		}
	case WM_ACTIVATEAPP:
		{
			g_bActiveApp = (BOOL)wParam;

			if( !g_bActiveApp )
			{
				AUDIOMGR->StopAll();
				AUDIOMGR->StopBGM();

				CIMEWND->SetCtrlPushed( FALSE );


			}
			else
			{
				if( OPTIONMGR->GetGameOption()->bSoundBGM )
					AUDIOMGR->PlayLastBGM();
			}
		}
		break;

	case WM_ACTIVATE:
		
		if( g_UserInput.GetKeyboard()->GetDirectInput() )
		{
			if( WA_INACTIVE != wParam )
			{
				g_UserInput.GetKeyboard()->ClearKeyState();
				g_UserInput.GetKeyboard()->UpdateKeyboardState();
				OBJECTMGR->SetAllNameShow( FALSE );
			}
		}
		break;

	case WM_USER+155:
		{
			ISC_BaseNetwork* lpNet = NETWORK->GetBaseNetwork();
			if(lpNet)
				lpNet->SocketEventProc();
		}
		return TRUE;		
		
	case WM_CLOSE:
		{
			if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN || GAMEIN->IsDisconnect())
			{
				MAINGAME->EndGame();
				break;
			}
			else if (!g_IsExit)
			{
				WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
				return 0;
			}
			else
				MAINGAME->EndGame();
			
#ifdef _NPROTECT_
			if(NPROTECTMGR->GetMsg())
			{
				MessageBox(hWnd, NPROTECTMGR->GetMsg(), "nProtect GameGuard", MB_OK);
			}
#endif
		}
		break;

	case WM_SETFOCUS:
		{ 
			CURSOR->Process();
		}
		return 0;
	case WM_SETCURSOR:
		{ 
			CURSOR->Process();			
		}
		break;	
	case WM_SYSKEYDOWN:		
		{
			if( wParam == VK_F4 )
			{
				if( MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE )
				{
					break;		
				}
				else if( MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_CHARMAKE ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN )
				{
					WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
					return 0;
				}
			}
		}
		return 0;

	case WM_SYSCOMMAND:
		{
			if( wParam == SC_KEYMENU )
				return 0;
		}
		break;

	case WM_SYSKEYUP:		
		{
		}
		return 0;

#ifdef _GMTOOL_
	case WM_MOVE:
		GMTOOLMGR->SetPositionByMainWindow();
		break;
#endif

	case WM_CREATE:
		{
#ifdef _GMTOOL_
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof( icex );
			icex.dwICC	= ICC_DATE_CLASSES;
			InitCommonControlsEx( &icex );
#endif

		}
		break;
	case WM_HOTKEY:
		if(wParam==0x7777)
		{
			g_ShowState=!g_ShowState;
			if(g_ShowState)ShowWindow(_g_hWnd, SW_SHOWNORMAL);
			else ShowWindow(_g_hWnd, SW_HIDE);
		}
		break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}


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
			break;
	}
    return FALSE;
}

BOOL EnterClientApplication()
{
	//taiyo
	g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 8, "YQMsoSecond");
	if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		CloseHandle(g_hMap); 
		g_hMap = NULL;
		return FALSE;
	}
	return TRUE;
}


void LeaveClientApplication()
{//taiyo
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}




//#ifdef TAIWAN_LOCAL
//int is_big5(BYTE *str)
//{
//	if( str[0] & 0x80 )
//	{
//		if ( str[0] == 0xa2)
//		{
//			if( str[1]>= 0xaf && str[1]<= 0xb8 )
//				return 1;
//			else if( str[1] >= 0xcf && str[1] <= 0xff )
//				return 1;
//		} 
//		if ( str[0] == 0xa3)
//		{
//			if( str[1]>=0x43 && str[1]<=0xff )
//				return 1;
//		}
//
//		if ( str[0] == 0xa4)
//		{
//			if( str[1] >= 0x40 && str[1] <= 0xff )
//				return 1;
//		}
//
//		if ( str[0] >= 0xa5 && str[0] <= 0xc5 )
//			return 1;
//
//		if ( str[0] == 0xc6 )
//		{
//			if( str[1] >= 0x00 && str[1] <= 0x7e )
//				return 1;
//		}
//	}
//	else
//	{
//		return 1;
//	}
//	return 0;
//}
//#endif

//BOOL CheckResourceFile()
//{
//	HANDLE hSrch;
//	WIN32_FIND_DATA wfd;
//	SYSTEMTIME time;
//	char Dir[MAX_PATH];
//	char filename[MAX_PATH];
//	BOOL bResult = TRUE;
//	CMHFile file;
//	char temp[256];
//	WORD year, month, day, hour, min;	
//
//	GetWindowsDirectory( Dir, MAX_PATH );
//	strcat( Dir, "\\mfl.dat" );
//		
//	GetCurrentDirectory( MAX_PATH, filename );
//	strcat( filename, "\\Resource\\*.bin" );
//	hSrch = FindFirstFile( filename, &wfd );
//	while( bResult )
//	{
//		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );
//
//		if( !file.Init( Dir, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
//		{
//			file.Release();
//			goto filesave;
//		}
//		while( !file.IsEOF() )
//		{
//			file.GetString( temp );
//			if( strcmp( wfd.cFileName, temp ) == 0 )
//			{
//				year = file.GetWord();
//				month = file.GetWord();
//				day = file.GetWord();
//				hour = file.GetWord();
//				min = file.GetWord();
//				
//				if( year != time.wYear || month != time.wMonth || day != time.wDay ||
//					hour != time.wHour || min != time.wMinute )
//				{
//					file.Release();
//					return FALSE;
//				}
//				else
//					break;
//			}
//		}
//		file.Release();
//		bResult = FindNextFile( hSrch, &wfd );
//	}
//
//	bResult = TRUE;
//	GetCurrentDirectory( MAX_PATH, filename );
//	strcat( filename, "\\Resource\\SkillArea\\*.bsad" );
//	hSrch = FindFirstFile( filename, &wfd );
//	while( bResult )
//	{
//		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );
//
//		if( !file.Init( Dir, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
//		{
//			file.Release();
//			goto filesave;
//		}
//		while( !file.IsEOF() )
//		{
//			file.GetString( temp );
//			if( strcmp( wfd.cFileName, temp ) == 0 )
//			{
//				year = file.GetWord();
//				month = file.GetWord();
//				day = file.GetWord();
//				hour = file.GetWord();
//				min = file.GetWord();
//				
//				if( year != time.wYear || month != time.wMonth || day != time.wDay ||
//					hour != time.wHour || min != time.wMinute )
//				{
//					file.Release();
//					return FALSE;
//				}
//				else
//					break;
//			}
//		}
//		file.Release();
//		bResult = FindNextFile( hSrch, &wfd );
//	}
//
//	return TRUE;
//
//filesave:
//	SaveFileList();
//	return TRUE;
//}
//
//BOOL SaveFileList()
//{
//	DWORD count = 1;
//	HANDLE hSrch;
//	WIN32_FIND_DATA wfd;
//	SYSTEMTIME time;
//	char Dir[MAX_PATH];
//	char filename[MAX_PATH];
//	BOOL bResult = TRUE;
//
//	GetWindowsDirectory(Dir, MAX_PATH);
//	strcat(Dir, "\\mfl.dat");
//	FILE* fp = fopen(Dir, "w");
//	if (fp == NULL)
//	{
//		fclose(fp);
//
//		return FALSE;
//	}
//
//	GetCurrentDirectory(MAX_PATH, filename);
//	strcat(filename, "\\Resource\\*.bin");
//	hSrch = FindFirstFile(filename, &wfd);
//	while (bResult)
//	{
//		FileTimeToSystemTime(&wfd.ftLastWriteTime, &time);
//		fprintf(fp, "%d %s\t", count, wfd.cFileName);
//		fprintf(fp, "%4d %02d %02d %02d %02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
//		bResult = FindNextFile(hSrch, &wfd);
//		++count;
//	}
//
//	bResult = TRUE;
//	GetCurrentDirectory(MAX_PATH, filename);
//	strcat(filename, "\\Resource\\SkillArea\\*.bsad");
//	hSrch = FindFirstFile(filename, &wfd);
//	while (bResult)
//	{
//		FileTimeToSystemTime(&wfd.ftLastWriteTime, &time);
//		fprintf(fp, "%d %s\t", count, wfd.cFileName);
//		fprintf(fp, "%4d %02d %02d %02d %02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
//		bResult = FindNextFile(hSrch, &wfd);
//		++count;
//	}
//
//	fclose(fp);
//
//	return TRUE;
//}

//[检查启动参数][By:十里坡剑神][QQ:112582793][2017/11/23]
BOOL CheckRunParm(char* lpCmdLine)
{
	BOOL bRet = FALSE;
	// 打开共享的文件对象
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, "YuanQiMso");
	if (hMapFile)
	{
		LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		if (NULL == lpBase) 
			return bRet;
		else
		{
			char szBuffer[64] = { 0 };
			strcpy_s(szBuffer, 64, (char*)lpBase);
			int iLen = strlen(szBuffer);
			for (int i = 0; i < iLen; i++)
			{
				szBuffer[i] ^= i + 2;
			}
			if (strcmp(szBuffer, "OZU14790:9=4?=%#") == 0)
				bRet = TRUE;
			memset(lpBase, 0, 64);
			strcpy_s((char*)lpBase, 64, "KWAsl0");
			UnmapViewOfFile(lpBase);
			CloseHandle(hMapFile);
		}

		
	}
	return bRet;

}
//[虚拟机检测][By:十里坡剑神][QQ:112582793][2017/11/23]
BOOL IsInsideVMWare() 	
{
	BOOL rc = TRUE;
	__try
	{
		__asm
		{
			push   edx
				push   ecx
				push   ebx

				mov    eax, 'VMXh'
				mov    ebx, 0 
				mov    ecx, 10
				mov    edx, 'VX'

				in     eax, dx 

				cmp    ebx, 'VMXh' 
				setz   [rc]

			pop    ebx
				pop    ecx
				pop    edx
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		rc = FALSE;
	}
	return rc;
}



DWORD IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
	PCONTEXT ctx=ep->ContextRecord;
	ctx->Ebx=-1;//[未运行在VPC中][By:十里坡剑神][QQ:112582793][2019/3/21][22:22:02]
	ctx->Eip+=4;//[跳过Call VPC操作][By:十里坡剑神][QQ:112582793][2019/3/21][22:22:14]
	return EXCEPTION_CONTINUE_EXECUTION;
}
BOOL CheckVirtualPC1()
{
	DWORD dwECX = 0;
	bool b_IsVM = true;
	__asm
	{
		pushad;
		pushfd;
		mov eax,1;
		cpuid;
		mov dwECX,ecx;
		and ecx,0x80000000;
		test ecx,ecx;
		setz[b_IsVM];
		popfd;
		popad;
	}
	if(b_IsVM)
		return FALSE;
	else
		return TRUE;
}
BOOL CheckVirtualPC()
{
	bool rc = TRUE;
	__try
	{
		__asm
		{
			push ebx
				mov ebx,0
				mov eax,1
				__emit 0fh
				__emit 3fh
				__emit 07h
				__emit 0bh
				test ebx,ebx
				setz[rc]
			pop ebx
		}
	}
	__except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))
	{
		rc = FALSE;
	}
	return rc;
}
