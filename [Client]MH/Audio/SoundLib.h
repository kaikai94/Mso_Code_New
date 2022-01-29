




// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOUNDLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOUNDLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
/*
	¿Þ¼Õ ÁÂÇ¥°è.

	x´Â ¿À¸¥ÂÊ, y´Â À§, z´Â ¾ÕÀÌ¶ó °¡Á¤ÇÑ´Ù.
*/
#ifndef	_SoundLib_h_
#define	_SoundLib_h_



#ifdef SOUNDLIB_EXPORTS
#define SOUNDLIB_API extern "C" __declspec(dllexport)
#else

#define SOUNDLIB_API extern "C" __declspec(dllimport)
#endif

#include <windows.h>
#include "../4DyuchiGRX_common/typedef.h"


enum	SOUND_3D_DRIVER
{
	SOUND_DRIVER_TYPE_NONE,
	SOUND_DRIVER_TYPE_EAX3,
	SOUND_DRIVER_TYPE_EAX2,
	SOUND_DRIVER_TYPE_EAX1,
	SOUND_DRIVER_TYPE_DS3DHW,
	SOUND_DRIVER_TYPE_DS3DSW,
	SOUND_DRIVER_TYPE_MILES,
	SOUND_DRIVER_TYPE_DOLBY
};

enum	SOUND_SPEAKER_TYPE
{
	SOUND_SPEAKER_TYPE_STEREO,			// ÀÏ¹ÝÀûÀÎ 2Ã¼³Î.
	SOUND_SPEAKER_TYPE_HEADPHONE,		// ÇìµåÆù, »ç¿îµåÄ«µåÀÇ Á¾·ù¿¡ µû¶ó ÇìµåÆù ¿¥ÇÁµîÀÌ ÀÛµ¿ÇÑ´Ù.
	SOUND_SPEAKER_TYPE_4CH,				// 4Ã¼³Î Æ÷Áö¼Å´×.
	SOUND_SPEAKER_TYPE_6CH,				// 5.1Ã¼³Î Æ÷Áö¼Å´×.
	SOUND_SPEAKER_TYPE_8CH,				// 7.1Ã¼³Î Æ÷Áö¼Å´×. ÇÁ·ÎµðÁö7.1 µî¿¡¼­ Áö¿øµÈ´Ù.
	SOUND_SPEAKER_TYPE_UNKNOWN			// miles¿¡¼­ ¼­¶ó¿îµåÇüÀÌ¶õ°É Áö¿øÇÏ´Âµ¥ ¹ºÁö ¸ð¸§. 
};

typedef		void*	SOUND_FILE_HANDLE;

// IMilesSoundLib::GetLastError ·Î ¸®ÅÏµÇ´Â ¿¡·¯°ª.
enum SOUND_ERROR_CODE
{
	SOUND_ERROR_NOERROR,
	SOUND_ERROR_ALREADY_INITIALIZED,
	SOUND_ERROR_INVALID_ARGUMENT,
	SOUND_ERROR_MILES_STARTUP_FAILED,
	SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,			// InitÁß »ç¿îµå Ä«µå°¡ ¾øÀ» °æ¿ì ÀÌ ¸Þ½ÃÁö¸¦ ³»°í Æ¢¾î³ª¿Â´Ù.
	SOUND_ERROR_3DDRIVER_STARTUP_FAILED,			// InitÁß 3D µå¶óÀÌ¹ö¸¦ ¸øÃ£À¸¸é ¿¡·¯¸¦ ³½´Ù. ÀÌ·² °æ¿ì°¡ ÀÖÀ»±î?
	SOUND_ERROR_BGMDRIVER_NOT_LOADED,
	SOUND_ERROR_UNABLE_OPEN_FILE,
	SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
	SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
};

struct	ISoundEffect
{
	virtual	void	_stdcall	Play( BOOL bLoop)=0;
	/*
		»ç¿îµå Àç»ý.
		
		BOOL	bLoop     ÀÌ°ÍÀÌ TRUE¸é ·çÇÁ¸¦ µ·´Ù.

		¼³Á¤µÈ À§Ä¡, ¼³Á¤µÈ º¼·ý, ¼³Á¤µÈ max,minÀ» ¹ÙÅÁÀ¸·Î ¼Ò¸®¸¦ Àç»ýÇÑ´Ù.
		³»ºÎÀûÀ¸·Î´Â Play ÇÏ´Â ¼ø°£ MilesÀÇ »ùÇÃ°ú ÆÄÀÏÀ» ¿¬°á½ÃÅ°°í start ÇÑ´Ù.
	*/
	virtual void	_stdcall	Stop(void)=0;
	/*
		¼Ò¸®¸¦ Á¤Áö½ÃÅ²´Ù.
		³»ºÎÀûÀ¸·Î´Â Stop ÇÏ´Â ¼ø°£ ¼Ò¸®¸¦ end ½ÃÅ°°í MilesÀÇ »ùÇÃÀ» ¸±¸®Áî ÇÑ´Ù.
	*/

	virtual	void	_stdcall	SetMaxMinDistance( float fMaxDist, float fMinDist)=0;
	/*
		¼Ò¸®°¡ µé¸®´Â ÃÖ´ë °Å¸®, ÃÖ¼Ò °Å¸®¸¦ ³Ö´Â´Ù.

		float fMaxDist			// ÃÖ´ë °Å¸®. 
		float fMinDist			// ÃÖ¼Ò °Å¸®. °Å¸® 0ºÎÅÍ ÀÌ °Å¸®±îÁö´Â º¼·ýÀÌ ÁÙ¾îµéÁö ¾Ê´Â´Ù.

		fMinDist¿Í fMaxDist´Â °øºÎ¸¦ Á» ÇÏ°Å³ª ½ÃÇàÂø¿À¸¦ °Þ¾î°¡¸é¼­ ¼¼ÆÃ ÇØ¾ßÇÒ µí.
	*/

	virtual	void	_stdcall	SetPosition( VECTOR3* pPosition)=0;
	/*
		ÀÌÆåÆ®ÀÇ À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.

		VECTOR3* pPosition			¿ùµå¿¡¼­ÀÇ ÀÌÆåÆ®ÀÇ ÁÂÇ¥¸¦ ¼³Á¤ÇÑ´Ù.

		SetPositionÀ» ÇÒ °æ¿ì, IMilesSoundLib::SetListener ¿¡¼­ ¼³Á¤ÇÑ Ä«¸Þ¶ó¸¦ Âü°íÇØ¼­ Ä«¸Þ¶ó¿¡ »ó´ëÀûÀÎ À§Ä¡¸¦ ¼¼ÆÃ ÇÑ´Ù.
	*/

	virtual	void	_stdcall	SetRelativePosition( VECTOR3* pPosition)=0;
	/*

		ÀÌÆåÆ®ÀÇ »ó´ëÀû À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.


		VECTOR3* pPosition			Ä«¸Þ¶ó¿¡¼­ÀÇ »ó´ë À§Ä¡.

		SetPosition()°ú ´Ù¸£°Ô ÀÌ°ÍÀº Ä«¸Þ¶ó¸¦ 0,0,0¿¡ À§Ä¡½ÃÅ² »óÅÂ·Î °¡Á¤ÇÏ°í ±× Àý´ë°ªÀ» ¼¼ÆÃÇÏ´Â È¿°ú¸¦ ÁØ´Ù.
	*/

	virtual	void	_stdcall	SetVolume( float fVolume)=0;
	/*
		º¼·ýÀ» ¼³Á¤ÇÑ´Ù.

		float fVolume			0.0f ºÎÅÍ 1.0f ±îÁö º¼·ýÀ» ¼³Á¤ÇÒ ¼ö ÀÖ´Ù.
	*/
	virtual	float	_stdcall	GetVolume(void)=0;
	/*
		º¼·ýÀ» ¾ò´Â´Ù.
	*/

	virtual	BOOL	_stdcall	IsAlive(void)=0;
	/*

		»ç¿îµå°¡ Àç»ýµÇ°í sampleÀ» Àâ°í ÀÖ´Â°¡¸¦ ¸®ÅÏÇÑ´Ù.
	*/

	virtual	BOOL	_stdcall	IsLoop(void)=0;
	/*
		·çÇÁ ÇÃ·¢ÀÌ ›�?µÇ¾îÀÖ´Â°¡.?
	*/

	virtual	DWORD	_stdcall	AddRef()=0;
	virtual	DWORD	_stdcall	Release()=0;
	/*

		·¹ÆÛ·±½º Ä«¿îÅÍ¸¦ »«´Ù.
		ÀÏ¹ÝÀûÀ¸·Î Release() ÇÏ´Â ¼ø°£ ·¹ÆÛ·±½ºÄ«¿îÅÍ°¡ 0ÀÌ µÇ¸é¼­ ¸Þ¸ð¸®¿¡¼­ »ç¶óÁø´Ù.
	*/
};

struct	IMilesSoundLib
{
	virtual	SOUND_ERROR_CODE _stdcall Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware) = 0;// »ç¿îµå ¸Þ´ÏÀú ÃÊ±âÈ­.
	/*
		IMilesSoundLibÀ» ÃÊ±âÈ­ ÇÑ´Ù.

		DWORD	dwMaxSoundFileCount´Â ÃÖ´ëÇÑ ¿Ã·Á ³õÀ» ¼ö ÀÖ´Â wav, mp3ÆÄÀÏÀÇ °¹¼ö,
		DWORD	dwMaxSoundEffectCount´Â °ÔÀÓ °´Ã¼°¡ ¼Ò¸®¸¦ ³¾ ¼Ò¸® °´Ã¼ ÃÖ´ë °¹¼ö.
		char*	szMilesPass´Â ¸¶ÀÏÁî°¡ ±ò·ÁÀÖ´Â µð·ºÅä¸® ±âº»ÀûÀ¸·Î "redist"ÀÌÁö¸¸ "miles"·Î ¿Å±â±æ..
		BOOL	bTryEAX´Â 3D µå¶óÀÌ¹ö¸¦ ÀâÀ» ¶§ eax¸¦ ½Ãµµ ÇÏ´Â°¡ ¿©ºÎ¸¦ ¾Ë¸°´Ù.
		BOOL	bForceSoftware´Â ¼ÒÇÁÆ®¿þ¾îµå¶óÀÌ¹ö¸¦ °­Á¦·Î ½ÃµµÇÑ´Ù. ´ëºÎºÐÀÇ °æ¿ì ¼º°øÇÒ °ÍÀÌ°í, ÀÌ¶§ bTryEAX´Â ¹«½ÃµÈ´Ù.
		¸®ÅÏ°ªÀº º° ¹®Á¦ ¾øÀÌ ¼º°øÇÏ¸é SOUND_ERROR_NOERROR,
		Init°¡ µÎ¹øÂ° µÇ´Â °æ¿ì SOUND_ERROR_ALREADY_INITIALIZED,
		µé¾î¿Â ÀÎÀÚ°¡ ÀÌ»óÇÒ °æ¿ì SOUND_ERROR_INVALID_ARGUMENT,
		¸¶ÀÏÁî ÃÊ±âÈ­¿¡ ½ÇÆÐÇÏ¸é SOUND_ERROR_MILES_STARTUP_FAILED,
		BGMµå¶óÀÌ¹ö¸¦ Ã£Áö ¸øÇÒ °æ¿ì, »ç¿îµåÄ«µå°¡ ¾ø´Ù¸é SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,
		3D »ç¿îµå µå¶óÀÌ¹ö¸¦ ¿Ã¸®Áö ¸øÇÏ¸é SOUND_ERROR_3DDRIVER_STARTUP_FAILED
		¸¦ “Ê¾î³½´�?
		ÀÌÁß »ç¿îµåÄ«µå°¡ ¾øÀ»°æ¿ì ¸¦ ´ëºñÇØ SOUND_ERROR_BGMDRIVER_STARTUP_FAILED´Â Ã³¸®ÇØÁÖ°í,
		³ª¸ÓÁö ¿¡·¯´Â ÇÁ·Î±×·¡¸Ó¸¦ Ã£µµ·Ï. -_-;
	*/

	virtual	SOUND_3D_DRIVER		_stdcall	Get3DDriver(void)=0;
	/*
		ÇöÀç ÀâÇôÀÖ´Â 3D µå¶óÀÌ¹ö¸¦ ¸®ÅÏÇÑ´Ù.
	*/

	virtual	void				_stdcall	SetListener( VECTOR3* pPosition, VECTOR3* pAngle)=0;
	/*
		Ã»ÀÚÀÇ À§Ä¡¿Í º¸´Â °¢À» °áÁ¤ÇÑ´Ù.
		VECTOR3* pPosition		 Ã»ÀÚÀÇ À§Ä¡.
		VECTOR3* pAngle			Ã»ÀÚ°¡ º¸´Â °¢µµ, radius ´ÜÀ§ÀÌ´Ù.

		ÇÑ °ÔÀÓ ÇÁ·¹ÀÓ ³»¿¡¼­ ´Ù¸¥ ISoundEffect¿¡ SetPositionÇÏ±â Àü¿¡ ¸ÕÀú ¼¼ÆÃ µÇ´Â °ÍÀÌ ¸ÖÂÄÇÏ´Ù.
	*/

	virtual	SOUND_SPEAKER_TYPE	_stdcall Get3DSpeakerType(void)=0;
	/*
		ÀâÇôÀÖ´Â 3D driver°¡ »ç¿ëÇÏ´Â ½ºÇÇÄ¿ ¹èÄ¡ÀÇ ÇüÅÂ¸¦ ¾Ë¾Æ³½´Ù.
		ÀÌ´Â Á¦¾îÆÇ/»ç¿îµå ÀÇ °í±ÞÁ¤º¸¿¡ ÀÖ´Â ½ºÇÇÄ¿ ¹èÄ¡¿Í °°À»µí.
	*/

	virtual	void				_stdcall Set3DSpeakerType( SOUND_SPEAKER_TYPE Type)=0;
	/*
		3D driver°¡ »ç¿ëÇÏ´Â ½ºÇÇÄ¿ Å¸ÀÔÀ» °­Á¦·Î ¼³Á¤ÇÑ´Ù.
	*/

	// BGM
	virtual	SOUND_ERROR_CODE _stdcall PlayBGM( char* szBGMFileName)=0;
	/*
		szBGMFileNameÀ¸·Î ¸î½ÃµÈ ÆÄÀÏÀ» ¹é±×¶ó¿îµå·Î ÇÃ·¹ÀÌ ÇÑ´Ù.
		
		char* szBGMFileName		¿¡´Â BGMÆÄÀÏ ÀÌ¸§À» ³Ö´Â´Ù. .wav, .mp3µîÀÌ °¡´ÉÇÏ´Ù.

		¼º°øÇÒ °æ¿ì SOUND_ERROR_NOERROR,
		Init°¡ ¼º°øÇÏÁö ¸øÇÑ °æ¿ì SOUND_ERROR_BGMDRIVER_NOT_LOADED À» ¸®ÅÏÇÏ°í ¾Æ¹«Áþµµ ¾ÈÇÑ´Ù.
		ÆÄÀÏ ÀÌ¸§ÀÌ Àß¸ø‰çÀ» °æ¿ì SOUND_ERROR_UNABLE_OPEN_FILE
		À» ¸®ÅÏÇÑ´Ù.

	*/

	virtual	void			 _stdcall StopBGM(void)=0;
	/*
		ÇÃ·¹ÀÌ µÇ°í ÀÖ´Â BGMÀ» Á¤ÁöÇÑ´Ù.
		Init°¡ ¼º°øÇÏÁö ¾ÈÀº »óÅÂ¿¡¼­ È£ÃâµÇµµ »ó°ü ¾ø´Ù. 
	*/

	virtual void			_stdcall SetBGMVolume( float fVolume)=0;
	/*
	 *	BGMÀÇ º¼·ýÀ» ¼³Á¤ÇÑ´Ù. ¹üÀ§´Â 1.0f ¿¡¼­ 0.0f±îÁö.
	 */

	// Sound Effect File
	virtual SOUND_ERROR_CODE _stdcall CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char* szFileName)=0;
	/*
		»ç¿îµå ÆÄÀÏ¿¡ ´ëÇÑ ÇÚµéÀ» ÇÏ³ª ¸¸µç´Ù. »ý¼º½ÃÀÇ ¼Óµµ¸¦ À§ÇØ »ç¿îµå ÀÌÆåÆ®´Â ÆÄÀÏ ÀÌ¸§À¸·Î ¹Ù·Î »ý¼ºµÇÁö ¾ÈÀ¸¹Ç·Î
		ÆÄÀÏ ÇÚµéÀ» ÇÏ³ª ¸¸µé°í ÀÌ ÇÚµé·Î »ç¿îµå ÀÌÆåÆ®¸¦ ¸¸µç´Ù.

		SOUND_FILE_HANDLE* pOutHandle		»ç¿îµå ÆÄÀÏ ÇÚµéÀ» ¹ÞÀ» Æ÷ÀÎÅÍ.
		char* szFileName					ÇÒ´çÇÒ ÆÄÀÏ ÀÌ¸§.

		¼º°øÇÒ °æ¿ì SOUND_ERROR_NOERROR,
		BGMµå¶óÀÌ¹ö°¡ ¿Ã¶ó°¡Áö ¸øÇÑ °æ¿ì(»ç¿îµåÄ«µå°¡ ¾ø´Â °æ¿ì µî..) SOUND_ERROR_BGMDRIVER_NOT_LOADED,
		ÁÖ¾îÁø ÀÎÀÚ¿¡ ¹®Á¦°¡ ÀÖÀ» °æ¿ì SOUND_ERROR_INVALID_ARGUMENT,
		ÃÖ´ë ÆÄÀÏ °³Ã¼ °¹¼ö¸¦ ÃÊ°úÇÒ °æ¿ì SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
		ÁÖ¾îÁø ÆÄÀÏÀ» ¿­Áö ¸øÇßÀ» °æ¿ì SOUND_ERROR_UNABLE_OPEN_FILE
		À» ¸®ÅÏÇÑ´Ù.

		³»ºÎÀûÀ¸·Î ¸®¼Ò½º °øÀ¯°¡ µÇ¹Ç·Î °°Àº ÆÄÀÏÀÌ¸§¿¡ ´ëÇØ Creat¸¦ ÇÏ¸é ¹Ì¸® ¹ß±Þ µÇ¾ú´ø ÇÚµéÀ» ¸®ÅÏÇÏ°í,
		·¹ÆÛ·±½º Ä«¿îÅÍ°¡ 1 ¿Ã¶ó°£´Ù.
	*/

	virtual	void			 _stdcall ReleaseSoundEffectFile( SOUND_FILE_HANDLE hFile)=0;
	/*
		hFile·Î ÁÖ¾îÁø ÆÄÀÏÀ» ¸±¸®Áî ÇÑ´Ù.
		
		ÇÏ³ªÀÇ ÆÄÀÏ¿¡ ´ëÇØ ÇÚµéÀÌ ¿©·¯°³ ¹ß±ÞµÉ °ÍÀÌ¹Ç·Î, ³»ºÎÀûÀ¸·Î hFileÀÇ ·¹ÆÛ·±½º Ä«¿îÅÍ¸¦ ÇÏ³ª ¶³¾î¶ß¸°´Ù.
		·¹ÆÛ·±½º Ä«¿îÅÍ°¡ 0ÀÌ µÇ´Â ¼ø°£ ½ÇÁ¦ ¸Þ¸ð¸®¿¡¼­ ¾ø¾îÁø´Ù.
	*/

	virtual	SOUND_ERROR_CODE _stdcall CreateSoundEffect( ISoundEffect** ppSoundEffect, SOUND_FILE_HANDLE hSound)=0;
	/*
		hSound ÆÄÀÏ ÇÚµéÀ» ÇÃ·¹ÀÌ ÇÏ´Â »ç¿îµå °´Ã¼¸¦ ¸¸µç´Ù.

		¸¸µå´Â ¼ø°£ ÇÃ·¹ÀÌ°¡ µÇ´Â °ÍÀº ¾Æ´Ï°í ¸¸µç ÈÄ ¹ÞÀº ÀÎÅÍÆäÀÌ½º·Î Play ÇØÁà¾ß ÇÑ´Ù.

		ISoundEffect** ppSoundEffect		ÀÎÅÍÆäÀÌ½º Æ÷ÀÎÅÍ.
		SOUND_FILE_HANDLE hSound			¸¸µé·Á´Â »ç¿îµåÀÇ ÆÄÀÏ ÇÚµé.


		¼º°øÇÏ¸é SOUND_ERROR_NOERROR,
		ÀÎÀÚ·Î ÁÖ¾îÁø hSound°¡ ¾öÇÑ °ªÀÌ¸é SOUND_ERROR_INVALID_ARGUMENT,
		Init¶§ ÁÖ¾îÁø ÃÖ´ë ÀÌÆåÆ® °¹¼ö¸¦ ÃÊ°úÇÏ¸é SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
		¸¦ ¸®ÅÏÇÑ´Ù.

		¹ÞÀº ISoundEffect °´Ã¼´Â Áö¿ï¶§ ISoundEffect->Release()¸¦ »ç¿ëÇÑ´Ù.
	*/


	virtual	void			_stdcall	Run()=0;

	virtual	DWORD			_stdcall	GetAllocatedSoundEffectCount()=0;
	/*
		ÇöÀç ÀâÇôÀÖ´Â »ç¿îµå ÀÌÆÑÆ®ÀÇ °¹¼ö¸¦ ¸®ÅÏÇÑ´Ù.

		µð¹ö±×¿ë.
	*/
	virtual	DWORD			_stdcall	GetAllocatedSoundFileCount()=0;
	/*
		ÇöÀç ÀâÇôÀÖ´Â »ç¿îµå ÆÄÀÏÀÇ °¹¼ö¸¦ ¸®ÅÏÇÑ´Ù.

		µð¹ö±×¿ë.
	*/

	
	virtual	DWORD			_stdcall	ReleaseAllSoundEffect(void)=0;
	/*
		ÀâÇôÀÖ´ø ¸ðµç Effect¸¦ Áö¿î´Ù.

		ÀÌ½ÃÁ¡±îÁö ¸±¸®Áî µÇÁö ¾ÊÀº Sound EffectÀÇ °¹¼ö°¡ ¸®ÅÏµÈ´Ù.

		À¢¸¸ÇÏ¸é ÀÌ ÇÔ¼ö ¾²Áö ¸»°í ±×¶§±×¶§ ÇÏ³ªÇÏ³ª Á÷Á¢ Áö¿öÁÙ °ÍÀ» ÃßÃµÇÑ´Ù.
	*/

	virtual	DWORD			_stdcall	ReleaseAllSoundFile(void)=0;
	/*
		ÀâÇôÀÖ´ø ¸ðµç FileÀ» Áö¿î´Ù.

		ÀÌ½ÃÁ¡±îÁö ¸±¸®Áî µÇÁö ¾ÊÀº Sound Effect FileÀÇ °¹¼ö°¡ ¸®ÅÏµÈ´Ù.

		À¢¸¸ÇÏ¸é ÀÌ ÇÔ¼ö ¾²Áö ¸»°í ±×¶§±×¶§ ÇÏ³ªÇÏ³ª Á÷Á¢ Áö¿öÁÙ °ÍÀ» ÃßÃµÇÑ´Ù.
	*/

	virtual	void _stdcall Delete() = 0;
	/*
		ISoundLibÀÇ ¼Ò¸êÀÚ.

		¸®¼Ò½º¸¦ Á¦´ë·Î ÇØÁ¦ÇÏÁö ¾ÊÀº»óÅÂ¿¡¼­ È£ÃâµÇ¸é ¹«½Ã¹«½ÃÇÑ ÀÎÅÍ·´Æ®µéÀÌ °É¸± °ÍÀÌ¸®.
	*/

	virtual	void _stdcall GetErrorString( char** pszErrorString, SOUND_ERROR_CODE)=0;
	/*
		SOUND_ERROR_NOERROR ¿ÜÀÇ ¿¡·¯°ªÀ» ¹Þ¾ÒÀ»¶§ ±×°ÍµéÀ» ¹®ÀÚ¿­·Î Ä¡È¯ÇØÁØ´Ù.
		µð¹ö±ë¿ë ÇÔ¼ö.
	*/

	virtual	void	_stdcall	SetMaxVoiceCount( DWORD dwMaxVoice)=0;
	virtual	DWORD	_stdcall	GetMaxVoiceCount()=0;

};


SOUNDLIB_API BOOL CreateSoundLib( IMilesSoundLib** ppISoundLib);



#endif


