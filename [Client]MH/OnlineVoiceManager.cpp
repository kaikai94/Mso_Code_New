#include "stdafx.h"
#include "OnlineVoiceManager.h"
#ifdef _XDYY_
#include "Win32\include\GCloudVoice.h"
#include "Win32\include\GCloudVoiceErrno.h"
#include "Win32\include\GCloudVoiceExtension.h"
#include "Win32\include\GCloudVoiceVersion.h"
#include "Win32\include\GVoice.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "cOnlineVoiceDlg.h"
#include "PartyManager.h"
OnlineVoiceManager* OnlineVoiceManager::g_Instance = NULL;
gcloud_voice::IGCloudVoiceEngine* OnlineVoiceManager::VoiceEngine= NULL;
OnlineVoiceManager::OnlineVoiceManager(void)
{
	dwPartyID_ = 0;
	dwMemberID_ = 0;
	m_CurRoomName = "";
	m_bInit_ = FALSE;
}

OnlineVoiceManager::~OnlineVoiceManager(void)
{
	if(VoiceEngine)
	{
		delete VoiceEngine;
		VoiceEngine = NULL;
	}
}
void OnlineVoiceManager::ClearVoice()
{
	dwPartyID_ = 0;
	dwMemberID_ = 0;
	m_CurRoomName = "";
	m_bInit_ = FALSE;
}
OnlineVoiceManager* OnlineVoiceManager::getinstance()
{
	if(!g_Instance)
		g_Instance = new OnlineVoiceManager;
	return g_Instance;
}
BOOL OnlineVoiceManager::InitVoice()
{
	char msg[512] = {0,};
	char buf[32]={0,};
	sprintf(buf,"%d",HEROID);
	if (VoiceEngine)
		delete VoiceEngine;
	VoiceEngine = gcloud_voice::GetVoiceEngine();
	if(!VoiceEngine)
	{
		ASSERTMSG(0,"语音引擎未初始化");
		return FALSE;
	}
	ret=VoiceEngine->SetAppInfo("1119048749", "fe2fefd5b43ea8d14cdc562bbb21ad31",buf);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"设置语音服务信息失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->Init();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"语音组件初始化失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->SetNotify(this);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		
		sprintf(msg,"语音回调注册失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->SetMode(gcloud_voice::IGCloudVoiceEngine::GCloudVoiceMode::RealTime);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"语音模式设置失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	return TRUE;
}

void OnlineVoiceManager::OnJoinRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName, int memberID)
{//[加入房间回调][By:十里坡剑神][QQ:112582793][2019/3/13][9:56:57]

	char msg[512] = {0,};
	m_bInit_ = TRUE;
	ret = VoiceEngine->OpenMic();
	if (ret != gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		m_bInit_ = FALSE;
		sprintf(msg,"麦克风开启失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
	ret = VoiceEngine->OpenSpeaker();
	if (ret != gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		m_bInit_ = FALSE;
		sprintf(msg,"扬声器开启失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
	if(m_bInit_)
	{
		dwMemberID_ = memberID;
		MSG_DWORD3 msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_VOICE_ADD;
		msg.dwObjectID = HEROID;
		msg.dwData1 = memberID;
		msg.dwData2 = dwPartyID_;
		msg.dwData3 = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2330));
		GAMEIN->GetOnlineVoiceDlg()->UpdateDailog();
	}
	else
	{
		dwPartyID_ = 0;
		dwMemberID_ = 0;
		m_CurRoomName = "";
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2334));
	}
}
void OnlineVoiceManager::OnStatusUpdate(gcloud_voice::GCloudVoiceCompleteCode status, const char *roomName, int memberID)
{//[掉线回调][By:十里坡剑神][QQ:112582793][2019/3/13][9:57:06]
	dwPartyID_ = 0;
	dwMemberID_ = 0;
	m_CurRoomName = "";
	m_bInit_ = FALSE;
	GAMEIN->GetOnlineVoiceDlg()->UpdateDailog();
}
void OnlineVoiceManager::OnQuitRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName)
{//[退出房间回调][By:十里坡剑神][QQ:112582793][2019/3/13][9:57:16]
	MSG_DWORD3 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_VOICE_QUIT;
	msg.dwObjectID = HEROID;
	msg.dwData1 = dwMemberID_;
	msg.dwData2 = dwPartyID_;
	msg.dwData3 = HEROID;
	NETWORK->Send(&msg,sizeof(msg));

	CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2331));
	dwMemberID_ = 0;
	m_bInit_ = FALSE;
	GAMEIN->GetOnlineVoiceDlg()->UpdateDailog();
}
void OnlineVoiceManager::OnMemberVoice(const unsigned int *members, int count)
{//[成员说话回调][By:十里坡剑神][QQ:112582793][2019/3/13][9:57:25]
	PARTYMGR->SetMemberVoiceBtn(*members);
}
void OnlineVoiceManager::OnMemberVoice(const char *roomName, unsigned int member, int status)
{//[成员说话回调][By:十里坡剑神][QQ:112582793][2019/3/13][9:57:33]

}

void OnlineVoiceManager::CloseMic()
{//[关闭自己麦克风][By:十里坡剑神][QQ:112582793][2019/3/13][9:42:08]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->CloseMic();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"关闭麦克风失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpenMic()
{//[打开自己麦克风][By:十里坡剑神][QQ:112582793][2019/3/13][9:42:19]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->OpenMic();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"打开麦克风失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetMicVolume(int volume)
{//[设置麦克风音量][By:十里坡剑神][QQ:112582793][2019/3/13][16:42:32]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->SetMicVolume(volume);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"设置麦克风音量发生错误，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
int	 OnlineVoiceManager::GetMicLevel(bool fadeOut)
{//[获取麦克风音量][By:十里坡剑神][QQ:112582793][2019/3/13][16:42:23]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return 0;
	return VoiceEngine->GetMicLevel(fadeOut);
}
void OnlineVoiceManager::CloseSomeOneMic(DWORD CharID)
{//[关闭某成员麦克风][By:十里坡剑神][QQ:112582793][2019/3/13][9:41:43]
	if(!VoiceEngine||dwMemberID_==0||m_CurRoomName.length()<=0) return;

	ret=VoiceEngine->ForbidMemberVoice(CharID,true,m_CurRoomName.c_str());
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"关闭成员麦克风失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpneSomeOneMic(DWORD CharID)
{//[打开某成员麦克风][By:十里坡剑神][QQ:112582793][2019/3/13][9:41:59]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->ForbidMemberVoice(CharID,false,m_CurRoomName.c_str());
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"打开成员麦克风失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::JoinRoom()
{//[加入语音聊天室][By:十里坡剑神][QQ:112582793][2019/3/13][9:44:45]

	if(dwPartyID_==0) return;

	char buf[8]={0,};
	sprintf(buf,"%d",dwPartyID_);
	m_CurRoomName = "VoiceRoom_";
	m_CurRoomName += buf;

	if(!VoiceEngine)
		if(!InitVoice()) return;
	ret=VoiceEngine->JoinTeamRoom(m_CurRoomName.c_str(),10000);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"加入聊天室失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::QuiteRoom()
{//[退出语音聊天室][By:十里坡剑神][QQ:112582793][2019/3/13][9:54:03]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->QuitRoom(m_CurRoomName.c_str(),10000);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"退出聊天室失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::CloseSpeaker()
{//[关闭扬声器][By:十里坡剑神][QQ:112582793][2019/3/13][16:41:24]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->CloseSpeaker();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"关闭扬声器失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpenSpeaker()
{//[打开扬声器][By:十里坡剑神][QQ:112582793][2019/3/13][16:41:34]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->OpenSpeaker();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"打开扬声器失败!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetSpeakerVolume(int volume)
{//[设置扬声器音量][By:十里坡剑神][QQ:112582793][2019/3/13][16:41:40]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->SetSpeakerVolume(volume);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"扬声器音量设置失败，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
int OnlineVoiceManager::GetSpeakerVolume()
{//[获取扬声器音量][By:十里坡剑神][QQ:112582793][2019/3/13][16:41:50]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return 0;
	return VoiceEngine->GetSpeakerLevel();
}
void OnlineVoiceManager::Poll()
{//[语音时间循环][By:十里坡剑神][QQ:112582793][2019/3/13][16:41:59]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->Poll();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"语音进程出错!，ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetVoiceRoomInfo(DWORD PartyID)
{//[加入队伍后记录队伍ID生成相同的房间号][By:十里坡剑神][QQ:112582793][2019/3/21][17:01:13]
	dwPartyID_ = PartyID;
}
#endif