










// Skill.cpp: implementation of the CSkill class.

//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Skill.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill()

{
	m_bUse = FALSE;
	*skillName = 0;
}


CSkill::~CSkill()



{


}



void CSkill::SetMugongBase(MUGONGBASE * mugong)


{

	memcpy(&m_MugongBaseInfo, mugong, sizeof(MUGONGBASE));
}






