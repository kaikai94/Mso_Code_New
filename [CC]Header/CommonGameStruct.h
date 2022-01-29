




#ifndef __COMMONGAMESTRUCT_H__
#define __COMMONGAMESTRUCT_H__
//#ifdef _JAPAN_LOCAL_
//enum ATTRIBUTE_ELEMENT
//{
//	ATTR_NONE = 0,
//	ATTR_FIRE = 1,
//	ATTR_EARTH = 2,
//	ATTR_IRON = 3,
//	ATTR_WATER = 4,
//	ATTR_TREE = 5,
//
//	ATTR_MAX = 5,
//
//	ATTR_NOATTR = 6,	
//	ATTR_ALLATTR = 7,	
//};
//#else
enum ATTRIBUTE_ELEMENT
{
	ATTR_NONE = 0,
	ATTR_FIRE = 1,
	ATTR_WATER = 2,
	ATTR_TREE = 3,
	ATTR_IRON = 4,
	ATTR_EARTH = 5,

	ATTR_MAX = 5,

	ATTR_NOATTR = 6,	
	ATTR_ALLATTR = 7,		
};
//#endif


template < class T >
union ATTRIBUTE_VAL
{
private:
	struct{
	T FireElement;			
	T WaterElement;			
	T TreeElement;			
	T GoldElement;			
	T EarthElement;			
	T NoneElement;			

	};

	T Element[ATTR_MAX+1];
public:
	ATTRIBUTE_VAL()
	{
		NoneElement = 0;
	}
	T GetElement_Val(WORD Attrib)
	{
		if(Attrib == ATTR_NONE)	
			return 0;

		if(Attrib > ATTR_MAX+1)
		{
			
		}
		return Element[Attrib-1];
	}
	void SetElement_Val(WORD Attrib,T val)
	{
		if(Attrib == ATTR_NONE)		
			return;
		Element[Attrib-1] = val;
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += attr_val.Element[n];
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val,float fRate)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += (T)(attr_val.Element[n]*fRate);
	}
	void CheckLimit(T max_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
		{
			if(Element[n] > max_val)
				Element[n] = max_val;
		}
	}
};

typedef ATTRIBUTE_VAL<float>	ATTRIBUTEATTACK;	
typedef ATTRIBUTE_VAL<float>	ATTRIBUTEREGIST;	


struct player_calc_stats
{
	WORD GenGol;
	WORD MinChub;
	WORD CheRyuk;
	WORD SimMaek;

	WORD MeleeAttackPowerMin;	
	WORD MeleeAttackPowerMax;
	WORD RangeAttackPowerMin;
	WORD RangeAttackPowerMax;
	
	float MeleeAttackPowerAddRateMin;
	float MeleeAttackPowerAddRateMax;
	float RangeAttackPowerAddRateMin;
	float RangeAttackPowerAddRateMax;

	DWORD MaxLife;
	DWORD MaxShield;
	DWORD MaxNaeRyuk;
	
	DWORD Critical;
	DWORD Decisive;
	WORD PhysicalDefense;
	ATTRIBUTEATTACK AttributeAttack;	
	ATTRIBUTEREGIST AttributeResist;
};

struct titan_calc_stats
{
	DWORD MaxFuel;
	DWORD MaxSpell;

	DWORD Critical;

	WORD PhysicalDefense;

	WORD MeleeAttackPowerMin;
	WORD MeleeAttackPowerMax;
	WORD RangeAttackPowerMin;
	WORD RangeAttackPowerMax;

	float AttackRange;

	float MoveSpeed;

	ATTRIBUTEATTACK AttributeAttack;

	ATTRIBUTEREGIST AttributeResist;
};

struct monster_stats
{
	WORD	Attack1OccurProportion;		
	WORD	PhysicalAttackMin1;						
	WORD	PhysicalAttackMax1;
	WORD	PhysicalAttackMin2;					
	WORD	PhysicalAttackMax2;
	WORD	AttributeAttackMin1;
	WORD	AttributeAttackMax1;
	WORD	AttributeAttackMin2;
	WORD	AttributeAttackMax2;
	WORD	PhysicalDefense;	
	ATTRIBUTEREGIST AttributeResist;
};

struct TILEINDEX
{
	int nx,nz;	

	inline TILEINDEX operator +(const TILEINDEX &ti)
	{
		TILEINDEX result;
		result.nx	=	this->nx + ti.nx;
		result.nz	=	this->nz + ti.nz;
		return	result;
	}
	inline BOOL operator ==(const TILEINDEX &ti)
	{
		return (this->nx == ti.nx) && (this->nz == ti.nz);
	}
};

#define MAX_REGIST_HYPERLINK  70

enum LINKTYPE {
	emLink_Null,
	emLink_Page,	
	emLink_End,		
	emLink_Open,	
	emLink_Quest,	
	emLink_QuestLink,			
	emLink_QuestStart,			
	emLink_QuestContinue,		
	emLink_QuestEnd,			
	emLink_MapChange,			
	emLink_EventQuestStart,
	emLink_Image,	
	emLink_MaryDlg,              
	emLink_MaryDelDlg,
	emLink_Change_State_1 = 16,
	emLink_Change_State_2,
	emLink_Change_State_3,
	emLink_Change_State_4,
	emLink_Change_Live,
};

struct DIALOGUE {
	void Init()
	{
		wType = 0;
		dwColor = RGB(255,255,255);
		memset(str, 0, 256);
		wLine = 0;
	}
	WORD	wType; 
	DWORD	dwColor;
	char	str[256];
	WORD	wLine;
};

struct HYPERLINK
{
	WORD		wLinkId;
	WORD		wLinkType;
	DWORD		dwData;
};

struct  HYPER
{
	void Init()
	{
		bUse = FALSE;
		dwListItemIdx = 0;
	}

	HYPERLINK	sHyper;			
	DWORD		dwListItemIdx;	

	BOOL		bUse;
};
//////////////////////////////////////////////////////////////////////////
struct QString
{
	QString()	{	Str = NULL;	Line=0;	}
	~QString()	{	if(Str) delete [] Str;	Str=0;}

	char* Str;
	int		Line;
	DWORD	Color;

	void operator = (char* pchar)
	{
		Str = new char[strlen(pchar)+1];
		strcpy_s(Str, strlen(pchar) + 1,pchar);
	}
	operator LPCTSTR() const
	{
		return Str;
	}
	operator PCHAR() const
	{
		return Str;
	}

};
#endif //__COMMONGAMESTRUCT_H__


