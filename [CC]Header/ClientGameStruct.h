#ifndef __CLIENTGAMESTRUCT_H__
#define __CLIENTGAMESTRUCT_H__


class CObject;
typedef CObject* LPOBJECT;


template<class T>
class ObjectArray {
public:
	DWORD nTempCount;
	DWORD nObjectNum;
	T pObjectArray[MAX_TARGET_NUM];
	void Init()	{	nObjectNum = 0;	memset(pObjectArray, 0, sizeof(T)*MAX_TARGET_NUM); }
	void AddObject(T Object)
	{
		pObjectArray[nObjectNum++] = Object;
	}
	void SetPositionHead()	{	 nTempCount = 0;	}
	T GetNextObject()		{


		if(nTempCount >= nObjectNum)
			return NULL;	

		else
			return pObjectArray[nTempCount++];

	}
};

struct GameLoadingParam
{
	DWORD selectedCharNum;
	DWORD m_MapNum;
};

#include "cStatic.h"
#include "cGuagen.h"

struct TITANSTATS_ATTRDEF
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib,WORD value,DWORD color=0)
	{
		char temp[32];
		sprintf(temp,"%d%%",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		pGuage.GetElement_Val(attrib)->SetValue(value*0.01f);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);

	}
};

struct TITAN_STATS_CTRL
{
	cStatic*			titanType;			
	cStatic*			titanAtt;			
	cStatic*			titanLongAtt;		
	cStatic*			titanMagicAtt;		
	cStatic*			titanCritical;	
	cStatic*			titanDistance;	
	cStatic*			titanDef;			
	cStatic*			titanLife;		
	TITANSTATS_ATTRDEF	TitanAttrDef;		
};

struct PARSE_TYPE_INFO
{
	int nType;
	char cpTypeName[MAX_PARSE_TYPE_NAME_LENGTH];
	int nKeyValue;
};
/////////////////////////////////////////////////////////////////////////
struct SOSMEMBERINFO
{
	char strName[MAX_NAME_LENGTH+1];
	DWORD level;
};

#endif //__CLIENTGAMESTRUCT_H__


