#pragma once
DWORD DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
DWORD DayOfMonth_Yundal[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char Days[7][16] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
static char WeatherState[][16] = { "CLEAR", "SNOW" };


typedef DWORD	QSTATETYPE;

#define MAX_BIT ((sizeof(QSTATETYPE))*(8))

#define YEARTOSECOND	31536000
#define MONTHTOSECOND	2592000
#define DAYTOSECOND		86400
#define HOURTOSECOND	3600
#define MINUTETOSECOND	60

#define SECTOMILLISEC	1000

struct stTIME {
	QSTATETYPE		value;

	stTIME() :value(0) {}

	void SetTime(QSTATETYPE time) { value = time; }
	void SetTime(DWORD year, DWORD month, DWORD day, DWORD hour, DWORD minute, DWORD second)
	{
		value = 0;
		QSTATETYPE ch = 0;

		ch = year;
		value = (value | (ch << 28));
		ch = month;
		value = (value | (ch << 24));
		ch = day;
		value = (value | (ch << 18));
		ch = hour;
		value = (value | (ch << 12));
		ch = minute;
		value = (value | (ch << 6));
		ch = second;
		value = (value | ch);
	}

	DWORD GetYear()
	{
		return (value >> 28);
	}
	DWORD GetMonth() { DWORD msk = value << 4;		return msk >> 28; }
	DWORD GetDay() { DWORD msk = value << 8;		return msk >> 26; }
	DWORD GetHour() { DWORD msk = value << 14;		return msk >> 26; }
	DWORD GetMinute() { DWORD msk = value << 20;		return msk >> 26; }
	DWORD GetSecond() { DWORD msk = value << 26;		return msk >> 26; }
	inline BOOL	operator >(const stTIME& time);
	inline void operator +=(const stTIME& time);
	inline void operator -=(const stTIME& time);
	inline void operator =(const stTIME& time);

	enum stTIEM_KIND { ST_SEC, ST_MIN, ST_HOUR, ST_DAY };
	inline void AddTimeByValue(DWORD tVal, int flg_valueKind = ST_MIN);
};

inline BOOL	 stTIME::operator >(const stTIME& time)
{
	BOOL bResult = FALSE;
	stTIME ctime = time;

	if (this->GetYear() == ctime.GetYear())
	{
		if (this->GetMonth() > ctime.GetMonth())
			bResult = TRUE;
		else if (this->GetMonth() == ctime.GetMonth())
		{
			if (this->GetDay() > ctime.GetDay())
				bResult = TRUE;
			else if (this->GetDay() == ctime.GetDay())
			{
				if (this->GetHour() > ctime.GetHour())
					bResult = TRUE;
				else if (this->GetHour() == ctime.GetHour())
				{
					if (this->GetMinute() > ctime.GetMinute())
						bResult = TRUE;
					else if (this->GetMinute() == ctime.GetMinute())
						if (this->GetSecond() > ctime.GetSecond())
							bResult = TRUE;
				}
			}
		}
	}
	else if (this->GetYear() > ctime.GetYear())
		bResult = TRUE;

	return bResult;
}
inline void	 stTIME::operator +=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	year = this->GetYear() + atime.GetYear();
	month = this->GetMonth() + atime.GetMonth();
	day = this->GetDay() + atime.GetDay();
	hour = this->GetHour() + atime.GetHour();
	minute = this->GetMinute() + atime.GetMinute();
	second = this->GetSecond() + atime.GetSecond();

	if (this->GetMonth() <= 0)			return;
	else if (this->GetMonth() == 1)	calcmonth = 11;
	else								calcmonth = this->GetMonth() - 1;

	if (second >= 60)
	{
		++minute;
		second -= 60;
	}
	if (minute >= 60)
	{
		++hour;
		minute -= 60;
	}
	if (hour >= 24)
	{
		++day;
		hour -= 24;
	}
	if ((systime.wYear % 4) == 0)
	{
		if (day > (int)(DayOfMonth_Yundal[calcmonth]))
		{
			++month;
			day -= DayOfMonth_Yundal[calcmonth];
		}
	}
	else
	{
		if (day > (int)(DayOfMonth[calcmonth]))
		{
			++month;
			day -= DayOfMonth[calcmonth];
		}
	}
	if (month > 12)
	{
		++year;
		month -= 12;
	}

	this->SetTime(year, month, day, hour, minute, second);
}
inline void	 stTIME::operator -=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	year = this->GetYear() - atime.GetYear();
	month = this->GetMonth() - atime.GetMonth();
	day = this->GetDay() - atime.GetDay();
	hour = this->GetHour() - atime.GetHour();
	minute = this->GetMinute() - atime.GetMinute();
	second = this->GetSecond() - atime.GetSecond();

	if (this->GetMonth() <= 0)			return;
	else if (this->GetMonth() == 1)	calcmonth = 11;
	else								calcmonth = this->GetMonth() - 2;

	if (second < 0)
	{
		--minute;
		second += 60;
	}
	if (minute < 0)
	{
		--hour;
		minute += 60;
	}
	if (hour < 0)
	{
		--day;
		hour += 24;
	}
	if ((systime.wYear % 4) == 0)
	{
		if (day < 0)
		{
			--month;
			day += DayOfMonth_Yundal[calcmonth];
		}
	}
	else
	{
		if (day < 0)
		{
			--month;
			day += DayOfMonth[calcmonth];
		}
	}
	if (month <= 0 && 0 < year)
	{
		--year;
		month += 12;
	}

	this->SetTime(year, month, day, hour, minute, second);
}
inline void	 stTIME::operator =(const stTIME& time)
{
	stTIME atime = time;
	this->SetTime(atime.GetYear(), atime.GetMonth(), atime.GetDay(), atime.GetHour(), atime.GetMinute(), atime.GetSecond());
}

inline void stTIME::AddTimeByValue(DWORD tVal, int flg_valueKind)
{
	DWORD day = 0;
	switch (flg_valueKind)
	{
	case ST_SEC:
		day = tVal / 86400;
		break;
	case ST_MIN:
		day = tVal / 1440;
		break;
	case ST_HOUR:
		day = tVal / 24;
		break;
	case ST_DAY:
		day = tVal;
		break;
	default:
		//__asm int 3;
		break;
	}

	DWORD Curyear = this->GetYear();
	DWORD CurMonth = this->GetMonth();
	day += this->GetDay();
	DWORD hour = this->GetHour() + (tVal % (24 * 60)) / 60;
	DWORD minute = this->GetMinute() + (tVal % (24 * 60)) % 60;

	DWORD* pDayOfMonth = NULL;
	if (Curyear % 4 == 0)
	{
		pDayOfMonth = DayOfMonth_Yundal;
	}
	else
	{
		pDayOfMonth = DayOfMonth;
	}

	while (day > pDayOfMonth[CurMonth - 1])
	{
		day -= pDayOfMonth[CurMonth - 1];
		CurMonth++;

		if (CurMonth > 12)
		{
			++Curyear;
			CurMonth = 1;

			if (Curyear % 4 == 0)
			{
				pDayOfMonth = DayOfMonth_Yundal;
			}
			else
			{
				pDayOfMonth = DayOfMonth;
			}
		}
	}

	this->SetTime(Curyear, CurMonth, day, hour, minute, 0);

}

struct stPlayTime {
	DWORD value;

	stPlayTime::stPlayTime()
	{
		value = 0;
	}
	void GetTime(int& Year, int& Day, int& Hour, int& Minute, int& Second)
	{
		int mv = 0;

		Year = value / YEARTOSECOND;
		mv = value%YEARTOSECOND;

		Day = mv / DAYTOSECOND;
		mv = mv%DAYTOSECOND;

		Hour = mv / HOURTOSECOND;
		mv = mv%HOURTOSECOND;

		Minute = mv / MINUTETOSECOND;
		Second = mv%MINUTETOSECOND;
	}
};