// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserManager.h"
#include "Network.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserManager gUserMGR;
extern int g_nServerSetNum;

CUserManager::CUserManager()
{
//#ifdef _HK_LOCAL_
//	m_LimitUserLevel = eUSERLEVEL_GM;
//#else
	m_LimitUserLevel = eUSERLEVEL_GM;
//#endif
	LoadVersion();
	LoadGMUserInfo();
	m_LimitAge = 0;
}

CUserManager::~CUserManager()
{

}
void CUserManager::SetVersion(char *szV)
{
	if(szV[0] != 0)
		strcpy(CLIENT_VERSION, szV);
}
BOOL CUserManager::IsValidVersion(char * szVersion)
{
	if(strcmp(CLIENT_VERSION, szVersion) == 0)
		return TRUE;
	return FALSE;
}

void CUserManager::LoadVersion()
{
	struct VERINFO
	{
		DWORD  dwDate;
		char name[33];
		char ver[33];
		bool bIsNew;
	};
	/*CMHFile file;
	if( !file.Init("./ClientVerInfo.ver", "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		strcpy( CLIENT_VERSION, "None!!" );
		file.Release();
		return;
	}

	strcpy(CLIENT_VERSION, file.GetString());
	file.Release();*/
	VERINFO info;
	FILE * fp;
	fopen_s(&fp,"./ClientVerInfo.ver","r");
	if(fp)
	{
		fread(&info,sizeof(info),1,fp);
		fclose(fp);
		strcpy(CLIENT_VERSION, info.ver);
	}
	else
	{
		g_Console.LOG(4,"�汾�ļ���ȡ����");
		strcpy(CLIENT_VERSION, "None!!");
	}
}
BOOL CUserManager::IsValidGMUser(std::string UserID)
{
	std::map<std::string,WORD>::iterator iter;
	iter = m_GMUserInfoTable.find(UserID);
	if(iter!=m_GMUserInfoTable.end())
	{
		if(iter->second==2)
			return FALSE;
	}
	else 
		return TRUE;
}
void CUserManager::LoadGMUserInfo()
{
	CMHFile file;
	if( !file.Init("./GMUserInfo.txt", "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		std::string str = "";
		WORD UserLevel = 0;
		str = file.GetString();
		UserLevel = file.GetWord();
		if(!str.empty())
			m_GMUserInfoTable[str] = UserLevel;
	}
	file.Release();
}
void CUserManager::SaveVersion()
{
	CMHFile file;
	file.Init("./ClientVerInfo.ver", "w");
	file.SetString(CLIENT_VERSION);
	file.Release();
}
char * CUserManager::GetVersion()
{
	return CLIENT_VERSION;
}

BOOL CUserManager::IsValidUserLevel(BYTE UserLevel)
{
	if(m_LimitUserLevel >= UserLevel)
		return TRUE;
	else
		return FALSE;
}

void CUserManager::LoadDistributeDesc()
{
	char filename[256];
	sprintf(filename,"serverset/%d/DistributeDesc.txt",g_nServerSetNum);
	char buf[256];
	CMHFile file;
	
	if(file.Init(filename, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ))
	{
		while(1)
		{
			if( file.IsEOF() )
				break;
			file.GetString(buf);
			if(strcmp(buf, "*LIMITAGE") == 0)
			{
				m_LimitAge = file.GetByte();
			}
			else if(strcmp(buf, "*DEFAULTUSERLEVEL") == 0 )
			{
				m_LimitUserLevel = file.GetByte();
			}
		}
	}
}
