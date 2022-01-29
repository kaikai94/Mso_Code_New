




// MiniMapDlg.h: interface for the CMiniMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)
#define AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"
#include "cFindPatnIcon.h"
class cStatic;
class cPushupButton;


struct MINIMAPIMAGE
{
	cImageSelf pMiniMapImage;
	DWORD ImageWidth, ImageHeight;
};

struct stParticle
{

	VECTOR2 vPos;
	VECTOR2 vDir;
	stParticle()
	{
		vPos.x = vPos.y = 0;
		vDir.x = vPos.y = 0;
	}
};

struct stNumImage	
{
	cImage* pImage;
	DWORD dwW,dwH;
	stNumImage():pImage(NULL),dwW(0),dwH(0){}
};

struct stCipherNum	
{
	stCipherNum():dwNumber(99),dwRealCipherNum(0),bIsAni(FALSE){}
	DWORD dwNumber;

	DWORD dwRealCipherNum;
	bool bIsAni;
};


class CMiniMapDlg : public cDialog  
{
//	DWORD TempID;
	
	cStatic* m_pMapName;

	cStatic* m_pPosXY;	

	cPushupButton* m_pZoom;	

	MAPTYPE m_MapNum;

	CMiniMapHeroIcon m_HeroIcon;
	//[小地图寻路图标][BY:十里坡剑神][2019/1/28][10:22:34]
	CFindPatnIcon	m_FindPathIcon;
	CYHHashTable<CMiniMapIcon> m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon> m_IconPool;
	CMiniMapIconImage m_pIconImage[eMNMICON_MAX];
	
	int m_CurMode;
	MINIMAPIMAGE m_MinimapImage[eMINIMAPMODE_MAX];



	cImage m_TooltipImage;
	float m_DisplayMinX,m_DisplayMinZ;		

	float m_DisplayMaxX,m_DisplayMaxZ;		
	VECTOR2 m_DiplayOriginPosOfImage;
	VECTOR2 m_ImageDisplayWH;



	void InitIconImage();

	void SetZoomRate(float zoomrate);

	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,CObject* pObject);
	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember);
	void ReleaseMinniMapIcon(CMiniMapIcon* pIcon);

	void LoadMinimapImageInfo(MAPTYPE MapNum);

	void AddIcon(WORD Kind,CObject* pObject);
	void AddIcon(WORD Kind,PARTY_MEMBER* pMember);

#define NUMIMAGE_W 8

#define NUMIMAGE_H 14


#define BASIC_ANI_TIMELENGTH	2000
#define BETWEEN_ANI_TIMELENGTH	500		
#define NUM_CHANGE_TIMELENGTH	10		
#define	DOT_NUM					2

	enum{DEFAULT_IMAGE = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, IMG_DOT, IMAGE_COUNT, CIPHER_NUM = 9};

	stNumImage	m_stNumImage[IMAGE_COUNT];	
	VECTOR2		m_vPos[CIPHER_NUM];		
	VECTOR2		m_vDotPos[DOT_NUM];		

	DWORD		m_dwTotalMoney;			

	DWORD		m_dwOldTotalMoney;			
	DWORD		m_dwTempMoney;		

	stCipherNum m_stCipherNum[CIPHER_NUM];	
	DWORD		m_dwAniStartTime;		
	DWORD		m_dwNumChangeTime;		
	DWORD		m_dwIntervalAniTime;	
	DWORD		m_dwMaxCipher;			
	DWORD		m_dwCipherCount;		
	bool		m_bIsAnimationing;		
	bool		m_bDoSequenceAni;			

public:


	CMiniMapDlg();
	virtual ~CMiniMapDlg();
	virtual DWORD ActionEvent( CMouse* mouseInfo );
	void InitMiniMap(MAPTYPE MapNum);
	

	void Linking();
	virtual void SetActive( BOOL val );

	void ToggleMinimapMode();
	
	void AddMonsterIcon(CObject* pObject);
	void AddBossMonsterIcon(CObject* pObject);
	void AddStaticNpcIcon(CObject* pObject);


	void AddPartyMemberIcon(PARTY_MEMBER* pMember);
	void AddPartyMasterIcon(CObject* pObject);

	void AddQuestIcon( CObject* pTarget, int nKind );
	
	void AddHeroIcon(CObject* pObject);

	void RemoveIcon(DWORD ObjectID);

	void FitToCenterIcon(CMiniMapIcon* pIcon);
	void Process();
	void Render();

	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	
	int GetCurMode()	{ return m_CurMode; }
	
	BOOL CanActive();

	void RefreshMode();
	void ShowQuestMarkIcon( CObject* pTarget, int nKind );

	void InitNumImage();				
	void ReleaseNumImage();				
	void SetNumImagePos();			
	void ConvertCipherNum();		
	bool IsNumChanged();				

	void DoAni();
	void DoSequenceAni();
	void JPRender();
	void JPProcess();

	
	void InitForAni();
	void InitForSequenceAni();
	void JPInit();
	//[小地图自动寻路][BY:十里坡剑神][2019/1/28][10:29:00]
	void ClearMovePoint() 
	{ 
		m_FindPathIcon.ClearPoint();
	}		
	void SetMovePoint(float x,float z){m_FindPathIcon.SetMousePoint(x,z);}
	void Move_UsePath();
};


#endif // !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)


