////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改				      //
////////////////////////////////////////////////////////////////////
enum SKILL
{
	ePhyDefenceUp=9,
	eAttrCounterFire=19,
	eAttrCounterWater=20,
	eAttrCounterTree=21,
	eAttrCounterIron=22,
	eAttrCounterEarth=23,
	eDamageDown=34,
	eAttackUp =35,
	eContinueRecover=36,
	eReverseVampiricNaeryuk=37,
	eAttackPhyLastUp=39,
	eAttackAttLastUp=40,
	eReverseVampiricLife=45,
	eSkipEffect=133,
	eMaxSkill,
};
enum NOTMAINSKILLTIME
{
	ePhy=9,
	eFire=19,
	eWater=20,
	eTree=21,
	eIron=22,
	eEarth=23,
	eDamage=34,
	eAttUp =35,
	eRecover=36,
	eNaeryuk=37,
	ePhyLastUp=39,
	eLastUp=40,
	eReverseLife=45,
	eSkip=133,
	eTuNa,
	eMaxSkillTime,
};
enum COUNT
{
	eReMoveTime,
	eCurTime,
	eSkillTime,
	eForDaZuo,		//打坐
	eForActtact,	//技能释放检查
	eMovePoint,		//挂机点时间检查
	eTimeForTuna,	//吐纳
	eProtectLifeTime,
	eProtectNaeRyukTime,
	eProtectShieldTime,
	eUseItemTime,
	eUsePetItem,
	eMaxTime,
};
enum VALUE
{
	eAutoTuNaLife,		//吐纳生命值
	ePressKey1,			//技能按键1
	ePressKey2,			//技能按键2
	ePressPage1,		//技能按键分页1
	ePressPage2,		//技能按键分页2
	eDaZuoLife,			//打坐生命值
	eDaZuoShield,		//打坐护体值
	eDaZuoNaeRyuk,		//打坐内力值
	eDaZuoMonsterLife,	//怪物生命值
	ePointX,			//挂机坐标点
	ePointY,
	eLastMap,			//挂机地图
	eWantMoney,			//悬赏金额
	eBuildTime,			//复活次数
	eReBuildTime,		//复活时间
	eDistance,
	eProtectLifeVaule,
	eProtectNaeRyukVaule,
	eProtectShieldVaule,
	eMaxValue,
};
enum SWITCH
{
	eOpen,				//[开关][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoDazuoH,		//[打坐生命][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoDazuoL,		//[打坐内力][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoDazuoT,		//[打坐护体][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoSkill1,		//[技能1][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoSkill2,		//[技能2][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoToNa,			//[吐纳][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoUseWuShuang,	//[自动无双][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoJoinGroup,		//[自动组队][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoWant,			//[自动悬赏][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoCloseWin,		//[关闭掠夺窗口][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoReBuild,		//[自动复活][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoAttrPlay,		//[自动反击][By:十里坡剑神][QQ:112582793][2018/1/12]
	eUsePoHun,			//[破魂使用][By:十里坡剑神][QQ:112582793][2018/1/12]
	eUseSheXin,			//[摄心使用][By:十里坡剑神][QQ:112582793][2018/1/12]
	eReBuildType,		//[复活位置][By:十里坡剑神][QQ:112582793][2018/1/12]
	eProtectLife,		//[生命保护][By:十里坡剑神][QQ:112582793][2018/1/12]
	eProtectNaeRyuk,	//[内力保护][By:十里坡剑神][QQ:112582793][2018/1/12]
	eProtectShield,		//[护体保护][By:十里坡剑神][QQ:112582793][2018/1/12]
	eProtectItem,		//[自动使用物品][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoOpenBox,		//[自动解开箱子][By:十里坡剑神][QQ:112582793][2018/1/12]
	eAutoUseShopItem,	//[商城物品自动解封][By:十里坡剑神][QQ:112582793][2018/1/12]
	eUseTimer,
	eMaxSet,
};
enum ePROTECTLIFE
{
	eMaxLife=10,
};
enum ePROTECTNAERYUK
{
	eMaxNaeRyuk=10,
};
enum ePROTECTSHIELD
{
	eMaxShield=10,
};
enum eEtcItem
{
	eMaxEtcItem=70,
};
enum eTimeSwitch
{
	eSwitch1,
	eSwitch2,
	eSwitch3,
	eSwitch4,
	eSwitch5,
	eSwitch6,
	eSwitch7,
	eSwitch8,
	eSwitchMax,
};

enum eTimeKey
{
	eKey1,
	eKey2,
	eKey3,
	eKey4,
	eKey5,
	eKey6,
	eKey7,
	eKey8,
	eKeyMax,
};
enum eTimePage
{
	ePage1,
	ePage2,
	ePage3,
	ePage4,
	ePage5,
	ePage6,
	ePage7,
	ePage8,
	ePageMax,
};
struct AICFIG
{
	DWORD TimeValue[eSwitchMax];
	DWORD SetValue[eMaxValue];
	DWORD SkillInfo[eMaxSkill];
	DWORD ProtectLife[eMaxLife];
	DWORD ProtectNaeRyuk[eMaxNaeRyuk];
	DWORD ProtectShield[eMaxShield];
	DWORD EtcItem[eMaxEtcItem];
	BYTE  TimeKey[eKeyMax];
	BYTE  TimePage[ePageMax];
	BOOL  TimeSwitch[eSwitchMax];
	BOOL AutoSwitch[eMaxSet];

};
struct SKILLSET
{
	SKILLSET(){ memset(this, 0, sizeof(SKILLSET)); }
	char SkillName[MAX_NAME_LENGTH + 1];
	WORD StateIcon;
	WORD SkillIndex;
};
struct sUseItem
{
	sUseItem(){ memset(this, 0, sizeof(sUseItem)); }
	char  ItemName[MAX_ITEMNAME_LENGTH + 1];
	DWORD ItemIdx;
	WORD  ItemType;
};
struct AiLimitInfo
{
	AiLimitInfo(){ memset(this, 0, sizeof(AiLimitInfo)); }
	WORD wMapNum;		//[地图编号][BY:十里坡剑神][QQ:112582793][2019-8-2][12:09:42]
	WORD wBeginH;		//[开始时间-小时][BY:十里坡剑神][QQ:112582793][2019-8-2][12:09:51]
	WORD wBeginM;		//[开始时间-分钟][BY:十里坡剑神][QQ:112582793][2019-8-2][12:10:09]
	WORD wEndH;			//[结束时间-小时][BY:十里坡剑神][QQ:112582793][2019-8-2][12:10:21]
	WORD wEndM;			//[结束时间-分钟][BY:十里坡剑神][QQ:112582793][2019-8-2][12:10:34]
	BOOL bIsLimit;		//[是否限制][BY:十里坡剑神][QQ:112582793][2019-8-2][12:10:42]

};