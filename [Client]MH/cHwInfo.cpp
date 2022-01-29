#include "stdafx.h"
#include "cHwInfo.h"
#include "md5.h"

cHwInfo::cHwInfo()
{
}


cHwInfo::~cHwInfo()
{
}
std::string&   cHwInfo::erase_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
{
	while (true)   {
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.erase(pos, old_value.length());
		else   break;
	}
	return   str;
}
BOOL cHwInfo::ExeCoutionCmd(char* szCmd, std::string strEnSearch,CmdExeType eType)
{
	BOOL bRet = FALSE;
	
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;

	char szBuffer[MAX_COMMAND_SIZE + 1] = { 0, };
	std::string strBuffer;
	unsigned long count = 0;
	long ipos = 0;

	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	bRet = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bRet)
	{
		goto lbReturn;
	}

	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	
	bRet = CreateProcess(NULL, szCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bRet)
	{
		goto lbReturn;
	}

	WaitForSingleObject(pi.hProcess, 500);
	bRet = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bRet)
	{
		goto lbReturn;
	}
	bRet = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch);
	if (ipos < 0)
	{
		goto lbReturn;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
	}
	//int index = 0;
	/*if (!strBuffer.empty())
	{
		while ((index = strBuffer.find(' ', index)) != std::string::npos)
		{
			strBuffer.erase(index, 1);
		}
	}*/
	strBuffer = erase_all(strBuffer, "\n", "");
	strBuffer = erase_all(strBuffer, "\r", "");
	strBuffer = erase_all(strBuffer, " ", "");
	switch (eType)
	{
	case eGetCpuInfo:

		m_CPUID = strBuffer;
		break;
	case eGetBoardInfo:
		m_BoardID = strBuffer;
		break;
	default:
		goto lbReturn;
		break;
	}
	bRet = TRUE;
lbReturn:
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return bRet;
}

BOOL cHwInfo::MakeHwID()
{
	BOOL bRet = FALSE;
	char szCmd[] = "wmic cpu get processorid";
	bRet = ExeCoutionCmd("wmic cpu get processorid", "ProcessorId", eGetCpuInfo);
	bRet = ExeCoutionCmd("wmic baseboard get serialnumber", "SerialNumber", eGetBoardInfo);
	std::string HwBuff = m_CPUID + m_BoardID;
	MD5_CTX md5;
	bRet = md5.GetStringMd5(m_HwID, (unsigned char*)HwBuff.c_str(), HwBuff.length());
	return bRet;
}