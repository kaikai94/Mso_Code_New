#if !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)
#define AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000


#include "cWindowDef.h"

#include "ClientGameDefine.h"
#include "ClientGameStruct.h"

class cWindow;
class cImage;
class CMHFile;



#define SCRIPTMGR	USINGTON(cScriptManager)
enum ePATH_FILE_TYPE
{
	PFT_HARDPATH = 0,
	PFT_ITEMPATH	,
	PFT_MUGONGPATH	,
	PFT_ABILITYPATH ,
	PFT_BUFFPATH ,
	PFT_MINIMAPPATH ,
	PFT_JACKPOTPATH,
	PFT_IMAGENAME,  //[Í¼Æ¬³ÆºÅ][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/29]
};

struct sIMAGHARDPATH
{

	int	idx;
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
};


class cScriptManager  
{
public:
	cScriptManager();

	virtual ~cScriptManager();

	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");


	cWindow * GetInfoFromFile(cWindow * wnd, CMHFile * fp);
	cWindow * GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp);

	void GetImage( int idx, cImage * pImage, cImageRect * rect );
	void GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type = PFT_HARDPATH );

	void CheckPosForResolution(cPoint* pPos);

	BOOL InitParseTypeData();		
	void ReleaseParseTypeData();	

	////////////////////////////////////////////////////////////
	void InitScriptManager();
	//[Í¼Æ¬³ÆºÅ][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/29]
	IMAGENAMEINFO* GetImageNameInfo(int idx){return m_ImageNameInfo.GetData(idx);}

	void cScriptManager::ReleaseImagePathTable();
protected:
	void GetImage( CMHFile * fp, cImage * pImage );

	char * GetMsg( int idx );

	int ChangeHashKey(const char* cpTypeName, int nMaxNum);		
	PARSE_TYPE GetParseType(const char* str);		
	CYHHashTable<PARSE_TYPE_INFO> m_ParseTypeTable; 
	///////////////////////////////////////////////////////

	CYHHashTable<sIMAGHARDPATH> m_ImageHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_ItemHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MugongHardPath;
	CYHHashTable<sIMAGHARDPATH> m_AbilityHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_BuffHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MiniMapHardPath;
	CYHHashTable<sIMAGHARDPATH> m_JackPotHardPath;
	//[Í¼Æ¬³ÆºÅ][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/29]
	CYHHashTable<sIMAGHARDPATH> m_ImageNamePath;
	CYHHashTable<IMAGENAMEINFO> m_ImageNameInfo;
};
EXTERNGLOBALTON(cScriptManager);
#endif // !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)



