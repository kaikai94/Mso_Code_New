#pragma once
#include <string>
#include <map>
#include <vector>
#define FILEMGR cFileManager::GetInstance()
typedef std::map<std::string,std::string> PathFile;
class cFileManager
{
	std::string strPathDirectory;//[补丁路径][BY:十里坡剑神][QQ:112582793][2019-4-30][19:18:40]
	PathFile m_PathFileList;
public:
	cFileManager(void);
	~cFileManager(void);
	GETINSTANCE(cFileManager);
	void Init();
	void getFilesAll(std::string path, std::vector<std::string>& files);
	void GetJustCurrentDir(std::string path,std::vector<std::string>& files);
	std::vector<std::string> split(std::string& str,const char* c);
	std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value);
	bool SendFile(DWORD dwConnectionIndex,std::string strFileName);
	bool SendVerFile(DWORD dwConnectionIndex);
	std::string GetFileName(std::string strPatch);
	void ReLoadFileInfo();
};
