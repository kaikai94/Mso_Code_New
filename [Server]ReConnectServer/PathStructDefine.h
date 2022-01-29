#pragma once
struct MSGFILEINFO : public MSGROOT
{
	MSGFILEINFO(){memset(this,0,sizeof(MSGFILEINFO));}
	char strFileName[MAX_PATH];
	DWORD dwDataSize;
};
struct MSGFILEDATA : public MSGFILEINFO
{
	MSGFILEDATA(){memset(this,0,sizeof(MSGFILEDATA));}
	char* Data[10240];
};