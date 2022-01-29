#pragma once
#include <deque>
#include <vector>
#include <map>
enum eWMStatus
{
	eWMStatus_Inactive,
	eWMStatus_Active,
	eWMStatus_Success,
	eWMStatus_Fail,
};


class CWayPointManager
{
public:
	CWayPointManager();
	virtual ~CWayPointManager();


private:
	std::deque<VECTOR3>		m_deq_WayPoint;			
	VECTOR3					m_FlagDestination;		

	eWMStatus				m_WMStatus;				


public:
	void AddWayPoint(const VECTOR3& p_WayPoint);

	BOOL PopWayPoint(VECTOR3* p_WayPoint);

	void ClearAllWayPoint();

	BOOL IsWayPointEmpty();

	eWMStatus GetWMStatus() const { return m_WMStatus; }
	void SetWMStatus(eWMStatus val) { m_WMStatus = val; }


	VECTOR3 GetFlagDestination() const { return m_FlagDestination; }
	void SetFlagDestination(VECTOR3 val) { m_FlagDestination = val; }

	std::deque<VECTOR3>& GetVec_WayPoint() { return m_deq_WayPoint; }
	void SetVec_WayPoint(std::deque<VECTOR3>& p_vec_WayPoint) { m_deq_WayPoint = p_vec_WayPoint; }
	
	void ChangeDestination( const VECTOR3& p_Dest );

	void Debug_TRACEWayPoint();

};

