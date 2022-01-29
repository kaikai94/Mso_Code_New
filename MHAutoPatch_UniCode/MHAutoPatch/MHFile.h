



// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#define TEXT_DELIMITER			'^'
#define TEXT_NEWLINECHAR		'n'
#define TEXT_TABCHAR			't'
#define TEXT_SPACECHAR			's'
#define TEXT_EMPTYCHAR			'e'
#pragma once

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR		0x00000001
#define MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT	0x00000010
struct KEY
{
	WORD OpenEncryptNum;
	char OpenEncryptStr[33];
	char SaveEncryptStr[33];
	WORD SaveEncryptNum;
};
struct MHFILE_HEADER
{
	DWORD	dwVersion;
	DWORD	dwType;		
	DWORD	dwFileSize;
};

enum{
	MHFILE_NORMALMODE,
	MHFILE_PACKEDFILE,
	MHFILE_ENGINEPACKEDFILE,
};
class line_node;

class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename);
	void Release();



	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();

	BYTE GetByte();
	BOOL GetBool();
	

	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);
		

	BOOL Seek(int n);
	

	BOOL IsEOF();

	char* GetString();
	int GetString(char* pBuf);

	std::wstring GetStringW();
	int GetString(std::wstring& pBuf);
	std::wstring stringToWstring(const std::string& str);
protected:
	DWORD			m_bReadMode;
	


	int				m_Dfp;

	MHFILE_HEADER	m_Header;				
	char			m_crc1;					
	char			m_crc2;					
	char*			m_pData;				
	BOOL			m_bInit;
	char			m_sFileName[MAX_PATH];


public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetFileName()	{ return m_sFileName; }
	char*	GetData()		{ return m_pData;}
	DWORD	GetDataSize()	{ return m_Header.dwFileSize; }
};
void _parsingKeywordString(char * inString, char ** outString);

#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)


