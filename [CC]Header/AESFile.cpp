#include "stdafx.h"
#ifdef _AES_FILE_
#include "AESFile.h"
#include "aes.h"

cAESFile::cAESFile()
{
	m_nVersionCount = sizeof(dwVersion) / sizeof(DWORD);	
	m_dwLastVersion = dwVersion[m_nVersionCount-1];
	memcpy(m_aLastKey, aKey[m_nVersionCount-1], MAX_KEY_LENGTH);
}

cAESFile::~cAESFile()
{
}

void cAESFile::Init()
{
}

BOOL cAESFile::IsExistKey()
{
	int n=0;
	int keylen=0;
	while(m_aCurKey[n++])
	{
		keylen++;
	};
	return (keylen > 0) ? TRUE : FALSE;
}

BOOL cAESFile::SetBinVersion( DWORD	BinVersion )
{
	for(int i=0; i<m_nVersionCount; i++)
	{	
		if(dwVersion[i] == BinVersion)
		{
			m_dwCurVersion = dwVersion[i];
			memcpy(m_aCurKey, aKey[i], MAX_KEY_LENGTH);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL cAESFile::AESEncryptData(FILE* fp, char* pFileName, char* pData, DWORD dwDataSize)
{
	if(fp == NULL)			{	return FALSE;	}
	if(pFileName == NULL)	{	return FALSE;	}
	if(pData == NULL)		{	return FALSE;	}	

	int n, keylen;
	int filesize, offset;
	char *p;
	unsigned char key[512];
	unsigned char buffer[1024];

	aes_context aes_ctx;

	AESFILE_HEADER header={0};
	header.dwDataSize = dwDataSize;
	header.dwVersion = m_dwLastVersion;
	if( header.dwDataSize == 0 )
		header.dwType = 1;
	else
		header.dwType = rand()%header.dwDataSize + 1;
	header.dwVersion += (header.dwType+header.dwDataSize);
	fwrite( &header, sizeof(header), 1, fp );

	p = (char*)m_aLastKey;
	keylen = 0;
	memset( key, 0, sizeof( key ) );

	while( sscanf( p, "%02X", &n ) > 0 &&
		keylen < (int) sizeof( key ) )
	{
		key[keylen++] = (unsigned char) n;
		p += 2;
	}
	filesize = dwDataSize;
	aes_setkey_enc( &aes_ctx, key, 256 );

	fseek(fp, 1, SEEK_CUR);

	char crc = (char)header.dwType;

	for( offset = 0; offset < filesize; offset += 16 )
	{
		n = ( filesize - offset > 16 ) ? 16 : (int)
			( filesize - offset );

		for(int i=0; i<16; i++)
		{
			buffer[i] = pData[offset+i];
			crc = crc + pData[offset+i];

		}

		aes_crypt_ecb( &aes_ctx, AES_ENCRYPT, buffer, buffer );
		if( fwrite( buffer, 1, 16, fp ) != 16 )
		{
			fprintf( stderr, "fwrite(%d bytes) failed\n", 16 );
			memset( buffer, 0, sizeof( buffer ) );
			memset( &aes_ctx, 0, sizeof(  aes_context ) );
			return FALSE;		
		}
	}

	fwrite( &crc, sizeof( char ), 1, fp );	
	fseek(fp, sizeof(header), SEEK_SET);
	fwrite( &crc, sizeof( char ), 1, fp );		
	fseek(fp, 0, SEEK_END);

	return TRUE;
}


BOOL cAESFile::AESDecryptData(FILE* fp, char* pData, DWORD dwDataSize, DWORD dwType)
{
	if(fp == NULL)		{	return FALSE;	}
	if(pData == NULL)	{	return FALSE;	}

	int n, keylen;
	int filesize, offset;
	char *p;
	char crc, crc1, crc2;
	unsigned char key[512];
	unsigned char buffer[1024];
	aes_context aes_ctx;

	filesize = dwDataSize;

	fread( &crc1, sizeof(char), 1, fp );	

	p = (char*)m_aCurKey;
	keylen = 0;
	memset( key, 0, sizeof( key ) );
	while( sscanf( p, "%02X", &n ) > 0 &&
		keylen < (int) sizeof( key ) )
	{
		key[keylen++] = (unsigned char) n;
		p += 2;
	}

	aes_setkey_dec( &aes_ctx, key, 256 );

	crc = (char)dwType;

	for( offset = 0; offset < filesize; offset += 16 )
	{
		if( fread( buffer, 1, 16, fp ) != 16 )
		{
			memset( buffer, 0, sizeof( buffer ) );
			memset( &aes_ctx, 0, sizeof(  aes_context ) );
			return FALSE;		
		}
		aes_crypt_ecb( &aes_ctx, AES_DECRYPT, buffer, buffer );

		for(int i=0; i<16; i++)
		{
			pData[offset+i] = buffer[i];
			crc = crc + pData[offset+i];
		}
	}

	fread( &crc2, sizeof(char), 1, fp );		// crc2
	if((crc1 != crc2) || (crc1 != crc))
	{
		memset( buffer, 0, sizeof( buffer ) );
		memset( &aes_ctx, 0, sizeof(  aes_context ) );
		return FALSE;
	}
	return TRUE;
}

#endif