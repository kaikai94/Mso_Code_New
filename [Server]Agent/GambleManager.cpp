
#include "stdafx.h"
#include <time.h>
#include "GambleManager.h"
#include "AgentDBMsgParser.h"
#include "Console.h"


GambleManager::GambleManager()
{
   IsCreate=TRUE;

   CheckTimer=0;

   Number1=0;
   
   Number2=0;

   Number3=0;

}
GambleManager::~GambleManager()
{

}

void  GambleManager::Process()
{

   DWORD curTime = time(NULL);

   if(curTime-CheckTimer>5000)
   {
   		SYSTEMTIME st;

		GetLocalTime( &st );

		if(st.wHour == OPEN_HOUR && st.wMinute == OPEN_MIN)
		{

			if(IsCreate)
			{
				CreateGambleInfo(st);  

				IsCreate=FALSE;
			}
		}

	    CheckTimer=time(NULL);
   }

}

void GambleManager::GambleOnceExec()
{

   	SYSTEMTIME st;

	GetLocalTime( &st );

	CreateGambleInfo(st);  

}

void GambleManager::CreateGambleInfo(SYSTEMTIME st)
{
    Number1=GetRandNumber(1);
	Number2=GetRandNumber(2);
	Number3=GetRandNumber(3);
	GambleInfoInsert(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,Number1,Number2,Number3);
	g_Console.LOG(4,"The Gamble System Exec Year:%d  Month:%d  Day:%d  Number1:%d  Number2:%d  Number3:%d",st.wYear,st.wMonth,st.wDay,Number1,Number2,Number3);
	GambleInfoOpen(st.wYear,st.wMonth,st.wDay);
} 

int GambleManager::GetRandNumber(int Index)
{
   time_t t;
   srand(time(&t)+Index);       // 产生随机数种子 2015-01-26
   int Rand=rand()%6+1;         // 点数为1-6点
   return Rand;
}

void GambleManager::ClearUserData()
{
	SYSTEMTIME st;
	GetLocalTime( &st );
	GameleClearUserData(st.wYear,st.wMonth,st.wDay);
}