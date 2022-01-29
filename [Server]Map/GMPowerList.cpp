// GMPowerList.cpp: implementation of the CGMPowerList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMPowerList.h"
#include "Network.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGMPowerList);

CGMPowerList::CGMPowerList()
{	
	m_CheatUserList.Initialize(50);
}

CGMPowerList::~CGMPowerList()
{	
	RemoveCheatUserList();
}

void CGMPowerList::AddCheatUserList( GM_INFO* pGMInfo )
{
	if(m_CheatUserList.GetData(pGMInfo->dwConnectionIndex))
	{
		char buf[64];
		sprintf(buf, "Error! GMInfo : %u", pGMInfo->dwConnectionIndex);
		ASSERTMSG(0, buf);		
	}

	GM_INFO* pAddGMInfo = new GM_INFO;
	memcpy( pAddGMInfo, pGMInfo, sizeof(GM_INFO) );
	m_CheatUserList.Add(pAddGMInfo, pAddGMInfo->dwConnectionIndex);
}

void CGMPowerList::RemoveCheatUserList()
{
	GM_INFO* pGMInfo = NULL;

	m_CheatUserList.SetPositionHead();

	while (pGMInfo = m_CheatUserList.GetData())
	{
		SAFE_DELETE(pGMInfo);
	}

	m_CheatUserList.RemoveAll();
}

GM_INFO* CGMPowerList::GetCheatUserInfo(DWORD dwUserIdx)
{
	GM_INFO* pGMInfo = NULL;

	m_CheatUserList.SetPositionHead();

	while (pGMInfo = m_CheatUserList.GetData())
	{
		if(pGMInfo->dwConnectionIndex == dwUserIdx)
		{
			return pGMInfo;
		}
	}

	return NULL;
}