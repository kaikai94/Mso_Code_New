#include "stdafx.h"
#include "AddFont.h"
CAddMemFont::CAddMemFont(void)
: m_hFont( NULL )
{
	ZeroMemory( m_szFilePath, MAX_PATH );
	GetCurrentDirectory(MAX_PATH, m_szFilePath);
	_tcscat_s(m_szFilePath, MAX_PATH, _T("\\"));

}


CAddMemFont::~CAddMemFont(void)
{
	RemoveFontMemResourceEx(m_hFont);
}

bool CAddMemFont::AddFont(LPCTSTR szfilePath)
{
	_tcscat_s(m_szFilePath, MAX_PATH, szfilePath);

	HANDLE hFile = CreateFile( m_szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwFileSize	= GetFileSize(hFile, NULL);
	BYTE* lpBuffer		= new BYTE[dwFileSize+1];

	DWORD dwReadSize	= 0;
	if (!ReadFile( hFile, lpBuffer, dwFileSize, &dwReadSize, NULL))
	{
		delete[] lpBuffer;
		CloseHandle(hFile);
		return false;
	}

	lpBuffer[dwReadSize] = '\0';

	DWORD dwFontNumber	= 0;
	m_hFont = AddFontMemResourceEx(lpBuffer, dwReadSize, 0, &dwFontNumber);
	if ( m_hFont == NULL )
	{
		delete[] lpBuffer;
		CloseHandle(hFile);
		return false;
	}

	return true;
}
