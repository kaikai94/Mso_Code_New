#include "stdafx.h"
#include "cNetWork.h"
#include <process.h>
SOCKET cNetWork::sClient;

HANDLE Htrans=NULL;
cNetWork::cNetWork(void)
{

}

cNetWork::~cNetWork(void)
{
}
void cNetWork::CloseSocket()
{
	closesocket(sClient);
}
bool cNetWork::SetServerInfo(char * IP,int port)
{
	sIP=IP;
	sPort=port;

	return true;
}
bool cNetWork::SendFile(FileInfo & fileinfo)
{
	WaitForSingleObject(Htrans,INFINITE); 
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(sPort);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(sIP.c_str());
	sClient = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1  ==:: connect(sClient, (sockaddr*)&sockAddr, sizeof(sockAddr)))
	{
		return false;
	}
	FileInfo* info=new FileInfo;
	strcpy_s(info->file_path,MAX_PATH,fileinfo.file_path);
	strcpy_s(info->PlayerName,16,fileinfo.PlayerName);
	info->file_count = fileinfo.file_count;
	info->GetFromTem = fileinfo.GetFromTem;
	Htrans=(HANDLE) _beginthreadex(NULL,0,cNetWork::file_connect,info,0,NULL);
	return true;
}
std::string cNetWork::GetFileName(std::string str)
{
	std::string result = str;
	size_t Pos = result.find_last_of("\\");
	result.replace(0,Pos+1,"");
	return result;
}
std::string cNetWork::GetFilePath(std::string str)
{
	std::string result = str;
	size_t Pos = result.find_last_of(":");
	result.replace(0,Pos+2,"");
	Pos = result.find_last_of("\\");
	result.replace(Pos+1,result.length(),"");
	return result;
}

unsigned int __stdcall cNetWork:: file_connect(PVOID pM)
{
	std::string FilePath;
	char patbuf[MAX_PATH];
	GetTempPath(MAX_PATH,patbuf);
	FilePath = patbuf;
	FilePath+="mydata\\";
	FileInfo* fileInfo=(FileInfo*)pM;
	if (!fileInfo->GetFromTem)
		FilePath += GetFilePath(fileInfo->file_path);
	FilePath+= cNetWork::GetFileName(fileInfo->file_path);
	HANDLE file;
	DWORD  f_hsize;

	file= CreateFile (FilePath.c_str(), GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if(file == INVALID_HANDLE_VALUE )//尝试打开文件
	{	
		//std::cout<<"文件打开失败"<<std::endl;
		closesocket(sClient);
		return 0;
	}


	SendMsg msg;
	ZeroMemory(&msg,sizeof(SendMsg));

	strcpy_s(msg.file_path,MAX_PATH,fileInfo->file_path);
	strcpy_s(msg.PlayerName,16,fileInfo->PlayerName);
	msg.file_count = fileInfo->file_count;
	
	//msg.op=SEND_FILE;
	char sendBuf[1024];
	ZeroMemory(sendBuf,sizeof(sendBuf));
	memcpy(sendBuf,&msg,sizeof(SendMsg));
	::send(cNetWork::sClient,sendBuf,sizeof(sendBuf),0);
	char recBuf[10];
	ZeroMemory(recBuf,sizeof(recBuf));
	int ret=::recv(cNetWork::sClient,recBuf,sizeof(recBuf),0);
	if(ret<=0)
	{
		CloseHandle(file);
		delete fileInfo;
		closesocket(sClient);
		return 0;
	}

	char buf[1024]="\0";
	DWORD nBytesRead;
	
	do 
	{
		ReadFile(file,buf, sizeof(buf),&nBytesRead,NULL);	
		if(SOCKET_ERROR  == ::send(cNetWork::sClient, buf, nBytesRead, MSG_PARTIAL))
		{
			CloseHandle(file);
			delete fileInfo;
			closesocket(cNetWork::sClient);
			return 0;		
		}
		if(nBytesRead < sizeof(buf))//到达文件末尾
			break;

	} while(TRUE);

	CloseHandle(file);
	//DeleteFile(FilePath.c_str());
	closesocket(sClient);
	delete fileInfo;
	return 1;
}