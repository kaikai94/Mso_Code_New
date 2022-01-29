




// TrafficLog.h: interface for the CTrafficLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAFFICLOG_H__3B802B9B_36F2_496B_9A01_B2D1386084B5__INCLUDED_)
#define AFX_TRAFFICLOG_H__3B802B9B_36F2_496B_9A01_B2D1386084B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TRAFFIC	USINGTON(CTrafficLog)
#define MAX_LOG_BUFFER_SIZE		102400		// 100KB
class CTrafficLog  
{
public:
	CTrafficLog();
	virtual ~CTrafficLog();

//KES
protected:

//	DWORD	m_dwStarRecordTime;

	DWORD	m_dwStartTime;			

	DWORD	m_dwTotalReceiveSize[MP_MAX];	//ī�װ��� �����ð������� �� ���� ��Ŷ��
	DWORD	m_dwTotalSendSize[MP_MAX];		//ī�װ��� �����ð������� �� ���� ��Ŷ��
	DWORD	m_dwTotalReceiveNum[MP_MAX];	//ī�װ��� �����ð������� �� ���� ��Ŷ��
	DWORD	m_dwTotalSendNum[MP_MAX];		//ī�װ��� �����ð������� �� ���� ��Ŷ��
	
//MOVE�� ����ȭ
	DWORD	m_dwMoveSizeReceive[2][4];	//0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION
	DWORD	m_dwMoveNumReceive[2][4];
	DWORD	m_dwMoveSizeSend[2][4];
	DWORD	m_dwMoveNumSend[2][4];
	

public:
	void StartRecord();

	void EndRecord();
	void AddReceivePacket( DWORD dwCategory, DWORD dwLength );
	void AddSendPacket( DWORD dwCategory, DWORD dwLength );



	void AddReceiveMovePacket( DWORD dwObjectID, WORD Protocol, DWORD dwLength );
	void AddSendMovePacket( DWORD dwObjectID, WORD Protocol, DWORD dwLength );

	void Process();
	void WriteTrafficLogFile();
	void Clear();

// 06. 01. ������ Ʈ���� �α� ��� - �̿���
private:

	DWORD m_dwCheckTime;
	DWORD m_dwUnValuedCount;
	DWORD m_dwValuedCount;

	struct USERTRAFFIC{
		DWORD dwConnectionIndex;
		DWORD dwUserID;
		
		DWORD dwLoginTime;
		DWORD dwTotalPacketCount;
		
		DWORD dwValuedCount;
		DWORD dwUnValuedCount;
		
		bool  bLogin;
	};


	CYHHashTable<USERTRAFFIC>		m_UserTrafficTable;
	CMemoryPoolTempl<USERTRAFFIC>*	m_pUserTrafficPool;

	DWORD m_dwLastPacketCheckTime;
	DWORD m_dwLastPacketWriteTime;


	void ResetUserPacket(DWORD dwTime);

	void WriteUserTrafficLogFile();

public:
	void Init();
	void End();
	void AddUser(DWORD dwUserID, DWORD dwConnectionIndex);
	void RemoveUser(DWORD dwUserID);
	void AddUserPacket(DWORD dwUserID, BYTE Category);
};
EXTERNGLOBALTON(CTrafficLog);

#endif // !defined(AFX_TRAFFICLOG_H__3B802B9B_36F2_496B_9A01_B2D1386084B5__INCLUDED_)



