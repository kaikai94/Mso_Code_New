#include "stdafx.h"
#include "CheckResourceFile.h"
#include <fstream>
#include <WinInet.h>
TCHAR FullPath[MAX_PATH];
CheckResourceFile::CheckResourceFile(void)
{
	GetCurrentDirectory(MAX_PATH,FullPath);
}

CheckResourceFile::~CheckResourceFile(void)
{
}
bool CheckResourceFile::GetMyUpData(std::wstring Url)
{
	try
	{
		HINTERNET hINet,hHttpFile;
		TCHAR szSizeBuffer[32]={0,};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		hINet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
		if(!hINet)
			return false;
		hHttpFile = InternetOpenUrl(hINet,Url.c_str(),NULL,0,INTERNET_FLAG_RELOAD,0);
		if(!hHttpFile)
			return false;
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH,szSizeBuffer,&dwLengthSizeBuffer,NULL);
		int FileSize = _wtol(szSizeBuffer);
		m_MyUpdata.resize(FileSize);
		char * buff = NULL;
		buff = new char[FileSize + 1];
		memset(buff, 0, FileSize + 1);

		DWORD dwByteRead;
		BOOL bRead = InternetReadFile(hHttpFile, buff, FileSize, &dwByteRead);
		if(!bRead)
			return false;
		m_MyUpdata = m_CodePage.AsciiToUnicode(buff);
		delete[] buff;
		buff = NULL;
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hINet);
		return true;
	}
	catch (...)
	{
		MessageBox(NULL,L"网络异常，获取更新信息失败",L"错误",MB_OK);
		return false;
	}
}
void CheckResourceFile::CheckFile(std::vector<std::wstring>& WatiDownLoadFile)
{
	std::wstring Path, Md5;
	std::vector<std::wstring>Tmp;
	std::map<std::wstring, std::wstring>::iterator miter;

	std::vector<std::wstring> vFileList, vFileInfo;

	vFileList = TextPars.split(m_MyUpdata,L"#\r\n");

	std::vector<std::wstring>::iterator iter;
	iter = vFileList.begin();
	while(1)
	{
		if(iter==vFileList.end())
			break;
		else
		{
			vFileInfo=TextPars.split(*iter,L"|");
			if(vFileInfo.size()==2)
			{
				Path = FullPath;
				m_PatchFile[vFileInfo[0]]=vFileInfo[1];
				Path+=vFileInfo[0];
				if (MakeMD5.GetFileMd5(Md5, m_CodePage.UnicodeToAscii(Path).c_str()))
				{
					miter= m_PatchFile.find(vFileInfo[0]);
					if(miter!=m_PatchFile.end())
					{
						if(miter->second!=Md5)
						{
							WatiDownLoadFile.push_back(miter->first);
						}
					}
				}
				else
				{
					WatiDownLoadFile.push_back(vFileInfo[0]);
				}
				Md5.clear();
				vFileInfo.clear();
			}
		}
		iter++;
	}
}
std::wstring CheckResourceFile::GetFileName(std::wstring strPatch)
{
	std::wstring temp(strPatch);
	int pos = temp.find_last_of(L"\\");
	temp.replace(0, pos + 1, L"");
	return temp;
}
