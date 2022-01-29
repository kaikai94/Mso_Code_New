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
		ASSERTMSG(0,"��������δ��ʼ��");
		return FALSE;
	}
	ret=VoiceEngine->SetAppInfo("1119048749", "fe2fefd5b43ea8d14cdc562bbb21ad31",buf);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"��������������Ϣʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->Init();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"���������ʼ��ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->SetNotify(this);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		
		sprintf(msg,"�����ص�ע��ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	ret=VoiceEngine->SetMode(gcloud_voice::IGCloudVoiceEngine::GCloudVoiceMode::RealTime);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		sprintf(msg,"����ģʽ����ʧ�ܣ�ErrNum:%d",ret);
		ASSERTMSG(0,msg);
		return FALSE;
	}
	return TRUE;
}

void OnlineVoiceManager::OnJoinRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName, int memberID)
{//[���뷿��ص�][By:ʮ���½���][QQ:112582793][2019/3/13][9:56:57]

	char msg[512] = {0,};
	m_bInit_ = TRUE;
	ret = VoiceEngine->OpenMic();
	if (ret != gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		m_bInit_ = FALSE;
		sprintf(msg,"��˷翪��ʧ�ܣ�ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
	ret = VoiceEngine->OpenSpeaker();
	if (ret != gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		m_bInit_ = FALSE;
		sprintf(msg,"����������ʧ�ܣ�ErrNum:%d",ret);
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
{//[���߻ص�][By:ʮ���½���][QQ:112582793][2019/3/13][9:57:06]
	dwPartyID_ = 0;
	dwMemberID_ = 0;
	m_CurRoomName = "";
	m_bInit_ = FALSE;
	GAMEIN->GetOnlineVoiceDlg()->UpdateDailog();
}
void OnlineVoiceManager::OnQuitRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName)
{//[�˳�����ص�][By:ʮ���½���][QQ:112582793][2019/3/13][9:57:16]
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
{//[��Ա˵���ص�][By:ʮ���½���][QQ:112582793][2019/3/13][9:57:25]
	PARTYMGR->SetMemberVoiceBtn(*members);
}
void OnlineVoiceManager::OnMemberVoice(const char *roomName, unsigned int member, int status)
{//[��Ա˵���ص�][By:ʮ���½���][QQ:112582793][2019/3/13][9:57:33]

}

void OnlineVoiceManager::CloseMic()
{//[�ر��Լ���˷�][By:ʮ���½���][QQ:112582793][2019/3/13][9:42:08]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->CloseMic();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�ر���˷�ʧ�ܣ�ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpenMic()
{//[���Լ���˷�][By:ʮ���½���][QQ:112582793][2019/3/13][9:42:19]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->OpenMic();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"����˷�ʧ�ܣ�ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetMicVolume(int volume)
{//[������˷�����][By:ʮ���½���][QQ:112582793][2019/3/13][16:42:32]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->SetMicVolume(volume);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"������˷�������������ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
int	 OnlineVoiceManager::GetMicLevel(bool fadeOut)
{//[��ȡ��˷�����][By:ʮ���½���][QQ:112582793][2019/3/13][16:42:23]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return 0;
	return VoiceEngine->GetMicLevel(fadeOut);
}
void OnlineVoiceManager::CloseSomeOneMic(DWORD CharID)
{//[�ر�ĳ��Ա��˷�][By:ʮ���½���][QQ:112582793][2019/3/13][9:41:43]
	if(!VoiceEngine||dwMemberID_==0||m_CurRoomName.length()<=0) return;

	ret=VoiceEngine->ForbidMemberVoice(CharID,true,m_CurRoomName.c_str());
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�رճ�Ա��˷�ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpneSomeOneMic(DWORD CharID)
{//[��ĳ��Ա��˷�][By:ʮ���½���][QQ:112582793][2019/3/13][9:41:59]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->ForbidMemberVoice(CharID,false,m_CurRoomName.c_str());
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�򿪳�Ա��˷�ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::JoinRoom()
{//[��������������][By:ʮ���½���][QQ:112582793][2019/3/13][9:44:45]

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
		sprintf(msg,"����������ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::QuiteRoom()
{//[�˳�����������][By:ʮ���½���][QQ:112582793][2019/3/13][9:54:03]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->QuitRoom(m_CurRoomName.c_str(),10000);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�˳�������ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::CloseSpeaker()
{//[�ر�������][By:ʮ���½���][QQ:112582793][2019/3/13][16:41:24]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->CloseSpeaker();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�ر�������ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::OpenSpeaker()
{//[��������][By:ʮ���½���][QQ:112582793][2019/3/13][16:41:34]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->OpenSpeaker();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"��������ʧ��!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetSpeakerVolume(int volume)
{//[��������������][By:ʮ���½���][QQ:112582793][2019/3/13][16:41:40]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret = VoiceEngine->SetSpeakerVolume(volume);
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"��������������ʧ�ܣ�ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
int OnlineVoiceManager::GetSpeakerVolume()
{//[��ȡ����������][By:ʮ���½���][QQ:112582793][2019/3/13][16:41:50]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return 0;
	return VoiceEngine->GetSpeakerLevel();
}
void OnlineVoiceManager::Poll()
{//[����ʱ��ѭ��][By:ʮ���½���][QQ:112582793][2019/3/13][16:41:59]
	if(!VoiceEngine||dwPartyID_==0||m_CurRoomName.length()<=0) return;
	ret=VoiceEngine->Poll();
	if(ret!=gcloud_voice::GCloudVoiceErrno::GCLOUD_VOICE_SUCC)
	{
		char msg[512] = {0,};
		sprintf(msg,"�������̳���!��ErrNum:%d",ret);
		ASSERTMSG(0,msg);
	}
}
void OnlineVoiceManager::SetVoiceRoomInfo(DWORD PartyID)
{//[���������¼����ID������ͬ�ķ����][By:ʮ���½���][QQ:112582793][2019/3/21][17:01:13]
	dwPartyID_ = PartyID;
}
#endif