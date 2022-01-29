#include "stdafx.h"
#include "cChangeCharCode.h"
#include <Winnls.h>
#pragma comment(lib,"Kernel32.lib")
cChangeCharCode::cChangeCharCode(void)
{

}

cChangeCharCode::~cChangeCharCode(void)
{
}
void cChangeCharCode::Init()
{
	IsSimple = FALSE;
	LANGID lid = GetSystemDefaultLangID();

	if(lid == 0x0804||lid==0x1004)
		IsSimple = TRUE;
}
char* cChangeCharCode::ChangeCharCode(char* szString)
{
	char * buff=NULL;
#ifdef _USEBIG5_
	if(IsSimple)
	{
		buff = GB2312ToBIG5(szString);
	}
#else
	if(!IsSimple)
	{
		buff = GB2312ToBIG5(szString);
	}
	else
		buff = szString;
#endif
	
	return buff;
}
char* cChangeCharCode::BIG5ToGB2312(const char* szBIG5String)
{

	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);

	wchar_t* szUnicodeBuff = BIG5ToUnicode(szBIG5String);

	char* szGB2312Buff = UnicodeToGB2312(szUnicodeBuff);

	int nLength = LCMapString(lcid,LCMAP_SIMPLIFIED_CHINESE, szGB2312Buff,-1,NULL,0);

	char* pBuffer = new char[nLength + 1];

	LCMapString(0x0804,LCMAP_SIMPLIFIED_CHINESE,szGB2312Buff,-1,pBuffer,nLength);

	pBuffer[nLength] = 0;


	delete[] szUnicodeBuff;

	delete[] szGB2312Buff;

	return pBuffer;

}
char* cChangeCharCode::UnicodeToGB2312(const wchar_t* szUnicodeString)
{

	UINT nCodePage = 936; //GB2312

	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);

	char* pBuffer=new char[nLength+1];

	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);

	pBuffer[nLength]=0;

	return pBuffer;

}

wchar_t* cChangeCharCode::BIG5ToUnicode(const char* szBIG5String)
{

	UINT nCodePage = 950; //BIG5

	int nLength=MultiByteToWideChar(nCodePage,0,szBIG5String,-1,NULL,0);

	wchar_t* pBuffer = new wchar_t[nLength+1];

	MultiByteToWideChar(nCodePage,0,szBIG5String,-1,pBuffer,nLength);

	pBuffer[nLength]=0;

	return pBuffer;

}
char* cChangeCharCode::GB2312ToBIG5(const char* szGBString)
{

	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);

	int nLength = LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,NULL,0);

	char* pBuffer=new char[nLength+1];

	LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,pBuffer,nLength);

	pBuffer[nLength]=0;

	wchar_t* pUnicodeBuff = GB2312ToUnicode(pBuffer);

	char* pBIG5Buff = UnicodeToBIG5(pUnicodeBuff);

	delete[] pBuffer;

	delete[] pUnicodeBuff;

	return pBIG5Buff;

} 

wchar_t* cChangeCharCode::GB2312ToUnicode(const char* szGBString)
{

	UINT nCodePage = 936; //GB2312

	int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);

	wchar_t* pBuffer = new wchar_t[nLength+1];

	MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);

	pBuffer[nLength]=0;

	return pBuffer;

}

char* cChangeCharCode::UnicodeToBIG5(const wchar_t* szUnicodeString)
{

	UINT nCodePage = 950; //BIG5

	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);

	char* pBuffer=new char[nLength+1];

	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);

	pBuffer[nLength]=0;

	return pBuffer;
}

