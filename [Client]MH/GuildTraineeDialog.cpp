








#include "stdafx.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "interface\cCheckBox.h"
#include "interface\cStatic.h"
#include "interface\cButton.h"
#include "interface\cTextArea.h"
#include "GameIn.h"
#include "GuildCreateDialog.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"

#include "interface\cPushupButton.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameEventManager.h"

#include ".\guildtraineedialog.h"





CGuildTraineeDialog::CGuildTraineeDialog(void)
{
//	m_type = WT_GUILDTRAINEEDLG;
	ZeroMemory(m_pGuildMark, sizeof(m_pGuildMark));
	m_nPageIndex = 0;
	m_backnumber = 0;
}


CGuildTraineeDialog::~CGuildTraineeDialog(void)
{
	ZeroMemory(m_pGuildMark, sizeof(m_pGuildMark));
}


void CGuildTraineeDialog::SetActive(BOOL val)
{
	if( val )	
	{
		// ���� ��ư Init ��Ŵ
		for(int i=0; i<5; ++i)
			m_pPageListBtn[i]->SetActive( TRUE );
		
		for(int i=0; i<4; ++i)
		{
			m_pCheckBox[i]->SetActive( TRUE );
			m_pLockImage[i]->SetActive( FALSE );
		}

		m_pPageBtn[0]->SetActive( TRUE );
		m_pPageBtn[1]->SetActive( TRUE );
		m_pJoinBtn->SetActive( TRUE );

		m_CheckIndex = 0;
		ZeroMemory(m_MunHaInfo, sizeof(m_MunHaInfo));

		// ���� ù������ Ǫ��
		m_pPageListBtn[0]->SetPush(TRUE);
	}

	else
	{
		GAMEEVENTMGR->AddEvent( eGameEvent_Munpaunder );

		m_backnumber = 0;	// ���� ������ �������� �ȵǰ� �ϱ����ؼ�

		if( HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}


	cDialog::SetActive(val);
}

void CGuildTraineeDialog::Linking()
{
	for( int i=0; i<MAX_TRAINEEMUNPA_PERPAGE; ++i)
	{
		m_pCheckBox[i] = (cCheckBox*)GetWindowForID( GU_CHK1+i );
//		m_pRanking[i] = (cStatic*)GetWindowForID( GU_RANK1+i );
		m_pMemberNum[i] = (cStatic*)GetWindowForID( GU_NUMBER1+i );
		m_pMunpaName[i] = (cStatic*)GetWindowForID( GU_NAME1+i );
		m_pMunpaMark[i] = (cStatic*)GetWindowForID( GU_MARK1+i );
		m_pLockImage[i] = (cStatic*)GetWindowForID( GU_lock1+i );
		m_pMunpaBook[i] = (cTextArea*)GetWindowForID( GU_BOOK1+i );
	}

	for(int i=0; i<5; ++i)
		m_pPageListBtn[i] = (cPushupButton*)GetWindowForID( GU_SEARCH1LISTBTN+i );

	m_pPageBtn[0] = (cButton*)GetWindowForID( GU_SEARCHFRONTLISTBTN );
	m_pPageBtn[1] = (cButton*)GetWindowForID( GU_SEARCHBACKLISTBTN );
	m_pJoinBtn = (cButton*)GetWindowForID( GU_OKBTN );
}

void CGuildTraineeDialog::Render()
{
	cDialog::Render();

	if(IsActive())
	{
		for( int i = 0; i < MAX_TRAINEEMUNPA_PERPAGE; i++ )
		{
			if(m_pGuildMark[i] && m_MunHaInfo[i].nMunhaGuildMark )
			{
				VECTOR2 pos2;

				pos2.x = m_pMunpaMark[i]->GetAbsX();
				pos2.y = m_pMunpaMark[i]->GetAbsY();
				m_pGuildMark[i]->Render(&pos2);	
			}
		}
	}
}

void CGuildTraineeDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	switch(lId)
	{
	case GU_CHK1:
		{
			SetCheckBtn(0);
		}
		break;
	case GU_CHK2:
		{
			SetCheckBtn(1);
		}
		break;
	case GU_CHK3:
		{
			SetCheckBtn(2);
		}
		break;
	case GU_CHK4:
		{
			SetCheckBtn(3);
		}
		break;
	case GU_OKBTN:	// �����ϱ�
		{
			int count = 0;
			int level = HERO->GetCharacterTotalInfo()->Level;
			int Munpa = HERO->GetCharacterTotalInfo()->MunpaID;
			
			for( int i = 0; i < 4; i++ )
			{
				if( !m_pCheckBox[i]->IsChecked() )
					count++;
			}

			// �����ڵ� //////////////////////////////////////////////////////////////
			if( count == 4 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );

				return;
			}
			else if( Munpa != 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 28 ) );
				return;
			}
			else if( level >= 40)

			{

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1466 ) );
				return;
			}
			//////////////////////////////////////////////////////////////////////////

			MSG_MUNHANOTEINFO msg;
			ZeroMemory(&msg, sizeof(MSG_MUNHANOTEINFO));
 			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_JOIN_SYN );

			msg.dwObjectID = HEROID;
			msg.dwData = m_MunHaInfo[m_CheckIndex].nMunpaIdx;
			SafeStrCpy(msg.Intro, CHATMGR->GetChatMsg(1469), MAX_CHAT_LENGTH+1);

			NETWORK->Send(&msg, sizeof(msg));
			GAMEIN->GetGuildCreateDlg()->SetActive( FALSE );
		}

		break;
	case GU_SEARCHFRONTLISTBTN:	// ������ư
		{
            SetPageIndex(FALSE);
		}
		break;
	case GU_SEARCH1LISTBTN:	// 1��������	lId:1318
		{
			SendPage(1);
		}
		break;
	case GU_SEARCH2LISTBTN:	// 2��������	lId:1319
		{
			SendPage(2);
		}
		break;
	case GU_SEARCH3LISTBTN:	// 3��������	lId:1320
		{
			SendPage(3);
		}
		break;
	case GU_SEARCH4LISTBTN:	// 4��������	lId:1321
		{
			SendPage(4);
		}
		break;
	case GU_SEARCH5LISTBTN:	// 5��������	lId:1322
		{
			SendPage(5);
		}
		break;
	case GU_SEARCHBACKLISTBTN:	// ������ư
		{
			SetPageIndex(TRUE);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CGuildTraineeDialog::SetCheckBtn(int index)
{
	// üũ�� �� üũ�ڽ��� TRUE �������� FALSE
	for( int i = 0; i < 4; i++ )
		m_pCheckBox[i]->SetChecked( FALSE );

	m_pCheckBox[index]->SetChecked( TRUE );

	m_CheckIndex = index;
}

//#define TRAINEE_TEST

void CGuildTraineeDialog::SendPage(int Index, BOOL bFirst)
{
#ifdef TRAINEE_TEST
	MSG_MUNHAINFO temp;
	ZeroMemory(&temp, sizeof(temp));
	WORD aaa = Index + (m_nPageIndex * 5);
	temp.Index = aaa;
	temp.MaxCount = 50;
	for( int i = 0; i < 4; i++ )
	{
		temp.sMunHa_Info[i].nMunhaCount = 10;
		temp.sMunHa_Info[i].nMunhaGuildMark = 23;
		temp.sMunHa_Info[i].nMunpaIdx = aaa+i+1;
		temp.sMunHa_Info[i].nMunpaRank = aaa+i+1;
		sprintf( temp.sMunHa_Info[i].strName, "%d�� ����", aaa+i+1 );

		sprintf( temp.sMunHa_Info[i].strIntro, "���̻�����ĥ�ȱ������̻�����ĥ�ȱ������̻�����ĥ�ȱ���" );
	}
	SetPage(&temp);
#else

	if(bFirst)	// â�� �Ӷ��� �ʱ�ȭ ������(�̵��ÿ��� �ʱ�ȭ �Ǹ� ������ �ȵ�)
		m_nPageIndex = 0;

    MSG_WORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_MUNPA_CATALOG_SYN;
	msg.dwObjectID = HEROID;
	WORD temp = Index + (m_nPageIndex * 5);
	msg.wData = temp;


	if(Index > 0)
		m_pPageListBtn[Index-1]->SetPush( TRUE );

	if(m_backnumber != temp)
	{
		NETWORK->Send(&msg, sizeof(msg));
		m_backnumber = temp;	// ���� ������ �������� �ȵǰ� �ϱ����ؼ�
	}

#endif
}

void CGuildTraineeDialog::SetPage(MSG_MUNHAINFO* pmsg)
{
	int real = m_nPageIndex * 5;
	int Index = pmsg->Index - real;	// �ش� ������ ���
	int page = Index - 1;
	char buf[16] = { NULL };


	m_MaxCount = pmsg->MaxCount;	// �� �ۼ�
	// ���� ������������ �ִ��� üũ
	int MaxPage = GetBasePage(m_MaxCount);

	for( int i = 0; i < 5; i++ )
	{

		if(real+i < MaxPage)
		{
			sprintf(buf, "%d", real+i+1);
			m_pPageListBtn[i]->SetText(buf, RGB(255, 255, 255));

			m_pPageListBtn[i]->SetActive( TRUE );
			m_pPageListBtn[i]->SetPush( FALSE );

			m_pPageListBtn[page]->SetPush( TRUE );
		}
		else
		{
			m_pPageListBtn[i]->SetActive( FALSE );
		}
	}

	if(real <= 0)
		m_pPageBtn[0]->SetActive( FALSE );
	else
		m_pPageBtn[0]->SetActive( TRUE );

	if(MaxPage < real+5)
		m_pPageBtn[1]->SetActive( FALSE );
	else
		m_pPageBtn[1]->SetActive( TRUE );


    SetMunpaInfo(pmsg->sMunHa_Info);
}

int CGuildTraineeDialog::GetBasePage(int count)
{

	// ���� ������������ �ִ��� üũ
	int MaxPage = count / 4;
	if(count % 4)
		MaxPage++;

	return MaxPage;
}

void CGuildTraineeDialog::SetPageIndex(BOOL Flag)
{
	if(Flag)
		m_nPageIndex++;
	else

		m_nPageIndex--;

	// magi82(40) ���ϻ� �����ϱ�â�� ���� ������ �ٿ�Ǵ� ������ ����ڵ�..
	if( m_nPageIndex < 0)
		m_nPageIndex = 0;

	SendPage(1);
}

void CGuildTraineeDialog::SetMunpaInfo(MUNHA_INFO temp[])
{
	char buf[256] = {NULL};

	ZeroMemory(m_MunHaInfo, sizeof(m_MunHaInfo));
	CopyMemory(m_MunHaInfo, temp, sizeof(m_MunHaInfo));

	for( int i = 0; i < MAX_TRAINEEMUNPA_PERPAGE; i++ )
	{
		m_pCheckBox[i]->SetActive( TRUE );
		m_pCheckBox[i]->SetChecked( FALSE );

//		m_pRanking[i]->SetStaticText("");
		m_pMemberNum[i]->SetStaticText("");
		m_pMunpaName[i]->SetStaticText("");
		m_pMunpaBook[i]->SetScriptText("");
	}

	for( int i = 0; i < MAX_TRAINEEMUNPA_PERPAGE; i++ )
	{
		if(m_MunHaInfo[i].nMunpaIdx > 0)
		{
//			sprintf( buf, "%d", m_MunHaInfo[i].nMunpaRank );
//			m_pRanking[i]->SetStaticText( buf );

			if(m_MunHaInfo[i].nMunhaCount >= 25)
			{
				m_pMemberNum[i]->SetFGColor(RGBA_MAKE(255, 0, 0, 255));
				m_pCheckBox[i]->SetActive( FALSE );
				m_pLockImage[i]->SetActive( TRUE );
			}
			else
			{
				m_pMemberNum[i]->SetFGColor(RGBA_MAKE(255, 255, 255, 255));
				m_pCheckBox[i]->SetActive( TRUE );
				m_pLockImage[i]->SetActive( FALSE );
			}

			sprintf( buf, "%d / 25", m_MunHaInfo[i].nMunhaCount );
			m_pMemberNum[i]->SetStaticText( buf );

			m_pMunpaName[i]->SetStaticText( m_MunHaInfo[i].strName );

			m_pMunpaBook[i]->SetScriptText( m_MunHaInfo[i].strIntro );

			m_pGuildMark[i] = GUILDMARKMGR->GetGuildMark( m_MunHaInfo[i].nMunhaGuildMark );
		}
		else
		{
			m_pCheckBox[i]->SetActive( FALSE );
		}

	}
}




