#include "stdafx.h"
#include "TClient.h"


TClientDMI::TClientDMI()
{
	m_blDMIInit = FALSE;
	m_strNameSpace = NULL;
	m_pIWbemLocator = NULL;
	m_pIWbemServices = NULL;
	m_pIEnumWbem = NULL;
	m_strClassName = NULL;

	for (int i = 0; i < DEF_MAX_WBEMOBJ; i++)
	{
		m_szIWbemObject[i] = NULL;
	}

}
void TClientDMI::Release()
{
	if (m_pIEnumWbem != NULL)
	{
		m_pIEnumWbem->Release();
		m_pIEnumWbem = NULL;
	}

	if (m_strClassName != NULL)
	{
		SysFreeString(m_strClassName);
		m_strClassName = NULL;
	}

	if (m_strNameSpace != NULL)
	{
		SysFreeString(m_strNameSpace);
		m_strNameSpace = NULL;
	}

	if (m_pIWbemServices != NULL)
	{
		m_pIWbemServices->Release();
		m_pIWbemServices = NULL;
	}

	if (m_pIWbemLocator != NULL)
	{
		m_pIWbemLocator->Release();
		m_pIWbemLocator = NULL;
	}
	CoUninitialize();
}
TClientDMI::~TClientDMI()
{
	Release();
}
//////////////////////////////////////////////////////////////////////////  
//    
//          init DMI interface  
//  
//////////////////////////////////////////////////////////////////////////  


HRESULT TClientDMI::_initDMI()
{
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_SECURE_REFS, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&m_pIWbemLocator);
	if (FAILED(hr))
	{
		return hr;
	}
	m_strNameSpace = SysAllocString(L"\\\\.\\root\\cimv2");
	hr = m_pIWbemLocator->ConnectServer(m_strNameSpace, NULL, NULL, 0L, 0L, NULL, NULL, &m_pIWbemServices);
	if (FAILED(hr))
	{
		return hr;
	}
	m_blDMIInit = TRUE;
	return hr;
}

//////////////////////////////////////////////////////////////////////////  
//              common query process  
//step1. Init DMI interface  
//step2. Get ClassInformation  
//step3. Get Instance  
//step4. Get the property of Instance  
//////////////////////////////////////////////////////////////////////////  

std::wstring TClientDMI::getDMIInformation(OLECHAR *pWin32_ClassName, LPCWSTR pUniqueCaption, LPCWSTR pUniqueValue, LPCWSTR pTargetCaption)
{
	std::wstring strRet = L"";
	//  TCHAR szTarget[MAX_PATH] = { 0 };
	HRESULT hr = S_OK;
	if (!m_blDMIInit)
	{
		hr = _initDMI();
	}
	if (SUCCEEDED(hr))
	{
		VARIANT var;
		ULONG uReturned;
		HRESULT hr;
		VariantInit(&var);

		m_pIEnumWbem = NULL;
		m_strClassName = NULL;
		m_strClassName = SysAllocString(pWin32_ClassName);

		if (!m_strClassName)
		{
			return strRet;
		}
		hr = m_pIWbemServices->CreateInstanceEnum(m_strClassName, 0, NULL, &m_pIEnumWbem);

		if (FAILED(hr))
		{
			return strRet;
		}

		uReturned = 1;
		hr = m_pIEnumWbem->Next(2000 * DEF_MAX_WBEMOBJ, DEF_MAX_WBEMOBJ, m_szIWbemObject, &uReturned);

		if (FAILED(hr))
		{
			return strRet;
		}
		VariantClear(&var);

		UINT i = 0;
		for (i = 0; i < uReturned; i++)
		{
			if (m_szIWbemObject[i] != NULL)
			{
				VariantClear(&var);
				var.bstrVal = NULL;

				if (pUniqueCaption != NULL)
				{
					hr = m_szIWbemObject[i]->Get(pUniqueCaption, 0, &var, NULL, NULL);
					if (hr != WBEM_S_NO_ERROR)
					{
						return strRet;
					}
				}

				if (pUniqueCaption == NULL || var.bstrVal != NULL &&
					wcsncmp((V_BSTR(&var)), pUniqueValue, wcslen(pUniqueValue)) == 0)
				{
					VariantClear(&var);
					var.bstrVal = NULL;
					hr = m_szIWbemObject[i]->Get(pTargetCaption, 0, &var, NULL, NULL);
					if (var.vt != VT_NULL)
					{
						SAFEARRAY *pArr = nullptr;
						BSTR vbStr = nullptr;
						pArr = V_ARRAY(&var);
						if (SafeArrayGetDim(pArr) != 1)
						{
							strRet = var.bstrVal;
						}
						else
						{
							LONG lLeft = 0;
							LONG lRight = 0;
							SafeArrayGetLBound(pArr, 1, &lLeft);
							SafeArrayGetUBound(pArr, 1, &lRight);

							LONG arrayCount = lLeft;
							while (arrayCount <= lRight)
							{
								if (SafeArrayGetElement(pArr, &arrayCount, &vbStr) == S_OK)
								{
									strRet += vbStr;
									strRet += L" ";
								}
								arrayCount++;
							}

						}
						if (vbStr != nullptr)
						{
							SysFreeString(vbStr);
							vbStr = nullptr;
						}
						break;
					}
				}
			}
		}
		VariantClear(&var);
	}

	return strRet;
}
