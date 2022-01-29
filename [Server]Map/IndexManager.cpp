#include "stdafx.h"
#include "IndexManager.h"
#include "ServerSystem.h"
//extern WORD g_MapNum;
IndexManager::IndexManager(void)
{

}

IndexManager::~IndexManager(void)
{
}
DWORD IndexManager::GetIndex()
{
	if(m_dwStartIndex<65533)
	{
		DWORD Temp = m_dwStartIndex;
		m_dwStartIndex++;
		return Temp;
	}
	else
	{
		m_dwStartIndex = 1;
		return m_dwStartIndex;
	}
	
}