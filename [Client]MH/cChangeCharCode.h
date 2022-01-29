#pragma once
#include <string>
#define CODEMGR cChangeCharCode::GetInstance()
class cChangeCharCode
{
	BOOL IsSimple;
public:
	cChangeCharCode(void);
	~cChangeCharCode(void);
	GETINSTANCE(cChangeCharCode);
	void Init();
	char* BIG5ToGB2312(const char* szBIG5String);
	char* GB2312ToBIG5(const char* szGBString);
	char* ChangeCharCode(char* szString);
	char* UnicodeToGB2312(const wchar_t* szUnicodeString);
	char* UnicodeToBIG5(const wchar_t* szUnicodeString);

	wchar_t* BIG5ToUnicode(const char* szBIG5String);
	wchar_t* GB2312ToUnicode(const char* szGBString);

	BOOL GetIsSimple(){return IsSimple;}
	void SetIsSimple(BOOL val){IsSimple = val;}
};
