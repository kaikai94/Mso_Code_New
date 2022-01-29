#include "stdafx.h"
#include "cOnlineVoiceDlg.h"
#include "WindowIDEnum.h"
#include "OnlineVoiceManager.h"
#include "./interface/cGuageBar.h"
#include "./interface/cButton.h"
#include "./interface/cCheckBox.h"
#include "ChatManager.h"
cOnlineVoiceDlg::cOnlineVoiceDlg(void)
{
	m_MicVoice		= NULL;
	m_SpeakVoice	= NULL;
	m_JoinRoom		= NULL;
	m_QuitRoom		= NULL;
	m_MicCheck		= NULL;
	m_SpkCheck		= NULL;
}

cOnlineVoiceDlg::~cOnlineVoiceDlg(void)
{
}
void cOnlineVoiceDlg::Linking()
{
#ifdef _XDYY_
	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);

	m_MicVoice		= (cGuageBar*)GetWindowForID(OV_TEAMVOICE_SET_MIC_VOICE);
	m_SpeakVoice	= (cGuageBar*)GetWindowForID(OV_TEAMVOICE_SET_SPEAK_VOICE);
	m_JoinRoom		= (cButton*)GetWindowForID(OV_TEAMVOICE_JION_BTN);
	m_QuitRoom		= (cButton*)GetWindowForID(OV_TEAMVOICE_QUIT_BTN);
	m_MicCheck		= (cCheckBox*)GetWindowForID(OV_TEAMVOICE_OPEN_MIC_CHECK);
	m_SpkCheck		= (cCheckBox*)GetWindowForID(OV_TEAMVOICE_OPEN_SPEAK_CHECK);
	BOOL bIsInit = VOICEMGR->IsInint();
	if( !bIsInit )
	{
		FullColor = HalfColor;
	}
	m_MicVoice->SetImageRGB(FullColor);
	m_MicVoice->SetDisable(!bIsInit);
	m_MicVoice->SetGuageLock(!bIsInit,FullColor);
	
	m_SpeakVoice->SetImageRGB(FullColor);
	m_SpeakVoice->SetDisable(!bIsInit);
	m_SpeakVoice->SetGuageLock(!bIsInit,FullColor);
	
	m_SpeakVoice->SetDisable(!bIsInit);
	m_QuitRoom->SetDisable(!bIsInit);
	m_MicCheck->SetDisable(!bIsInit);
	m_SpkCheck->SetDisable(!bIsInit);
#endif
}
void cOnlineVoiceDlg::UpdateDailog()
{
#ifdef _XDYY_
	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);

	BOOL bIsInit = VOICEMGR->IsInint();
	if( !bIsInit )
	{
		FullColor = HalfColor;
	}
	m_MicVoice->SetImageRGB(FullColor);
	m_MicVoice->SetDisable(!bIsInit);
	m_MicVoice->SetGuageLock(!bIsInit,FullColor);

	m_SpeakVoice->SetImageRGB(FullColor);
	m_SpeakVoice->SetDisable(!bIsInit);
	m_SpeakVoice->SetGuageLock(!bIsInit,FullColor);

	m_QuitRoom->SetDisable(!bIsInit);
	m_MicCheck->SetDisable(!bIsInit);
	m_SpkCheck->SetDisable(!bIsInit);
	m_JoinRoom->SetDisable(bIsInit);

	m_MicCheck->SetChecked(bIsInit);
	m_SpkCheck->SetChecked(bIsInit);
	if(bIsInit)
	{
		m_MicVoice->SetCurValue(VOICEMGR->GetMicLevel());
		m_SpeakVoice->SetCurValue(VOICEMGR->GetSpeakerVolume());
	}
#endif
}
void cOnlineVoiceDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
#ifdef _XDYY_
	if(we &WE_BTNCLICK)
	{
		switch(lId)
		{
		case OV_TEAMVOICE_JION_BTN:	VOICEMGR->JoinRoom();	break;		//[加入聊天室][By:十里坡剑神][QQ:112582793][2019/3/20][19:15:24]
		case OV_TEAMVOICE_QUIT_BTN: VOICEMGR->QuiteRoom();	break;		//[退出聊天室][By:十里坡剑神][QQ:112582793][2019/3/20][19:15:31]
		}
	}
	else if(we&WE_CHECKED)
	{
		switch(lId)
		{
		case OV_TEAMVOICE_OPEN_MIC_CHECK:	VOICEMGR->OpenMic();		break;	//[麦克风开关][By:十里坡剑神][QQ:112582793][2019/3/20][19:16:00]
		case OV_TEAMVOICE_OPEN_SPEAK_CHECK:	VOICEMGR->OpenSpeaker();	break;	//[扬声器开关][By:十里坡剑神][QQ:112582793][2019/3/20][19:16:19]
		}
	}
	else if(we &WE_NOTCHECKED)
	{
		switch(lId)
		{
		case OV_TEAMVOICE_OPEN_MIC_CHECK:	VOICEMGR->CloseMic();	break;	//[麦克风开关][By:十里坡剑神][QQ:112582793][2019/3/20][19:16:00]
		case OV_TEAMVOICE_OPEN_SPEAK_CHECK:	VOICEMGR->CloseSpeaker();	break;	//[扬声器开关][By:十里坡剑神][QQ:112582793][2019/3/20][19:16:19]
		}
	}
#endif
}
void cOnlineVoiceDlg::OnVoiceSetEvent(LONG lId, void * p, DWORD we,long Value)
{
#ifdef _XDYY_
	if(!VOICEMGR->IsInint())
		return;
	switch(lId)
	{
		case OV_TEAMVOICE_SET_MIC_VOICE:	VOICEMGR->SetMicVolume(Value);		break;
		case OV_TEAMVOICE_SET_SPEAK_VOICE:	VOICEMGR->SetSpeakerVolume(Value);	break;
	}
#endif
}