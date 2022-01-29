



// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
#include <fstream>
KEY g_Key;
BOOL m_bIsInitKey = FALSE;
void Readf(int n,unsigned char tu[],int tu_l)
{
	unsigned char t,c;
	unsigned char a[100]={0,};

	int j,k;

	for (j = 0;j<n;j++)
	{
		a[j]&= 0x00;

		for(k = 0;k<8;k++)
		{
			t = tu[64+j*8+k];
			t = t&0x01;
			a[j]|=t;
			c = a[j];
			a[j] = (c>>1)|(c<<7);
		}
	}
	a[n]='\0';
	memcpy(&g_Key,a,sizeof(KEY));
	m_bIsInitKey = TRUE;
}
void Readout()
{
	FILE * fp = NULL;
	long L = 0;
	unsigned char * f=NULL;
	int n=0;
	fopen_s(&fp,"./AutoNoteImage/00BG.bmp","rb");
	if(fp)
	{
		fseek(fp,0,2);
		L = ftell(fp);
		f = new unsigned char[L+1];
		memset(f,0,L+1);
		rewind(fp);
		fread(f,L,1,fp);
		fclose(fp);
	}
	else
		return;
	Readf(sizeof(g_Key),f,L);
	if (f != NULL)
	{
		delete[]f;
		f = NULL;
	}
}

CMHFile::CMHFile()

{
	fp = NULL;
	m_bReadMode = MHFILE_NORMALMODE;
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;
	if(!m_bIsInitKey)
		Readout();
}

CMHFile::~CMHFile()
{
	Release();
}


BOOL CMHFile::Init(char* filename,char* mode, DWORD dwFlag)
{
	if( strcmp( "rb", mode ) == 0 )
		m_bReadMode = MHFILE_PACKEDFILE;
	else
		m_bReadMode = MHFILE_NORMALMODE;

	if( m_bReadMode == MHFILE_PACKEDFILE )
		return OpenBin( filename );
	else if( m_bReadMode == MHFILE_NORMALMODE )
	{		
		fp = fopen(filename,mode);
		if(fp)
			return TRUE;
		else
		{
			if(!(dwFlag & MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
			{			
				char buffer[255];
				sprintf(buffer, "[%s] file is not found",filename);
				MessageBox(NULL, buffer, 0,0);
			}		
			return FALSE;
		}
	}
	return TRUE;
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
	static char buf[256];
	GetString( buf );
	return buf;
}

void CMHFile::GetString(char* pBuf)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD j = 0;
		BOOL bStart, bEnd;
		bStart = bEnd = FALSE;
		
		if( m_Dfp == (int)m_Header.dwFileSize ) return;
		
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
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
				m_Dfp++;
				if( bStart )
					bEnd = TRUE;
			}
			else
			{
				if( bEnd ) break;
				pBuf[j] = m_pData[i];

				j++;
				m_Dfp++;
				bStart = TRUE;
			}
		}
		pBuf[j] = 0;	
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fscanf(fp,"%s",pBuf);
}

void CMHFile::GetLine(char * pBuf, int len)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{		
		DWORD j = 0;
		
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
		{
			if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
			{
				m_Dfp += 2;
				break;
			}
			else
			{
				pBuf[j] = m_pData[i];
				j++;
				m_Dfp++;
			}
		}
		pBuf[j] = 0;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fgets(pBuf, len, fp);
}

void CMHFile::GetLineX(char * pBuf, int len)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD j = 0;
		
		if( m_Dfp > 2 ) m_Dfp -= 2;
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
		{
			if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
			{
				m_Dfp += 2;
				break;
			}
			else
			{
				pBuf[j] = m_pData[i];
				j++;
				m_Dfp++;
			}
		}
		pBuf[j] = 0;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fgets(pBuf, len, fp);
}

int CMHFile::GetInt()
{
	return atoi(GetString());
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
	return (DWORD)_atoi64(GetString());
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
DWORD CMHFile::GetExpPoint()
{
	return (DWORD)atoi(GetString());
}

EXPTYPE CMHFile::GetExpPoint64()
{
	return (EXPTYPE)_atoi64(GetString());
}
BOOL CMHFile::GetLevel()
{
	return (LEVELTYPE)atoi(GetString());
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
	if( m_bReadMode == MHFILE_PACKEDFILE )
		return (m_Header.dwFileSize == (DWORD)m_Dfp) ? TRUE : FALSE;
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return feof(fp) ? TRUE : FALSE;

	return FALSE;
}

BOOL CMHFile::Seek(int n)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )

	{
		m_Dfp += n;
		return TRUE;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return fseek(fp,n,SEEK_CUR);

	return TRUE;
}

char* CMHFile::GetStringInQuotation()
{
	char buf[256]={0,};
	static char buf2[256];
	int c;
	
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		while( (c=m_pData[m_Dfp]) != '"' )
		{
			++m_Dfp;
			if( c == 10 )					
			{
				buf2[0] = 0;
				return buf2;
			}
		}
		int n = 0;
		while( 1 )
		{
			++m_Dfp;
			buf[n] = m_pData[m_Dfp];
			if( buf[n] == '"' )
			{
				buf[n] = 0;
				++m_Dfp;
				break;
			}
			if( buf[n] == 10 )			
			{
				buf[n] = 0;
				break;
			}
			++n;
		}
		char* aa = &buf2[0];
		memcpy( buf2, buf, n+1 );
	}

	else if( m_bReadMode == MHFILE_NORMALMODE )
	{
		while( (c=fgetc(fp)) != '"')
		{
			if( c == 10 )
			{
				Seek(-1);
				buf2[0] = 0;
				return buf2;
			}
		}
		int n=0;
		while(1)
		{
			ASSERT(!feof(fp));
			buf[n] = fgetc(fp);
			if(buf[n] == '"' )
			{
				buf[n] = 0;
				break;
			}
			if(buf[n] == 10) 
			{
				Seek(-1);
				buf[n] = 0;
				break;
			}
			++n;
		}
		char * aa = &buf2[0];
		memcpy( buf2, buf, n+1 );	
	}

	return buf2;
}

void CMHFile::GetStringInQuotation(char* pBuf)
{
	strcpy(pBuf,GetStringInQuotation());
}

void CMHFile::SetString(char * str)
{
	fprintf(fp, "%s", str);
}

BOOL CMHFile::IsLineEnd()
{
	BOOL bEnd = FALSE;
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD i = m_Dfp;


		if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
		{
			bEnd = TRUE;
		}
		else
		{
			bEnd = FALSE;
		}
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
	{
		if( fgetc(fp) == 10 )
			bEnd = TRUE;
		else
			bEnd = FALSE;
	}

	return bEnd;
}



BOOL CMHFile::OpenBin( char* filename )
{
	ASSERTMSG(m_bIsInitKey,"¼ÓÃÜÎ´³õÊ¼»¯");
	if( filename == NULL )
		return FALSE;
	
	int totalsize = 0;
	std::ifstream Infile;
	std::filebuf *pbuf=NULL;
	Infile.open(filename,std::ios::binary);
		int err = 0;
	err = GetLastError();
	pbuf = Infile.rdbuf();
	if(Infile.is_open())
	{
		Clear();
		pbuf->sgetn((char*)&m_Header, sizeof(m_Header));
        for(int i=0;i<g_Key.OpenEncryptNum;i++)
			pbuf->sgetn(&m_crc1, sizeof(char));

		if( m_Header.dwFileSize )	
		{
			m_pData = new char[m_Header.dwFileSize+1];
			memset( m_pData, 0, m_Header.dwFileSize );
			pbuf->sgetn(m_pData, m_Header.dwFileSize);
			m_pData[m_Header.dwFileSize] = '\0';
		}
		for(int i=0;i<g_Key.OpenEncryptNum;i++)  
			pbuf->sgetn(&m_crc2, sizeof(char));
	}
	else
	{
		ASSERTMSG(0,filename);
		return FALSE;
	}
	Infile.close();
	if( !CheckCRC() )		return FALSE;
	m_bInit = TRUE;
	return TRUE;
	//if( filename == NULL )
	//	return FALSE;
	//int totalsize;
	//fp = fopen( filename, "rb" );

	//if( fp )
	//{
	//	Clear();
	//	fread( &m_Header, sizeof(m_Header), 1, fp );	// header
	//	for (int i=0;i<EncryptNum;i++)
	//		fread( &m_crc1, sizeof(char), 1, fp );			// crc1
	//	if( m_Header.dwFileSize )						// data
	//	{
	//		m_pData = new char[m_Header.dwFileSize+1];
	//		fread( m_pData, sizeof(char), m_Header.dwFileSize, fp );
	//		m_pData[m_Header.dwFileSize]=0;
	//	}
	//	for (int i=0;i<EncryptNum;i++)
	//		fread( &m_crc2, sizeof(char), 1, fp );			// crc2
	//}	
	//else
	//{
	//	ASSERTMSG(0,filename);
	//	return FALSE;
	//}
	//fclose( fp );
	//fp = NULL;

	////if( !CheckHeader() )	return FALSE;
	//if( !CheckCRC() )		return FALSE;

	//m_bInit = TRUE;

	//return TRUE;
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

BOOL CMHFile::CheckHeader()
{
	return TRUE;
	if( m_Header.dwVersion != (DWORD)(20040308+m_Header.dwType+m_Header.dwFileSize) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Type Error!!", "Error!!", MB_OK );
		return FALSE;
	}	

	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	char crc = (char)m_Header.dwType;
	int j = 0;
	for( DWORD i = 0; i < m_Header.dwFileSize; ++i )
	{
		if(j>=(strlen(g_Key.OpenEncryptStr)-1))
			j = 0;
		else
			j++;
		crc += m_pData[i];
		m_pData[i] -= ((char)i^g_Key.OpenEncryptStr[j]);
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= m_Header.dwType;
	}
	return TRUE;
}

BOOL CMHFile::IsInited()
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
		return m_bInit ? TRUE : FALSE;
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return fp ? TRUE : FALSE;

	return FALSE;
}
