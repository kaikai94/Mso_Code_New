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
//         ���汣��       ����BUG     �����޸�					  //
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
	CInitSock initsock;	//Socket��ʼ��
	cNetWork g_NETWORK; //������
	
	PathPrarm  *prarm;
	//����·����ȡ
	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
	std::string GetProcessFullPath(DWORD dwPID);
	//�����༶Ŀ¼
	bool createDirectory(std::string folder);
	//���Ŀ¼�������ļ�������Ŀ¼
	BOOL DoRemoveDirectory(std::string chrDirName);
public:
	Client(void);
	~Client(void);
	static std::vector<std::string> m_FilePath; //�ļ�·����������
	static std::map<std::string,LOCALFILEINFO> m_WatiSendFile; //�跢���ļ���Ϣ
	static std::string TempPathInfo; //��ʱ·��
	GETINSTANCE(Client);
	//�����ļ���С������Ϣ
	std::string MakeFileSizeStr(_fsize_t size);
	//���ý��շ�����IP�Ͷ˿�
	void SetServerInfo(char * IP,int port);
	//��ȡĿ¼����ϸ�ļ���Ϣ���ṹ
	void getFilesAll(std::string path, std::map<std::string,LOCALFILEINFO>& files);
	//��ȡĿ¼�������ļ�·��������
	void getFilesAll(std::string path, std::vector<std::string>& files);
	//��ȡ������Ϣ
	void GetProcessFileInfo();
	//�������̳�����ʱĿ¼
	void CopyProcessFile();
	//��ȡĿ¼�ļ�·��
	void GetPathFileInfo(std::string path,bool SomeFile = false);
	//[ɾ���ļ�][By:ʮ���½���][QQ:112582793][2018/4/5]
	void DoDeleteFile(std::string path,bool SomeFile = false);
	
	//����Ŀ¼�ļ�����ʱĿ¼
	void CopyPathFileInfo(std::string path,bool SomeFile = false);
	//·����ȡ�ļ���
	std::string GetFileName(std::string str);
	std::string GetFilePath(std::string str);
	//���ļ���Ϣд�뵽�ı��ļ�
	void WriteFileInfo();
	//��ȡĿ¼��Ϣ
	void GetPathInfo(std::string path);
	void BeginGetProces();
	void BeginGetPathFile(std::string path,bool SomeFile = false);
	void BeginGetPathInfo(std::string path,bool SomeFile = false);
	void DeleteFileForPath(std::string path,bool SomeFile = false);
	void FuckRubbish();
	void DoDeleteRubbish(std::string path);
	//�̺߳���
	int GetProcess();
	int GetPathFile(PVOID pParm);
	int GetPathInfoT(PVOID pParm);
	int MyDeleteFile(PVOID pParm);
	//����ģʽ
};

#define sClient Client::GetInstance()