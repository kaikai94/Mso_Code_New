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
//         ���汣��       ����BUG     �����޸�				      //
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
	eForDaZuo,		//����
	eForActtact,	//�����ͷż��
	eMovePoint,		//�һ���ʱ����
	eTimeForTuna,	//����
	eProtectLifeTime,
	eProtectNaeRyukTime,
	eProtectShieldTime,
	eUseItemTime,
	eUsePetItem,
	eMaxTime,
};
enum VALUE
{
	eAutoTuNaLife,		//��������ֵ
	ePressKey1,			//���ܰ���1
	ePressKey2,			//���ܰ���2
	ePressPage1,		//���ܰ�����ҳ1
	ePressPage2,		//���ܰ�����ҳ2
	eDaZuoLife,			//��������ֵ
	eDaZuoShield,		//��������ֵ
	eDaZuoNaeRyuk,		//��������ֵ
	eDaZuoMonsterLife,	//��������ֵ
	ePointX,			//�һ������
	ePointY,
	eLastMap,			//�һ���ͼ
	eWantMoney,			//���ͽ��
	eBuildTime,			//�������
	eReBuildTime,		//����ʱ��
	eDistance,
	eProtectLifeVaule,
	eProtectNaeRyukVaule,
	eProtectShieldVaule,
	eMaxValue,
};
enum SWITCH
{
	eOpen,				//[����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoDazuoH,		//[��������][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoDazuoL,		//[��������][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoDazuoT,		//[��������][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoSkill1,		//[����1][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoSkill2,		//[����2][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoToNa,			//[����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoUseWuShuang,	//[�Զ���˫][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoJoinGroup,		//[�Զ����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoWant,			//[�Զ�����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoCloseWin,		//[�ر��Ӷᴰ��][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoReBuild,		//[�Զ�����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoAttrPlay,		//[�Զ�����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eUsePoHun,			//[�ƻ�ʹ��][By:ʮ���½���][QQ:112582793][2018/1/12]
	eUseSheXin,			//[����ʹ��][By:ʮ���½���][QQ:112582793][2018/1/12]
	eReBuildType,		//[����λ��][By:ʮ���½���][QQ:112582793][2018/1/12]
	eProtectLife,		//[��������][By:ʮ���½���][QQ:112582793][2018/1/12]
	eProtectNaeRyuk,	//[��������][By:ʮ���½���][QQ:112582793][2018/1/12]
	eProtectShield,		//[���屣��][By:ʮ���½���][QQ:112582793][2018/1/12]
	eProtectItem,		//[�Զ�ʹ����Ʒ][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoOpenBox,		//[�Զ��⿪����][By:ʮ���½���][QQ:112582793][2018/1/12]
	eAutoUseShopItem,	//[�̳���Ʒ�Զ����][By:ʮ���½���][QQ:112582793][2018/1/12]
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
	WORD wMapNum;		//[��ͼ���][BY:ʮ���½���][QQ:112582793][2019-8-2][12:09:42]
	WORD wBeginH;		//[��ʼʱ��-Сʱ][BY:ʮ���½���][QQ:112582793][2019-8-2][12:09:51]
	WORD wBeginM;		//[��ʼʱ��-����][BY:ʮ���½���][QQ:112582793][2019-8-2][12:10:09]
	WORD wEndH;			//[����ʱ��-Сʱ][BY:ʮ���½���][QQ:112582793][2019-8-2][12:10:21]
	WORD wEndM;			//[����ʱ��-����][BY:ʮ���½���][QQ:112582793][2019-8-2][12:10:34]
	BOOL bIsLimit;		//[�Ƿ�����][BY:ʮ���½���][QQ:112582793][2019-8-2][12:10:42]

};