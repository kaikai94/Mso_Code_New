#pragma once
#include <vector>
#include <map>
#define sGAMECHECK GameCheckManager::GetInstance()
struct PROCINFO
{
	DWORD dwPId;
	std::string  ProcessName;
	std::string  ProcessPath;
};
class GameCheckManager
{
	std::map<std::string,PROCINFO> m_ProcessInfo;
	std::map<std::string,std::string> m_ModuleInfo;
	std::map<std::string,HWND>	m_WindowsText;

	std::vector<std::string> m_ProcessList;
	std::vector<std::string> m_ModuleList;
	std::vector<std::string> m_WindwosList;

	BOOL bIsExit;
	BOOL bIsInit;
	DWORD dwCheckTime;
	BOOL bLock;
	DWORD m_ExitTime;
public:
	GETINSTANCE(GameCheckManager);
	GameCheckManager(void);
	~GameCheckManager(void);
	void	InitGameCheck();
	BOOL GetProcessInfo();
	BOOL GetModuleInfo(DWORD dwPId);
	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
	std::string GetProcessFullPath(DWORD dwPID);
	void Process();
	void AddWindowText(std::string str,HWND hwnd);
	void LoadCheckList();
	BOOL CheckProcessList();
	BOOL CheckModuleList();
	BOOL CheckWindowList();
	void SetExit(BOOL val){bIsExit = val;}
	BOOL IsExit(){return bIsExit;}
	void SetEventHandle();
	void SetLock(BOOL val){bLock= val;}
	void SetExitTime();
	BOOL CheckAutoPatch();
	DWORD GetProcessIdFromName(std::string Name);
	BOOL KillProcessById(DWORD pID);
};
