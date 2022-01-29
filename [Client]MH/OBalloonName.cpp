








// OBalloonName.cpp: implementation of the COBalloonName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OBalloonName.h"
#include "ObjectManager.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cMultiLineText.h"
#include "./interface/cScriptManager.h"
#include "./interface/cFont.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////


DWORD  g_NameColor[21] ={0X1C19FF,0x1EFF16,0xFF00FF,0xFF0000,0xFFFF00,0xFFFFFF,0xFFFF,0xFF7F00,0x95FF91,0xF9F95C,0x71FFFF,
0X0300FA,0XE39D53,0XDC006B,0XFF73FD,0XB266B1,0X19FF8F,0XF7FA00,0X9EA000,0XDC00D9,0X23D5D7
};
static const DWORD nameColor[] = {0xffffff,0xED008C,0xf18b18,0xda743d,
0xca6459,0xb95374,0xa64095,0x8f29ba,0x6e08f1};
static const DWORD changeTime = 300;
#define COLOR_LEN 10
static const DWORD ImagechangeTime = 80;  //[图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]


COBalloonName::COBalloonName()

{
	m_wFontIdx	= 0;
	
	m_lTall		= 0;
	m_lPosX		= 0;

	m_lMasterTall	= 0;
	m_lMasterPosX	= 0;

	m_lNickTall = 0;
	m_lNickPosX = 0;
	m_lMarryPosX = 0;
	m_lMarryTall = 0;	
	m_lShiTuPosX=0;
    m_lShiTuTall=0;
	m_NameTime = 0;
	m_FlashNameFlag = 0;
	m_nameIndex = 0;
	m_ChangeMode = 0;

	m_fgColor	= RGB_HALF(255,255,255);


	m_bActive	= FALSE;

	m_bShow		= FALSE;

	ZeroMemory( m_szObjectName, sizeof(m_szObjectName) );
	ZeroMemory( m_szObjectNickName, sizeof(m_szObjectNickName) );
	ZeroMemory( m_szPetMasterName, sizeof(m_szPetMasterName) );
	ZeroMemory( m_szMarryName, sizeof(m_szMarryName) );
	
	m_pSiegeName = new cMultiLineText;
	m_bSiegeMap = FALSE;
	//图片称号	by:十里坡剑神	QQ:112582793
	ImageNameInfo = NULL;
	ImageNameCount = 0;
	m_ImageNameTime = 0;
	Ischange =FALSE;
	m_bIsShowTop = FALSE;
}

COBalloonName::~COBalloonName()
{
	SAFE_DELETE( m_pSiegeName );
}

void COBalloonName::InitName(char * name)
{
	if( !name ) return;
	if( *name == 0 ) return;
	SafeStrCpy( m_szObjectName, name, MAX_MONSTER_NAME_LENGTH +1 );	
	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName))/2);
}

void COBalloonName::SetName(char* name)
{
	if( !name ) return;
	if( *name == 0 ) return;
	SafeStrCpy( m_szObjectName, name, MAX_MONSTER_NAME_LENGTH +1 );

	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName))/2);
}

void COBalloonName::SetNickName(char* nickname)
{


	if( !nickname ) return;
	SafeStrCpy( m_szObjectNickName, nickname, MAX_GUILD_NICKNAME+1 );

	SetNickPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectNickName, strlen(m_szObjectNickName))/2);
}

void COBalloonName::SetPetMasterName(char* petname)
{

	if( !petname )	return;
	SafeStrCpy( m_szPetMasterName, petname, MAX_MASTERNAME_LENGTH+1 );

	SetPetMasterNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szPetMasterName, strlen(m_szPetMasterName))/2);
}
BOOL COBalloonName::Render(LONG absX, LONG absY)
{
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() )	return FALSE;
#endif
	if(!m_bActive)	return FALSE;
	if(!m_bShow)	return FALSE;
	absY -= 15;
	RECT rect = {(long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1};	

	if( m_bSiegeMap )
	{
		int nSize = strlen(m_szObjectName) + 2;
		m_pSiegeName->SetXY( rect.left,  rect.top );
		m_pSiegeName->Render();

		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));

		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( m_fgColor, 255 ));
	}

	else if( *m_szObjectName != 0 )
	{

		if(HERO->IsPKMode() == TRUE)
		{
		
			int nSize = strlen(m_szObjectName) + 2;
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));

			rect.left -= 1;
			rect.top -= 1;
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( m_fgColor, 255 ));
		
		}
		else
		{
			int nSize = strlen(m_szObjectName) + 2;
			BYTE bR =234; BYTE bG =0; BYTE bB =220;
			DWORD tColor =RGB_HALF(bR,bG,bB);

			if ((int)m_FlashNameFlag == 1 && GetTickCount() >= m_NameTime)
			{
				m_fgColor = g_NameColor[m_nameIndex];
				m_NameTime = GetTickCount() + changeTime;

				if (m_ChangeMode == 0)
					m_nameIndex++;
				else
					m_nameIndex--;

				if (m_nameIndex >= COLOR_LEN && m_ChangeMode == 0)
				{
					m_ChangeMode = 1;
					m_nameIndex = COLOR_LEN - 1;
				}
				else if(m_nameIndex <= 0 && m_ChangeMode == 1)
				{
					m_ChangeMode = 0;
					m_nameIndex = 0;
				}
			}

			CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
			rect.left -= 1;
			rect.top -= 1;
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( m_fgColor, 255 ));

		}
	}
	if(m_ImageName!=0&&!HERO->GetAvatarView())
	{//[图片称号][By:十里坡剑神][QQ:112582793][2017/12/9]
		if(ImageNameInfo)
		{
			if(ImageNameInfo->IsTrends&& GetTickCount() >= m_ImageNameTime)
			{
				m_ImageNameTime = GetTickCount() + ImagechangeTime;
				if(!Ischange)
				{
					ImageNameCount++;
				}
				else if(Ischange)
				{
					ImageNameCount= m_ImageName;

					Ischange = FALSE;
				}
				if(ImageNameCount>=ImageNameInfo->ImageCount+m_ImageName-1)
				{
					Ischange = TRUE;
				}
			}
			cImage  image;
			SCRIPTMGR->GetImage(ImageNameCount,&image,PFT_IMAGENAME);
			VECTOR2 curpos;

			curpos.x = (long)((absX-(image.GetImageRect()->right-image.GetImageRect()->left)/2)+ImageNameInfo->LeftPosition);
			curpos.y = (long)(absY-ImageNameInfo->Hight);
			image.RenderSprite(&ImageNameInfo->Scaling,NULL,0,&curpos,0xffffffff);

		}
	}

	RECT nickrect = {(long)(absX + m_lNickPosX), (long)(absY + m_lNickTall - 15.0f), 1, 1};
	if( *m_szObjectNickName != 0 )
	{
		int nSize = strlen(m_szObjectNickName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		nickrect.left -= 1;
		nickrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE( m_fgColor, 255 ));
	}

	if( *m_szObjectNickName && *m_szPetMasterName )	ASSERT(0);


	RECT masterrect = {(long)(absX + m_lMasterPosX), (long)(absY + m_lMasterTall - 12.0f), 1, 1};
	if( *m_szPetMasterName != 0 )
	{
		int nSize = strlen(m_szPetMasterName) + 2;

		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		masterrect.left -= 1;
		masterrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE( m_fgColor, 255 ));
	}

	RECT marryrect = {(long)(absX + m_lMarryPosX), (long)(absY + m_lMarryTall - 32), 1, 1};	
	if( *m_szMarryName != 0 )
	{   
		int nSize = strlen(m_szMarryName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szMarryName, nSize, &marryrect, RGBA_MERGE( RGB_HALF(70 ,70 ,70), 180));	
		marryrect.left -= 1;
		marryrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szMarryName, nSize, &marryrect, RGBA_MERGE( RGB_HALF(255 ,0 ,152), 255 ));	
	}

	RECT shiturect = {(long)(absX + m_lShiTuPosX), (long)(absY + m_lShiTuTall - 67), 1, 1};

	//[排行标志显示][By:十里坡剑神][QQ:112582793][2018/2/25]
	if(m_bIsShowTop)
	{
		cImage  TopListImage;
		TopListImage.SetNull();
		if(TopListFlg[0])
			SCRIPTMGR->GetImage(244,&TopListImage,PFT_HARDPATH);
		else if(TopListFlg[1])
			SCRIPTMGR->GetImage(245,&TopListImage,PFT_HARDPATH);
		else if(TopListFlg[2])
			SCRIPTMGR->GetImage(246,&TopListImage,PFT_HARDPATH);
		if(!TopListImage.IsNull())
		{
			VECTOR2 curpos;
			VECTOR2 Scaling;
			Scaling.x=Scaling.y=0.75;
			curpos.x = shiturect.left-20;
			curpos.y = shiturect.top;
			TopListImage.RenderSprite(&Scaling,NULL,0,&curpos,0xffffffff);
		}
	}

	return TRUE;
}

void COBalloonName::SetPosX(LONG	pos)
{ 
	m_lPosX = pos;	
}


void COBalloonName::SetNickPosX(LONG pos)
{
	m_lNickPosX = pos;
}


//
void COBalloonName::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	cImage Image;
	DWORD Color = TTCLR_CASTLEGUILD;

	switch( NameType )
	{
	case eSiegeWarNameBox_CastleGuild:
		Color = TTCLR_CASTLEGUILD;
		break;
	case eSiegeWarNameBox_SiegeGuild:
		Color = TTCLR_SIEGEGUILD;
		break;
	case eSiegeWarNameBox_Enermy:
		Color = TTCLR_ENERMY;
		break;
	}

	char buf[MAX_NAME_LENGTH] = { 0, };
	SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);	
	m_pSiegeName->Init( 0, RGB_HALF(255,255,255), &Image, Color );
	m_pSiegeName->SetImageAlpha( TOOLTIP_ALPHA );
	m_pSiegeName->AddNamePannel( dwLength );


	m_bSiegeMap = TRUE;
}

void COBalloonName::SetFontIndex(BOOL bFontIndex)
{
	m_wFontIdx = bFontIndex;

}
void COBalloonName::SetMarryName(char* marryname)
{
	if( !marryname )	return;

	if (strcmp(marryname, "0") == 0)
	{
		ZeroMemory( m_szMarryName, sizeof(m_szMarryName) );
		return;
	}

	SafeStrCpy( m_szMarryName, marryname, MAX_MASTERNAME_LENGTH+1 );

	SetMarryNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szMarryName, strlen(m_szMarryName))/2);
}


//[设置图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]
void COBalloonName::SetImageName(WORD val)
{
	if(val>0)
	{
		m_ImageName = val;
		ImageNameInfo=SCRIPTMGR->GetImageNameInfo(val);
		ImageNameCount = m_ImageName;
	}
	else
		m_ImageName = 0;
}
void COBalloonName::SetTopListFlg(WORD val)
{
	TopListFlg[val] = TRUE;
	m_bIsShowTop = TRUE;
}
void COBalloonName::InitTopListFlg()
{
	TopListFlg[0] = FALSE;
	TopListFlg[1] = FALSE;
	TopListFlg[2] = FALSE;
	m_bIsShowTop = FALSE;
}