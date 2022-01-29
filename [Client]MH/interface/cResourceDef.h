



#ifndef _cRESOURCEHEADER_H_

#define _cRESOURCEHEADER_H_

#include "MHFile.h"

#define SWINDOWTYPE(a)		switch(a){


#define CASE(a)			case a:{
#define BREAK			}break;
#define EWINDOWTYPE			}



#define CMD_ST(a)	char buffer[255]; strcpy(buffer, a); if(0){
#define CMD_CS(b)		}else if(strcmp(buffer, b) == 0){
#define CMD_EN			}

#define FILE_IMAGE_MSG			"./image/image_msg.bin"
#define FILE_IMAGE_PATH			"./image/image_path.bin"
#define FILE_CHAT_MSG			"./image/chat_msg.bin"
#define FILE_IMAGE_HARD_PATH	"./image/image_hard_path.bin"
#define FILE_IMAGE_ITEM_PATH	"./image/image_item_path.bin"
#define FILE_IMAGE_MUGONG_PATH	"./image/image_mugong_path.bin"
#define FILE_IMAGE_ABILITY_PATH "./image/image_ability_path.bin"

#define FILE_IMAGE_BUFF_PATH	"./image/image_Buff_path.bin"
#define FILE_IMAGE_MINIMAP_PATH	"./image/image_Minimap_path.bin"
#define FILE_IMAGE_JACKPOT_PATH	"./image/image_jackpot_path.bin"
#define FILE_CHATTIP_MSG		"./image/ChatTipMsg.bin"
//[Í¼Æ¬³ÆºÅ][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/29]
#define FILE_IMAGE_NAME_PATH	"./image/image_name.bin"
#define FILE_IMAGE_NAME_INFO	"./image/image_name_info.bin"

#endif //_cRESOURCEHEADER_H_


