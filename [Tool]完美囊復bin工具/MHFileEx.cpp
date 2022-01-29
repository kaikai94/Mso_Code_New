#define __MHFILEEX_C__

#include "stdafx.h"
#include "MHFileEx.h"
#include <fstream>
DWORD		m_FileSize;
extern KEY	m_Key;
extern BOOL bIsInitKey;
extern std::string CurPath;
CMHFileEx::CMHFileEx()
{
	memset( m_szFullFileName, 0, 256 );
	memset( m_szFileName, 0, 256 );
	memset( m_szExt, 0, 256 );
	memset( &m_Header, 0, sizeof(MHFILE_HEADER) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;	
	m_pBinData = NULL;

	memset( &m_DOFHeader, 0, sizeof(DOF_HEADER) );
	memset( &m_DOFTail, 0, sizeof(DOF_TAIL) );
	strcpy( m_szDOF, "dof" );
}

CMHFileEx::~CMHFileEx()
{
	Clear();
}

void CMHFileEx::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	if( m_pBinData )
	{
		delete [] m_pBinData;
		m_pBinData = NULL;
	}
}

BOOL CMHFileEx::New( const char* fullfilename )
{
	m_FileSize = 4096;
	Clear();
	m_pData = new char[m_FileSize];
	memset( m_pData, 0, m_FileSize );
	
	InitFileName( fullfilename );

	return TRUE;
}

BOOL CMHFileEx::OpenTxt( const char* fullfilename )
{
	locale::global(locale(""));
	InitFileName( fullfilename );
	m_FileSize = 0;

	filebuf *pbuf;
	ifstream filestr;
	filestr.open(fullfilename,ios::binary);
	locale::global(locale("C"));
	pbuf = filestr.rdbuf();
	m_Header.dwDataSize = pbuf->pubseekoff(0,ios::end,ios::in);
	pbuf->pubseekpos(0,ios::in);

	if( filestr.is_open() )
	{
		Clear();
		m_pData = new char[m_Header.dwDataSize+1];
		memset( m_pData, 0, m_Header.dwDataSize );

		pbuf->sgetn(m_pData,m_Header.dwDataSize);

		m_pData[m_Header.dwDataSize] = '\0';		
		
		m_DOFHeader.dwDataSize = m_Header.dwDataSize;

		filestr.close();
	}
	return TRUE;
}

BOOL CMHFileEx::OpenBin( const char* fullfilename )
{
	if(!bIsInitKey)
		MessageBox(NULL,"ÃÜÔ¿Î´³õÊ¼»¯!","´íÎó!",MB_OK);
	locale::global(locale(""));
	InitFileName( fullfilename );
	m_FileSize=0;
	filebuf *pbuf;
	ifstream filestr;
	filestr.open(fullfilename,ios::binary);
	locale::global(locale("C"));
	int err = 0;
	err = GetLastError();
	pbuf = filestr.rdbuf();
	m_FileSize = pbuf->pubseekoff(0,ios::end,ios::in);
	pbuf->pubseekpos(0,ios::in);
	if( filestr.is_open() )
	{
		Clear();
		pbuf->sgetn((char*)&m_Header, sizeof(m_Header));
       for(int i=0;i<m_Key.OpenEncryptNum;i++)
			pbuf->sgetn(&m_crc1, sizeof(char));

		if( m_Header.dwDataSize )	
		{
			m_pData = new char[m_Header.dwDataSize+1];
			memset( m_pData, 0, m_Header.dwDataSize );
			pbuf->sgetn(m_pData, m_Header.dwDataSize);
			m_pData[m_Header.dwDataSize] = '\0';
		}
		for(int i=0;i<m_Key.OpenEncryptNum;i++)  
			pbuf->sgetn(&m_crc2, sizeof(char));
	}	
	filestr.close();
	if( !CheckCRC() )		return FALSE;
	m_DOFHeader.dwDataSize = m_Header.dwDataSize;
	return TRUE;
}

BOOL CMHFileEx::SaveToTxt( const char* filename )
{
	//locale::global(locale(""));
	ofstream OutFile(filename);
	//locale::global(locale("C"));
	if(OutFile.is_open())
	{
		OutFile.write(m_pData,m_Header.dwDataSize);
	}
	OutFile.close();
	return TRUE;
}

BOOL CMHFileEx::SaveToTxt()
{
	MakeFullFileName();
	locale::global(locale(""));
	ofstream OutFile(m_szFullFileName,ios::binary);
	locale::global(locale("C"));
	if(OutFile.is_open())
	{
		OutFile.write(m_pData,m_Header.dwDataSize);
	}
	OutFile.close();
	
	return TRUE;
}

BOOL CMHFileEx::SaveToBin( const char* filename )
{
	if( m_pData )
	{
		locale::global(locale(""));
		ofstream OutFile(m_szFullFileName,ios::binary);
		locale::global(locale("C"));
		if( OutFile.is_open() )
		{
			ConvertBin();
			OutFile.write(reinterpret_cast<char*>(&m_Header),sizeof(m_Header));
			for(int i=0;i<m_Key.SaveEncryptNum;i++)  
				OutFile.write(&m_crc1,sizeof(char));
			OutFile.write(m_pBinData,m_Header.dwDataSize);
			for(int i=0;i<m_Key.SaveEncryptNum;i++)  
				OutFile.write(&m_crc2,sizeof(char));
		}
		OutFile.close();
	}

	return TRUE;
}

BOOL CMHFileEx::SaveToBin()
{
	MakeFullFileName();
	return SaveToBin( m_szFullFileName );
}

BOOL CMHFileEx::CheckCRC()
{
	char crc = (char)m_Header.dwType;
	int j = 0;
	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		if(j>=(strlen(m_Key.OpenEncryptStr)-1))
			j = 0;
		else
			j++;
		crc += m_pData[i];
		m_pData[i] -= ((char)i^m_Key.OpenEncryptStr[j]);
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= m_Header.dwType;
	}

	return TRUE;
}

BOOL CMHFileEx::ConvertBin()
{
	m_Header.dwVersion = 20040308;
	if( m_Header.dwDataSize == 0 )
		m_Header.dwType = 1;
	else
		m_Header.dwType = rand()%m_Header.dwDataSize + 1;
	m_Header.dwVersion += (m_Header.dwType+m_Header.dwDataSize);

	m_pBinData = new char[m_Header.dwDataSize+1];
	memcpy( m_pBinData, m_pData, m_Header.dwDataSize+1 );
	m_pBinData[m_Header.dwDataSize] = 0;

	char crc = m_Header.dwType;
	int j = 0;
	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		if(j>=(strlen(m_Key.SaveEncryptStr)-1))
			j = 0;
		else
			j++;
		m_pBinData[i] += ((char)i^m_Key.SaveEncryptStr[j]);
		if( i%m_Header.dwType == 0 )
			m_pBinData[i] += m_Header.dwType;
	}

	m_crc1 = m_crc2 = crc;

	return TRUE;
}

void CMHFileEx::InitFileName( const char* fullfilename )
{
	strcpy( m_szFullFileName, fullfilename );

	int len = strlen(m_szFullFileName);
	int i, j, k;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
			j = i;
		if( m_szFullFileName[i] == '\\' )
		{
			k = i;
			break;
		}
	}

	for( i = 0; i < len-j; ++i )
		m_szExt[i] = m_szFullFileName[j+1+i];
	for( i = 0; i < len-k; ++i )
		m_szFileName[i] = m_szFullFileName[k+1+i];	
}

void CMHFileEx::MakeFullFileName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szExt);
	int i, j;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	for( i = 0; i < lenext; ++i )
		m_szFullFileName[j+1+i] = m_szExt[i];

	m_szFullFileName[j+1+i] = 0;
}

void CMHFileEx::SetData( CString str )
{
	m_FileSize += (strlen( str )-m_Header.dwDataSize)-(m_FileSize-m_Header.dwDataSize);
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = str.GetLength();
	}
	else
	{
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = str.GetLength();
	}
}

BOOL CMHFileEx::MakeDOFName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szDOF);
	int i, j;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	for( i = 0; i < lenext; ++i )
		m_szFullFileName[j+1+i] = m_szDOF[i];

	m_szFullFileName[j+1+i] = 0;

	return TRUE;
}

BOOL CMHFileEx::SaveToDOF()
{
	MakeDOFName();
	return SaveToDOF( m_szFullFileName );
}

BOOL CMHFileEx::SaveToDOF( const char* filename )
{
	//if( m_pData )
	/*{
		m_fp = fopen( filename, "wb" );
		if( m_fp )
		{
			ConvertDOF();
			fwrite( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( m_pBinData, sizeof( char ), m_DOFHeader.dwDataSize, m_fp );
			fwrite( &m_DOFTail, sizeof( m_DOFTail ), 1, m_fp );
		}
		fclose( m_fp );
	}*/

	return TRUE;
}

BOOL CMHFileEx::ConvertDOF()
{
	// encode
	m_DOFHeader.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFTail.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFHeader.dwVersion = (DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize;
	m_DOFTail.dwVersion = (DWORD)DOFTAIL + m_DOFTail.dwSeed;

	m_pBinData = new char[m_DOFHeader.dwDataSize];
	memcpy( m_pBinData, m_pData, m_DOFHeader.dwDataSize );
//	m_pBinData[m_DSOFHeader.dwDataSize] = 0;

	m_crc1 = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		m_pBinData[i] += ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
		m_crc1 += m_pBinData[i];
	}

	return TRUE;
}

BOOL CMHFileEx::OpenDOF( const char* fullfilename )
{
	//InitFileName( fullfilename );

	//m_fp = fopen( fullfilename, "rb" );
	//if( m_fp )
	//{
	//	Clear();
	//	fread( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );	// header
	//	fread( &m_crc1, sizeof(char), 1, m_fp );				// crc1
	//	if( m_DOFHeader.dwDataSize )							// data
	//	{
	//		m_pData = new char[MEGA];
	//		memset( m_pData, 0, MEGA );
	//		fread( m_pData, sizeof(char), m_DOFHeader.dwDataSize, m_fp );
	//		m_pData[m_DOFHeader.dwDataSize] = 0;
	//	}
	//	fread( &m_DOFTail, sizeof(m_DOFTail), 1, m_fp );		// tail
	//}	
	//fclose( m_fp );

	//if( !CheckDOF() )	return FALSE;

	return TRUE;
}

BOOL CMHFileEx::CheckDOF()
{
	if( m_DOFHeader.dwVersion != ((DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFHeader.dwSeed == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Seed Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFHeader.dwDataSize > MEGA )
	{
		Clear();
		MessageBox( NULL, "Header DataSize Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFTail.dwVersion != ((DWORD)DOFTAIL + m_DOFTail.dwSeed) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	// decode
	char crc = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
	}
	if( m_crc1 != crc )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	return TRUE;
}
