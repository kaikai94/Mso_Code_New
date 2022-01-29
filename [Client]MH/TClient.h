#pragma once
#include<atlbase.h>
#include<objbase.h>
#include<WbemIdl.h>
#include<WbemCli.h>
#include<comdef.h>
#include <string>
#pragma comment(lib,"wbemuuid.lib")
#pragma comment(lib,"ole32.lib")

#define DEF_MAX_WBEMOBJ 20
#define DEF_MAX_PATH    260

class TClientDMI
{
public:
	TClientDMI();
	~TClientDMI();
public:
	std::wstring  getDMIInformation(OLECHAR  *pWin32_ClassName, LPCWSTR pUniqueCaption, LPCWSTR pUniqueValue, LPCWSTR pTargetCaption);
	void Release();
private:
	HRESULT  _initDMI();
private:
	BSTR     m_strNameSpace;
	BOOL     m_blDMIInit;           //DMI init flag
private:
	IWbemLocator*           m_pIWbemLocator;
	IWbemServices*          m_pIWbemServices;
	IEnumWbemClassObject*   m_pIEnumWbem;
	IWbemClassObject*       m_szIWbemObject[DEF_MAX_WBEMOBJ];
	BSTR                    m_strClassName;

};