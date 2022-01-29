#include "stdafx.h"
#include "cNetWork.h"
#include <process.h> 
#include <direct.h>
#include <io.h>
//进度条范围设置需包含此头文件
#include <commctrl.h>        //InitCommonControls
#pragma comment(lib, "comctl32")
#define PATH_DELIMITER '\\'
extern HINSTANCE hInst;								// 当前实例
extern HWND hWnd;
//全局变量
extern HWND List_Msg;
extern HWND Progress;
extern TCHAR Port[];
extern TCHAR TipMsg[];
extern int port;
extern bool StopTheard;
extern HWND Progresscount;
int cNetWork::filecount;
bool cNetWork::SetProgress;
extern HWND Progress1;
cNetWork::cNetWork(void)
{
	filecount=0;
	SetProgress = true;
}

cNetWork::~cNetWork(void)
{
}
void cNetWork::CloseSocket()
{
	closesocket(lis_socket_);
}
bool cNetWork::start_listen(char * ip,int port)
{
	lis_socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(lis_socket_ == INVALID_SOCKET){
		int err = 0;
		err = GetLastError();
		return false;
	}
	// 填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = inet_addr(ip);

	// 绑定这个套节字到一个本地地址

	if(::bind(lis_socket_, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR){	
		sprintf_s(TipMsg,256,"套接字绑定失败,错误代码:%d",GetLastError());
		SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);

		return false;
	}
	return true;
}

int cNetWork::accepeClinet()
{
	if(::listen(lis_socket_,5)==SOCKET_ERROR){
		sprintf_s(TipMsg,256,"开启监听失败,错误代码:%d",GetLastError());
		SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
		return false;
	};		
	HANDLE Hacc=(HANDLE) _beginthreadex(NULL,0,accept_fun,this,0,NULL);
	WaitForSingleObject(Hacc,INFINITE); 
	return true;

}
unsigned int __stdcall  cNetWork:: accept_fun(PVOID pM)
{
	cNetWork* transfer=(cNetWork*)pM;
	sockaddr_in client;
	int sClientLength = sizeof(client);
	SOCKET sClient = ::accept(transfer->lis_socket_,(sockaddr*)&client, &sClientLength);
	transfer->process_con_msg(sClient);
	return 0;
}
bool cNetWork::createDirectory(std::string folder) {
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
	result.replace(0,Pos+1,"");
	Pos = result.find_last_of("\\");
	result.replace(Pos+1,result.length(),"");
	return result;
}
bool cNetWork::process_con_msg( SOCKET& socket)	
{

	char* recBuf=new char[1024];
	ZeroMemory(recBuf,sizeof(1024));
	int nRec=::recv(socket,recBuf,1024,0);
	if(nRec<=0)
	{
		return false;
	}
	Msg* msgBase=(Msg*)recBuf;
	if(msgBase->op==10086)
	{
		sprintf_s(TipMsg,256,"文件已经发送完毕");
		SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
		closesocket(socket);
		return true;
	}
	apply_file_recieve(socket,msgBase);
	return true;
}

bool cNetWork::apply_file_recieve(SOCKET& socket,Msg*& msgBase)
{
	SendMsg* msgFile=(SendMsg*)msgBase;

	RecPrarm* thPrarm=new RecPrarm;
	thPrarm->msg=msgFile;
	thPrarm->socket=socket;
	sprintf_s(TipMsg,256,"--->开始接收玩家[%s]发送的文件,文件名[%s]<---",msgFile->PlayerName,(GetFileName(msgFile->file_path)).c_str());
	SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);

	HANDLE Hrec=(HANDLE) _beginthreadex(NULL,0,recieve_fun,thPrarm,0,NULL);
	WaitForSingleObject(Hrec,INFINITE); 
	return true;
}

unsigned int __stdcall cNetWork::recieve_fun(PVOID pM)
{
	std::string SavePath;
	SavePath.clear();
	RecPrarm* thrPar=(RecPrarm*)pM;
	SOCKET socket=thrPar->socket;
	SendMsg* msg=thrPar->msg;

	SavePath = ".\\";
	SavePath +=msg->PlayerName;
	SavePath += "\\";
	SavePath += GetFilePath(msg->file_path);
	createDirectory(SavePath);
	SavePath += GetFileName(msg->file_path);

	char sendBuf[10];
	::send(socket,sendBuf,sizeof(sendBuf),0);

	HANDLE	file = CreateFile ( SavePath.c_str(), GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( file == INVALID_HANDLE_VALUE )
	{
		sprintf_s(TipMsg,256,"--->文件创建失败[%s],路径[%s]<---",(GetFileName(msg->file_path)).c_str(),SavePath.c_str());
		SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
		return 0 ;
	}

	char buf[1024]={0,};
	BOOL bRead = TRUE;
	DWORD nbytes;
	DWORD nPos = 0;

	sprintf_s(TipMsg,256,"--->开始接收文件[%s]<---",(GetFileName(msg->file_path)).c_str());
	SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);

	sprintf_s(TipMsg,256,"[%d]/[%d]",cNetWork::filecount,msg->file_count);
	SendMessage(Progresscount,WM_SETTEXT,0,(LPARAM)TipMsg);

	SendMessage(Progress1,PBM_SETRANGE32,0,msg->file_count);
	if (cNetWork::SetProgress)
	{
		SendMessage(Progress1,PBM_SETPOS,0,0);
		cNetWork::SetProgress = false;
	}
	while(bRead)
	{
		int nResult = recv(socket, buf, 1024, MSG_PARTIAL);

		switch(nResult)
		{
		case 0:
			bRead = FALSE;//文件传输完毕
			break;
		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				sprintf_s(TipMsg,256,"--->传输错误[%d]<---",GetLastError());
				SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);
				bRead = FALSE;
			}
			else
			{
				break;
			}
		default:
			WriteFile(file,buf,nResult,&nbytes,NULL);
		}

		nPos += nResult;
		std::string str = MakeFileSizeStr(nPos);
		strcpy_s(TipMsg,256,str.c_str());
		SendMessage(Progress,WM_SETTEXT,0,(LPARAM)TipMsg);
	}
	cNetWork::filecount++;
	sprintf_s(TipMsg,256,"[%d]/[%d]",cNetWork::filecount,msg->file_count);
	SendMessage(Progresscount,WM_SETTEXT,0,(LPARAM)TipMsg);
	SendMessage(Progress1,PBM_SETPOS,cNetWork::filecount,0);
	if (cNetWork::filecount==msg->file_count)
	{
		sprintf_s(TipMsg,256,"--->接收完毕<---");
		SendMessage(Progress,WM_SETTEXT,0,(LPARAM)TipMsg);
		cNetWork::filecount=0;
		cNetWork::SetProgress = true;
	}
	CloseHandle ( file )  ;
	
	sprintf_s(TipMsg,256,"--->文件[%s]接收完成<---",(GetFileName(msg->file_path)).c_str());
	SendMessage(List_Msg,LB_SETCURSEL,SendMessage(List_Msg,LB_INSERTSTRING,-1,(LPARAM)TipMsg),0);

	closesocket(socket);
	delete thrPar;
	return 1;

}
std::string cNetWork::MakeFileSizeStr(_fsize_t size)
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
		sprintf_s(buf,512,"%.1f字节",temp);
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
		sprintf_s(buf,512,"这玩意儿有点大");
	}
	result = buf;
	return result;
}
