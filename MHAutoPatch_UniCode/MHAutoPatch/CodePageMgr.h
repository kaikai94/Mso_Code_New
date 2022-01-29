#pragma once
#include <string>
class CodePageMgr
{
public:
	CodePageMgr();
	~CodePageMgr();
	// ASCII与Unicode互转  
	std::wstring AsciiToUnicode(const std::string& str);
	std::string  UnicodeToAscii(const std::wstring& wstr);
	// UTF8与Unicode互转  
	std::wstring Utf8ToUnicode(const std::string& str);
	std::string  UnicodeToUtf8(const std::wstring& wstr);
	// ASCII与UTF8互转  
	std::string  AsciiToUtf8(const std::string& str);
	std::string  Utf8ToAscii(const std::string& str);
	//**************CString******************//  
	// ASCII与Unicode互转  
	CStringW    AsciiToUnicode_CSTR(const CStringA& str);
	CStringA    UnicodeToAscii_CSTR(const CStringW& wstr);
	// UTF8与Unicode互转  
	CStringW    Utf8ToUnicode_CSTR(const CStringA& str);
	CStringA    UnicodeToUtf8_CSTR(const CStringW& wstr);
	// ASCII与UTF8互转  
	CStringA    AsciiToUtf8_CSTR(const CStringA& str);
	CStringA    Utf8ToAscii_CSTR(const CStringA& str);
	/************string-int***************/
	// string 转 Int  
	int StringToInt(const std::string& str);
	std::string IntToString(int i);
	std::string IntToString(char i);
	std::string IntToString(double i);
};