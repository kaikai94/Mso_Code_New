#pragma once

// FileVersion.h: interface for the CFileVersion class.
//////////////////////////////////////////////////////////////////////
class CFileVersion
{ 
	// Construction
public: 
	CFileVersion();
	// Operations	
public: 
	static BOOL    Open(LPCTSTR lpszModuleName);
	static void    Close();
	static CString QueryValue(LPCTSTR lpszValueName, DWORD dwLangCharset = 0);
	static CString GetFileDescription()  {return QueryValue(_T("FileDescription")); };
	static CString GetFileVersion()      {return QueryValue(_T("FileVersion"));     };
	static CString GetInternalName()     {return QueryValue(_T("InternalName"));    };
	static CString GetCompanyName()      {return QueryValue(_T("CompanyName"));     };
	static CString GetLegalCopyright()   {return QueryValue(_T("LegalCopyright"));  };
	static CString GetOriginalFilename() {return QueryValue(_T("OriginalFilename"));};
	static CString GetProductName()      {return QueryValue(_T("ProductName"));     };
	static CString GetProductVersion()   {return QueryValue(_T("ProductVersion"));  };
	static BOOL    GetFixedInfo(VS_FIXEDFILEINFO& vsffi);
	static CString GetFixedFileVersion();
	static CString GetFixedProductVersion();
	// Attributes
protected:
	static LPBYTE  m_lpVersionData;
	static DWORD   m_dwLangCharset;
	// Implementation
public:
	~CFileVersion(); 
}; 
