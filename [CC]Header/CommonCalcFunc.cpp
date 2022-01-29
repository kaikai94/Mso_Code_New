







#include "stdafx.h"
#include "CommonCalcFunc.h"

#ifdef _MAPSERVER_
#include "Object.h"
#endif


DWORD CalcRangeAttackPower(WORD MinChub,WORD Weapon)
{
//#ifdef _JAPAN_LOCAL_
//
//	return Weapon + MinChub + MinChub/3;
//
//#else

	double dW = Weapon;
	double dS = MinChub;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));

//#endif	
}


DWORD CalcMeleeAttackPower(WORD GenGol,WORD Weapon)
{
//#ifdef _JAPAN_LOCAL_
//
//	return Weapon + GenGol + GenGol/3;
//
//#else


	double dW = Weapon;
	double dS = GenGol;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));

//#endif
}

WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{	
	int LevelGap = OperatorLevel - TargetLevel;
	float fRate = SeedVal + LevelGap*0.025f;
	if(fRate <= 0)
		return 0;

	return (WORD)(fRate * 100);

}

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{
	WORD Percent = GetPercent(SeedVal,OperatorLevel,TargetLevel);

	WORD randVal = rand()%100;
	if(randVal <= Percent)
		return TRUE;
	else
		return FALSE;
}


WORD GetStatRound( WORD StatVal )
{
	WORD RoundVal = StatVal;

	RoundVal += 5;

	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return RoundVal;
	else
		return StatVal;
}

WORD GetAddStatFromRound( WORD StatVal )
{

	/*
	WORD RoundVal = StatVal;

	RoundVal += 5;


	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return (RoundVal - StatVal);
	else
		return 0;
*/
	StatVal %= 10;

	if( StatVal < 5 )
		return 0;
	else
		return 10 - StatVal;
}

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget )
{

	if( SeedVal > 0.f )
	{
		LEVELTYPE OperLevel = pOperator->GetLevel();
		LEVELTYPE TargetLevel = pTarget->GetLevel();


		float fSAR = 0.f;
		if( OperLevel < TargetLevel )
		{
			if( TargetLevel - OperLevel < 10  )
				fSAR = SeedVal * ( 1.f - (float)( TargetLevel - OperLevel ) / 10.f );
			else
				fSAR = 0.f;
		}


		else
		{
			fSAR = SeedVal * ( 1.f + (float)( OperLevel - TargetLevel ) / 5.f );
		}
		
		if( pTarget->GetObjectKind() & eObjectKind_Monster )
		{
			fSAR *= 3.f;
		}
		
		float fRand = (float)(rand()%100) / 100.f;
		
		if( fSAR < fRand )
			return FALSE;
		
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif




