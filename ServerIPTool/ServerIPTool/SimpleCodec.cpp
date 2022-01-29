#include "stdafx.h"
#include "SimpleCodec.h"


CSimpleCodec::CSimpleCodec(char* filename, BOOL bEncoding, char* CodecKey)
{
	fopen_s(&m_fp,filename, bEncoding ? "wb" : "rb");
	m_CurCodecKeyPos = 0;
	m_CodecKeyLen = strlen(CodecKey);
	m_CodecKeybuf = new char[m_CodecKeyLen + 1];
	strcpy_s(m_CodecKeybuf, m_CodecKeyLen+1,CodecKey);
}

CSimpleCodec::~CSimpleCodec()
{
	fclose(m_fp);
	delete[] m_CodecKeybuf;
}

BOOL CSimpleCodec::Encode(void* pBufIn, int length)
{
#define MAX_ENCODEBUF_LENGTH	1024
	char tempBuf[MAX_ENCODEBUF_LENGTH];
	ASSERT(length <= MAX_ENCODEBUF_LENGTH);

	memcpy(tempBuf, pBufIn, length);
	for (int n = 0; n < length; ++n, ++m_CurCodecKeyPos)
	{
		if (m_CurCodecKeyPos == m_CodecKeyLen)
			m_CurCodecKeyPos = 0;
		tempBuf[n] ^= m_CodecKeybuf[m_CurCodecKeyPos];
	}

	int wrotelen = fwrite(tempBuf, length, 1, m_fp);
	return (wrotelen == 1);
}
BOOL CSimpleCodec::Decode(void* pBufOut, int length)
{
	int readlen = fread(pBufOut, length, 1, m_fp);
	if (readlen != 1)

		return FALSE;

	char* pchBufOut = (char*)pBufOut;
	for (int n = 0; n < length; ++n, ++m_CurCodecKeyPos)
	{
		if (m_CurCodecKeyPos == m_CodecKeyLen)
			m_CurCodecKeyPos = 0;
		pchBufOut[n] ^= m_CodecKeybuf[m_CurCodecKeyPos];
	}
	return TRUE;
}
