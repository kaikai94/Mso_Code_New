// QuickItem.cpp: implementation of the CQuickItem class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "QuickItem.h"
#include "./interface/cWindowManager.h"
#include "./interface/cFont.h"
#include "Item.h"
#include "MugongBase.h"
#include "ItemManager.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "SkillManager_client.h"
#include "GameIn.h"
#include "MugongDialog.h"
#include "ChatManager.h"
#include "./Interface/cScriptManager.h"
#include "./Input/UserInput.h"
#include "AbilityIcon.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuickItem::CQuickItem(CBaseItem * pItemLink)
{
	m_type = WT_QUICKITEM;
	m_pItemLink = pItemLink;
	m_QDur = 0;
	m_bRemainTime	= FALSE;

	m_dwLastSecond	= 0;
}

CQuickItem::~CQuickItem()
{
	m_pItemLink = NULL;
}

void CQuickItem::SetImage2(cImage * low, cImage * high)
{
	SetBasicImage(low);
	if(high)
	m_HighLayerImage = *high;
	
	SCRIPTMGR->GetImage( 127, &m_DelayImage  );	
	SCRIPTMGR->GetImage( 87, &m_OptionImage, PFT_MUGONGPATH );
}
void CQuickItem::Render()
{
	DWORD colorBackup = m_dwImageRGB;
	if( !m_bActive ) return;

	cIcon::Render();

	if(!m_HighLayerImage.IsNull())
		m_HighLayerImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	if(!m_LinkImage.IsNull())
	{
		VECTOR2 pos = { m_absPos.x+1, m_absPos.y+29 };
		m_LinkImage.RenderSprite(NULL, NULL, NULL, &pos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	
	if(m_pItemLink)
	{
		if(GetIconKind( m_pItemLink->GetItemIdx()) != eMUGONG_ICON && ITEMMGR->IsDupItem(m_pItemLink->GetItemIdx()))
		{		
			static char nums[4];
			wsprintf(nums,"%3d", m_QDur);	
			RECT rect={(LONG)m_absPos.x+21, (LONG)m_absPos.y+29, 1,1};
			CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		
		}

		if(GetIconKind( m_pItemLink->GetItemIdx()) == eABILITY_ICON)
		{
			((CAbilityIcon*)m_pItemLink)->RenderInfo(&m_absPos);
		}

		if(GetIconKind( m_pItemLink->GetItemIdx()) == eITEM_ICON)
		{
			CItem* pItem = ITEMMGR->GetItem(m_pItemLink->GetDBIdx());
			if( pItem )
			{
				if( pItem->GetItemBaseInfo()->ItemParam & ITEM_PARAM_TITAN_REGIST )
				{
					cDialog::Render();

					RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
					CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1515),4,&rect,RGBA_MAKE(250,250,128,255));

					DWORD dwRemainTime = 0;
					float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Item,pItem->GetItemIdx(), &dwRemainTime );

					if( delayRate > 0.0f )
					{
						VECTOR2 vScale = { 1.0f, delayRate };
						VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
						m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
					}
				}
				else if( pItem->GetItemKind() == eYOUNGYAK_ITEM_TITAN )
				{
					DWORD dwRemainTime = 0;
					float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime );
					if( delayRate > 0.0f )
					{
						VECTOR2 vScale = { 1.0f, delayRate };
						VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
						m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
					}

				}
				else if( pItem->GetItemKind() == eYOUNGYAK_ITEM)  //[吃药回气渲染][By:十里坡剑神][QQ:112582793][2017/11/27]
				{
					YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
					Tmp=GAMERESRCMNGR->GetYoungYakDelay()->GetData(pItem->GetItemIdx());
					if(Tmp)
					{
						DWORD dwRemainTime = 0;
						float delayRate = HERO->GetDelayGroup()->CheckYoungYakPotionDelay(Tmp->wtype,&dwRemainTime );
						if( delayRate > 0.0f )
						{
							VECTOR2 vScale = { 1.0f, delayRate };
							VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
							m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
						}
					}
				}
			}
		}
	}

	if( GetIconKind( m_pItemLink->GetItemIdx()) == eMUGONG_ICON )
	{
		if(((CMugongBase*)m_pItemLink)->GetOption())
			m_OptionImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		//////////////////////////////////////////////////////////////////////////
		DWORD dwRemainTime = 0;

		DWORD mugongIdx = 0;
		if( FALSE == HERO->InTitan() )
		{
			mugongIdx = m_pItemLink->GetItemIdx();
		}
		else

		{
			mugongIdx = m_pItemLink->GetItemIdx();
		}
		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill,(WORD)mugongIdx, &dwRemainTime );

		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}

		if( WINDOWMGR->IsMouseOverUsed() )
		{
			
			int mx = g_UserInput.GetMouse()->GetMouseX();
			int my = g_UserInput.GetMouse()->GetMouseY();
			
			if( PtInWindow( mx, my ) == TRUE )
			{
				
				if( dwRemainTime )
				{
					if( m_dwLastSecond != dwRemainTime/1000+1 )
					{
						m_dwLastSecond = dwRemainTime/1000+1;
						SetMugongIconToolTip( dwRemainTime );
						m_bRemainTime = TRUE;
					}
				}
				else if( m_bRemainTime == TRUE )
				{
					SetMugongIconToolTip( dwRemainTime );
					m_bRemainTime = FALSE;
					m_dwLastSecond = 0;
				}

				else
				{
					SetMugongIconToolTip( dwRemainTime );	
				}
			}
		}

	}
	
	m_dwImageRGB = colorBackup;
}


void CQuickItem::SetMugongIconToolTip( DWORD dwRemainTime )
{
	CSkillInfo* pCommSkill	= SKILLMGR->GetSkillInfo( GetSrcIdx() );
	
	if( pCommSkill )
	{
		char ToolTipText[256] = { 0, };

		WORD wCommSkillWeaponType = pCommSkill->GetWeaponType();
		WORD wCommSkillKind = pCommSkill->GetSkillKind();
		
		SetToolTip( "", RGB_HALF( 255, 255, 255), NULL, TTCLR_ITEM_CANEQUIP );
		
		wsprintf(ToolTipText, "[%s]", pCommSkill->GetSkillName() );
		AddToolTipLine( ToolTipText, TTTC_DEFAULT );
		
		CMugongBase* pMugongBase = GAMEIN->GetMugongDialog()->GetMugongAbs( GetSrcPosition() );
		
		if( pMugongBase )
		{
			int NeedExp = pCommSkill->GetNeedExp( pMugongBase->GetSung() );
			

			if( NeedExp > 0 )
			{
				if( pMugongBase->GetSung() < 12 )
				{
					float percent = pMugongBase->GetExpPoint() * 100.0f / NeedExp;
					if( percent > 99.99f )
					{
						if( pMugongBase->GetExpPoint() != NeedExp - 1 )
						{
							percent = 99.99f;
						}
					}
					sprintf( ToolTipText, CHATMGR->GetChatMsg(471), pMugongBase->GetSung(), percent );
				}

				else
				{
					if(SKILLMGR->IsDeadlyMugong(pMugongBase->GetItemIdx()) == FALSE)
						sprintf( ToolTipText, CHATMGR->GetChatMsg(405), pMugongBase->GetSung() );
				}
				AddToolTipLine( ToolTipText, TTTC_MUGONGEXP );
			}
			

			if( pMugongBase->GetSung() > 0 )
			{
				wsprintf( ToolTipText, CHATMGR->GetChatMsg(472), pCommSkill->GetNeedNaeRyuk( pMugongBase->GetSung() ) );
				AddToolTipLine( ToolTipText, TTTC_MUGONGNEEDMANA );
			}

			if( pCommSkill->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )

			{
				wsprintf( ToolTipText, CHATMGR->GetChatMsg(460), pCommSkill->GetSkillInfo()->RestrictLevel );
				AddToolTipLine( ToolTipText, TTTC_LIMIT );
				m_dwImageRGB = RGBA_MAKE( 255, 100, 100, 255 );
			}
			else
			{
				m_dwImageRGB = RGBA_MAKE( 255, 255, 255, 255 );
			}
		}
		
		if( wCommSkillKind == SKILLKIND_TITAN && HERO->InTitan() == FALSE )
			AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_LIMIT );

		if( wCommSkillKind != SKILLKIND_TITAN && HERO->InTitan() == TRUE )
			AddToolTipLine( CHATMGR->GetChatMsg(1656), TTTC_LIMIT );

		BOOL bFlag = FALSE;
		WORD wHeroWeaponType = HERO->GetWeaponEquipType();
		WORD wTitanWeaponType = HERO->GetTitanWeaponEquipType();


		if(HERO->InTitan() == TRUE)	
		{
			if( wCommSkillKind == SKILLKIND_TITAN )
			{
				if( wCommSkillWeaponType != WP_WITHOUT )	
				{
					if( wCommSkillWeaponType != wTitanWeaponType )
						bFlag = TRUE;					
				}
			}			
		}
		else
		{

			if( wCommSkillKind == SKILLKIND_OUTERMUGONG && wCommSkillWeaponType != wHeroWeaponType )

			{
				bFlag = TRUE;
			}
		}

		if( bFlag == TRUE )
		{
			switch(wCommSkillWeaponType)
			{
			case WP_GUM:
				AddToolTipLine( CHATMGR->GetChatMsg(465), TTTC_LIMIT );
				break;
			case WP_GWUN:	
				AddToolTipLine( CHATMGR->GetChatMsg(466), TTTC_LIMIT );
				break;
			case WP_DO:	
				AddToolTipLine( CHATMGR->GetChatMsg(467), TTTC_LIMIT );
				break;

			case WP_CHANG:	
				AddToolTipLine( CHATMGR->GetChatMsg(468), TTTC_LIMIT );
				break;
			case WP_GUNG:	
				AddToolTipLine( CHATMGR->GetChatMsg(469), TTTC_LIMIT );
				break;
			case WP_AMGI:	
				AddToolTipLine( CHATMGR->GetChatMsg(470), TTTC_LIMIT );
				break;
			}
		}

		if( wCommSkillKind == SKILLKIND_TITAN )

		{
			if( wCommSkillWeaponType != WP_WITHOUT )	
			{
				if( wHeroWeaponType != wTitanWeaponType )

					AddToolTipLine( CHATMGR->GetChatMsg(1660), TTTC_LIMIT );
			}
		}

		if( dwRemainTime )
		{
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(582), dwRemainTime/1000+1 );
			AddToolTipLine( ToolTipText, TTTC_QUESTITEM );
		}
	}		
}




