#pragma once
#include <vector>
#include <map>
#include "cTextParsing.h"
#include "md5.h"
#include "CodePageMgr.h"
class CheckResourceFile
{
	MD5_CTX		 MakeMD5;
	cTextParsing TextPars;
	std::wstring  m_MyUpdata;
	std::map<std::wstring, std::wstring> m_PatchFile;
public:
	CheckResourceFile(void);
	~CheckResourceFile(void);
	void CheckFile(std::vector<std::wstring>& WatiDownLoadFile);
	bool GetMyUpData(std::wstring Url);
	std::wstring GetFileName(std::wstring strPatch);
public:
	CodePageMgr	 m_CodePage;
};
