/*******************************************************************************\
 ** gcloud_voice:GCloudVoice.h
 ** Created by CZ  on 16/8/1
 **
 **  Copyright 2016 apollo. All rights reserved.
 \*******************************************************************************/

#ifndef GcloudVoiceErrno_h
#define GcloudVoiceErrno_h

namespace gcloud_voice
{
    
    enum GCloudVoiceErrno
    {
        GCLOUD_VOICE_SUCC           = 0,
		
		//common base err
		GCLOUD_VOICE_PARAM_NULL         = 0x1001,	//4097, some param is null
		GCLOUD_VOICE_NEED_SETAPPINFO    = 0x1002,	//4098, you should call SetAppInfo first before call other api
		GCLOUD_VOICE_INIT_ERR           = 0x1003,	//4099, Init Erro
		GCLOUD_VOICE_RECORDING_ERR      = 0x1004,	//4100, now is recording, can't do other operator
		GCLOUD_VOICE_POLL_BUFF_ERR      = 0x1005,	//4101, poll buffer is not enough or null 
		GCLOUD_VOICE_MODE_STATE_ERR     = 0x1006,	//4102, call some api, but the mode is not correct, maybe you shoud call SetMode first and correct
		GCLOUD_VOICE_PARAM_INVALID      = 0x1007,	//4103, some param is null or value is invalid for our request, used right param and make sure is value range is correct by our comment 
		GCLOUD_VOICE_OPENFILE_ERR       = 0x1008,   //4104, open a file err
		GCLOUD_VOICE_NEED_INIT          = 0x1009,   //4105, you should call Init before do this operator
		GCLOUD_VOICE_ENGINE_ERR         = 0x100A,   //4106, you have not get engine instance, this common in use c# api, but not get gcloudvoice instance first
		GCLOUD_VOICE_POLL_MSG_PARSE_ERR = 0x100B,   //4107, this common in c# api, parse poll msg err
		GCLOUD_VOICE_POLL_MSG_NO        = 0x100C,   //4108, poll, no msg to update


		//realtime err
		GCLOUD_VOICE_REALTIME_STATE_ERR = 0x2001,   //8193, call some realtime api, but state err, such as OpenMic but you have not Join Room first
		GCLOUD_VOICE_JOIN_ERR           = 0x2002,   //8194, join room failed
		GCLOUD_VOICE_QUIT_ROOMNAME_ERR  = 0x2003,   //8195, quit room err, the quit roomname not equal join roomname
		GCLOUD_VOICE_OPENMIC_NOTANCHOR_ERR = 0x2004,//8196, open mic in bigroom,but not anchor role
        GCLOUD_VOICE_CREATE_ROOM_ERR = 0x2005, // 8197, create room error
        GCLOUD_VOICE_NO_ROOM = 0x2006, // 8198, no such room
        GCLOUD_VOICE_QUIT_ROOM_ERR = 0x2007, //8199, quit room error
        GCLOUD_VOICE_ALREADY_IN_THE_ROOM = 0x2008, // 8200, already in the room which in JoinXxxxRoom

		//message err
		GCLOUD_VOICE_AUTHKEY_ERR        = 0x3001,   //12289, apply authkey api error
		GCLOUD_VOICE_PATH_ACCESS_ERR    = 0x3002,   //12290, the path can not access ,may be path file not exists or deny to access
		GCLOUD_VOICE_PERMISSION_MIC_ERR = 0x3003,	//12291, you have not right to access micphone in android
		GCLOUD_VOICE_NEED_AUTHKEY       = 0x3004,	//12292,you have not get authkey, call ApplyMessageKey first
		GCLOUD_VOICE_UPLOAD_ERR         = 0x3005,	//12293, upload file err
		GCLOUD_VOICE_HTTP_BUSY          = 0x3006,	//12294, http is busy,maybe the last upload/download not finish.
		GCLOUD_VOICE_DOWNLOAD_ERR       = 0x3007,	//12295, download file err
		GCLOUD_VOICE_SPEAKER_ERR        = 0x3008,   //12296, open or close speaker tve error
		GCLOUD_VOICE_TVE_PLAYSOUND_ERR  = 0x3009,   //12297, tve play file error
        GCLOUD_VOICE_AUTHING            = 0x300a,   // 12298, Already in applying auth key processing
        GCLOUD_VOICE_LIMIT              = 0x300b,   //12299, upload limit

		GCLOUD_VOICE_INTERNAL_TVE_ERR   = 0x5001,		//20481, internal TVE err, our used
		GCLOUD_VOICE_INTERNAL_VISIT_ERR = 0x5002,	//20482, internal Not TVE err, out used
		GCLOUD_VOICE_INTERNAL_USED      = 0x5003, //20483, internal used, you should not get this err num
        
        GCLOUD_VOICE_BADSERVER          = 0x06001, // 24577, bad server address,should be "udp://capi.xxx.xxx.com"
        
        GCLOUD_VOICE_STTING             =  0x07001, // 28673, Already in speach to text processing
        
        GCLOUD_VOICE_CHANGE_ROLE = 0x08001, // 32769, change role error
        GCLOUD_VOICE_CHANGING_ROLE=0x08002, // 32770, is in changing role
        GCLOUD_VOICE_NOT_IN_ROOM =0x08003, // 32771, no in room
        GCLOUD_VOICE_COORDINATE  = 0x09001, // 36865, sync coordinate error
        GCLOUD_VOICE_SMALL_ROOMNAME = 0x09002, // 36866, query with a small roomname
    };
    
    enum GCloudVoiceCompleteCode
    {
		GV_ON_JOINROOM_SUCC = 1,	//join room succ
		GV_ON_JOINROOM_TIMEOUT,  //join room timeout
		GV_ON_JOINROOM_SVR_ERR,  //communication with svr occur some err, such as err data recv from svr
        GV_ON_JOINROOM_UNKNOWN, //reserved, our internal unknow err

		GV_ON_NET_ERR,  //net err,may be can't connect to network

		GV_ON_QUITROOM_SUCC, //quitroom succ, if you have join room succ first, quit room will alway return succ

		GV_ON_MESSAGE_KEY_APPLIED_SUCC,  //apply message authkey succ
		GV_ON_MESSAGE_KEY_APPLIED_TIMEOUT,		//apply message authkey timeout
		GV_ON_MESSAGE_KEY_APPLIED_SVR_ERR,  //communication with svr occur some err, such as err data recv from svr
		GV_ON_MESSAGE_KEY_APPLIED_UNKNOWN,  //reserved,  our internal unknow err

        GV_ON_UPLOAD_RECORD_DONE,  //upload record file succ
        GV_ON_UPLOAD_RECORD_ERROR,  //upload record file occur error
        GV_ON_DOWNLOAD_RECORD_DONE,	//download record file succ
        GV_ON_DOWNLOAD_RECORD_ERROR,	//download record file occur error

        GV_ON_STT_SUCC, // speech to text successful
        GV_ON_STT_TIMEOUT, // speech to text with timeout
        GV_ON_STT_APIERR, // server's error
        
        GV_ON_RSTT_SUCC, // speech to text successful
        GV_ON_RSTT_TIMEOUT, // speech to text with timeout
        GV_ON_RSTT_APIERR, // server's error
        
		GV_ON_PLAYFILE_DONE,  //the record file played end
        
        GV_ON_ROOM_OFFLINE, // Dropped from the room
        GV_ON_UNKNOWN,
        GV_ON_ROLE_SUCC,    // Change Role Success
        GV_ON_ROLE_TIMEOUT, // Change Role with tiemout
        GV_ON_ROLE_MAX_AHCHOR, // To much Anchor
        GV_ON_ROLE_NO_CHANGE, // The same role
        GV_ON_ROLE_SVR_ERROR, // server's error
        
        GV_ON_RSTT_RETRY, // need retry stt
		GV_ON_JOINROOM_RETRY_FAIL, //join room try again fail
    };
    
    /**
     * Event of GCloudVoice.
     *
     */
    enum GCloudVoiceEvent
    {
        EVENT_NO_DEVICE_CONNECTED = 0, // not any device is connected
        EVENT_HEADSET_DISCONNECTED = 10, // a headset device is connected
        EVENT_HEADSET_CONNECTED = 11, // a headset device is disconnected
        EVENT_BLUETOOTH_HEADSET_DISCONNECTED = 20, // a bluetooth device is connected
        EVENT_BLUETOOTH_HEADSET_CONNECTED = 21, // a bluetooth device is disconnected
        EVENT_MIC_STATE_OPEN_SUCC         =  30,
        EVENT_MIC_STATE_OPEN_ERR      =  31,
        EVENT_MIC_STATE_NO_OPEN       =  32,
		EVENT_MIC_STATE_OCCUPANCY     =  33,// indicates the mic is occupancyed by others
        EVENT_SPEAKER_STATE_OPEN_SUCC  =  40,
        EVENT_SPEAKER_STATE_OPEN_ERR   = 41,
        EVENT_SPEAKER_STATE_NO_OPEN    = 42,
		EVENT_AUDIO_INTERRUPT_BEGIN   = 50,
		EVENT_AUDIO_INTERRUPT_END    = 51,
        EVENT_AUDIO_RECORDER_EXCEPTION    = 52,// indicates the recorder thread occur exception,or you can resume the audio;
        EVENT_AUDIO_RENDER_EXCEPTION    = 53,// indicates the render thread occur exception,or you can resume the audio;
    };
    
    /**
     * Event of GCloudVoice.
     *
     */
    enum GCloudVoiceDeviceState
    {
        AUDIO_DEVICE_UNCONNECTED = 0, // not any audio device is connected
        AUDIO_DEVICE_WIREDHEADSET_CONNECTED = 1, // have a wiredheadset device is connected
        AUDIO_DEVICE_BULETOOTH_CONNECTED = 2, // have a bluetooth device is disconnected
        
    };

} // endof namespace gcloud_voice

#endif /* GcloudVoiceErrno_h */
