//////////////////////////////////////////////////////////////////////
// Coolwater.Fire version 2001.1.15
// Ultimate Database Gasoline Engine
// If it does not work correctly, plz enter www.coolwater.fire.com 
// programed by Park Sung Jun 
#if !defined(AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_)
#define AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_

#pragma once

#include "stdafx.h"
#include <sql.h>
#include <sqlext.h>
#include <initguid.h>

#pragma pack(push, 1)



#define STR_LEN 256
#define MSG_BUF_SIZE 256
#define MAX_LENGTH_QUERY 3072
#define MAX_COL_LENGTH 20					//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
#define MAX_COL_NAME_LENGTH 24	
#define	MAX_NUM_SQL_CONNECTION			5 	//[数据库最大连接数][By:十里坡剑神[QQ:112582793][2018/1/3] 
#define MAX_COL_NUM 65 						//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
#define MAX_ROW_NUM 100 

#define MAX_LARGECOL_LENGTH 6144	
#define MAX_LARGECOL_NUM 20 

#define MAX_MIDDLECOL_LENGTH 311	
#define MAX_MIDDLECOL_NUM 20 
#define MAX_MIDDLEROW_NUM 20




///////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*VOIDFUNC)(void);

typedef struct {
	DWORD				dwMessageID;		

	BOOL				bNotifyOnlyError;	
	LPVOID				pData;				
	BYTE				bFNNo;				
	DWORD				dwResult;		

	DWORD				dwID;
} DBMESSAGE, *LPDBMESSAGE;

struct DESC_DBTHREAD
{
	BYTE		bUsingThread;
	BYTE		bUsingDLL;
	BYTE		bUsingEventObject;
	HWND		hWndToPostMessage;
    void		(*ReadDBResult)(DWORD ret,  LPDBMESSAGE pMessage);
	void		(*WriteMessage)(char* pmsg);
	
	DWORD		dwMessageMaxSize;		
	DWORD		dwNumofMessage;		
	DWORD		dwResultMaxSize;		
	DWORD		dwNumofResult;			
	DWORD		dwSizeofStack;			
	DWORD		dwDBMessageID;

	char		Default_User_table[128];
	char		Default_User_account[128];
	char		Default_User_pw[128];
	char		Default_User_guid[128];
	char		AdminID[128];

	char		ClientID[128];
	char		AdminPW[128];
	char		ClientPW[128];
	char		DBName[128];
	char		AdminCursor[128];
	char		ClientCursor[128];

};



typedef struct {
	LPDBMESSAGE	lpDBMessage;
	DWORD		Ret;
} DBRESULTMESSAGE, *LPDBRESULTMESSAGE;



typedef struct
{
	char Data1[MAX_COL_LENGTH];
} DBDATA1, *LPDATA1;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
} DBDATA2, *LPDATA2;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
} DBDATA3, *LPDATA3;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
} DBDATA4, *LPDATA4;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
} DBDATA5, *LPDATA5;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
} DBDATA6, *LPDATA6;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
} DBDATA7, *LPDATA7;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
} DBDATA8, *LPDATA8;
typedef struct
{

	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
	char Data9[MAX_COL_LENGTH];
} DBDATA9, *LPDATA9;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];

	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
	char Data9[MAX_COL_LENGTH];
	char Data10[MAX_COL_LENGTH];
} DBDATA10, *LPDATA10;

typedef struct
{
	TCHAR	szDBName[STR_LEN];			
	TCHAR	szID[STR_LEN];			
	TCHAR	szPW[STR_LEN];				
	SQLHDBC	hDBC;						
	HSTMT	hStmt;						
	TCHAR	szCursorName[STR_LEN];		
	int		nCursorNum;					
	HSTMT	hScrollStmt;
} DBCON, *LPDBCON;

typedef struct
{
	UCHAR   Data[MAX_COL_NUM][MAX_COL_LENGTH]; 
	SDWORD  Length[MAX_COL_NUM];			
	int		nNum;						
} QUERYST, *LPQUERY;

typedef struct
{
	UCHAR   Data[MAX_LARGECOL_NUM][MAX_LARGECOL_LENGTH];  
	SDWORD  Length[MAX_LARGECOL_NUM];			
	int		nNum;						
} LARGEQUERYST, *LPLARGEQUERY;

typedef struct
{
	UCHAR   Data[MAX_MIDDLECOL_NUM][MAX_MIDDLECOL_LENGTH];  
	SDWORD  Length[MAX_MIDDLECOL_NUM];		
	int		nNum;						
} MIDDLEQUERYST, *LPMIDDLEQUERY;

typedef struct
{
	int		nNum;							
	LPVOID	pData;
	LPVOID	pLength;
} QUERYDA, *LPQUERYDA;

typedef struct
{
	UCHAR	 szData[MAX_COL_LENGTH];
} DBSTRING, *LPDBSTRING;

typedef struct
{
	SDWORD	 pLeng;
} DBSDWORD, *LPDBSDWORD;


typedef struct
{
	int			nNumData;
	int			nColNum;
	LPVOID		pData[MAX_ROW_NUM];
	SQLSMALLINT QueryType[MAX_COL_NUM];
} BINDST, *LPBIND;

typedef struct
{
	UCHAR		Data[MAX_COL_NUM][MAX_COL_LENGTH];  

	SDWORD		Length[MAX_COL_NUM];			
	SDWORD		LengthRead[MAX_COL_NUM];		
} ROWWISEBIND, *LPROWWISEBIND;


typedef struct
{
	LPDBCON pCon;
	char	szBuffer[MAX_LENGTH_QUERY];
	DWORD	dwNumRow;
} DBFreeQuery, *LPDBFreeQuery;
typedef struct
{
	LPDBCON pCon;
	char	szTable[MAX_COL_NAME_LENGTH];
	DWORD	dwValue;
	char	lpCol[MAX_COL_NAME_LENGTH];
	SQLSMALLINT	sqltype;
} DBSingleQuery, *LPDBSingleQuery;
typedef struct
{
	LPDBCON pCon;
	char	szBuffer[MAX_LENGTH_QUERY];
	LPBIND	lpBind;
	int		StSize;
	SQLSMALLINT	sqlType;
	int		rowset;
} DBGenerateResultSet, *LPDBGenerateResultSet;
typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];

} DBExcuteIUD, *LPDBExcuteIUD;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	lpCol[MAX_COL_NAME_LENGTH];
	char	lpValue[MAX_COL_LENGTH];
	char	DestCol[MAX_COL_NAME_LENGTH];
	char	DestValue[MAX_COL_LENGTH];
} DBUpdateRecord, *LPDBUpdateRecord;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	lpCol[MAX_COL_NAME_LENGTH];
	char	lpValue[MAX_COL_LENGTH];
} DBDeleteRecord, *LPDBDeleteRecord;
typedef struct

{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	LPVOID	lpCol;
	LPVOID	lpValue;
	BYTE	bNumData;
} DBInsertRecord, *LPDBInsertRecord;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	pColID[MAX_COL_NAME_LENGTH];
	char	pColPW[MAX_COL_NAME_LENGTH];
	char	pID[MAX_COL_NAME_LENGTH];
	char	pPW[MAX_COL_NAME_LENGTH];
	char	pRetCol[MAX_COL_NAME_LENGTH];
	LPVOID	pRetValue;
	int		iLength;
	SQLSMALLINT sqlType;
} DBLogin, *LPDBLogin;
typedef struct
{
	LPDBCON pCon;
	char	pID[MAX_COL_NAME_LENGTH];
	char	pPW[MAX_COL_NAME_LENGTH];
	LPVOID	pRetValue;
} DBSimpleLogin, *LPDBSimpleLogin;

typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];
	int		nSize;
	LPVOID	pPara;
	long	nBuffersize;
	LPVOID	pSettingData;
} DBSaveDataByPtr, *LPDBSaveDataByPtr;

typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];
	int		nSize;
	LPVOID	pData;
} DBLoadDataByPtr, *LPDBLoadDataByPtr;

#pragma pack(pop)


#endif // !defined(AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_)



