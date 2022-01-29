// WeatherManager.cpp: implementation of the CWeatherManager class.
//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeatherManager.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "MHfile.h"
#include "StatusIconDlg.h"
#include "MHCamera.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define EffectDist 1000.0f

GLOBALTON(CWeatherManager);

CWeatherManager::CWeatherManager()
{
	m_wState    = eWS_None;
	m_bSnow     = FALSE; 
	m_bSnowFt   = TRUE;
	m_bSnowNet  = FALSE;
}

CWeatherManager::~CWeatherManager()
{
	
}

void CWeatherManager::Init()
{
	CMHFile file;
	
	if(!file.Init("Resource/Client/WeatherEffect.bin","rb"))
		return ;
	for( int i = 1; i < eWS_Max; i++ )
	{
		char filename[256] = {0,};

		file.GetString( filename );

		if( m_WeatherEffect[i].Init( filename, NULL, eEngineObjectType_GameSystemObject, 0, -1 ) )

		{
			m_WeatherEffect[i].SetRepeat( TRUE );
		}

	}

}

void CWeatherManager::Release()
{ 
	for( int i = 1; i < eWS_Max; i++ )
	{
		m_WeatherEffect[i].Release();
	}
}


void CWeatherManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	BOOL change = FALSE;

	if(HERO == NULL)
		return;

	switch( Protocol ) 
	{


	case MP_WEATHER_STATE:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;			

			


			if( msg->wData >= eWS_Max)

				return;

			if( m_wState != msg->wData )
			{
				m_wState = msg->wData; 
				change = TRUE;
			}
			
			switch( m_wState )
			{
			case eWS_None:

				{
					STATUSICONDLG->RemoveIcon( HERO, 50 );
					if( change ) CHATMGR->AddMsg( CTC_OPERATOR, CHATMGR->GetChatMsg( 587 ) ); 
					m_WeatherEffect[eWS_Snow].Hide(); 					
					m_bSnowNet = FALSE;
				}
				break;
			case eWS_Snow:
				{
					STATUSICONDLG->AddIcon( HERO,  50 );
					if( change ) CHATMGR->AddMsg( CTC_OPERATOR, CHATMGR->GetChatMsg( 585 ) );
						m_WeatherEffect[eWS_Snow].Show();
						m_bSnowNet = TRUE;
				}
				break;
			}
		}
		break;
	}
}

void CWeatherManager::SetWeather(BOOL bSnow)
{
	m_bSnow = bSnow;

	if(m_bSnow)
	{
		m_WeatherEffect[eWS_Snow].Hide();
		m_bSnowFt = TRUE;
	}
	else
	{
		if(m_bSnowNet)
		{
			if(m_bSnowFt) m_WeatherEffect[eWS_Snow].Show();
			m_bSnowFt = FALSE;
		}
	}
}

void CWeatherManager::SetWeather(WORD MapNum, stWeatherTime &stWeather)
{
	if(stWeather.State >= eWS_Max)
		return;

	MSG_WEATHER msg;

	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_SET;
	msg.dwObjectID = HERO->GetID();

	msg.MapNum = MapNum;
	
	memcpy( &msg.WeatherTime, &stWeather, sizeof(stWeatherTime) );
	
	NETWORK->Send( &msg, sizeof( msg ) );
}

void CWeatherManager::SetWeather(WORD MapNum, WORD wState)
{
	if( wState >= eWS_Max)
		return;

	MSG_WORD2 msg;

	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_EXE;
	msg.dwObjectID = HERO->GetID();
	msg.wData1 = wState;	
	msg.wData2 = MapNum;	
		
	NETWORK->Send( &msg, sizeof( msg ) );
}


void CWeatherManager::ReturnSchedule(WORD MapNum)

{
	MSG_WORD msg;
	
	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_RETURN;
	msg.dwObjectID = HERO->GetID();
	msg.wData = MapNum;
		
	NETWORK->Send( &msg, sizeof( msg ) );
}


void CWeatherManager::Process()
{
	if(m_wState == eWS_None)

	{
		return;
	}


	if(m_wState >= eWS_Max)
		return;

	MHCAMERADESC* pCameraDesc = CAMERA->GetCameraDesc();

	m_EffectPos = pCameraDesc->m_CameraPos;
}

void CWeatherManager::Render()
{
	if(m_wState == eWS_None)
	{
		return;
	}

	if(m_wState >= eWS_Max)
		return;

	m_WeatherEffect[m_wState].SetEngObjPosition( &m_EffectPos );
}




