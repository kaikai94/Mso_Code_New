#pragma once
#include <string>
#include <vector>
class cTextParsing
{
public:
	cTextParsing(void);
public:
	~cTextParsing(void);
	void getFilesAll(std::wstring path, std::vector<std::wstring>& files);
	void GetJustCurrentDir(std::wstring path, std::vector<std::wstring>& files);
	std::vector<std::wstring> split(std::wstring& str, const TCHAR* c);
	std::wstring&   replace_all(std::wstring&   str, const   std::wstring&   old_value, const   std::wstring&   new_value);
};
