// MyDBWork.cpp: implementation of the CMyDBWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDBWork.h"
//#include <icrsint.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDBWork::CMyDBWork()
{
	m_pConn = NULL;
	m_pRecord = NULL; 
	
	char szPath[MAX_BUFF]; memset(szPath, 0, MAX_BUFF);
	if (strlen(szPath) > 0) {
		strcat_s(szPath, "DBError\\");
	}
	
//	SetConnStr();
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)){
	}
}
bool CMyDBWork::IsConnect()
{
	if (m_pConn == NULL)
		return false;
	else
		return true;
}
CMyDBWork::~CMyDBWork()
{
	CoUninitialize();
}

bool CMyDBWork::CollectMsg(const char *szColumnName, long	&lTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			lTemp = RusultGet.lVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, BYTE &btTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			btTemp = RusultGet.bVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, DWORD &dwTemp	)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			dwTemp = RusultGet.ulVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, UINT &iTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			iTemp = RusultGet.ulVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, int &iTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			iTemp = RusultGet.intVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::CollectMsg(const char *szColumnName, float &fTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			fTemp = RusultGet.fltVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, double &dbTemp	)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			dbTemp = RusultGet.dblVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, int i, CString &chTemp) {
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try {
			_variant_t vt;
			vt = m_pRecord->GetCollect(szColumnName);//ȡ�����ʱ����
			chTemp = vt;
		}
		catch (_com_error e) {
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}
bool CMyDBWork::CollectMsg(const char *szColumnName, CString &chTemp){
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			chTemp.Format(_T("%s"),RusultGet.bstrVal);
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
} 

bool CMyDBWork::CollectMsg(const char *szColumnName, CString szBuff, int nBuffSize)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column(szColumnName);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			WideCharToMultiByte(CP_ACP, 0, RusultGet.bstrVal, -1, (LPSTR)(LPCTSTR)szBuff, nBuffSize, NULL, NULL);
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}

bool CMyDBWork::CollectMsg(ULONG nIndex, long	&lTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			lTemp = RusultGet.lVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}

	return bResult;
}
bool CMyDBWork::CollectMsg(ULONG nIndex, BYTE &btTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			btTemp = RusultGet.bVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}
bool CMyDBWork::CollectMsg(ULONG nIndex, DWORD &dwTemp	)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			dwTemp = RusultGet.ulVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(ULONG nIndex, UINT &iTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			iTemp = RusultGet.ulVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(ULONG nIndex, int &iTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			iTemp = RusultGet.intVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::CollectMsg(ULONG nIndex, float &fTemp		)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			fTemp = RusultGet.fltVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	
	return bResult;
}
bool CMyDBWork::CollectMsg(ULONG nIndex, double &dbTemp	)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			dbTemp = RusultGet.dblVal;
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}

// bool CMyDBWork::CollectMsg(ULONG nIndex, char &chTemp){
// 	bool bResult = false;
// 	if (adStateClosed != m_pRecord->State) {
// 		try{
// 			_variant_t Column((long)nIndex);
// 			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
// 			chTemp = char(RusultGet.cVal);
// 			bResult = true;
// 		}
// 		catch(_com_error e){
// 			char szLog[MAX_BUFF]; memset(szLog, 0, MAX_BUFF);
// 			sprintf_s(szLog, "%s\n", CW2A(e.Description()));
// 			m_WriteLog.WriteLog(szLog, strlen(szLog), LOG_ERR);
// 		}
// 	}
// 	return bResult;
// } 


bool CMyDBWork::CollectMsg(ULONG nIndex, char *szBuff, int nBuffSize)
{
	bool bResult = false;
	if (adStateClosed != m_pRecord->State) {
		try{
			_variant_t Column((long)nIndex);
			_variant_t RusultGet = m_pRecord->Fields->GetItem(Column)->Value;
			WideCharToMultiByte(CP_ACP, 0, RusultGet.bstrVal, -1, szBuff, nBuffSize, NULL, NULL);
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}

 




bool CMyDBWork::ConnDB(void){
	bool bResult = false;
	if (NULL == m_pConn || adStateClosed == m_pConn->State) {
		if ((m_szConnStr.GetLength() > 0)) {
 			HRESULT hr = CoInitialize(NULL);
 			if (FAILED(hr))
			{
				::MessageBox(AfxGetMainWnd()->m_hWnd,_T("�����ʼ������"),_T("���Ӵ���"),MB_OK);
 			}
			else
			{
				try
				{  
					//Connecting
					if ( ! FAILED(m_pConn.CreateInstance(_uuidof(Connection)))){ //�������ӳ�ʱʱ��
						m_pConn->CommandTimeout = 30;		//in seconds
						if (!FAILED(m_pConn->Open((_bstr_t)(m_szConnStr), "", "", adModeUnknown)))
						{
							bResult = true;
						}
					}
				}
				catch(_com_error e){
					CString szLog, tmp;
					tmp = (char *)(e.Description());
					szLog.Format(_T("�������ݿ����:%s\n"), tmp);
					::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("���Ӵ���"), MB_OK);
				}
			}
		}
	}
	
	return bResult;
}


bool CMyDBWork::SelectSQL(CString szSQL){
	bool bResult = false;
	if (m_pConn == NULL)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd,_T("���ݿ�δ����"),_T("���ݲ�ѯ"),MB_OK);
		return bResult;
	}
	if ((szSQL.GetLength() > 0) && (m_szConnStr.GetLength() > 0)) {
		if (NULL != m_pRecord/*|| adStateClosed != m_pRecord->State*/)
			m_pRecord->Close();
		
		try
		{
			if (!FAILED(m_pRecord.CreateInstance(__uuidof(Recordset)))) {
				HRESULT hr = 0;
				if (NULL == m_pConn || adStateClosed == m_pConn->State)
					hr = m_pRecord->Open((_bstr_t)szSQL, _variant_t(m_szConnStr), adOpenKeyset, adLockOptimistic, adCmdText);
				else
					hr = m_pRecord->Open((_bstr_t)szSQL, m_pConn.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
				if (SUCCEEDED(hr))	
					bResult = true;
			}
			m_count=m_pRecord->GetRecordCount();
		}
		catch (_com_error e)
		{
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("ִ��SQL��ѯ�������:%s"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	return bResult;
}
long CMyDBWork::GetCount()
{
	return m_count;
}
//nRefreshNum��Ӱ��ļ�¼��
bool CMyDBWork::ExecuteSQL(CString szSQLStr, long &nRefreshNum){
	bool bResult = false;
	if ((szSQLStr.GetLength() > 0)) {
		if (NULL == m_pConn || adStateClosed == m_pConn->State)
		{
			::MessageBox(AfxGetMainWnd()->m_hWnd,_T("���ݿ�δ����"),_T("SQL���ִ��"),MB_OK);
			return bResult;
		}
		try
		{
			_variant_t RefreshNum;
			m_pConn->Execute(_bstr_t(szSQLStr), &RefreshNum, adCmdText);
			bResult = true;
			nRefreshNum = RefreshNum.lVal;
		}
		catch (_com_error e)
		{
			nRefreshNum = 0;
			CString szLog,tmp;
			tmp = (char*)(e.Description());
			szLog.Format(_T("ִ��SQL��ѯ�������:%s"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����"), MB_OK);
		}
	}
	return bResult;
}

int CMyDBWork::GetConnStr(CString szConnStr, int nBuffSize){
	int nResult = 0;
	if (!szConnStr.IsEmpty() && nBuffSize > 1 && (m_szConnStr.GetLength() > 0)){
		if ((m_szConnStr.GetLength()) <= nBuffSize) 	{
			szConnStr=m_szConnStr;
			nResult = m_szConnStr.GetLength();
		}
	}

	return nResult;
}

//bool CMyDBWork::SetConnStr	(void){								//�����������ݿ���ַ��� 
//	bool bResult = true;
//	try{
//		GetPrivateProfileStringW(_T("DATA_BASE"),   _T("ConnectStr"), _T(""), (LPWSTR)(LPCWSTR)m_szConnStr, MAX_BUFF, _T(".\\Config.ini"));
//		if ((m_szConnStr.GetLength() < 1)) {
//			CString szUdlPath = _T(".\\Test.udl");
//			WIN32_FIND_DATAW findData;
//			if (INVALID_HANDLE_VALUE == FindFirstFileW(szUdlPath, &findData)){
//				FILE *hFile;
//				_wfopen_s(&hFile,szUdlPath, _T("a"));
//				if (NULL != hFile) {
//					fclose(hFile);
//				}
//			}
//			m_szConnStr.Format(_T("File Name=%s"), szUdlPath);
//		}
//	}
//	catch(...){
//		bResult = false;
//	}
//	
//	return bResult;
//}
bool CMyDBWork::SetConnStr	(CString szConnStr){
	bool bResult = false;
	if (!szConnStr.IsEmpty() && szConnStr.GetLength() > 0) {
		m_szConnStr=szConnStr;
		bResult = true;
	}
	return bResult;
}

void CMyDBWork::CloseDB(void){
	CloseRecord();
	try{
		if (NULL != m_pConn)
			m_pConn->Close();
	}
	catch(_com_error e){
		CString szLog, tmp;
		tmp = (char *)(e.Description());
		szLog.Format(_T("�ر����ݿ����Ӵ���:%s\n"), tmp);
		::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"),MB_OK);
	}
}

void CMyDBWork::CloseRecord	(void){
	try{
		if (NULL != m_pRecord){
			m_pRecord->Close();
			m_pRecord = NULL;
		}
	}
	catch(_com_error e){
		CString szLog, tmp;
		tmp = (char *)(e.Description());
		szLog.Format(_T("�ر����ݼ�¼������:%s\n"), tmp);
		::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
	}
}	

bool CMyDBWork::BindingRecord(CADORecordBinding *RsObject, CString szSQLStr)
{
	bool bResult = false;
	
	CloseRecord();
	if (SelectSQL(szSQLStr)) {
		try{
			IADORecordBindingPtr picRs(m_pRecord);
			if (! FAILED(picRs->BindToRecordset(RsObject)))
				bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("�󶨼�¼������:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::IsRecordEOF()
{
	bool bResult = true;
	if (NULL != m_pRecord) {
		if (!m_pRecord->adoEOF) 
			bResult = false;
	}
	return bResult;
}

bool CMyDBWork::IsRecordBOF()
{
	bool bResult = true;
	if (NULL != m_pRecord) {
		if (!m_pRecord->adoBOF) 
			bResult = false;
	}
	return bResult;
}

bool CMyDBWork::RecordMoveNext()
{
	bool bResult = false;
	if (NULL != m_pRecord) {
		if (!m_pRecord->adoEOF) {
			try{
				m_pRecord->MoveNext();
				bResult = true;
			}
			catch(_com_error e){
				CString szLog, tmp;
				tmp = (char *)(e.Description());
				szLog.Format(_T("RecordMoveNext����:%s\n"), tmp);
				::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
			}
		}
	}
	return bResult;
}

bool CMyDBWork::RecordMovePrevious()
{
	bool bResult = false;
	if (NULL != m_pRecord) {
		if (!m_pRecord->adoBOF) {
			try{
				m_pRecord->MovePrevious();
				bResult = true;
			}
			catch(_com_error e){
				CString szLog, tmp;
				tmp = (char *)(e.Description());
				szLog.Format(_T("RecordMovePrevious����:%s\n"), tmp);
				::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
			}
		}
	}
	return bResult;
}

bool CMyDBWork::RecordMoveFirst()
{
	bool bResult = false;
	if (NULL != m_pRecord) {
		try{
			m_pRecord->MoveFirst();
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("RecordMoveFirst����:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	return bResult;
}

bool CMyDBWork::RecordMoveLast()
{
	bool bResult = false;
	if (NULL != m_pRecord) {
		try{
			m_pRecord->MoveLast();
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("RecordMoveLast����:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::BeginTrans()
{
	bool bResult = false;
	if (NULL != m_pConn) {
		try{
			m_pConn->BeginTrans();
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("BeginTrans����:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::CommitTrans()
{
	bool bResult = false;
	if (NULL != m_pConn) {
		try{
			m_pConn->CommitTrans();
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("CommitTrans����:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	
	return bResult;
}

bool CMyDBWork::RollbackTrans()
{
	bool bResult = false;
	if (NULL != m_pConn) {
		try{
			m_pConn->RollbackTrans();
			bResult = true;
		}
		catch(_com_error e){
			CString szLog, tmp;
			tmp = (char *)(e.Description());
			szLog.Format(_T("RollbackTranst����:%s\n"), tmp);
			::MessageBox(AfxGetMainWnd()->m_hWnd, szLog, _T("����!"), MB_OK);
		}
	}
	
	return bResult;
}





