#include "stdafx.h"
#include "GameCheckManager.h"
#include "shlwapi.h"
#include <tlhelp32.h>
#include <algorithm>
#include "MHFile.h"
#include "ObjectManager.h"
#pragma comment(lib,"shlwapi.lib")
#include <Psapi.h>  
#pragma comment (lib,"Psapi.lib")
#include "ChatManager.h"
#include "ReConnect.h"
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam);
//GameCheckManager* GameCheckManager::g_AiInstance = NULL;
HANDLE		 m_CheckHandle[2];
DWORD WINAPI GameCheck(LPVOID lpparentet);
extern HWND _g_hWnd;
extern BOOL   g_IsExit;
GameCheckManager::GameCheckManager(void)
{
	bIsInit = FALSE;
	bLock = FALSE;
	m_ExitTime = 0;
}

GameCheckManager::~GameCheckManager(void)
{
	m_CheckHandle[0] = NULL;
	m_CheckHandle[1] = NULL;
	bIsInit = FALSE;
	//[Map清理][BY:十里坡剑神][QQ:112582793][2019-4-7][12:51:26]
	m_ProcessInfo.clear();
	m_ModuleInfo.clear();
	m_WindowsText.clear();

	m_ProcessList.clear();
	m_ModuleList.clear();
	m_WindwosList.clear();
}

void GameCheckManager::LoadCheckList()
{
	m_WindwosList.clear();
	m_ProcessList.clear();
	m_ModuleList.clear();
	char buff[512] = {0,};
	
	std::string str;
	CMHFile file;
	if (file.Init(CHATMGR->GetChatMsg(2372), "rb") == FALSE)
	{
		MSG_HACKUSER msg;
		memset(&msg,0,sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName, CHATMGR->GetChatMsg(2364));
		sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
		sprintf(msg.PrcessPath, CHATMGR->GetChatMsg(2372));
		msg.UserID = REDISCONNECT->GetCharID();
		NETWORK->Send(&msg,sizeof(msg));
		g_IsExit = TRUE;
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return;
	}
	file.GetLineX(buff,512);
	while(1)
	{
		if(file.IsEOF())
			break;
		WORD type = file.GetWord();
		switch(type)
		{
		case 1:
			file.GetLine(buff,512);
			str = buff;
			std::transform(str.begin(),str.end(),str.begin(),::toupper);
			m_WindwosList.push_back(str);
			break;
		case 2:
			file.GetLine(buff,512);
			str = buff;
			std::transform(str.begin(),str.end(),str.begin(),::toupper);
			m_ProcessList.push_back(str);
			break;
		case 3:
			file.GetLine(buff,512);
			str = buff;
			std::transform(str.begin(),str.end(),str.begin(),::toupper);
			m_ModuleList.push_back(str);
			break;
		}
	}
	file.Release();
}
void GameCheckManager::InitGameCheck()
{
	if(bIsInit) return;
	bIsExit = FALSE;
	m_CheckHandle[0] = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_CheckHandle[1] = CreateEvent(NULL,FALSE,FALSE,NULL);
	HANDLE h = NULL;
	h=CreateThread(NULL, 0, GameCheck, NULL, 0, NULL);
	if (h!=NULL)
		CloseHandle(h);
	dwCheckTime = gCurTime;
	bIsInit = TRUE;
	m_ExitTime = 0;
}
void GameCheckManager::Process()
{
	if(!bIsInit) return;
	if (gCurTime - dwCheckTime >= HACKER_CHECK_TIME&&!bLock)
	{	
		bLock = TRUE;
		SetEvent(m_CheckHandle[0]);
		dwCheckTime = gCurTime;
	}
	if (m_ExitTime != 0)
	{
		if (gCurTime - m_ExitTime >= HACKER_CHECK_TIME)
		{
			g_IsExit = TRUE;
			PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
		}
	}
}
void GameCheckManager::SetEventHandle()
{
	SetEvent(m_CheckHandle[0]);
	CloseHandle(m_CheckHandle[0]);
	CloseHandle(m_CheckHandle[1]);
	bIsInit = FALSE;
}

BOOL GameCheckManager::GetProcessInfo()
{
	m_ProcessInfo.clear();
	m_ModuleInfo.clear();
	m_WindowsText.clear();
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 stcPe32={0,};
	stcPe32.dwSize = sizeof(PROCESSENTRY32);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hProcessSnap==INVALID_HANDLE_VALUE)
		return FALSE;

	if(!Process32First(hProcessSnap,&stcPe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	do 
	{
		try
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,stcPe32.th32ProcessID);
			if(hProcess)
			{
				int nPriority = GetPriorityClass(hProcess);
				CloseHandle(hProcess);
			}
			PROCINFO info;
			info.dwPId =stcPe32.th32ProcessID;
			info.ProcessName = stcPe32.szExeFile;
			info.ProcessPath = GetProcessFullPath(stcPe32.th32ProcessID);
			std::transform(info.ProcessName.begin(),info.ProcessName.end(),info.ProcessName.begin(),::toupper);
			m_ProcessInfo[info.ProcessName] = info;
			GetModuleInfo(stcPe32.th32ProcessID);
		}
		catch (...)
		{
			//CHATMGR->AddMsg(CTC_SYSMSG,"异常处理");
		}

	} while (Process32Next(hProcessSnap,&stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}
BOOL GameCheckManager::GetModuleInfo(DWORD dwPId)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = {sizeof(MODULEENTRY32)};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPId);
	if(hModuleSnap==INVALID_HANDLE_VALUE)
		return FALSE;
	if(!Module32First(hModuleSnap,&me32))
	{
		CloseHandle(hModuleSnap);
		return FALSE;
	}
	do 
	{
		try
		{
			std::string szModule(me32.szModule);
			std::transform(szModule.begin(),szModule.end(),szModule.begin(),::toupper);
			m_ModuleInfo[szModule] = me32.szExePath;
		}
		catch (...)
		{

		}

	} 
	while (Module32Next(hModuleSnap,&me32));
	CloseHandle(hModuleSnap);
	return TRUE;
}

BOOL GameCheckManager::DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)  
{  
	TCHAR           szDriveStr[500] = {0,};
	TCHAR           szDrive[3] = { 0, };
	TCHAR           szDevName[100] = { 0, };
	INT             cchDevName = 0;  
	INT             i = 0;  

	//[检查参数][By:十里坡剑神][QQ:112582793][2017/12/27]
	if(!pszDosPath || !pszNtPath )  
		return FALSE;  

	//[获取本地磁盘字符串 ][By:十里坡剑神][QQ:112582793][2017/12/27]
	if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))  
	{  
		for(i = 0; szDriveStr[i]; i += 4)  
		{  
			if(!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))  
				continue;  

			szDrive[0] = szDriveStr[i];  
			szDrive[1] = szDriveStr[i + 1];  
			szDrive[2] = '\0';  
			if(!QueryDosDevice(szDrive, szDevName, 100))//[查询 Dos 设备名 ][By:十里坡剑神][QQ:112582793][2017/12/27]
				return FALSE;  

			cchDevName = lstrlen(szDevName);  
			if(_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//[命中][By:十里坡剑神][QQ:112582793][2017/12/27]
			{  
				lstrcpy(pszNtPath, szDrive);//[复制驱动器 ][By:十里坡剑神][QQ:112582793][2017/12/27]
				lstrcat(pszNtPath, pszDosPath + cchDevName);//[复制路径][By:十里坡剑神][QQ:112582793][2017/12/27]

				return TRUE;  
			}             
		}  
	}  
	lstrcpy(pszNtPath, pszDosPath);  
	return FALSE;  
}  
//[获取进程完整路径][By:十里坡剑神][QQ:112582793][2017/12/27]
std::string GameCheckManager::GetProcessFullPath(DWORD dwPID)  
{  
	TCHAR       szImagePath[MAX_PATH];  
	TCHAR       pszFullPath[MAX_PATH];  
	HANDLE      hProcess; 
	std::string strPath;
	strPath = "";
	if(!pszFullPath)  
		return strPath;  

	pszFullPath[0] = '\0';  
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);  
	if(!hProcess)  
		return strPath;  

	if(!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))  
	{  
		CloseHandle(hProcess);  
		return strPath;  
	}  

	if(!DosPathToNtPath(szImagePath, pszFullPath))  
	{  
		CloseHandle(hProcess);  
		return strPath;  
	}  

	CloseHandle(hProcess);  
	strPath = pszFullPath;
	return strPath;  
}  
void GameCheckManager::AddWindowText(std::string str,HWND hwnd)
{
	std::transform(str.begin(),str.end(),str.begin(),::toupper);
	m_WindowsText[str] = hwnd;
}
BOOL GameCheckManager::CheckAutoPatch()
{
	BOOL bRet = FALSE;
	GetProcessInfo();
	std::map<std::string,PROCINFO>::iterator iter;
	iter = m_ProcessInfo.find("TDUN.EXE");
	if(iter!=m_ProcessInfo.end())
		bRet = TRUE;
	return bRet;
}
void GameCheckManager::SetExitTime()
{
	m_ExitTime = gCurTime;
}
BOOL GameCheckManager::CheckProcessList()
{
#ifdef _GMTOOL_
	return FALSE;
#endif
	if(m_ProcessInfo.size()==0||m_ProcessList.size()==0)
	{
		MSG_HACKUSER msg;
		memset(&msg,0,sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName, CHATMGR->GetChatMsg(2365));
		sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
		sprintf(msg.PrcessPath,CHATMGR->GetChatMsg(2371),m_ProcessInfo.size(),m_ProcessList.size());
		msg.UserID = REDISCONNECT->GetCharID();
		NETWORK->Send(&msg,sizeof(msg));
		g_IsExit = TRUE;
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return FALSE;
	}
	std::vector<std::string>::iterator iter;
	std::map<std::string,PROCINFO>::iterator mIter;
	iter = m_ProcessList.begin();
	while(TRUE)
	{
		if(bIsExit)
			return TRUE;
		if(iter==m_ProcessList.end())
			break;
		else
		{
			mIter = m_ProcessInfo.find(iter->c_str());
			if(mIter!=m_ProcessInfo.end())
			{
				//[检测到指定进程，发送消息到服务端记录][By:十里坡剑神][QQ:112582793][2017/12/27]
				MSG_HACKUSER msg;
				memset(&msg,0,sizeof(msg));
				msg.Category = MP_HACKCHECK;
				msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
				sprintf(msg.ProcessName,mIter->second.ProcessName.c_str());
				sprintf(msg.PrcessPath,mIter->second.ProcessPath.c_str());
				sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
				msg.UserID = REDISCONNECT->GetCharID();

				CHATMGR->AddMsg(CTC_TIP_KILL,CHATMGR->GetChatMsg(2344),mIter->second.ProcessName.c_str());
				NETWORK->Send(&msg,sizeof(msg));
				
				NETWORK->Disconnect();
				return TRUE;
			}
		}
		iter++;
	}
	return FALSE;
}
BOOL GameCheckManager::CheckModuleList()
{
#ifdef _GMTOOL_
	return FALSE;
#endif
	if(m_ModuleInfo.size()==0||m_ModuleList.size()==0)
	{
		MSG_HACKUSER msg;
		memset(&msg,0,sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName,CHATMGR->GetChatMsg(2366));
		sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
		sprintf(msg.PrcessPath, CHATMGR->GetChatMsg(2370), m_ModuleInfo.size(), m_ModuleList.size());
		msg.UserID = REDISCONNECT->GetCharID();
		NETWORK->Send(&msg,sizeof(msg));
		g_IsExit = TRUE;
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return FALSE;
	}
	std::map<std::string,std::string>::iterator mIter;
	std::vector<std::string>::iterator iter;
	iter = m_ModuleList.begin();
	while(TRUE)
	{
		if(bIsExit)
			return TRUE;
		if(iter==m_ModuleList.end())
			break;
		else
		{
			mIter = m_ModuleInfo.find(iter->c_str());
			if(mIter!=m_ModuleInfo.end())
			{
				MSG_HACKUSER msg;
				memset(&msg,0,sizeof(msg));
				msg.Category = MP_HACKCHECK;
				msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
				sprintf(msg.ProcessName,mIter->first.c_str());
				sprintf(msg.PrcessPath,mIter->second.c_str());
				sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
				msg.UserID = REDISCONNECT->GetCharID();
				CHATMGR->AddMsg(CTC_TIP_KILL,CHATMGR->GetChatMsg(2343),mIter->first.c_str());
				NETWORK->Send(&msg,sizeof(msg));
				NETWORK->Disconnect();
				return TRUE;
			}
		}
		iter++;
	}
	return FALSE;
}
BOOL GameCheckManager::CheckWindowList()
{
#ifdef _GMTOOL_
	return FALSE;
#endif
	if(m_WindowsText.size()==0||m_WindwosList.size()==0)
	{
		MSG_HACKUSER msg;
		memset(&msg,0,sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName, CHATMGR->GetChatMsg(2367));
		sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
		sprintf(msg.PrcessPath, CHATMGR->GetChatMsg(2369), m_WindowsText.size(), m_WindwosList.size());
		msg.UserID = REDISCONNECT->GetCharID();
		NETWORK->Send(&msg,sizeof(msg));
		g_IsExit = TRUE;
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return FALSE;
	}
	std::vector<std::string>::iterator iter;
	iter = m_WindwosList.begin();
	while(TRUE)
	{
		if(bIsExit)
			return TRUE;
		if(iter==m_WindwosList.end())
			break;
		else
		{
			std::map<std::string,HWND>::iterator mIter1;
			mIter1 = m_WindowsText.begin();
			while(TRUE)
			{
				if(bIsExit)
					return TRUE;
				if(mIter1==m_WindowsText.end())
					break;
				else
				{
					if(mIter1->first.find(iter->c_str())!=mIter1->first.npos)
					{
						DWORD dwPId= 0;
						GetWindowThreadProcessId(mIter1->second,&dwPId);
						MSG_HACKUSER msg;
						memset(&msg,0,sizeof(msg));
						msg.Category = MP_HACKCHECK;
						msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
						sprintf(msg.ProcessName,iter->c_str());
						sprintf(msg.UserName,REDISCONNECT->GetUserAccount());
						sprintf(msg.PrcessPath,GetProcessFullPath(dwPId).c_str());
						msg.UserID = REDISCONNECT->GetCharID();
						CHATMGR->AddMsg(CTC_TIP_KILL,CHATMGR->GetChatMsg(2345),mIter1->first.c_str());
						NETWORK->Send(&msg,sizeof(msg));
						NETWORK->Disconnect();
						return TRUE;
					}
				}
				mIter1++;
			}
		}
		iter++;
	}

	return FALSE;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam)
{
	char str[512] = {0,};
	::GetWindowText(hwnd,str,sizeof(str));
	if(strlen(str)!=0)
		sGAMECHECK->AddWindowText(str,hwnd);
	return TRUE;
}

DWORD WINAPI GameCheck(LPVOID lpparentet)
{	
	while(TRUE)
	{
		//[每次循环检测发送一次加速检测消息][BY:十里坡剑神][QQ:112582793][2019-6-25][14:47:21]
		//DWORD dwFirst = GetTickCount();
		//Sleep(HACKER_CHECK_TIME);
		//DWORD dwEnd = GetTickCount();

		//if (dwEnd - dwFirst != HACKER_CHECK_TIME)
		//{
		//	MSG_DWORD3BYTE2 msg;
		//	msg.Category = MP_HACKCHECK;
		//	msg.Protocol = MP_HACKE_CHECKMSG5_SYN;
		//	msg.dwObjectID = HEROID;
		//	msg.dwData1 = dwEnd - dwFirst;
		//	sprintf_s(msg.UserID, REDISCONNECT->GetUserAccount());
		//	sprintf_s(msg.UserPwd, REDISCONNECT->GetUserPassWord());
		//	//msg.dwObjectID = REDISCONNECT->GetCharID();
		//	NETWORK->Send(&msg, sizeof(msg));
		//}

		DWORD dwID = WaitForMultipleObjects(2,m_CheckHandle,FALSE,INFINITE);
		if(sGAMECHECK->IsExit())
		{
			return 0;
		}
		switch(dwID)
		{
		case 0:
			{
				sGAMECHECK->GetProcessInfo();
				::EnumWindows(EnumWindowsProc,NULL);
				Sleep(2000);
				SetEvent(m_CheckHandle[1]);
			}
			break;
		case 1:
			{
				if(sGAMECHECK->CheckProcessList())
				{
					sGAMECHECK->SetLock(FALSE);
					sGAMECHECK->SetExitTime();
					return 0;
				}

				if(sGAMECHECK->CheckModuleList())
				{
					sGAMECHECK->SetLock(FALSE);
					sGAMECHECK->SetExitTime();
					return 0;
				}

				if(sGAMECHECK->CheckWindowList())
				{
					sGAMECHECK->SetLock(FALSE);
					sGAMECHECK->SetExitTime();
					return 0;
				}
				sGAMECHECK->SetLock(FALSE);
			}
			break;
		}
	}	
}
DWORD GameCheckManager::GetProcessIdFromName(std::string Name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapshot,&pe))
		return 0;
	std::string name1;
	do 
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		name1 = pe.szExeFile;
		if(name1==Name)
		{
			id = pe.th32ProcessID;
			break;
		}
		if(Process32Next(hSnapshot,&pe)==FALSE)
			break;

	} while (1);
	CloseHandle(hSnapshot);
	return id;
}
BOOL GameCheckManager::KillProcessById(DWORD pID)
{
	HANDLE Hwnd;
	Hwnd = OpenProcess(PROCESS_TERMINATE|SYNCHRONIZE,0,pID);
	if(Hwnd)
	{
		if(TerminateProcess(Hwnd,0))
			return TRUE;
	}
	return FALSE;
}
