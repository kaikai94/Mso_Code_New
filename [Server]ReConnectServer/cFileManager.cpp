#include "StdAfx.h"
#include "cFileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <io.h>
#include "Console.h"
#include "AddableInfo.h"
#include "Network.h"
cFileManager::cFileManager(void)
{
	std::ifstream Infile;
	std::locale::global(std::locale(""));
	Infile.open("./PathDirectory.txt");
	std::locale::global(std::locale("C"));
	if(!Infile.is_open())
		g_Console.LOGEX(4,RGB(255,0,0),"补丁路径读取失败");
	std::stringstream os;
	os<<Infile.rdbuf();
	
	strPathDirectory = os.str();
	
	g_Console.LOGEX(4,RGB(0,255,0),"当前补丁路径[%s]",strPathDirectory.c_str());
	
	Infile.close();
}

cFileManager::~cFileManager(void)
{
}
void cFileManager::Init()
{
	std::vector<std::string> FileList;
	getFilesAll(strPathDirectory,FileList);
	g_Console.LOGEX(4,RGB(0,255,0),"补丁文件数量[%d]",FileList.size());
	std::vector<std::string>::iterator vIter;
	vIter = FileList.begin();
	while(1)
	{
		if(vIter==FileList.end())
			break;
		else
			m_PathFileList[GetFileName(*vIter)] = *vIter;
		vIter++;
	}
}
void cFileManager::ReLoadFileInfo()
{
	m_PathFileList.clear();
	Init();
}
bool cFileManager::SendVerFile(DWORD dwConnectionIndex)
{
	std::string strVerPatch;
	strVerPatch = strPathDirectory;
	strVerPatch+="\\MyUpdata.txt";

	HANDLE file;
	DWORD  f_hsize;
	file= CreateFile (strVerPatch.c_str(), GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if(file == INVALID_HANDLE_VALUE )
	{
		g_Console.LOGEX(4,RGB(255,0,0),"文件[MyUpdata.txt]打开失败");
		return false;
	}
	f_hsize = GetFileSize(file,NULL);

	//[发送文件信息][BY:十里坡剑神][QQ:112582793][2019-5-1][3:23:13]
	MSGFILEINFO msgStart;
	msgStart.Category = MP_AUTOPATH;
	msgStart.Protocol = MP_AUTOPATH_FILEINFO_ACK;
	sprintf_s(msgStart.strFileName,"MyUpdata.txt",MAX_PATH);
	msgStart.dwDataSize = f_hsize;
	g_Network.Send2User(dwConnectionIndex,(char*)&msgStart,sizeof(msgStart));

	char buffer[10240]={0,};
	DWORD nBytesRead = 0;
	while(1)
	{
		ReadFile(file,buffer, sizeof(buffer),&nBytesRead,NULL);	
		if(nBytesRead<=0)
			break;
		MSGFILEDATA msg;
		msg.Category = MP_AUTOPATH;
		msg.Protocol = MP_AUTOPATH_GETFILE_ACK;
		memcpy(msg.Data,&buffer,nBytesRead);
		msg.dwDataSize = nBytesRead;
		sprintf_s(msg.strFileName,"MyUpdata.txt",MAX_PATH);
		g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
		Sleep(50);
	}
	CloseHandle(file);
	MSGFILEINFO msgEnd;
	msgEnd.Category = MP_AUTOPATH;
	msgEnd.Protocol = MP_AUTOPATH_FILEINFO_DONE;
	sprintf_s(msgEnd.strFileName,"MyUpdata.txt",MAX_PATH);
	g_Network.Send2User(dwConnectionIndex,(char*)&msgEnd,sizeof(msgEnd));
	return true;
}
bool cFileManager::SendFile(DWORD dwConnectionIndex,std::string strFileName)
{
	PathFile::iterator Iter;
	Iter = m_PathFileList.find(strFileName);
	if(Iter==m_PathFileList.end())
		return false;
	HANDLE file;
	DWORD  f_hsize;
	file= CreateFile (Iter->second.c_str(), GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if(file == INVALID_HANDLE_VALUE )
	{
		g_Console.LOGEX(4,RGB(255,0,0),"文件[%s]打开失败",strFileName.c_str());
		return false;
	}
	f_hsize = GetFileSize(file,NULL);

	MSGFILEINFO msgStart;
	msgStart.Category = MP_AUTOPATH;
	msgStart.Protocol = MP_AUTOPATH_FILEINFO_ACK;
	sprintf_s(msgStart.strFileName,strFileName.c_str(),MAX_PATH);
	msgStart.dwDataSize = f_hsize;
	g_Network.Send2User(dwConnectionIndex,(char*)&msgStart,sizeof(msgStart));

	char buffer[10240]={0,};
	DWORD nBytesRead = 0;
	while(1)
	{
		ReadFile(file,buffer, sizeof(buffer),&nBytesRead,NULL);	
		if(nBytesRead<=0)
			break;
		MSGFILEDATA msg;
		msg.Category = MP_AUTOPATH;
		msg.Protocol = MP_AUTOPATH_GETFILE_ACK;
		memcpy(msg.Data,&buffer,nBytesRead);
		msg.dwDataSize = nBytesRead;
		sprintf_s(msg.strFileName,strFileName.c_str(),MAX_PATH);
		g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
		Sleep(50);
	}
	CloseHandle(file);
	MSGFILEINFO msgEnd;
	msgEnd.Category = MP_AUTOPATH;
	msgEnd.Protocol = MP_AUTOPATH_FILEINFO_DONE;
	sprintf_s(msgEnd.strFileName,strFileName.c_str(),MAX_PATH);
	g_Network.Send2User(dwConnectionIndex,(char*)&msgEnd,sizeof(msgEnd));
	return true;
}
std::string cFileManager::GetFileName(std::string strPatch)
{
	std::string temp(strPatch);
	int pos = temp.find_last_of("\\");
	temp.replace(0,pos+1,"");
	return temp;
}
void cFileManager::getFilesAll(std::string path, std::vector<std::string>& files)
{
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;  
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) 
	{
		do {
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0) 
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

void cFileManager::GetJustCurrentDir(std::string path,std::vector<std::string>& files)
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
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) 
				{
					files.push_back(fileinfo.name);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}
}
std::vector<std::string> cFileManager::split(std::string& str,const char* c)
{
	char *cstr, *p;
	std::vector<std::string> res;
	cstr = new char[str.size()+1];
	strcpy(cstr,str.c_str());
	p = strtok(cstr,c);
	while(p!=NULL)
	{
		res.push_back(p);
		p = strtok(NULL,c);
	}
	delete [] p ;
	p = NULL;
	return res;
}
std::string&   cFileManager::replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
	while(true)   {     
		std::string::size_type   pos(0);     
		if(   (pos=str.find(old_value))!=std::string::npos   )     
			str.replace(pos,old_value.length(),new_value);     
		else   break;     
	}     
	return   str;     
}