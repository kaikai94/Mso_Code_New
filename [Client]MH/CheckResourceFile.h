#pragma once
#include <vector>
#include <map>
#include "md5.h"
#define FILECHECK USINGTON(CheckResourceFile)
class CheckResourceFile
{
	MD5_CTX	 MakeMD5;
	std::string  m_MyUpdata;
	std::map<std::string,std::string> m_PatchFile;
	std::string m_strUrl;
	std::wstring m_BIOSVersion;
	std::wstring m_BaseBoard;
	std::string m_City;
	std::string m_Province;
	bool m_bIsInit;
	bool m_bIsDangerous;
	bool m_bInsideVMWare;
	bool m_bIsSend;
public:
	CheckResourceFile(void);
	~CheckResourceFile(void);
	bool InitFile();
	bool CheckFile(std::string);
	bool GetMyUpData(std::string);
	std::vector<std::string> split(std::string&,const char*);
	std::string GetFileName(std::string);
	void LoadUrl();
	void DeleteFileForDmp(std::string path);
	bool CheckLocal();
	void CheckComputer();
	void SendCheckResult();
	std::string  UnicodeToAscii(const std::wstring& wstr);
	void GetLocalInfo(char * msg);
	void SetSendInit(){ m_bIsSend = false; }
};
EXTERNGLOBALTON(CheckResourceFile);