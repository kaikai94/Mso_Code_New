









// MouseCursor.cpp: implementation of the CMouseCursor class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MouseCursor.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMouseCursor)



CMouseCursor::CMouseCursor()
{

}

CMouseCursor::~CMouseCursor()
{

}


void CMouseCursor::Init( HINSTANCE hInstance )
{
	m_bActive = FALSE;
	m_hCursor[eCURSOR_DEFAULT]		= LoadCursorFromFile("./image/mouse/default.cur");
	m_hCursor[eCURSOR_ATTACK]		= LoadCursorFromFile("./image/mouse/attack.cur");
	m_hCursor[eCURSOR_ITEMSELECT]	= LoadCursorFromFile("./image/mouse/itemsel.cur");
	m_hCursor[eCURSOR_ITEMDRAGGING]	= LoadCursorFromFile("./image/mouse/itemsel.cur");
	m_hCursor[eCURSOR_DEALER]		= LoadCursorFromFile("./image/mouse/dealer.cur");
	m_hCursor[eCURSOR_CANTMOVE]		= LoadCursorFromFile("./image/mouse/cantmove.cur");
	m_hCursor[eCURSOR_DEFAULTCHEAT] = LoadCursorFromFile("./image/mouse/defaultcheat.cur");
	m_hCursor[eCURSOR_TITANREPAIR]	= LoadCursorFromFile("./image/mouse/repair.cur");	// magi82 - Titan(070511) 타이탄 커서가 없어서 임시로 아무거나 넣음
	m_hCursor[eCURSOR_MINING]	= LoadCursorFromFile("./image/mouse/Mining.cur");			// 2007. 7. 11. CBH - 채광스킬 커서 추가
	m_hCursor[eCURSOR_COLLECTION]	= LoadCursorFromFile("./image/mouse/Collection.cur");	// 2007. 7. 11. CBH - 채집스킬 커서 추가
	m_hCursor[eCURSOR_HUNT]	= LoadCursorFromFile("./image/mouse/Hunt.cur");					// 2007. 7. 11. CBH - 사냥스킬 커서 추가
	m_curCursor = eCURSOR_DEFAULT;
	
	m_bCheatCursor = FALSE;
}

void CMouseCursor::Release()
{

}


void CMouseCursor::Process()
{
	::SetCursor(m_hCursor[m_curCursor]);
}

void CMouseCursor::SetCursor( eCURSOR cursor )
{
#ifdef _GMTOOL_
	if( m_bCheatCursor )
		return;
#endif
	if( m_curCursor == cursor ) return;
	m_curCursor = cursor;
	::SetCursor(m_hCursor[m_curCursor]);
}

void CMouseCursor::SetActive( BOOL bActive )
{
	if( m_bActive == bActive ) return;	
	m_bActive = bActive;
	if( bActive )
		::SetCursor(m_hCursor[m_curCursor]);
	
	::ShowCursor( bActive );
}


void CMouseCursor::SetCheatMode( BOOL bCheatMode )
{
	if( m_bCheatCursor == bCheatMode )
		return;

	if( bCheatMode )
		SetCursor(eCURSOR_DEFAULTCHEAT);
	else

		SetCursor(eCURSOR_DEFAULT);

	m_bCheatCursor = bCheatMode;

	SetActive( TRUE );
}






