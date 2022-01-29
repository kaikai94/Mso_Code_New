// BigMapDlg.h: interface for the CBigMapDlg class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_)
#define AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"
#include "cFindPatnIcon.h"
class CBigMapDlg : public cDialog
{
protected:

	CMiniMapHeroIcon				m_HeroIcon;	

	CYHHashTable<CMiniMapIcon>		m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon>	m_IconPool;

	CMiniMapIconImage				m_pIconImage[eBMICON_MAX];

	CMiniMapIcon*					m_pIconForToolTip;

	cImageSelf		m_MapImage;
	WORD			m_MapNum;

	cImage			m_TooltipImage;
	//[小地图寻路图标][BY:十里坡剑神][2019/1/28][10:22:34]
	CFindPatnIcon	m_FindPathIcon;
public:
	CBigMapDlg();
	virtual ~CBigMapDlg();


	virtual void SetActive( BOOL val );
	virtual void Render();
	void Linking();
	void LoadMinimapImageInfo( MAPTYPE MapNum );
	void InitBigMap( MAPTYPE MapNum );


	void Process();

	void InitIconImage();


	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,CObject* pObject );
	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,PARTY_MEMBER* pMember );
	void ReleaseMinniMapIcon( CMiniMapIcon* pIcon );
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	
	void AddHeroIcon( CObject* pObject );
	void AddStaticNpcIcon( CObject* pObject );
	void AddPartyMemberIcon( PARTY_MEMBER* pMember );
	void AddIcon( WORD Kind, CObject* pObject );
	void AddIcon( WORD Kind, PARTY_MEMBER* pMember );

	void RemoveIcon( DWORD ObjectID );
	virtual DWORD ActionEvent( CMouse* mouseInfo );


	BOOL CanActive();

	void ShowQuestMarkIcon( CObject* pTarget, int nKind );
	//[小地图自动寻路][BY:十里坡剑神][2019/1/28][10:29:00]
	void OnActionEvent( LONG id, void* p, DWORD we );

	void ClearMovePoint() 
	{ 
		m_FindPathIcon.ClearPoint();
	}
	void SetMovePoint(float x,float z){m_FindPathIcon.SetMousePoint(x,z);}
	void Move_UsePath();
};

#endif // !defined(AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_)


