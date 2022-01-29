#include "stdafx.h"
#include "CodePageMgr.h"


CodePageMgr::CodePageMgr()
{
}


CodePageMgr::~CodePageMgr()
{
}

std::wstring CodePageMgr::AsciiToUnicode(const std::string& str) {
	// Ԥ��-�������п��ֽڵĳ���    
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// ��ָ�򻺳�����ָ����������ڴ�    
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// ��ʼ�򻺳���ת���ֽ�    
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}
std::string CodePageMgr::UnicodeToAscii(const std::wstring& wstr) {
	// Ԥ��-�������ж��ֽڵĳ���    
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// ��ָ�򻺳�����ָ����������ڴ�    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// ��ʼ�򻺳���ת���ֽ�    
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}
std::wstring CodePageMgr::Utf8ToUnicode(const std::string& str) {
	// Ԥ��-�������п��ֽڵĳ���    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	// ��ָ�򻺳�����ָ����������ڴ�    
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// ��ʼ�򻺳���ת���ֽ�    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}
std::string CodePageMgr::UnicodeToUtf8(const std::wstring& wstr) {
	// Ԥ��-�������ж��ֽڵĳ���    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// ��ָ�򻺳�����ָ����������ڴ�    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// ��ʼ�򻺳���ת���ֽ�    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}
std::string CodePageMgr::AsciiToUtf8(const std::string& str) {
	return UnicodeToUtf8(AsciiToUnicode(str));
}
std::string CodePageMgr::Utf8ToAscii(const std::string& str) {
	return UnicodeToAscii(Utf8ToUnicode(str));
}
// ASCII��Unicode��ת  
CStringW    CodePageMgr::AsciiToUnicode_CSTR(const CStringA& str) {
	return AsciiToUnicode(LPCSTR(str)).c_str();
}
CStringA    CodePageMgr::UnicodeToAscii_CSTR(const CStringW& wstr) {
	return UnicodeToAscii(LPCWSTR(wstr)).c_str();
}
// UTF8��Unicode��ת  
CStringW    CodePageMgr::Utf8ToUnicode_CSTR(const CStringA& str) {
	return Utf8ToUnicode(LPCSTR(str)).c_str();
}
CStringA   CodePageMgr::UnicodeToUtf8_CSTR(const CStringW& wstr) {
	return UnicodeToUtf8(LPCWSTR(wstr)).c_str();
}
// ASCII��UTF8��ת  
CStringA    CodePageMgr::AsciiToUtf8_CSTR(const CStringA& str) {
	return UnicodeToUtf8_CSTR(AsciiToUnicode_CSTR(str));
}
CStringA   CodePageMgr::Utf8ToAscii_CSTR(const CStringA& str) {
	return UnicodeToAscii_CSTR(Utf8ToUnicode_CSTR(str));
}
// string �� Int ��ת  
int CodePageMgr::StringToInt(const std::string& str) {
	return atoi(str.c_str());
}
std::string CodePageMgr::IntToString(int i) {
	char ch[1024];
	memset(ch, 0, 1024);
	sprintf_s(ch, sizeof(ch), "%d", i);
	return ch;
}
std::string CodePageMgr::IntToString(char i) {
	char ch[1024];
	memset(ch, 0, 1024);
	sprintf_s(ch, sizeof(ch), "%c", i);
	return ch;
}
std::string CodePageMgr::IntToString(double i) {
	char ch[1024];
	memset(ch, 0, 1024);
	sprintf_s(ch, sizeof(ch), "%f", i);
	return ch;
}
