#include "stdafx.h"
#include "FieldBossMonsterManager.h"
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
#include "AIGroupManager.h"
#include "AISystem.h"
#include "RegenManager.h"
#include "RegenPrototype.h"
#include "MHFile.h"
#include "..\[cc]BattleSystem\BattleSystem_Server.h"
#include "UserTable.h"
#include "ObjectFactory.h"
#include "AttackManager.h"
#include "PackedData.h"
#include "CharMove.h"
#include "MHError.h"
#include "GridSystem.h"
#include "GridTable.h"
#include "TileManager.h"

GLOBALTON(CFieldBossMonsterManager);

CFieldBossMonsterManager::CFieldBossMonsterManager(void)
{
	m_pFieldGroupPool = new CMemoryPoolTempl<FIELDGROUP>;
	m_pFieldGroupPool->Init( 10, 2, "FieldGroupPool" );
	m_FieldGroupTable.Initialize(10);

	m_pFieldMonsterInfoPool = new CMemoryPoolTempl<FIELDMONSTERINFO>;
	m_pFieldMonsterInfoPool->Init( 40, 10, "FieldMonsterInfoPool" );
	m_FieldMonsterInfoTable.Initialize(40);

	m_pFieldBossDropItemPool = new CMemoryPoolTempl<FIELDBOSSDROPITEMLIST>;
	m_pFieldBossDropItemPool->Init( 20, 10, "FieldBossDropItemPool" );
	m_FieldBossDropItemTable.Initialize(20);
}

CFieldBossMonsterManager::~CFieldBossMonsterManager(void)
{
	m_FieldGroupTable.SetPositionHead();
	m_FieldMonsterInfoTable.SetPositionHead();
	m_FieldBossDropItemTable.SetPositionHead();

	FIELDGROUP* pGroup = NULL;
	
	while( pGroup = m_FieldGroupTable.GetData() )
	{
		m_pFieldGroupPool->Free( pGroup );
	}
	m_FieldGroupTable.RemoveAll();
	SAFE_DELETE( m_pFieldGroupPool );

	FIELDMONSTERINFO* pMonsterInfo = NULL;
	
	while( pMonsterInfo = m_FieldMonsterInfoTable.GetData() )
	{
		m_pFieldMonsterInfoPool->Free( pMonsterInfo );
	}
	m_FieldMonsterInfoTable.RemoveAll();
	SAFE_DELETE( m_pFieldMonsterInfoPool );

	FIELDBOSSDROPITEMLIST* pItemInfo = NULL;
	
	while( pItemInfo = m_FieldBossDropItemTable.GetData() )
	{
		m_pFieldBossDropItemPool->Free( pItemInfo );
	}
	m_FieldBossDropItemTable.RemoveAll();
	SAFE_DELETE( m_pFieldBossDropItemPool );
}

void CFieldBossMonsterManager::Init()
{
	CMHFile file;

	if(!file.Init("Resource/Server/FieldBossList.bin","rb"))
		return ;

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		FIELDMONSTERINFO* pMonsterInfo = m_pFieldMonsterInfoPool->Alloc();

		pMonsterInfo->m_MonsterKind	= file.GetWord();
		pMonsterInfo->m_ObjectKind	= file.GetWord();
		pMonsterInfo->m_SubKind		= file.GetWord();
		pMonsterInfo->m_SubCount	= file.GetWord();

		pMonsterInfo->m_dwRegenTimeMin	= file.GetDword();
		pMonsterInfo->m_dwRegenTimeMax	= file.GetDword();
		pMonsterInfo->m_dwDistructTime	= file.GetDword() * 60000;

		pMonsterInfo->m_dwRecoverStartTime	= file.GetDword() * 1000;
		pMonsterInfo->m_dwRecoverDelayTime	= file.GetDword() * 1000;
		pMonsterInfo->m_fLifeRate	= file.GetFloat() / 100;
		pMonsterInfo->m_fShieldRate	= file.GetFloat() / 100;

		m_FieldMonsterInfoTable.Add(pMonsterInfo, pMonsterInfo->m_MonsterKind);
	}

	file.Release();
	if(!file.Init("Resource/Server/FieldBossDropItemList.bin","rb"))
		return ;

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		FIELDBOSSDROPITEMLIST* pItemList = m_pFieldBossDropItemPool->Alloc();

		pItemList->m_wMonsterKind = file.GetWord();
		SafeStrCpy(pItemList->MonsterName,file.GetString(),MAX_MONSTER_NAME_LENGTH+1);
		for(WORD group = 0; group < MAX_FIELDBOSS_DROPITEM_GROUP_NUM; group++)
		{
			file.GetWord();
			pItemList->m_ItemList[ group ].m_wCount = file.GetWord();
			pItemList->m_ItemList[ group ].m_wDropRate = file.GetWord();
			pItemList->m_ItemList[ group ].m_wDamageRate = file.GetWord();

			for(WORD count = 0; count < MAX_FIELDBOSS_DROPITEM_NUM; count++)
			{
				SafeStrCpy(pItemList->m_ItemList[ group ].Name[count],file.GetString(),MAX_ITEMNAME_LENGTH+1);
				pItemList->m_ItemList[ group ].m_wItemIndex[ count ] = file.GetWord();
				pItemList->m_ItemList[ group ].m_wItemCount[ count ] = file.GetWord();
			}
		}

		m_FieldBossDropItemTable.Add(pItemList, pItemList->m_wMonsterKind);
	}

	file.Release();
}

void CFieldBossMonsterManager::Process()
{
	FIELDGROUP* pGroup = NULL;
	
	m_FieldGroupTable.SetPositionHead();

	while( pGroup = m_FieldGroupTable.GetData() )
	{
		if(pGroup)
		{
			BOOL bAllDead = TRUE;

			if( pGroup->m_pBoss )
			{
				bAllDead = FALSE;

				if( !pGroup->m_pBoss->IsBattle() )
				{
					if( pGroup->m_pBoss->IsDistruct() )
					{
						if( pGroup->m_SubList.GetCount() )
						{
							PTRLISTPOS pos = pGroup->m_SubList.GetHeadPosition();
							CFieldSubMonster* pSub;

							while(pos)
							{
								pSub = (CFieldSubMonster*)pGroup->m_SubList.GetNext(pos);

								if( pSub )
								{
									if( pSub->IsBattle() )
										continue;

									pGroup->m_SubList.Remove((void*)pSub);

									g_pServerSystem->RemoveMonster( pSub->GetID() );
								}
							}
						}					
						g_pServerSystem->RemoveBossMonster( pGroup->m_pBoss->GetID(), TRUE );
						pGroup->m_pBoss = NULL;
					}
					else
						pGroup->m_pBoss->Recover();
				}
				
			}
			
			if( pGroup->m_SubList.GetCount() )
			{
				bAllDead = FALSE;

				PTRLISTPOS pos = pGroup->m_SubList.GetHeadPosition();
				CFieldSubMonster* pSub;

				while(pos)
				{
					pSub = (CFieldSubMonster*)pGroup->m_SubList.GetNext(pos);

					if( pSub )
					{
						if( pGroup->m_pBoss )
						{
							pSub->ResetDistructTime();
						}

						if( !pSub->IsBattle() )
						{
							if( pSub->IsDistruct() )
							{
								pGroup->m_SubList.Remove((void*)pSub);

								g_pAISystem->ReleaseMonsterID( pSub->GetID() );
								g_pAISystem->ReleaseMonsterID( pSub->GetSubID() );
								g_pServerSystem->RemoveMonster( pSub->GetID() );
							}
							else
								pSub->Recover();
						}
					}
				}
			}

			if( bAllDead )
			{
				CAIGroup* pAIGroup = GROUPMGR->GetGroup(pGroup->m_GroupID, pGroup->m_GridID);

				if(pAIGroup)
				{
					DWORD delaytime = pGroup->m_dwRegenTimeMax - pGroup->m_dwRegenTimeMin;

					if( delaytime )
						delaytime = rand() % (delaytime) * 60000;

					delaytime += pGroup->m_dwRegenTimeMin * 60000;
					
					pAIGroup->GetRegenInfo()->SetRegenDelayTime(delaytime);

					g_pAISystem->RemoveObject(pGroup->m_BossObjectID);
				

					m_FieldGroupTable.Remove(pGroup->m_BossObjectID);

					pGroup->m_BossObjectID = 0;
					pGroup->m_dwRegenTimeMax = 0;
					pGroup->m_dwRegenTimeMin = 0;
					pGroup->m_GridID = 0;
					pGroup->m_GroupID = 0;
					pGroup->m_pBoss = NULL;

					m_pFieldGroupPool->Free(pGroup);
				}
				else
				{
					g_pAISystem->RemoveObject(pGroup->m_BossObjectID);

					m_FieldGroupTable.Remove(pGroup->m_BossObjectID);

					pGroup->m_BossObjectID = 0;
					pGroup->m_dwRegenTimeMax = 0;
					pGroup->m_dwRegenTimeMin = 0;
					pGroup->m_GridID = 0;
					pGroup->m_GroupID = 0;
					pGroup->m_pBoss = NULL;

					m_pFieldGroupPool->Free(pGroup);
				}
			}
		}
	}
}

void CFieldBossMonsterManager::AddRegenPosition(float x, float z)
{
	if( m_RegnePosCount >= MAX_REGENPOS_NUM )
		return;
	
	m_RegenPos[ m_RegnePosCount ].x = x;
	m_RegenPos[ m_RegnePosCount ].z = z;

	m_RegnePosCount++;
}

VECTOR3 CFieldBossMonsterManager::GetRegenPosition(int index)
{
	if(index < 0)
	{
		if( m_RegnePosCount != 0)
			index = rand() % m_RegnePosCount;
		else
		{
			VECTOR3 temp = {0.0f, 0.0f, 0.0f};
			return temp;
		}
	}

	return m_RegenPos[ index ];
}

void CFieldBossMonsterManager::BossDead(CFieldBossMonster* pBoss)
{
	FIELDGROUP* pGroup = NULL;
	
	DWORD ID = pBoss->GetID();

	pGroup = m_FieldGroupTable.GetData(ID);

	if(pGroup)
	{
		if( pGroup->m_pBoss == pBoss )
		{
			CGridTable* pGridTable = g_pServerSystem->GetGridSystem()->GetGridTable(pGroup->m_pBoss);
			pGridTable->RemoveObject(pGroup->m_pBoss);
			g_pServerSystem->GetMap()->GetTileManager()->RemoveObject(pGroup->m_pBoss);

			BATTLESYSTEM->DeleteObjectFromBattle(pGroup->m_pBoss);		
			pGroup->m_pBoss = NULL;
		}
	}
}

void CFieldBossMonsterManager::SubDead(CFieldSubMonster* pSub)	
{
	FIELDGROUP* pGroup = NULL;
	
	DWORD ID = pSub->GetBossID();

	pGroup = m_FieldGroupTable.GetData(ID);

	if(pGroup)
	{
		pGroup->m_SubList.Remove((void*)pSub);
		g_pAISystem->ReleaseMonsterID( pSub->GetID() );
		g_pAISystem->ReleaseMonsterID( pSub->GetSubID() );
	}
}

void CFieldBossMonsterManager::AddFieldBossMonster( CFieldBossMonster* pBoss )
{
	FIELDMONSTERINFO* pBossInfo = m_FieldMonsterInfoTable.GetData( pBoss->GetMonsterKind() );
	if( pBossInfo == NULL )
	{
		char buff[512]={0,};
		sprintf(buff,"王怪编号不存在:[%d]",pBoss->GetMonsterKind());
		ASSERTMSG(0,buff);
		return;
	}
	FIELDMONSTERINFO* pSubInfo = m_FieldMonsterInfoTable.GetData( pBossInfo->m_SubKind );
	if( pSubInfo == NULL )
	{
		char buff[512]={0,};
		sprintf(buff,"怪物编号不存在，王怪:[%d]-精英:[%d]",pBoss->GetMonsterKind(),pBossInfo->m_SubKind);
		ASSERTMSG(0,buff);
		return;
	}
	
	pBossInfo->m_dwCountTime = pBossInfo->m_dwDistructTime;
	
	pBoss->SetInfo( pBossInfo );	

	FIELDGROUP* pGroup = m_pFieldGroupPool->Alloc();

	if( !pGroup )
	{
		FILE* file = fopen(m_LogFile, "a+");
		fprintf(file, "%s\t%d\t%u\t%d\t%u\t\n",
			"分组内存分配失败", pBoss->GetMonsterKind(), pBoss->GetID(), pBoss->GetGridID(), gCurTime);
		fclose(file);
		return;
	}

	pGroup->m_GroupID = pBoss->GetMonsterGroupNum();
	pGroup->m_GridID = pBoss->GetGridID();
	pGroup->m_pBoss = pBoss;
	pGroup->m_BossObjectID = pBoss->GetID();

	pGroup->m_dwRegenTimeMin = pBossInfo->m_dwRegenTimeMin;
	pGroup->m_dwRegenTimeMax = pBossInfo->m_dwRegenTimeMax;	

	if(pSubInfo)
	{
		pSubInfo->m_dwCountTime = pSubInfo->m_dwDistructTime;

		DWORD dwGridID = pBoss->GetGridID();		
		WORD wObjectKind = eObjectKind_FieldSubMonster;	
		WORD wMonsterKind = pBossInfo->m_SubKind;		
		DWORD dwGroupID = (WORD)RECALL_GROUP_ID;			
		if(pGroup->m_GroupID == (WORD)EVENTRECALL_GROUP_ID)
			dwGroupID = (WORD)EVENTRECALL_GROUP_ID;
		WORD DropItemID = 0;							
		DWORD dwDropRatio = 100;		
		BOOL bRandomPos = FALSE;						
		BOOL bEventRegen = FALSE;		

		for( WORD count = 0; count < pBossInfo->m_SubCount; count++ )
		{
			CFieldSubMonster* pSub = NULL;

			DWORD dwID = g_pAISystem->GeneraterMonsterID();		
			DWORD dwSubID = g_pAISystem->GeneraterMonsterID();	

			VECTOR3 BossPos;
			VECTOR3 TargetPos;

			pBoss->GetPosition(&BossPos);

			TargetPos.x = BossPos.x + ( (rand() % 2) ? -1 : 1 ) * (rand() % 10) * 100;
			TargetPos.z = BossPos.z + ( (rand() % 2) ? -1 : 1 ) * (rand() % 10) * 100;

			pSub = (CFieldSubMonster*)REGENMGR->RegenObject( dwID, dwSubID, dwGridID, wObjectKind, wMonsterKind, &TargetPos, dwGroupID, DropItemID, dwDropRatio, bRandomPos, bEventRegen);

			if( pSub )
			{
				pSub->SetInfo( pSubInfo );
				pSub->SetBossID( pGroup->m_BossObjectID );
				pSub->SetRegenPosIndex(pBoss->GetRegenPosIndex());
				pGroup->m_SubList.AddTail( pSub );
			}
			else
			{
				//[刷怪失败][By:十里坡剑神][QQ:112582793][2018/1/10]
			}
		}
	}
	m_FieldGroupTable.Add( pGroup, pGroup->m_BossObjectID );
}

FIELDBOSSDROPITEMLIST* CFieldBossMonsterManager::GetFieldBossDropItemList( WORD MonsterKind )
{
	FIELDBOSSDROPITEMLIST* pItemInfo = m_FieldBossDropItemTable.GetData(MonsterKind);

	return pItemInfo;
}
