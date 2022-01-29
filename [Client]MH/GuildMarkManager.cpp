







// GuildMarkManager.cpp: implementation of the CGuildMarkManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMarkManager.h"

#include "GuildMarkImg.h"
#include "GuildMark.h"
#include "cImageSelf.h"
#include "FreeImage/FreeImage.h"
#include "MHFile.h"
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//06. 06. �����º� ���� �߰� - �̿���
//�����̸� �տ� ������ ��ȣ ���ڸ� �ο�

extern int g_nServerSetNum;
//////////////////////////////////////////////////////////////////////////////


CGuildMarkManager::CGuildMarkManager()
{
	m_pLoadingImg = NULL;
	m_MarkTable.Initialize(32);
}

CGuildMarkManager::~CGuildMarkManager()
{
//	Release();
}

BOOL CGuildMarkManager::Init()
{

	if(m_pLoadingImg == NULL)
		m_pLoadingImg = LoadMarkImg(0);

	if(m_pLoadingImg == NULL)
		return FALSE;


	return TRUE;
}

void CGuildMarkManager::Release()
{
	m_MarkTable.SetPositionHead();
	while(CGuildMark* pMark = m_MarkTable.GetData())
	{
		pMark->Clear();
		delete pMark;		
	}
	
	m_MarkTable.RemoveAll();



	if( m_pLoadingImg )
	{
		m_pLoadingImg->Release();
		delete m_pLoadingImg;
		m_pLoadingImg = NULL;

	}
}


CGuildMarkImg* CGuildMarkManager::LoadMarkImg(MARKNAMETYPE markname)
{
	/*
	//������ ���� �߰�
	char filename[256];

 	sprintf(filename,"Image/MunpaMark/%d.tga", markname);
	*/

	//////////////////////////////////////////////////////////////////////////////
	//06. 06. �����º� ���� �߰� - �̿���
	//�����̸� �տ� ������ ��ȣ ���ڸ� �ο�
	char filename[256];
 	sprintf(filename, "Image/MunpaMark/%02d_%d.tga", g_nServerSetNum, markname);
	//////////////////////////////////////////////////////////////////////////////

	cImageSelf* pImg = new cImageSelf;
	if(pImg->LoadSprite(filename) == FALSE)
	{
		delete pImg;
		pImg = NULL;
		return NULL;
	}

	CGuildMarkImg* pMarkImg = new CGuildMarkImg(markname,pImg);
	return pMarkImg;
}

BOOL CGuildMarkManager::LoadMarkInfo()
{


	//�̹��� �о��
	return TRUE;
}

void CGuildMarkManager::SendGuildMarkImgRequest(MARKNAMETYPE MarkName)
{
	MSG_DWORD msg;
	SetProtocol(&msg,MP_GUILD,MP_GUILDMARK_REQUEST_SYN);
	msg.dwObjectID = HEROID;
	msg.dwData = MarkName;
	NETWORK->Send(&msg,sizeof(msg));
}
void CGuildMarkManager::SendGuildMarkImgUpdate(DWORD GuildIdx,char* pImgData)
{
	MSG_GUILDMARK_IMG msg;
	SetProtocol(&msg,MP_GUILD,MP_GUILDMARK_UPDATE_SYN);
	msg.dwObjectID = HEROID;
	msg.GuildIdx = GuildIdx;
	memcpy(msg.imgData,pImgData,GUILDMARK_BUFSIZE);
	NETWORK->Send(&msg,sizeof(msg));
}
void CGuildMarkManager::UpdateGuildMarkImg(DWORD GuildIdx,MARKNAMETYPE markname,char* pImgData)
{
	/*
	// �׸� ���Ϸ� ����
	char filename[256];
	sprintf(filename, "Image/MunpaMark/%d.tga", markname);
	*/

	//////////////////////////////////////////////////////////////////////////////
	//06. 06. �����º� ���� �߰� - �̿���

	//�����̸� �տ� ������ ��ȣ ���ڸ� �ο�
	char filename[256];
 	sprintf(filename, "Image/MunpaMark/%02d_%d.tga", g_nServerSetNum, markname);
	//////////////////////////////////////////////////////////////////////////////

	char tempBuf[GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKWIDTH];
	memset(tempBuf,0,GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKWIDTH);
	memcpy(tempBuf,pImgData,GUILDMARK_BUFSIZE);
	WriteTGA(filename,tempBuf,
		GUILDMARKWIDTH,	GUILDMARKWIDTH, 
		GUILDMARKBPP*GUILDMARKWIDTH,
		GUILDMARKBPP*8);

	// ��ũ ���� ���� 
	
	// ���� ���� ��ũ�� ������ ����
	CGuildMark* pMark = m_MarkTable.GetData(markname);
	if(pMark)
	{
		CGuildMarkImg* pMarkImg = LoadMarkImg(markname);
		if(pMarkImg)
			pMark->SetGuildMarkImg(pMarkImg);		
	}
}



CGuildMark* CGuildMarkManager::GetGuildMark(MARKNAMETYPE markname)
{
	if(markname == 0)
		return NULL;
	// ���� ��ũ ������ �׳� �����Ѵ�.
	CGuildMark* pMark = m_MarkTable.GetData(markname);
	if(pMark)
		return pMark;

	// ��ũ �̹����� �ε��Ѵ�
	CGuildMarkImg* pMarkImg = LoadMarkImg(markname);

	// ��ũ �̹����� ���ٸ� �ε��̹����� �ٲٰ� ��ũ�� ��û�Ѵ�.
	if(pMarkImg == NULL)
	{
		pMarkImg = m_pLoadingImg;
		SendGuildMarkImgRequest(markname);
	}

	// ��ũ�̹����� ��ũ�� �����ϰ� �����Ѵ�.
	pMark = new CGuildMark;
	pMark->SetGuildMarkImg(pMarkImg);
	m_MarkTable.Add(pMark,markname);
	return pMark;
}

void CGuildMarkManager::OnRecvGuildMarkImg(MSG_GUILDMARK_IMG* pmsg)
{
	UpdateGuildMarkImg(pmsg->GuildIdx,pmsg->MarkName,pmsg->imgData);
}

BOOL CGuildMarkManager::ChangeGuildMarkSyn(DWORD GuildIdx,char* filename)

{
	char markf[256];
	sprintf( markf, "Image/munpamark/%s", filename );
	char imgData[GUILDMARK_BUFSIZE];
	if(ReadBitInfoFromFile(markf,imgData) == FALSE)
	{
		//�׸� ������ �дµ� �����߽��ϴ�.
		//return FALSE;
		strcat(markf, ".bmp");
		if(ReadBitInfoFromFile(markf,imgData) == FALSE)
			return FALSE;
	}

	SendGuildMarkImgUpdate(GuildIdx,imgData);
	return TRUE;
}

BOOL CGuildMarkManager::ReadBitInfoFromFile(char* filename,char* imgData)
{
//	FIBITMAP* pBitMap = FreeImage_LoadBMP(filename);
	FIBITMAP* pBitMap = FreeImage_Load(FIF_BMP, filename, BMP_DEFAULT);


	BITMAPINFO* pBitInfo = FreeImage_GetInfo(pBitMap);

	if( (pBitMap == 0) || (pBitInfo == 0) )
		return FALSE;
	//����� Ʋ��
	if( pBitInfo->bmiHeader.biHeight != GUILDMARKHEIGHT ||
		pBitInfo->bmiHeader.biWidth != GUILDMARKWIDTH )
		return FALSE;

	FIBITMAP* pConvertedBitmap = FreeImage_ConvertTo16Bits565(pBitMap);

	int bytesPerLine = GUILDMARKWIDTH*GUILDMARKBPP;
	BYTE* pData = FreeImage_GetBits(pConvertedBitmap);

	for(int y=0;y<GUILDMARKHEIGHT;++y)
	{
		memcpy(
			&imgData[(GUILDMARKHEIGHT-y-1)*bytesPerLine],
			&pData[y*bytesPerLine],
			bytesPerLine);
	}
	

	FreeImage_Unload(pBitMap);
	FreeImage_Unload(pConvertedBitmap);

	return TRUE;
}


CGuildMark* CGuildMarkManager::LoadGuildMarkImg( MARKNAMETYPE markname )
{
	CGuildMarkImg* pMarkImg = LoadMarkImg( markname );
	if( pMarkImg == NULL )


		return NULL;


	CGuildMark* pMark = new CGuildMark;
	pMark->SetGuildMarkImg(pMarkImg);
	m_MarkTable.Add(pMark,markname);
	return pMark;
}





