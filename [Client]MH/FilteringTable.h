



// FilteringTable.h: interface for the CFilteringTable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CFILTERINGTABLE_H_
#define _CFILTERINGTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MHCLIENT_
#define FILTERTABLE	USINGTON(CFilteringTable)
#else
#define FILTERTABLE	CFilteringTable::GetInstance()
#endif


#define FILEPATH_FILTERWORD "./Resource/FilterWord.bin"


enum FILTERKIND
{
	eFilter_GM,
	eFilter_System,
	eFilter_Slang,
	eFilter_Byte,
	
	eFilter_Count,
};



enum eFILTER_METHOD
{
	eFM_WHOLE_MATCH,
	eFM_INCLUDE,		
	eFM_ALLOWSPACE,			
};


//#define _FILTER_TEST_

struct FILTER_NODE
{
	char cChar;
	char cExChar;
	char bEndFlag;


	FILTER_NODE* pChild;
	FILTER_NODE* pSibling;

	FILTER_NODE():pChild(0),pSibling(0),bEndFlag(0),cExChar(0),cChar(0) {}
};


class CFilteringTable
{


//#ifdef _JAPAN_LOCAL_
//
//protected:
//	struct RANGE_ARRAY
//	{
//		WORD Start;
//		WORD End;
//
//		BOOL IsInRange( WORD DBChar )
//		{
//
//			if( DBChar >= Start && DBChar <= End ) return TRUE;
//			return FALSE;
//		}
//	};
//
//	RANGE_ARRAY*	m_RangeArray;
//	DWORD			m_dwRangeArrayCount;
//
//public:
//
//	BOOL IsInDBCRange( WORD DBChar )
//	{
//		for( DWORD i = 0 ; i < m_dwRangeArrayCount ; ++i )
//		{
//			if( m_RangeArray[i].IsInRange( DBChar ) )
//				return TRUE;
//		}
//		return FALSE;
//	}
//	
//
//
//#endif


protected:

	FILTER_NODE		m_RootNode[eFilter_Count];
	int				m_nStrPos;
	BOOL			m_bSearched;
	BOOL			m_bSpread;
	char			m_strBuf[256];

	FILTER_NODE*	m_pCurNode;

	char*			m_pCurStrPos;

#ifdef _FILTER_TEST_
	int				m_nMemory;
#endif

	
protected:

	void AddWord( char* pWord, int nKind );
	void AddNode( FILTER_NODE* pNode, char* pWord );
	void DeleteNode( FILTER_NODE* pNode );
	void CleanNode( FILTER_NODE* pNode );

	void SearchNode( FILTER_NODE* pNode, char* pStr, int nDepth, BOOL bDBC );	

	BOOL LoadFilterWord( char* strFile );
	BOOL LoadFilterWordBinary( char* strFile );
	void SettingFilterWord( char* pStr, DWORD bufLen );

	BOOL FM_WholeMatch( char* pStr, int nKind );

	BOOL FM_Include( char* pStr, int nKind );

	int RemoveSpace( char* pStrIn, char* pStrOut );

public:

#ifndef _MHCLIENT_
	MAKESINGLETON( CFilteringTable )

#endif

	CFilteringTable();
	virtual ~CFilteringTable();
	
	void Init();
	void Release();

	BOOL FilterWordInString( char* pStr, int nKind, int nMethod );

	BOOL IsInvalidCharInclude( unsigned char* pStr, BOOL AllowAllDBC = FALSE );
	BOOL IsUsableName( char* pName, BOOL bUseByte = TRUE );
	BOOL FilterChat( char* pChat );

	BOOL IsCharInString( char* pStr, char* pNoChar );
	BOOL IsSendItemInfoMsg(char * pStr);

#ifdef _FILTER_TEST_
	int	GetMemory() { return m_nMemory; }

#endif

};

#ifdef _MHCLIENT_
EXTERNGLOBALTON(CFilteringTable)
#endif





#endif // _CFILTERINGTABLE_H_


