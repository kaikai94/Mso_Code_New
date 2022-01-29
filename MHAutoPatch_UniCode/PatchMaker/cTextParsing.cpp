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
void cTextParsing::getFilesAll(std::string path, std::vector<std::string>& files)
{
	long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	std::string p;  
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) 
	{
		do {
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (_tcscmp(fileinfo.name,".") != 0  &&  _tcscmp(fileinfo.name,"..") != 0) 
				{
					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			} 
			else 
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void cTextParsing::GetJustCurrentDir(std::string path,std::vector<std::string>& files)
{
	long hFile  = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1)
	{
		do 
		{
			if ((fileinfo.attrib & _A_SUBDIR)) 
			{  
				if (_tcscmp(fileinfo.name, ".") != 0 && _tcscmp(fileinfo.name, "..") != 0) 
				{
					files.push_back(fileinfo.name);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}
}
std::vector<std::string> cTextParsing::split(std::string& str,const TCHAR* c)
{
	TCHAR *cstr, *p;
	std::vector<std::string> res;
	cstr = new TCHAR[str.size()+1];
	_tcscpy(cstr,str.c_str());
	p = _tcstok(cstr,c);
	while(p!=NULL)
	{
		res.push_back(p);
		p = _tcstok(NULL,c);
	}
	return res;
}
std::string&   cTextParsing::replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
	while(true)   {     
		std::string::size_type   pos(0);     
		if(   (pos=str.find(old_value))!=std::string::npos   )     
			str.replace(pos,old_value.length(),new_value);     
		else   break;     
	}     
	return   str;     
}