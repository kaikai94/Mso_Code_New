// MixManager.cpp: implementation of the CMixManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixManager.h"
#include "MHFile.h"
#include "GameResourceStruct.h"
#ifdef _GMTOOL_
#include <fstream>
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMixManager)
CMixManager::CMixManager()
{
	//m_MixItemTable.Initialize(MAX_ITEM_NUM);
}
void CMixManager::Release()
{
	m_MixItemTable.clear();
}
CMixManager::~CMixManager()
{
	Release();

}

void CMixManager::LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
	pResInfo->wMixKind = fp->GetWord();
	
#ifdef _GMTOOL_
	if(pResInfo->wMixKind >= 7)
	{
		std::ofstream OutFile;
		OutFile.open("./log/MixLoadMixResult.txt",std::ios::app);
		OutFile<<pResInfo->wResItemIdx<<std::endl;
		OutFile.close();
		ASSERT(pResInfo->wMixKind < 7);
	}
#else
	ASSERT(pResInfo->wMixKind < 7);
#endif

	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	if(pResInfo->wMaterialItemNum !=0)
	{
		pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
		for( int i = 0 ; i < pResInfo->wMaterialItemNum ; ++i)
		{
			pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
			SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
			pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
		}
	}
}
void CMixManager::LoadMixList()
{
	Release();


	CMHFile file;
	if(!file.Init("Resource/ItemMixList.bin", "rb"))
		return;

	
	WORD wTmpItemIdx = 0;

	WORD wTmpRestIdx = 0;
	WORD wTmpResultNum = 0;
	std::map<WORD,ITEM_MIX_INFO>::iterator Iter;
	while(1)
	{
		if(file.IsEOF())
			break;

		wTmpItemIdx = file.GetWord();
		Iter = m_MixItemTable.find(wTmpItemIdx);
		if(Iter!=m_MixItemTable.end())
		{
			SafeStrCpy(Iter->second.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			Iter->second.wResultItemNum =wTmpResultNum;
			for(wTmpRestIdx = 0 ;wTmpRestIdx < Iter->second.wResultItemNum; ++wTmpRestIdx)
			{
				if(Iter->second.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &Iter->second.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
		}
		else
		{
			ITEM_MIX_INFO  pInfo ;
			memset(&pInfo,0,sizeof(ITEM_MIX_INFO));
			pInfo.wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			pInfo.wResultItemNum = wTmpResultNum;		


			if(!pInfo.psResultItemInfo)
			{
				pInfo.psResultItemInfo = new ITEM_MIX_RES[pInfo.wResultItemNum];
				memset(pInfo.psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo.wResultItemNum);
			}
			for(wTmpRestIdx = 0 ; wTmpRestIdx < pInfo.wResultItemNum ; ++wTmpRestIdx)
			{	
				if(pInfo.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &pInfo.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
			m_MixItemTable[pInfo.wItemIdx] = pInfo;
		}			
	}
	file.Release();
}

ITEM_MIX_INFO * CMixManager::GetMixItemInfo(WORD wItemIdx)
{
	std::map<WORD,ITEM_MIX_INFO>::iterator Iter;
	Iter = m_MixItemTable.find(wItemIdx);
	if(Iter!=m_MixItemTable.end())
		return &Iter->second;
	else 
		return NULL;
}





