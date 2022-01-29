// OBalloonName.h: interface for the COBalloonName class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_)
#define AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cMultiLineText;

class COBalloonName  
{

public:
	BOOL m_bIsShowTop;
	COBalloonName();
	virtual ~COBalloonName();

	void InitName(char * name);
	void SetName(char* nickname);
	void SetNickName(char* nickname);
	void SetPetMasterName(char* petname);
	// set marryname 2014-05-09
	void SetMarryName(char* marryname);

	void SetFlashNameFlag(WORD Flag){m_FlashNameFlag=Flag;}

	BOOL IsActive()						{ return m_bActive;	}
	BOOL IsShow()						{ return m_bShow;	}
	void SetActive(BOOL val)			{ m_bActive = val;	}
	void Show( BOOL val )				{ m_bShow	= val; 	}
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)			{ m_lTall = Tall;	}
	void SetPosX(LONG	pos);
	void SetNickPosX(LONG pos); 

	void SetPetMasterNameTall(LONG Tall)			{ m_lMasterTall = Tall;	}
	void SetPetMasterNamePosX(LONG pos)				{ m_lMasterPosX = pos;	}
	void SetNickNameTall(LONG	nickTall)			{ m_lNickTall = nickTall;	}
	void SetNickNamePosX(LONG	nickpos)			{ m_lNickPosX = nickpos;	}
	void SetMarryNamePosX(LONG pos)					{ m_lMarryPosX = pos; }
	void SetShiTuNamePosX(LONG pos)					{ m_lShiTuPosX = pos;}

	void SetFGColor(DWORD col)			{ m_fgColor = col;	}
	
	LONG GetPosX()						{ return m_lPosX; }
	LONG GetTall()						{ return m_lTall; }

	LONG GetNickPosX()					{ return m_lNickPosX; }

	void SetSiegeName( DWORD NameType, DWORD dwLength );

	void SetFontIndex(BOOL bFontIndex);
	//[设置图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]
	void SetImageName(WORD val);
	//[设置排行标志][By:十里坡剑神][QQ:112582793][2018/2/24]
	void SetTopListFlg(WORD val);
	void InitTopListFlg();
protected:

	BOOL m_bActive;			
	BOOL m_bShow;			
	LONG m_lTall;
	LONG m_lPosX;
	LONG m_lNickTall;
	LONG m_lNickPosX;

	LONG m_lMarryPosX;
	LONG m_lMarryTall;

	LONG m_lShiTuPosX;
    LONG m_lShiTuTall;

	LONG m_lMasterTall;
	LONG m_lMasterPosX;
	char m_szObjectName[MAX_MONSTER_NAME_LENGTH +1];
	BOOL m_wFontIdx;
	DWORD m_fgColor;
	char m_szObjectNickName[MAX_GUILD_NICKNAME+1];
	char m_szPetMasterName[MAX_MASTERNAME_LENGTH+1];
	char m_szMarryName[MAX_MASTERNAME_LENGTH+1];
	WORD m_FlashNameFlag;  
	DWORD m_NameTime;
	int m_nameIndex;
	BYTE m_ChangeMode;
	
	BOOL m_bSiegeMap;
	cMultiLineText* m_pSiegeName;
	//[设置图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]
	WORD m_ImageName;
	int ImageNameCount;
	DWORD m_ImageNameTime;
	BOOL  Ischange;
	IMAGENAMEINFO* ImageNameInfo;
	//[排行标志][By:十里坡剑神][QQ:112582793][2018/2/24]
	BOOL TopListFlg[3];

};

#endif // !defined(AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_)


