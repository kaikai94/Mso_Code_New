#pragma once

#define  MAX_KEY_LENGTH  512

struct AESFILE_HEADER
{
	DWORD dwVersion;
	DWORD dwType;
	DWORD dwDataSize;
};
const DWORD dwVersion[] = 
{
	20180918,
	20180919,
};
const unsigned char aKey[][MAX_KEY_LENGTH]=
{
	"",
	"so0Vk10s&lU6wf4zBz4u",
};
class cAESFile
{
	int   m_nVersionCount;
	DWORD m_dwCurVersion;
	DWORD m_dwLastVersion;

	unsigned char m_aCurKey[MAX_KEY_LENGTH];
	unsigned char m_aLastKey[MAX_KEY_LENGTH];

public:
	cAESFile(void);
	virtual ~cAESFile(void);
	BOOL SetBinVersion(DWORD BinVersion);
	BOOL IsExistKey();
	void Init();
	BOOL AESEncryptData(FILE *fp,char * pFileName,char* pData,DWORD dwDataSize);
	BOOL AESDecryptData(FILE *fp,char * pData,DWORD dwDataSize,DWORD dwType);
};
