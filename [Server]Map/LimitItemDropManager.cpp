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
{//[输出实时掉落时间信息][By:十里坡剑神][QQ:112582793][2018/5/17]
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	char buf[256] = {0,};
	sprintf(buf,"./DropItemInfo/DropInfo_%d.txt",g_MapNum);
	std::ofstream OutFile(buf,std::ios::app);

	OutFile<<"*****************************************************************************"<<std::endl;
	OutFile<<"*********************";
	sprintf(buf,"时间[%-2d月%-2d日%-2d]时%-2d分%-2d秒]",
		systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
	OutFile<<"时间["<<systime.wMonth<<"月";
	OutFile<<buf;
	OutFile<<"*********************"<<std::endl;
	OutFile<<"*****************************************************************************"<<std::endl;
	std::map<WORD,LIMITDROPINFO*>::iterator iter;
	iter = m_LimitItemDropInfo.begin();
	for(;iter!=m_LimitItemDropInfo.end();iter++)
	{
		for(int i = 0;i<iter->second[0].wCount;i++)
		{
			sprintf(buf,"*物品名[%-16s]\t击杀[%-6d]次掉落\t当前击杀次数[%-6d]*",
				iter->second[i].strItemName.c_str(),iter->second[i].dwDropTimes,
				iter->second[i].dwCurDropTimes);
			OutFile<<buf<<std::endl;
		}
	}
	OutFile<<"*****************************************************************************"<<std::endl<<std::endl;
	OutFile.close();
	g_Console.LOG(4,"掉落控制信息输出成功");
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
{//[掉落处理，检查是否该掉落控制物品][By:十里坡剑神][QQ:112582793][2018/5/17]
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
				sprintf(buf, "稀有物品掉落:物品名=[%s],角色ID=[%d],时间=[%d月%d日%d时%d分%d秒]",iter->second[i].strItemName.c_str(),
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
	g_Console.LOG(4,"掉落控制信息加载成功");
}
void LimitItemDropManager::LoadDropItemInfo()
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"Resource/Server/LimitItem/LimitItemDropInfo_%02d.bin",g_MapNum);
	
	if(!file.Init(filename,"rb"))
	{
		g_Console.LOG(4,"掉落控制信息未加载");
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
{//[显示掉落规则信息][By:十里坡剑神][QQ:112582793][2019/3/27][17:50:01]
	if(m_CtrlDropItemList.size()==0)
	{
		g_Console.LOG(4,"指定掉落信息列表为空");
	}
	else
	{
		g_Console.LOG(4,"***********指定掉落信息列表***********");
		std::map<DWORD,WORD>::iterator iter;
		iter = m_CtrlDropItemList.begin();
		while(TRUE)
		{
			if(iter==m_CtrlDropItemList.end())
				break;
			else
			{
				g_Console.LOG(4,"角色ID[%d]---物品编号[%d]",iter->first,iter->second);
			}
			iter++;
		}
		g_Console.LOG(4,"**************************************");
	}
	if(m_DropItemList.size()==0)
	{
		g_Console.LOG(4,"随机掉落信息列表为空");
	}
	else
	{
		g_Console.LOG(4,"***********随机掉落信息列表***********");
		std::map<std::string,WORD>::iterator iter2;
		iter2 = m_DropItemList.begin();
		while(TRUE)
		{
			if(iter2==m_DropItemList.end())
				break;
			else
			{
				g_Console.LOG(4,"编号[%s]---物品编号[%d]",iter2->first.c_str(),iter2->second);
			}
			iter2++;
		}
		g_Console.LOG(4,"**************************************");
	}
}
void LimitItemDropManager::AddCtrlRandDropItem(WORD wItemIdx)
{//[添加随机掉落控制][By:十里坡剑神][QQ:112582793][2019/3/27][17:40:53]
	char buff[64]={0,};
	sprintf(buff,"%d",nDropItemCount++);
	m_DropItemList[buff] = wItemIdx;
	g_Console.LOG(4,"随机掉落信息添加成功:物品编号[%d] 编号[%d]",wItemIdx,nDropItemCount);
}
void LimitItemDropManager::AddCtrlDropItem(WORD wItemIdx,DWORD dwCharIdx)
{//[添加指定掉落控制][By:十里坡剑神][QQ:112582793][2019/3/27][17:40:45]
	m_CtrlDropItemList[dwCharIdx] = wItemIdx;
	g_Console.LOG(4,"掉落信息添加成功:物品编号[%d] 角色ID[%d]",wItemIdx,dwCharIdx);
}
void LimitItemDropManager::DelCtrlDropItem(std::string str)
{//[删除随机掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:40:34]
	if(m_DropItemList.size()==0)
	{
		g_Console.LOG(4,"牛巨随机掉落控制信息为空");
		return;
	}
	std::map<std::string,WORD>::iterator iter;
	iter = m_DropItemList.find(str);
	if(iter!=m_DropItemList.end())
	{
		m_DropItemList.erase(iter);
		nDropItemCount--;
		g_Console.LOG(4,"随机掉落信息删除成功:编号[%s]",str.c_str());
	}
	else
	{
		g_Console.LOG(4,"未查询到随机掉落信息:编号[%s]",str.c_str());
	}
}
void LimitItemDropManager::DelCtrlDropItem(DWORD dwCharIdx)
{//[删除指定掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:40:15]
	if(m_CtrlDropItemList.size()==0)
	{
		g_Console.LOG(4,"牛巨掉落控制信息为空");
		return;
	}
	std::map<DWORD,WORD>::iterator iter;
	iter = m_CtrlDropItemList.find(dwCharIdx);
	if(iter!=m_CtrlDropItemList.end())
	{
		m_CtrlDropItemList.erase(iter);
		g_Console.LOG(4,"掉落信息删除成功:角色ID[%d]",dwCharIdx);
	}
	else
	{
		g_Console.LOG(4,"未查询到掉落信息:角色ID[%d]",dwCharIdx);
	}
}
void LimitItemDropManager::ClearCtrlDropItem(BYTE type)
{
	switch(type)
	{
	case 1:
		{//[指定掉落规则清空][By:十里坡剑神][QQ:112582793][2019/3/27][17:25:41]
			if(m_CtrlDropItemList.size()==0)
			{
				g_Console.LOG(4,"牛巨掉落控制信息为空");
				return;
			}
			else
			{
				m_CtrlDropItemList.clear();
				g_Console.LOG(4,"清除成功,当前掉落信息数[%d]",m_CtrlDropItemList.size());
			}
		}
		break;
	case 2:
		{//[随机掉落规则清空][By:十里坡剑神][QQ:112582793][2019/3/27][17:25:54]
			if(m_DropItemList.size()==0)
			{
				g_Console.LOG(4,"牛巨随机掉落控制信息为空");
				return;
			}
			else
			{
				m_DropItemList.clear();
				g_Console.LOG(4,"清除成功,当前随机掉落信息数[%d]",m_DropItemList.size());
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
		if(seed>90)//[随机掉落][By:十里坡剑神][QQ:112582793][2018/6/7]
		{
			wItemIdx = sIter->second;
			m_DropItemList.erase(sIter);
			nDropItemCount--;
			return TRUE;
		}
			
	}
	return FALSE;
}