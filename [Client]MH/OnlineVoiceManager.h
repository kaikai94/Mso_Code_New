//#pragma once
#ifdef _XDYY_
#include "GCloudVoice.h"


class OnlineVoiceManager :
	public gcloud_voice::IGCloudVoiceNotify
{
	std::string m_CurRoomName;
	static gcloud_voice::IGCloudVoiceEngine* VoiceEngine;
	static OnlineVoiceManager* g_Instance;
	DWORD dwPartyID_;
	DWORD dwMemberID_;
	gcloud_voice::GCloudVoiceErrno ret;
	BOOL m_bInit_;
public:
	OnlineVoiceManager(void);
	virtual ~OnlineVoiceManager(void);

	static OnlineVoiceManager* getinstance();
	virtual void OnJoinRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName, int memberID);
	virtual void OnStatusUpdate(gcloud_voice::GCloudVoiceCompleteCode status, const char *roomName, int memberID);
	virtual void OnQuitRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char *roomName);
	virtual void OnMemberVoice(const unsigned int *members, int count);
	virtual void OnMemberVoice(const char *roomName, unsigned int member, int status);

	BOOL InitVoice();

	void CloseMic();
	void OpenMic();
	void SetMicVolume(int volume);
	int	 GetMicLevel(bool fadeOut = true);

	void CloseSpeaker();
	void OpenSpeaker();

	void CloseSomeOneMic(DWORD CharID);
	void OpneSomeOneMic(DWORD CharID);
	void SetSpeakerVolume(int volume);
	int GetSpeakerVolume();

	void JoinRoom();
	void QuiteRoom();


	void Poll();
	void SetVoiceRoomInfo(DWORD PartyID);	
	BOOL IsInint(){return m_bInit_;}
	void ClearVoice();
};
#define VOICEMGR OnlineVoiceManager::getinstance()
#endif