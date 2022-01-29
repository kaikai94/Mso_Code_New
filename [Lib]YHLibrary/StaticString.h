






#ifndef _STATICSTRING_H_
#define _STATICSTRING_H_




struct StaticString
{
	StaticString()	{	Str = NULL;	}
	~StaticString()	{	if(Str) delete [] Str;	Str=0; }

	char* Str;

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


#endif


