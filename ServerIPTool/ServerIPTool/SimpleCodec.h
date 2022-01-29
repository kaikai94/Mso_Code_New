#pragma once
class CSimpleCodec
{
	FILE* m_fp;
	int m_CurCodecKeyPos;
	char* m_CodecKeybuf;
	int m_CodecKeyLen;
public:
	CSimpleCodec(char* filename, BOOL bEncoding, char* CodecKey);
	~CSimpleCodec();
	BOOL Encode(void* pBufIn, int length);
	BOOL Decode(void* pBufOut, int length);
};
