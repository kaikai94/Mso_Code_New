




#ifndef __AGENTNETWORKMSGPARSER_H__
#define __AGENTNETWORKMSGPARSER_H__

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_AGENTSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[登录连接消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_NotifyLoginSyn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_NotifyToAgent_Alreadyout(char* pMsg);
void UserConn_NotifyOverlappedLogin(char* pMsg);
void UserConn_DisConn_OverLapLogin(char* pMsg);
void UserConn_DisConned_On_Login(char* pMsg);
void UserConn_DisConn_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_Check_OK(DWORD dwConnectionIndex, char* pMsg);
void UserConn_CharList_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_DirectCharList_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_CharSelect_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_ChannelInfo_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_ChannelInfo_Ack(char* pMsg,DWORD dwLength);
void UserConn_ChannelInfo_Nack(char* pMsg, DWORD dwLength);
void UserConn_GameIn_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_GameIn_Nack(char* pMsg);
void UserConn_GameIn_Ack(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_Char_Make_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_CharMake_Check_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_CharRemove_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_Map_Out(char* pMsg);
void UserConn_Map_Out_WithMapNum(char* pMsg);
void UserConn_SavePoint_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_BackToCharSel_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_BackToCharSel_Ack(char* pMsg);
void UserConn_Cheat_Using(char* pMsg);
void UserConn_LoginCheck_Delete(char* pMsg);
void UserConn_GameInPos_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_BackToBeforeMap_Syn(char* pMsg, DWORD dwLength);
void UserConn_BackToBeforeMap_Ack(char* pMsg);
void UserConn_MapOut_EventMap(char * pMsg);
void UserConn_Enter_EventMap_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_EventItem_Use(char* pMsg);
void UserConn_EventItem_Use2(char* pMsg, DWORD dwLength);
void UserConn_ChangeMap_ChannelInfo_Syn(DWORD dwConnectionIndex, char* pMsg);
void UserConn_ChangeMap_ChannelInfo_Ack(char* pMsg, DWORD dwLength);
void UserConn_ChangeMap_ChannelInfo_Nack(char* pMsg, DWORD dwLength);
void UserConn_TopList_Ack(DWORD dwConnectionIndex, char* pMsg);
void UserConn_BackDoor_Get(DWORD dwConnectionIndex);
/********************************************************************/
//[聊天客户端消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Chat_WhisperA_Syn(DWORD dwConnectionIndex, char* pMsg);
void Chat_Party_Syn(char* pMsg);
void Chat_Guild_Syn(char* pMsg, DWORD dwLength);
void Chat_GuildUnionA(char* pMsg, DWORD dwLength);
/********************************************************************/
//[聊天服务端消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_CHATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Chat_Whisper_Syn(DWORD dwConnectionIndex, char* pMsg);
void Chat_Whisper_GM_Syn(DWORD dwConnectionIndex, char* pMsg);
void Chat_Whisper_Ack(char* pMsg, DWORD dwLength);
void Chat_Whisper_Nack(char* pMsg, DWORD dwLength);
void Chat_Party(char* pMsg);
void Chat_Guild(char* pMsg, DWORD dwLength);
void Chat_GuildUnion(char* pMsg, DWORD dwLength);
void Chat_Shout_SendServer(char* pMsg);
/********************************************************************/

void TransToClientMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void TransToMapServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[广播数据包][BY:十里坡剑神][QQ:112582793][2019-8-18][13:29:23]
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Packed_Normal(char* pMsg);
void Packed_ToMapServer(char* pMsg);
void Packed_ToBroadMapServer(DWORD dwConnectionIndex, char* pMsg);
/********************************************************************/
//[队伍服务端消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_PARTYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Party_ReQuest_Consent_Ack(char* pMsg);
void Party_ReQuest_Refusal_Ack(char* pMsg);
void Party_Error(char* pMsg);
/********************************************************************/
//[结婚消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][11:59:11]
void MP_MARRYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MarryNotice(char* pMsg);
void MarryLoginIn(char* pMsg);
void MarryLoginOut(char* pMsg);
void MarryReckon(char* pMsg);
/****************************************************************/
void MP_CLIENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[队伍客户端消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_PARTYUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Party_MasterToReQuest_Syn(char* pMsg);
/****************************************************************/
//[好友消息解析][BY:十里坡剑神][QQ:112582793][2019-8-18][12:07:34]
void MP_FRIENDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Friend_Login(char* pMsg);
void Friend_Add(char* pMsg);
void Friend_Add_Accept(char* pMsg);
void Friend_Add_Deny(char*pMsg);
void Friend_Del_Syn(char* pMsg);
void Friend_DelID_Syn(char* pMsg);
void Friend_AddID_Syn(char* pMsg);
void Friend_LogOut(char* pMsg);
void Friend_LogOutA(char* pMsg);
void Friend_List_Syn(char* pMsg);
void Friend_Add_Ack(char* pMsg);
void Friend_Add_Nack(char* pMsg);
void Friend_Accept_Add(char* pMsg);
void Friend_Accept_Add_Nack(char* pMsg);
void Friend_Login_Notify(char* pMsg);
void Friend_Add_Invite(char* pMsg);
void Friend_Add_NackA(char* pMsg);
/****************************************************************/
void MP_NOTEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_NOTEServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_WANTEDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MURIMNETServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MURIMNETUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_OPTIONUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_DebugMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILD_FIELDWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILD_FIELDWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
BOOL CheckCharacterMakeInfo( CHARACTERMAKEINFO* pmsg );
void MP_ITEMUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMUserMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMServerMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GTOURNAMENTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GTOURNAMENTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_JACKPOTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_JACKPOTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SkillUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_SkillServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_GUILD_UNIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_GUILD_UNIONServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_SIEGEWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARPROFITUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARPROFITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_WEATHERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_STREETSTALLUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_EXCHANGEUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SURVIVALUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SURVIVALServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_BOBUSANGUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_BOBUSANGServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMLIMITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_AUTONOTEUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_AUTONOTEServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_FORTWARServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
void MP_SINGEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
void MP_RechargMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[结婚客户端消息处理][By:十里坡剑神][QQ:112582793][2018/4/18]
void MP_MarryUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void Marry_Notice_Login_Syn(char* pMsg);
void Marry_Notice_LoginOut_Syn(char* pMsg);
void Marry_Beckon_Syn(char* pMsg);
void Marry_Reckon_No(char* pMsg);
/****************************************************************/
//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/23]
void MP_KillTopListInfoMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
void MP_SafeLockAGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[骇客检测客户端消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:16:25]
void MP_Hacker_Client_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[骇客检测服务端消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:17:21]
void MP_Hacker_Map_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[排行更新][BY:十里坡剑神][QQ:112582793][2019-9-4][23:56:29]
void MP_TopList_Client_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_TopList_Server_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-26][0:02:19]
void MP_UnionWar_Server_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-26][0:02:19]
//void MP_UnionWar_Clinet_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
#endif //__AGENTNETWORKMSGPARSER_H__


