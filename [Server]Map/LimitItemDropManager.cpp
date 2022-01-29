#include "stdafx.h"
#include <time.h>
#include "LimitItemDropManager.h"
#include "MHFile.h"
#include <fstream>
#include <iomanip>
extern WORD g_MapNum;

LimitItemDropManager::LimitItemDropManager(void)
{
	nDropItemCount = 0;
}

LimitItemDropManager::~LimitItemDropManager(void)
{
	Release();
	m_LimitItemDropInfo.clear();
	m_CtrlDropItemList.clear();
	m_DropItemList.clear();
}


void LimitItemDropManager::OutPutLimitItemDropTimeInfo()
{//[���ʵʱ����ʱ����Ϣ][By:ʮ���½���][QQ:112582793][2018/5/17]
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	char buf[256] = {0,};
	sprintf(buf,"./DropItemInfo/DropInfo_%d.txt",g_MapNum);
	std::ofstream OutFile(buf,std::ios::app);

	OutFile<<"*****************************************************************************"<<std::endl;
	OutFile<<"*********************";
	sprintf(buf,"ʱ��[%-2d��%-2d��%-2d]ʱ%-2d��%-2d��]",
		systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
	OutFile<<"ʱ��["<<systime.wMonth<<"��";
	OutFile<<buf;
	OutFile<<"*********************"<<std::endl;
	OutFile<<"*****************************************************************************"<<std::endl;
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.begin();
	for(;iter!=m_LimitItemDropInfo.end();iter++)
	{
		for(int i = 0;i<iter->second[0].wCount;i++)
		{
			sprintf(buf,"*��Ʒ��[%-16s]\t��ɱ[%-6d]�ε���\t��ǰ��ɱ����[%-6d]*",
				iter->second[i].strItemName.c_str(),iter->second[i].dwDropTimes,
				iter->second[i].dwCurDropTimes);
			OutFile<<buf<<std::endl;
		}
	}
	OutFile<<"*****************************************************************************"<<std::endl<<std::endl;
	OutFile.close();
	g_Console.LOG(4,"���������Ϣ����ɹ�");
}
void LimitItemDropManager::Release()
{
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.begin();
	for(;iter!=m_LimitItemDropInfo.end();iter++)
	{
		if(iter->second)
		{
			delete[] iter->second;
			iter->second = NULL;
		}
	}
}
void LimitItemDropManager::AddKillTime(WORD MonsterKind)
{
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.find(MonsterKind);
	if(iter!=m_LimitItemDropInfo.end())
	{
		for(int i = 0;i<iter->second[0].wCount;i++)
		{
			iter->second[i].dwCurDropTimes++;
		}
	}
}
void LimitItemDropManager::WriteItemDropLog(char* pMsg)
{
	char buf[256]={0,};
	sprintf(buf,"./DropItemInfo/DropLog_%d.txt",g_MapNum);
	std::ofstream OutFile(buf,std::ios::app);
	OutFile<<pMsg<<std::endl;
	OutFile.close();
}
void LimitItemDropManager::DropLimitItem(WORD wMonsterKind,WORD& wDropItemIdx,DWORD dwPlayerID)
{//[���䴦������Ƿ�õ��������Ʒ][By:ʮ���½���][QQ:112582793][2018/5/17]
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.find(wMonsterKind);
	if(iter!=m_LimitItemDropInfo.end())
	{
		for(int i = 0;i<iter->second[0].wCount;i++)
		{
			if(iter->second[i].dwCurDropTimes>=iter->second[i].dwDropTimes)
			{
				wDropItemIdx =iter->second[i].dwItemIdx;
				iter->second[i].dwCurDropTimes = 0;
				char buf[256];
				SYSTEMTIME systime;
				GetLocalTime(&systime);
				sprintf(buf, "ϡ����Ʒ����:��Ʒ��=[%s],��ɫID=[%d],ʱ��=[%d��%d��%dʱ%d��%d��]",iter->second[i].strItemName.c_str(),
					dwPlayerID,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
				WriteItemDropLog(buf);
				break;
			}
			else
				//continue;
				iter->second[i].dwCurDropTimes++;
		}
	}
}
void LimitItemDropManager::ReLoadDropItemInfo()
{
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.begin();
	for(;iter!=m_LimitItemDropInfo.end();iter++)
	{
		if(iter->second)
		{
			delete[] iter->second;
			iter->second = NULL;
		}
	}
	if(m_LimitItemDropInfo.size()>0)
		m_LimitItemDropInfo.clear();
	LoadDropItemInfo();
	g_Console.LOG(4,"���������Ϣ���سɹ�");
}
void LimitItemDropManager::LoadDropItemInfo()
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"Resource/Server/LimitItem/LimitItemDropInfo_%02d.bin",g_MapNum);
	
	if(!file.Init(filename,"rb"))
	{
		g_Console.LOG(4,"���������Ϣδ����");
		return;
	}
	char buff[256] = {0,};
	WORD wMonsterIdx = 0;
	WORD wCount = 0;
	while(1)
	{
		if(file.IsEOF())
			break;
		wMonsterIdx = file.GetWord();
		file.GetString(buff);
		wCount = file.GetWord();
		LIMITDROPINFO * info = new LIMITDROPINFO[wCount];
		for(int i = 0;i<wCount;i++)
		{
			info[i].wCount = wCount;
			info[i].dwItemIdx = file.GetDword();
			info[i].strItemName = file.GetString();
			info[i].dwDropTimes  = file.GetBool();
			info[i].dwCurDropTimes = file.GetDword();
		}
		m_LimitItemDropInfo[wMonsterIdx] = info;
	}
	file.Release();
}
void LimitItemDropManager::ShowDropItemList()
{//[��ʾ���������Ϣ][By:ʮ���½���][QQ:112582793][2019/3/27][17:50:01]
	if(m_CtrlDropItemList.size()==0)
	{
		g_Console.LOG(4,"ָ��������Ϣ�б�Ϊ��");
	}
	else
	{
		g_Console.LOG(4,"***********ָ��������Ϣ�б�***********");
		std::map<DWORD,WORD>::iterator iter;
		iter = m_CtrlDropItemList.begin();
		while(TRUE)
		{
			if(iter==m_CtrlDropItemList.end())
				break;
			else
			{
				g_Console.LOG(4,"��ɫID[%d]---��Ʒ���[%d]",iter->first,iter->second);
			}
			iter++;
		}
		g_Console.LOG(4,"**************************************");
	}
	if(m_DropItemList.size()==0)
	{
		g_Console.LOG(4,"���������Ϣ�б�Ϊ��");
	}
	else
	{
		g_Console.LOG(4,"***********���������Ϣ�б�***********");
		std::map<std::string,WORD>::iterator iter2;
		iter2 = m_DropItemList.begin();
		while(TRUE)
		{
			if(iter2==m_DropItemList.end())
				break;
			else
			{
				g_Console.LOG(4,"���[%s]---��Ʒ���[%d]",iter2->first.c_str(),iter2->second);
			}
			iter2++;
		}
		g_Console.LOG(4,"**************************************");
	}
}
void LimitItemDropManager::AddCtrlRandDropItem(WORD wItemIdx)
{//[�������������][By:ʮ���½���][QQ:112582793][2019/3/27][17:40:53]
	char buff[64]={0,};
	sprintf(buff,"%d",nDropItemCount++);
	m_DropItemList[buff] = wItemIdx;
	g_Console.LOG(4,"���������Ϣ��ӳɹ�:��Ʒ���[%d] ���[%d]",wItemIdx,nDropItemCount);
}
void LimitItemDropManager::AddCtrlDropItem(WORD wItemIdx,DWORD dwCharIdx)
{//[���ָ���������][By:ʮ���½���][QQ:112582793][2019/3/27][17:40:45]
	m_CtrlDropItemList[dwCharIdx] = wItemIdx;
	g_Console.LOG(4,"������Ϣ��ӳɹ�:��Ʒ���[%d] ��ɫID[%d]",wItemIdx,dwCharIdx);
}
void LimitItemDropManager::DelCtrlDropItem(std::string str)
{//[ɾ������������][By:ʮ���½���][QQ:112582793][2019/3/27][17:40:34]
	if(m_DropItemList.size()==0)
	{
		g_Console.LOG(4,"ţ��������������ϢΪ��");
		return;
	}
	std::map<std::string,WORD>::iterator iter;
	iter = m_DropItemList.find(str);
	if(iter!=m_DropItemList.end())
	{
		m_DropItemList.erase(iter);
		nDropItemCount--;
		g_Console.LOG(4,"���������Ϣɾ���ɹ�:���[%s]",str.c_str());
	}
	else
	{
		g_Console.LOG(4,"δ��ѯ�����������Ϣ:���[%s]",str.c_str());
	}
}
void LimitItemDropManager::DelCtrlDropItem(DWORD dwCharIdx)
{//[ɾ��ָ���������][By:ʮ���½���][QQ:112582793][2019/3/27][17:40:15]
	if(m_CtrlDropItemList.size()==0)
	{
		g_Console.LOG(4,"ţ�޵��������ϢΪ��");
		return;
	}
	std::map<DWORD,WORD>::iterator iter;
	iter = m_CtrlDropItemList.find(dwCharIdx);
	if(iter!=m_CtrlDropItemList.end())
	{
		m_CtrlDropItemList.erase(iter);
		g_Console.LOG(4,"������Ϣɾ���ɹ�:��ɫID[%d]",dwCharIdx);
	}
	else
	{
		g_Console.LOG(4,"δ��ѯ��������Ϣ:��ɫID[%d]",dwCharIdx);
	}
}
void LimitItemDropManager::ClearCtrlDropItem(BYTE type)
{
	switch(type)
	{
	case 1:
		{//[ָ������������][By:ʮ���½���][QQ:112582793][2019/3/27][17:25:41]
			if(m_CtrlDropItemList.size()==0)
			{
				g_Console.LOG(4,"ţ�޵��������ϢΪ��");
				return;
			}
			else
			{
				m_CtrlDropItemList.clear();
				g_Console.LOG(4,"����ɹ�,��ǰ������Ϣ��[%d]",m_CtrlDropItemList.size());
			}
		}
		break;
	case 2:
		{//[�������������][By:ʮ���½���][QQ:112582793][2019/3/27][17:25:54]
			if(m_DropItemList.size()==0)
			{
				g_Console.LOG(4,"ţ��������������ϢΪ��");
				return;
			}
			else
			{
				m_DropItemList.clear();
				g_Console.LOG(4,"����ɹ�,��ǰ���������Ϣ��[%d]",m_DropItemList.size());
			}
		}
		break;
	}
}
BOOL LimitItemDropManager::CheckCtrlDropItem(DWORD dwCharIdx,WORD & wItemIdx)
{
	if(m_CtrlDropItemList.size()==0&&m_DropItemList.size()==0) return FALSE;

	std::map<DWORD,WORD>::iterator iter;
	iter = m_CtrlDropItemList.find(dwCharIdx);
	if(iter!=m_CtrlDropItemList.end())
	{
		wItemIdx = iter->second;
		m_CtrlDropItemList.erase(iter);
		return TRUE;
	}
	else if(m_DropItemList.size()>0)
	{
		std::map<std::string,WORD>::iterator sIter;
		sIter = m_DropItemList.begin();
		int seed = rand()%100;
		if(seed>90)//[�������][By:ʮ���½���][QQ:112582793][2018/6/7]
		{
			wItemIdx = sIter->second;
			m_DropItemList.erase(sIter);
			nDropItemCount--;
			return TRUE;
		}
			
	}
	return FALSE;
}