#include "stdafx.h"
#include "VideoCaptureManager.h"
#include "ChatManager.h"
#include "VideoCaptureDlg.h"
#include "GameIn.h"
#include "d3d8.h"
CVideoCaptureManager::CVideoCaptureManager()
{
}

CVideoCaptureManager::~CVideoCaptureManager()
{
}

BOOL CVideoCaptureManager::IsCapturing()
{
	return m_bandiCaptureLibrary.IsCapturing();
}

BOOL CVideoCaptureManager::CaptureStart()
{
	if( m_bandiCaptureLibrary.IsCapturing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2302 ) );	
		return FALSE;
	}
	void* pd3d9Device  = NULL;
	pd3d9Device=g_pExecutive->GetRenderer()->GetD3DDevice();
	if( !pd3d9Device )
		return FALSE;

	if( m_bandiCaptureLibrary.IsCreated() == FALSE )
	{
		if( FAILED( m_bandiCaptureLibrary.Create( BANDICAP_RELEASE_DLL_FILE_NAME ) ) )
		{
#ifdef _GMTOOL_
			CHATMGR->AddMsg( CTC_SYSMSG, "Video Capture DLL Load Error" );
#endif
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2303 ) );		
			return FALSE;
		}

		if( FAILED( m_bandiCaptureLibrary.Verify( "EYA_LUNAPLUS_20100218", "e5b68af6" ) ) )
		{
#ifdef _GMTOOL_
			CHATMGR->AddMsg( CTC_SYSMSG, "Dll许可认证失败" );
#endif
		}
	}

	if( m_bandiCaptureLibrary.IsCreated() )
	{
		BCAP_CONFIG cfg;
		m_bandiCaptureLibrary.SetMinMaxFPS( 30, 60 );

		SetCaptureOpt();

		char strDirectory[MAX_PATH];
		sprintf( strDirectory, "%sScreenShot", DIRECTORYMGR->GetFullDirectoryName( eLM_Root ) );
		if( ! ::CreateDirectory( strDirectory, NULL ) &&
			GetLastError() != ERROR_ALREADY_EXISTS )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2304 ) );		
			return FALSE;
		}
 
		TCHAR pathName[MAX_PATH];
		m_bandiCaptureLibrary.MakePathnameByDate( strDirectory, _T("录像"), _T("avi"), pathName, MAX_PATH);


		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2305 ), pathName );		

		HRESULT hr = m_bandiCaptureLibrary.Start( pathName, NULL, BCAP_MODE_D3D9, (LONG_PTR)pd3d9Device );
		if( FAILED( hr ) )
			return FALSE;
	}

	return TRUE;
}

void CVideoCaptureManager::CaptureStop()
{
	m_bandiCaptureLibrary.Stop();

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2329 ) );
}

void CVideoCaptureManager::Process()
{
	if( !m_bandiCaptureLibrary.IsCapturing() )
		return;
		void* pd3d9Device  = NULL;
	pd3d9Device=g_pExecutive->GetRenderer()->GetD3DDevice();
	if( !pd3d9Device )
		return;

	m_bandiCaptureLibrary.Work( (LONG_PTR)pd3d9Device );
}

void CVideoCaptureManager::SetCaptureOpt()
{
	CVideoCaptureDlg* pVideoCaptureDlg = GAMEIN->GetVideoCaptureDlg();
	if( !pVideoCaptureDlg )
		return;

	BCAP_CONFIG cfg;
	m_bandiCaptureLibrary.GetConfig( &cfg );

	switch( (CVideoCaptureDlg::eVideoCaptureSize)pVideoCaptureDlg->GetSelecedSize() )
	{
	case CVideoCaptureDlg::eVideoCaptureSize_Default:
		{
			cfg.Default();
		}
		break;

	case CVideoCaptureDlg::eVideoCaptureSize_500x375:
		{
			cfg.VideoSizeW = 500;
			cfg.VideoSizeH = 375;
		}
		break;
	}
	switch( (CVideoCaptureDlg::eVideoCaptureOpt)pVideoCaptureDlg->GetSelecedOpt() )
	{
	case CVideoCaptureDlg::eVideoCaptureOpt_High:
		{
			cfg.VideoCodec		= FOURCC_MJPG;
			cfg.VideoQuality	= 90;
			cfg.VideoFPS		= 30;

			cfg.AudioChannels	= 2;
			cfg.AudioCodec		= WAVETAG_NULL;
			cfg.AudioSampleRate = 44100;
		}
		break;

	case CVideoCaptureDlg::eVideoCaptureOpt_Low:
		{
			cfg.VideoCodec		= FOURCC_MPEG;
			cfg.VideoQuality	= 70;
			cfg.VideoFPS		= 24;

			cfg.AudioChannels	= 1;
			cfg.AudioCodec		= WAVETAG_NULL;
			cfg.AudioSampleRate = 22050;
		}
		break;
	}

	cfg.AdjustAudioMixer	= TRUE;

	cfg.IncludeCursor		= pVideoCaptureDlg->IsIncludeCursor();

	m_bandiCaptureLibrary.CheckConfig( &cfg );

	m_bandiCaptureLibrary.SetConfig( &cfg );
}