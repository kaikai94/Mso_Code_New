



// Crypt.cpp: implementation of the CCrypt class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Crypt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrypt::CCrypt()
{
	m_bInited	= FALSE;
}

CCrypt::~CCrypt()
{
}

void CCrypt::Create()
{
	HselInit eninit;
	eninit.iEncryptType		=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount		=	HSEL_DES_TRIPLE;
	eninit.iCustomize		=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag		=	HSEL_SWAP_FLAG_ON;
	
	if( !m_hEnStream.Initial(eninit) )
	{
		ASSERTMSG(0, "CCrypt Create Initial() failed");
		return;
	}
	m_eninit = m_hEnStream.GetHSELCustomizeOption();

	if( !m_hDeStream.Initial(eninit) )
	{
		ASSERTMSG(0, "CCrypt Create Initial() failed");
		return;
	}
	m_deinit = m_hDeStream.GetHSELCustomizeOption();	


}

void CCrypt::Init( HselInit	eninit, HselInit deinit )
{
	m_eninit = deinit;			
	m_deinit = eninit;	

	if( !m_hEnStream.Initial(deinit) )
	{
		ASSERTMSG(0, "CCrypt EnInit Initial() failed");
		return;
	}

	if( !m_hDeStream.Initial(eninit) )
	{
		ASSERTMSG(0, "CCrypt DeInit Initial() failed");

		return;
	}
	m_bInited = TRUE;
}

BOOL CCrypt::Encrypt( char * eBuf, int bufSize )
{
	if( !m_bInited )

		return TRUE;
	return m_hEnStream.Encrypt( eBuf, bufSize );

}

BOOL CCrypt::Decrypt( char * eBuf, int bufSize )
{
	if( !m_bInited )
		return TRUE;
	return m_hDeStream.Decrypt( eBuf, bufSize );
}	

char CCrypt::GetEnCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hEnStream.GetCRCConvertChar();
}
char CCrypt::GetDeCRCConvertChar()
{
	if( !m_bInited )
		return 0;
	return m_hDeStream.GetCRCConvertChar();

}


