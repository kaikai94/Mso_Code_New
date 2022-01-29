#pragma once
#include <string>
#include <windows.h>
#define HWINFO cHwInfo::GetInstance()
const long MAX_COMMAND_SIZE = 10000;
enum CmdExeType
{
	eGetCpuInfo,
	eGetBoardInfo,
};
class cHwInfo
{
	std::string m_CPUID;
	std::string m_BoardID;
	std::string m_HwID;
public:
	cHwInfo();
	~cHwInfo();
	GETINSTANCE(cHwInfo);
	std::string&   erase_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);
	BOOL ExeCoutionCmd(char* szCmd, std::string strEnSearch, CmdExeType eType);
	BOOL MakeHwID();
	std::string GetHwID(){ return m_HwID; }
};

