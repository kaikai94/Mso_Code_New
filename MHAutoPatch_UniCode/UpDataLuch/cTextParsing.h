#pragma once
#include <string>
#include <vector>
class cTextParsing
{
public:
	cTextParsing(void);
public:
	~cTextParsing(void);
	void getFilesAll(std::string path, std::vector<std::string>& files);
	void GetJustCurrentDir(std::string path,std::vector<std::string>& files);
	std::vector<std::string> split(std::string& str,const char* c);
	std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)   ;
};
