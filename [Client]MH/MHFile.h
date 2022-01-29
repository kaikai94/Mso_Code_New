



// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IFileStorage.h"

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR		0x00000001
#define MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT	0x00000010

struct MHFILE_HEADER
{
	DWORD	dwVersion;
	DWORD	dwType;		
	DWORD	FileSize;
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
	
	BOOL Init(char* filename,char* mode,DWORD dwFlag = 0);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	int GetString(char* pBuf);
	void GetLine(char * pBuf, int len);
	void GetLineX(char * pBuf, int len);	// ÁÖ¼®Ã³¸® µÈ °÷ ÀÐÀ»¶§ »ç¿ë
	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();

	BYTE GetByte();
	BOOL GetBool();
	BOOL GetHex(DWORD* pOut);
	DWORD GetExpPoint();
	EXPTYPE GetExpPoint64();	// magi82 - Exp Variable Type Change(070523)
	LEVELTYPE GetLevel();
	

	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);
		
	int GetWord(WORD* pOut);

	BOOL Seek(int n);
	

	BOOL IsEOF();


	BOOL IsInited();
	
	int GetChar();
protected:
	DWORD			m_bReadMode;
	I4DyuchiFileStorage* m_pFileStorage;
	void* m_pFilePointer;

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
	DWORD	GetDataSize()	{ return m_Header.FileSize; }
	BOOL	CheckLineEnd(int lineNum);
	WORD    GetOperationSystemType();
};


void _parsingKeywordString(char * inString, char ** outString);
void _parsingKeywordStringEx(char * inString, line_node ** Top);

#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)


