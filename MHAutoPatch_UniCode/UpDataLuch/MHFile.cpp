







// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"


int CyptNumber = 2;
CMHFile::CMHFile()
{
	fp = NULL;
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;
	memset( m_sFileName, 0, MAX_PATH );

}

CMHFile::~CMHFile()
{
	Release();
}

BOOL CMHFile::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	fopen_s(&fp, filename, "rb" );
	strcpy_s(m_sFileName,filename);
	if( fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, fp );
		
		for(int i  = 0; i<CyptNumber;i++ )
		{
			fread( &m_crc1, sizeof(char), 1, fp );
		}

		if( m_Header.FileSize )
		{
			m_pData = new char[m_Header.FileSize];
			fread( m_pData, sizeof(char), m_Header.FileSize, fp );
		}
		for(int i  = 0; i<CyptNumber;i++ )
		{
			fread( &m_crc2, sizeof(char), 1, fp );
		}	
	}	
	else
	{
		return FALSE;
	}

	fclose( fp );

	fp = NULL;
	if( !CheckCRC() )		return FALSE;

	m_bInit = TRUE;

	return TRUE;
}

BOOL CMHFile::CheckHeader()
{
	return TRUE;
	if( m_Header.dwVersion != (DWORD)(20040308+m_Header.dwType+m_Header.FileSize) )
	{
		Clear();
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		return FALSE;
	}	

	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	char crc = (char)m_Header.dwType;

	for (DWORD i = 0; i < m_Header.FileSize; ++i)
	{
		crc += m_pData[i];
		m_pData[i] -= (char)i;
		if (i%m_Header.dwType == 0)

			m_pData[i] -= (char)m_Header.dwType;
	}
	return TRUE;
}


void CMHFile::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	m_Dfp = 0;

	m_bInit = FALSE;
}

BOOL CMHFile::Init(char* filename)
{
	if (OpenBin(filename))
		return TRUE;
	else
		return FALSE;
}


void CMHFile::Release()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	Clear();
}

char* CMHFile::GetString()
{
	static char buf[512];

	GetString(buf);
	return buf;
}

int CMHFile::GetString( char* pBuf )
{
	char buf[512] = {0,};
	DWORD j = 0;
	BOOL bStart, bEnd;
	bStart = bEnd = FALSE;
	// yh;
	int rt = 0;

			if(m_Dfp == (int)m_Header.FileSize) return (rt=EOF);

			for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
			{
				if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
				{
					if( bStart )
						break;
					else
					{
						m_Dfp += 2;
						++i;
					}
				}
				else if( m_pData[i] == 0x20 || m_pData[i] == 0x09 )	// spacebar, tab
				{

					++m_Dfp;
					if( bStart )
						bEnd = TRUE;
				}
				else
				{
					if( bEnd ) break;

					buf[j] = m_pData[i];
					++j;
					++m_Dfp;
					bStart = TRUE;
				}
			}
			buf[j] = 0;

	_parsingKeywordString( buf, (char**)(&pBuf) );
	return rt;
}
int CMHFile::GetInt()
{
	int a = atoi(GetString());
	return a;
}
LONG CMHFile::GetLong()
{
	return (LONG)atoi(GetString());
}
float CMHFile::GetFloat()
{
	return (float)atof(GetString());
}

DWORD CMHFile::GetDword()
{
	return (DWORD)atoi(GetString());
}

WORD CMHFile::GetWord()
{
	return (WORD)atoi(GetString());
}

BYTE CMHFile::GetByte()
{
	return (BYTE)atoi(GetString());
}

BOOL CMHFile::GetBool()
{
	return (BOOL)atoi(GetString());
}

void CMHFile::GetFloat(float* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetFloat();
	}
}

void CMHFile::GetWord(WORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetWord();
	}
}

void CMHFile::GetDword(DWORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetDword();
	}
}
BOOL CMHFile::IsEOF()
{
	if (m_Header.FileSize == (DWORD)m_Dfp)
		return TRUE;
	else
		return FALSE;
}

BOOL CMHFile::Seek(int n)
{
	return fseek( fp, n, SEEK_CUR );
}

void _parsingKeywordString(char * inString, char ** outString)
{
	char * sp = inString;
	char * sp2 = *outString;
	while (*sp != 0)
	{

		if (IsDBCSLeadByte(*sp))
		{
			*sp2++ = *sp++;
			*sp2++ = *sp++;
		}
		else
		{
			switch (*sp)
			{
			case TEXT_DELIMITER:
			{
				++sp;
				char tmp = *sp;
				switch (tmp)
				{
				case TEXT_SPACECHAR:
				{
					*sp2 = ' ';
					++sp2;
				}
				break;
				case TEXT_EMPTYCHAR:
				{
					*outString[0] = 0;
				}
				break;
				case TEXT_DELIMITER:	//KES Ãß°¡
				{
					*sp2 = '^';
					++sp2;
				}
				break;
				}

			}
			break;
			default:
			{
				*sp2 = *sp;
				++sp2;
			}
			}
			++sp;
		}
	}
	*sp2 = 0;
}
