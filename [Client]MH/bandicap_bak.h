////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// BandiCaptureLibrary 1.3
/// 
/// Copyright(C) 2008-2009 BandiSoft.com All rights reserved.
///
/// Visit http://www.bandisoft.com for more information.
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BANDICAP_H
#define _BANDICAP_H

#include <windows.h>
#include <unknwn.h>
#include <mmsystem.h>
#include <stdio.h>

#ifndef ASSERT
#	include "assert.h"
#	define ASSERT(x) assert(x)
#endif

#ifndef _INC_TCHAR
#	ifdef _UNICODE
#		define _T(x)		L##x
#		define _stprintf	swprintf
#		define _stprintf_s  swprintf_s
#	else
#		define _T(x)	x
#		define _stprintf   sprintf
#		define _stprintf_s	sprintf_s
#	endif
#endif


#define _BCAP_MAKE_VER(a,b,c,d)	((a<<24)+(b<<16)+(c<<8)+d)
#define BCAP_VERSION			_BCAP_MAKE_VER(1,3,1,20)		// 1.3.1.20

#define FOURCC_MPEG mmioFOURCC('M','P','E','G')		// MPEG1
#define FOURCC_MJPG mmioFOURCC('M','J','P','G')		// MJPEG
#define FOURCC_MP4V mmioFOURCC('M','P','4','V')		// MPEG4
#define FOURCC_ARGB mmioFOURCC('A','R','G','B')		// 32bit Alpha DIB (ARGB)

#define WAVETAG_MP2		0x0050						// MPEG-1 Layer II
#define WAVETAG_PCM		0x0001						// PCM
#define WAVETAG_NULL	0x0000						// No Audio

enum 
{
	BCAP_MODE_GDI,										
	BCAP_MODE_D3D9,										
	BCAP_MODE_D3D9_SCALE,			
	BCAP_MODE_D3D9_SWAPCHAIN,		
	BCAP_MODE_D3D8,										
	BCAP_MODE_D3D10,										
	BCAP_MODE_GL,										
	BCAP_MODE_DDRAW7										
};

enum 
{
	BCAP_IMAGE_BMP,				
	BCAP_IMAGE_PNG,			
	BCAP_IMAGE_JPG,			
};

struct BCAP_CONFIG
{
	enum { F_AVI };
	enum { V_CBR, V_VBR };
	enum { A_CBR, A_VBR };

	BCAP_CONFIG() { Default(); };
	void Default()
	{
		FileType				= F_AVI;

		VideoSizeW				= 0;
		VideoSizeH				= 0;
		VideoFPS				= 30.000;

		VideoCodec				= FOURCC_MPEG;
		VideoRateControl		= V_VBR;
		VideoKBitrate			= 1024;
		VideoQuality			= 80;
		VideoKeyframeInterval	= 150;

		AudioChannels			= 2;
		AudioSampleRate			= 48000;

		AudioCodec				= WAVETAG_MP2;
		AudioRateControl		= A_CBR;
		AudioKBitrate			= 192;
		AudioQuality			= 30;

		LogoPath[0]				= '\0';
		LogoAlpha				= 0;
		LogoPosX				= 50;
		LogoPosY				= 100;
		LogoSizeW				= 100;
		LogoSizeH				= 100;

		IncludeCursor			= FALSE;
		AdjustAudioMixer		= TRUE;
	};

	INT		FileType;			

	INT		VideoSizeW;				
	INT		VideoSizeH;				
	FLOAT	VideoFPS;				

	DWORD	VideoCodec;			
	INT		VideoRateControl;		
	INT		VideoKBitrate;			
	INT		VideoQuality;			
	INT		VideoKeyframeInterval;	
	
	INT		AudioChannels;			
	INT		AudioSampleRate;		

	INT		AudioCodec;					
	INT		AudioRateControl;		
	INT		AudioKBitrate;			
	INT		AudioQuality;			

	WCHAR	LogoPath[MAX_PATH];				
	INT		LogoAlpha;						
	INT		LogoPosX;						
	INT		LogoPosY;						
	INT		LogoSizeW;						
	INT		LogoSizeH;						

	BOOL	IncludeCursor;		
	BOOL	AdjustAudioMixer;		
};


struct IBandiCapture : public IUnknown
{
	STDMETHOD (Verify)(LPCSTR szID, LPCSTR szKey) PURE;						

	STDMETHOD_(INT, GetVersion)() PURE;									

	STDMETHOD (Start)(LPCWSTR pwszFilePath, HWND hParentWnd,
					INT nCaptureMode, LONG_PTR lpParam) PURE;			
	STDMETHOD (Stop)() PURE;											
	STDMETHOD (Work)(LONG_PTR lpParam) PURE;							
	STDMETHOD_(BOOL, IsCapturing)() PURE;						

	STDMETHOD (CheckConfig)(BCAP_CONFIG *pCfg) PURE;				
	STDMETHOD (SetConfig)(BCAP_CONFIG *pCfg) PURE;					
	STDMETHOD (GetConfig)(BCAP_CONFIG *pCfg) PURE;					

	STDMETHOD_(INT, GetCaptureTime)() PURE;									
	STDMETHOD_(INT64, GetCaptureFileSize)() PURE;					

	STDMETHOD (SetPriority)(INT nPriority) PURE;					
	STDMETHOD_(INT, GetPriority)() PURE;							

	STDMETHOD (SetMinMaxFPS)(INT nMin, INT nMax) PURE;				
	STDMETHOD (GetMinMaxFPS)(INT *pnMin, INT *pnMax) PURE;			

	STDMETHOD (CaptureImage)(LPCWSTR pwszFilePath, INT nFileType, INT nQuality, INT nCaptureMode, 
					BOOL bIncludeCursor, LONG_PTR lpParam) PURE;	
};


typedef HRESULT (WINAPI *LPCREATEBANDICAP)(UINT SDKVersion, void** pp);


#define _FACBC  0x777
#define MAKE_BCHRESULT( code )				MAKE_HRESULT( 1, _FACBC, code )
#define MAKE_BCSTATUS( code )				MAKE_HRESULT( 0, _FACBC, code )

#define BC_OK								S_OK
#define BC_FALSE							S_FALSE
#define BC_OK_ONLY_VIDEO					MAKE_BCSTATUS(0x0010)
#define BC_OK_ONLY_AUDIO					MAKE_BCSTATUS(0x0011)
#define BC_OK_IS_NOT_CAPTURING				MAKE_BCSTATUS(0x0020)

#define BCERR_FAIL							E_FAIL

#define BCERR_INVALIDARG					MAKE_BCHRESULT(0x1010)
#define BCERR_VIDEO_CAPTURE					MAKE_BCHRESULT(0x1011)
#define BCERR_AUDIO_CAPTURE					MAKE_BCHRESULT(0x1012)
#define BCERR_FILE_CREATE					MAKE_BCHRESULT(0x1013)
#define BCERR_FILE_WRITE					MAKE_BCHRESULT(0x1014)
#define BCERR_NOT_ENOUGH_DISKSPACE			MAKE_BCHRESULT(0x1015)

#define BCERR_LOAD_LIBRARY_FAIL				MAKE_BCHRESULT(0x1020)
#define BCERR_ALREADY_CREATED				MAKE_BCHRESULT(0x1021)
#define BCERR_GET_PROC_ADDRESS_FAIL			MAKE_BCHRESULT(0x1022)
#define BCERR_LIBRARY_NOT_LOADED			MAKE_BCHRESULT(0x1023)
#define BCERR_UNSUPPORTED_OS				MAKE_BCHRESULT(0x1024)

#define BCERR_INVALID_VERSION				MAKE_BCHRESULT(0x1030)
#define BCERR_NOT_ENOUGH_MEMORY				MAKE_BCHRESULT(0x1031)


#ifndef _BANDICAP_NO_UTIL_CLASS

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Utility class for dll load
//
#ifdef _WIN64
	#ifdef _DEBUG
	#	define BANDICAP_DLL_FILE_NAME			_T("bdcap64_d.dll")
	#else
	#	define BANDICAP_DLL_FILE_NAME			_T("bdcap64.dll")
	#endif
	#define BANDICAP_RELEASE_DLL_FILE_NAME		_T("bdcap64.dll")
#else
	#ifdef _DEBUG
	#	define BANDICAP_DLL_FILE_NAME			_T("bdcap32_d.dll")
	#else
	#	define BANDICAP_DLL_FILE_NAME			_T("bdcap32.dll")
	#endif
	#define BANDICAP_RELEASE_DLL_FILE_NAME		_T("bdcap32.dll")
#endif


class CBandiCaptureLibrary : public IBandiCapture
{
public :
	CBandiCaptureLibrary()
	{
		m_hDll = NULL;
		m_pCreateBandiCap = NULL;
		m_pCap = NULL;
	}

	~CBandiCaptureLibrary()
	{
		Destroy();
	}

	HRESULT Create(LPCTSTR szDllPathName)
	{
		if(m_hDll) {ASSERT(0); return BCERR_ALREADY_CREATED;}

		OSVERSIONINFO osv;
		osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&osv)==FALSE || 
			osv.dwPlatformId <=VER_PLATFORM_WIN32_WINDOWS)		// does not support 98
		{
			return BCERR_UNSUPPORTED_OS;
		}

		m_hDll = LoadLibrary(szDllPathName);
		if(m_hDll==NULL)
		{
			m_hDll = LoadLibrary(BANDICAP_RELEASE_DLL_FILE_NAME);
			if(m_hDll==NULL)
			{
				ASSERT(0);
				return BCERR_LOAD_LIBRARY_FAIL;
			}
		}

		m_pCreateBandiCap = (LPCREATEBANDICAP)GetProcAddress(m_hDll, "CreateBandiCapture");

		if(m_pCreateBandiCap==NULL)
		{
			ASSERT(0);
			FreeLibrary(m_hDll);
			m_hDll = NULL;
			return BCERR_GET_PROC_ADDRESS_FAIL;
		}

		HRESULT hr = CreateNewBandiCaptureInstance(&m_pCap);
		if(FAILED(hr))
		{
			ASSERT(0);
			FreeLibrary(m_hDll);
			m_hDll = NULL;
			return hr;
		}

		return S_OK;
	}

	BOOL	IsCreated()
	{
		return m_pCap ? TRUE : FALSE;
	}

	void	Destroy()
	{
		if(m_pCap)
		{
			m_pCap->Stop();
			m_pCap->Release();
		}
		m_pCap = NULL;
		m_pCreateBandiCap = NULL;
		if(m_hDll) FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

	LPCTSTR MakePathnameByDate(LPCTSTR szTargetDirectory, LPCTSTR szName, LPCTSTR szExt, LPTSTR szBuf, int nBufLen)
	{
		SYSTEMTIME	sysTime;
		GetLocalTime(&sysTime);
#if _MSC_VER < 1400	// below 2005
		_stprintf(szBuf, _T("%s\\%s %d-%02d-%02d %02d-%02d-%02d-%03d.%s"), szTargetDirectory, szName, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, szExt);
#else				// 2005, 2008
		_stprintf_s(szBuf, nBufLen, _T("%s\\%s %d-%02d-%02d %02d-%02d-%02d-%03d.%s"), szTargetDirectory, szName, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, szExt);
#endif
		return szBuf;
	}

public :	
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** p) { return S_OK; }
	ULONG   STDMETHODCALLTYPE AddRef( void){return 0;}
	ULONG   STDMETHODCALLTYPE Release( void){return 0;}

public :		// IBandiCapture

	STDMETHODIMP	Verify(LPCSTR szID, LPCSTR szKey)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}
		return m_pCap->Verify(szID, szKey);
	}

	STDMETHODIMP_(INT)	GetVersion()
	{
		return m_pCap->GetVersion();
	}

	STDMETHODIMP	Start(LPCSTR pszFilePath, HWND hParentWnd, INT nCaptureMode, LONG_PTR lpParam)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}

		WCHAR wszPath[MAX_PATH+1];
		MultiByteToWideChar(CP_ACP, NULL, pszFilePath, -1, wszPath, MAX_PATH);
		return m_pCap->Start(wszPath, hParentWnd, nCaptureMode, lpParam);
	}

	STDMETHODIMP	Start(LPCWSTR pwszFilePath, HWND hParentWnd, INT nCaptureMode, LONG_PTR lpParam)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}
		return m_pCap->Start(pwszFilePath, hParentWnd, nCaptureMode, lpParam);
	}

	STDMETHODIMP Stop()
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}
		return m_pCap->Stop();
	}

	STDMETHODIMP Work(LONG_PTR lpParam)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}
		return m_pCap->Work(lpParam);
	}

	STDMETHODIMP_(BOOL) IsCapturing()
	{
		if(m_pCap==NULL) return FALSE;
		return m_pCap->IsCapturing();
	}

	STDMETHODIMP CheckConfig(BCAP_CONFIG *pCfg)
	{
		return m_pCap->CheckConfig(pCfg);
	}

	STDMETHODIMP SetConfig(BCAP_CONFIG *pCfg)
	{
		return m_pCap->SetConfig(pCfg);
	}

	STDMETHODIMP GetConfig(BCAP_CONFIG *pCfg)
	{
		return m_pCap->GetConfig(pCfg);
	}

	STDMETHODIMP_(INT) GetCaptureTime()
	{
		return m_pCap->GetCaptureTime();
	}

	STDMETHODIMP_(INT64) GetCaptureFileSize()
	{
		return m_pCap->GetCaptureFileSize();
	}

	STDMETHODIMP SetPriority(INT nPriority)
	{
		return m_pCap->SetPriority(nPriority);
	}

	STDMETHODIMP_(INT) GetPriority()
	{
		return m_pCap->GetPriority();
	}

	STDMETHODIMP SetMinMaxFPS(INT nMin, INT nMax)
	{
		return m_pCap->SetMinMaxFPS(nMin, nMax);
	}

	STDMETHODIMP GetMinMaxFPS(INT *pnMin, INT *pnMax)
	{
		return m_pCap->GetMinMaxFPS(pnMin, pnMax);
	}

	STDMETHODIMP CaptureImage(LPCSTR pszFilePath, INT nFileType, INT nQuality, INT nCaptureMode, 
							BOOL bIncludeCursor, LONG_PTR lpParam)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}

		WCHAR wszPath[MAX_PATH+1];
		MultiByteToWideChar(CP_ACP, NULL, pszFilePath, -1, wszPath, MAX_PATH);

		return m_pCap->CaptureImage(wszPath, nFileType, nQuality, nCaptureMode, bIncludeCursor, lpParam);
	}

	STDMETHODIMP CaptureImage(LPCWSTR pwszFilePath, INT nFileType, INT nQuality, INT nCaptureMode, 
							BOOL bIncludeCursor, LONG_PTR lpParam)
	{
		if(m_pCap==NULL){ASSERT(0); return E_FAIL;}
		return m_pCap->CaptureImage(pwszFilePath, nFileType, nQuality, nCaptureMode, bIncludeCursor, lpParam);
	}

private :
	HRESULT CreateNewBandiCaptureInstance(IBandiCapture** ppBaCapture)
	{
		if(m_pCreateBandiCap==NULL){ASSERT(0); return BCERR_LIBRARY_NOT_LOADED;}
		*ppBaCapture = NULL;
		HRESULT hr = m_pCreateBandiCap(BCAP_VERSION, (void**)ppBaCapture);
		if(FAILED(hr)) ASSERT(0);
		return hr;
	}

private :
	HMODULE				m_hDll;
	LPCREATEBANDICAP	m_pCreateBandiCap;

public :
	IBandiCapture*		m_pCap;
};


// PRESETS
enum BCAP_PRESET
{
	BCAP_PRESET_DEFAULT,
	BCAP_PRESET_HALFSIZE,
	BCAP_PRESET_320x240,
	BCAP_PRESET_400x300,
	BCAP_PRESET_512x384,
	BCAP_PRESET_576x432,
	BCAP_PRESET_640x480,
	BCAP_PRESET_800x600,

	BCAP_PRESET_MPEG1,
	BCAP_PRESET_MJPEG,
	BCAP_PRESET_MJPEG_HIGH_QUALITY,
	BCAP_PRESET_MPEG4,

	BCAP_PRESET_YOUTUBE,
	BCAP_PRESET_YOUTUBE_HIGH_QUALITY,
	BCAP_PRESET_YOUTUBE_HIGH_DEFINITION,
	BCAP_PRESET_NAVER_BLOG,
	BCAP_PRESET_DAUM_TVPOT,
	BCAP_PRESET_MNCAST,
};

// SETTING CONFIG BY PRESET
inline BOOL BCapConfigPreset(BCAP_CONFIG* pConfig, BCAP_PRESET preset)
{
	pConfig->Default();

	switch(preset)
	{
	case BCAP_PRESET_DEFAULT :
		break;

	case BCAP_PRESET_HALFSIZE :
		pConfig->VideoSizeW = -1;			// half size
		pConfig->VideoSizeH = -1;
		break;

	case BCAP_PRESET_320x240 :
		pConfig->VideoSizeW = 320;			// 320x240
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		break;
	case BCAP_PRESET_400x300 :
		pConfig->VideoSizeW = 400;			// 400x300
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		break;
	case BCAP_PRESET_512x384 :
		pConfig->VideoSizeW = 512;			// 512x384
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		break;
	case BCAP_PRESET_576x432 :
		pConfig->VideoSizeW = 576;			// 576x432
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		break;
	case BCAP_PRESET_640x480 :
		pConfig->VideoSizeW = 640;			// 640x480
		pConfig->VideoSizeH = 0;
		break;
	case BCAP_PRESET_800x600 :
		pConfig->VideoSizeW = 800;			// 800x600
		pConfig->VideoSizeH = 0;
		break;

	case BCAP_PRESET_MPEG1 :				// MPEG1
		pConfig->VideoCodec	= FOURCC_MPEG;
		pConfig->VideoQuality = 80;
		pConfig->AudioCodec	= WAVETAG_MP2;
		pConfig->AudioSampleRate = 44100;
		break;

	case BCAP_PRESET_MJPEG :				// MJPEG
		pConfig->VideoCodec	= FOURCC_MJPG;
		pConfig->VideoQuality = 80;
		pConfig->AudioCodec	= WAVETAG_PCM;
		pConfig->AudioSampleRate = 24000;
		break;
	case BCAP_PRESET_MJPEG_HIGH_QUALITY :	// MJPEG with High qulaity audio & video
		pConfig->VideoCodec	= FOURCC_MJPG;
		pConfig->VideoQuality = 90;
		pConfig->AudioCodec	= WAVETAG_PCM;
		pConfig->AudioSampleRate = 44100;
		break;

	case BCAP_PRESET_MPEG4 :				// MPEG4
		pConfig->VideoCodec	= FOURCC_MP4V;
		pConfig->VideoQuality = 80;
		pConfig->AudioCodec	= WAVETAG_PCM;
		pConfig->AudioSampleRate = 24000;
		break;

	case BCAP_PRESET_YOUTUBE :
		pConfig->VideoQuality = 70;
		pConfig->VideoSizeW = 320;			// 320x240
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		pConfig->AudioChannels = 1;			// mono
		pConfig->AudioSampleRate = 22050;	// 22k
		break;

	case BCAP_PRESET_YOUTUBE_HIGH_QUALITY :
		pConfig->VideoSizeW = 480;			// 480x272
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 30;
		pConfig->AudioChannels = 1;			// mono
		pConfig->AudioSampleRate = 44100;	// 44k
		break;

	case BCAP_PRESET_YOUTUBE_HIGH_DEFINITION :
		pConfig->VideoSizeW = 0;			// 1280x720
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 30;
		pConfig->AudioChannels = 2;			// stereo
		pConfig->AudioSampleRate = 44100;	// 44k
		break;

	case BCAP_PRESET_NAVER_BLOG :
	case BCAP_PRESET_DAUM_TVPOT :
		pConfig->VideoSizeW = 512;			// 512x384
		pConfig->VideoSizeH = 0;
		pConfig->VideoFPS = 24;
		pConfig->AudioSampleRate = 22050;	// 22k
		break;

	case BCAP_PRESET_MNCAST :
		pConfig->VideoSizeW = 500;			// 500x374
		pConfig->VideoSizeH = 0;
		break;

	default :								// error
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}


#endif // _BANDICAP_NO_UTIL_CLASS

#endif // _BANDICAP_H
