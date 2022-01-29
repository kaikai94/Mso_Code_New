#pragma once	
#include"intialSoket.h"
//#include<Windows.h>
 
#include "StructDef.h"
#include <string>

class cNetWork
{
public:
	
	cNetWork(void);
	~cNetWork(void);
	CInitSock	initsock;
	
	bool start_listen(char * ip,int port);
	bool process_con_msg( SOCKET& socket)	;
	bool apply_file_recieve(SOCKET& socket,Msg*& msgBase);
	void CloseSocket();
	int accepeClinet();
	void InitVal(){filecount=0;SetProgress=true;}
private:
	SOCKET lis_socket_;
public:
	static int filecount;
	static bool SetProgress;
	static unsigned int __stdcall file_connect(PVOID pM);
	static unsigned int __stdcall  accept_fun(PVOID pM);
	static unsigned int __stdcall recieve_fun(PVOID pM);
	static bool createDirectory(std::string folder);
	static std::string GetFileName(std::string str);
	static std::string GetFilePath(std::string str);
	//生成文件大小文字信息
	static std::string MakeFileSizeStr(_fsize_t size);
};

