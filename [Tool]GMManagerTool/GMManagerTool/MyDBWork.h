/* ****************************************************************************
 *  实现功能	：ADO数据库访封装类
 *  作者		：by:OpU_Eye QQ:418914674     
 *  版本		：V 15.0.1
 *  完成日期	：2011.12.15
 *  其它说明	：
 *  修改记录1	：2015平台移植修改
 *    日  期	：2017.1.3
 *    修改人	：by:OpU_Eye QQ:418914674            
 *  修改记录2：
 *****************************************************************************/

#if !defined(AFX_MYDBWORK_H__D777F369_68E5_4FA3_8198_6048A5DE7708__INCLUDED_)
#define AFX_MYDBWORK_H__D777F369_68E5_4FA3_8198_6048A5DE7708__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable:4146)
#import "msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;

//#include "CMyWriteLog.h"
#include <icrsint.h>
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));
#define MAX_BUFF 1024


class CMyDBWork  
{
public:
	CMyDBWork();
	virtual ~CMyDBWork();


public:
	bool	BindingRecord(CADORecordBinding * RsObject, CString szSQLStr);
 	bool	ConnDB		(void);								//连接数据库
 	int		GetConnStr	(CString szConnStr, int nBuffSize);	//获得连接数据库的字符串 
//	bool	SetConnStr	(void);								//设置连接数据库的字符串       
	bool	SetConnStr	(CString szConnStr);					//设置连接数据库的字符串       
 	bool	SelectSQL	(CString szSQL);				//执行select语句
 	bool	ExecuteSQL	(CString szSQLStr, long &nRefreshNum);
	void	CloseDB		(void);								//关闭记录集、连接对象	
	void	CloseRecord	(void);
	bool	IsConnect();
public:
	bool BeginTrans();				//启动一新事务，相当于建立回滚标志
	bool CommitTrans();				//提交BeginTrans之后所有的修改
	bool RollbackTrans();			//取消BeginTrans之后所有的操作
	bool RecordMoveLast();
	bool RecordMoveFirst();
	bool RecordMovePrevious();
	bool RecordMoveNext();
	bool IsRecordBOF();
	bool IsRecordEOF();
	long GetCount();
	

	bool	CollectMsg	(const char *szColumnName, long	&lTemp		);
	bool	CollectMsg	(const char *szColumnName, BYTE &btTemp		);
	bool	CollectMsg	(const char *szColumnName, DWORD &dwTemp	);
	bool	CollectMsg	(const char *szColumnName, UINT &iTemp		);
	bool	CollectMsg	(const char *szColumnName, int &iTemp		);
	bool	CollectMsg	(const char *szColumnName, float &fTemp		);
	bool	CollectMsg	(const char *szColumnName, double &dbTemp	);
	bool	CollectMsg	(const char *szColumnName, CString &chTemp		);
	bool	CollectMsg(const char *szColumnName,int i, CString &chTemp);
	bool	CollectMsg	(const char *szColumnName, CString szBuff, int nBuffSize);

	bool	CollectMsg	(ULONG nIndex, long	&lTemp		);
	bool	CollectMsg	(ULONG nIndex, BYTE &btTemp		);
	bool	CollectMsg	(ULONG nIndex, DWORD &dwTemp	);
	bool	CollectMsg	(ULONG nIndex, UINT &iTemp		);
	bool	CollectMsg	(ULONG nIndex, int &iTemp		);
	bool	CollectMsg	(ULONG nIndex, float &fTemp		);
	bool	CollectMsg	(ULONG nIndex, double &dbTemp	);
	//bool	CollectMsg	(ULONG nIndex, char &chTemp		);
	bool	CollectMsg	(ULONG nIndex, char *szBuff, int nBuffSize);
	
//private:
//private:
	CString m_szConnStr;
	long m_count;
//private:
	_ConnectionPtr m_pConn;
	_RecordsetPtr  m_pRecord;
	_CommandPtr    m_pCommand; 
	_ParameterPtr  m_pParameter;
};

#endif // !defined(AFX_MYDBWORK_H__D777F369_68E5_4FA3_8198_6048A5DE7708__INCLUDED_)



//////////////////////////////////////////////////////////////////////////
////////////////////以下是以绑定记录集方式提取数据的Demo//////////////////
//////////////////////////////////////////////////////////////////////////
// CMyDBWork DbObject;
// if(DbObject.ConnDB())
// {
// 	CCarInfoObj CarObj;
// 	if (DbObject.BindingRecord(&CarObj, "SELECT top(10)Car_mark, carid, gsm_number, car_type FROM car_info")) {
// 		while (! DbObject.IsRecordEOF()) {
// 			if (adFldOK == CarObj.m_nStatusArray[0] && adFldOK == CarObj.m_nStatusArray[0] && adFldOK== CarObj.m_nStatusArray[0]) {
// 				CString szShow;
// 				szShow.Format("CarMark:%s, CarID:%d, GSM:%s", CarObj.m_szCarMark, CarObj.m_nCarID, CarObj.m_szGSM);
// 				AfxMessageBox(szShow);
// 			}
// 			DbObject.RecordMoveNext();
// 		}
// 	}
// }
// else{
// 	char szBuff[100]; memset(szBuff, 0, 100);
// 	int nR = GetErrorStr(szBuff, GetLastError());
// 	int i = 0;
// }

//////////////////////////////////////////////////////////////////////////
/////////////////////////以下是关于CarObj的定义///////////////////////////
//////////////////////////////////////////////////////////////////////////
// #include "..\COMMFILE\MyDBWork.h"
// #include "icrsint.h"
//
// class CCarInfoObj : public CADORecordBinding  
// {
// 	BEGIN_ADO_BINDING(CCarInfoObj)
// 		ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_szCarMark, sizeof(m_szCarMark)	, m_nStatusArray[0], false)
// 		ADO_VARIABLE_LENGTH_ENTRY2(2, adInteger, m_nCarID	, sizeof(m_nCarID)		, m_nStatusArray[1], false)
// 		ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_szGSM	, sizeof(m_szGSM)		, m_nStatusArray[2], false)
// 		END_ADO_BINDING()
// 		
// public:
// 	char  m_szCarMark[20];
// 	ULONG m_nCarID;
// 	char  m_szGSM[20];
// 	ULONG m_nStatusArray[3];
// 	
// public:
// 	CCarInfoObj();
// 	virtual ~CCarInfoObj();
// 	
// };