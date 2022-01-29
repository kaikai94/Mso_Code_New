#include "StdAfx.h"
#include "cTextParsing.h"
#include <iostream>
#include <io.h>

cTextParsing::cTextParsing(void)
{
}

cTextParsing::~cTextParsing(void)
{
}
void cTextParsing::getFilesAll(std::wstring path, std::vector<std::wstring>& files)
{
	long hFile = 0;
	//文件信息
	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(path).append(L"\\*").c_str(), &fileinfo)) != -1)
	{
		do {
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (_tcscmp(fileinfo.name, L".") != 0 && _tcscmp(fileinfo.name, L"..") != 0)
				{
					getFilesAll(p.assign(path).append(L"\\").append(fileinfo.name), files);
				}
			} 
			else 
			{  
				files.push_back(p.assign(path).append(L"\\").append(fileinfo.name));
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void cTextParsing::GetJustCurrentDir(std::wstring path, std::vector<std::wstring>& files)
{
	long hFile  = 0;
	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(path).append(L"\\*").c_str(), &fileinfo)) != -1)
	{
		do 
		{
			if ((fileinfo.attrib & _A_SUBDIR)) 
			{  
				if (_tcscmp(fileinfo.name, L".") != 0 && _tcscmp(fileinfo.name, L"..") != 0)
				{
					files.push_back(fileinfo.name);
				}
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
}
std::vector<std::wstring> cTextParsing::split(std::wstring& str, const TCHAR* c)
{
	TCHAR *cstr = NULL;
	TCHAR *p = NULL;
	TCHAR * buff = NULL;
	std::vector<std::wstring> res;
	cstr = new TCHAR[str.size()+1];
	_tcscpy_s(cstr, str.size() + 1,str.c_str());
	p = _tcstok_s(cstr,c,&buff);
	while(p!=NULL)
	{
		res.push_back(p);
		p = _tcstok_s(NULL, c, &buff);
	}
	if (p != NULL)
	{
		delete[] p;
		p = NULL;
	}
	if (cstr != NULL)
	{
		delete[]cstr;
		cstr = NULL;
	}

	return res;
}
std::wstring&   cTextParsing::replace_all(std::wstring&   str, const   std::wstring&   old_value, const   std::wstring&   new_value)
{     
	while (true)   {
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}