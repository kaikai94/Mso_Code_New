#include "stdafx.h"
#include "Date.h"
Date::Date(void)
{
	m_DayMapInt[1]=eMonday;
	m_DayMapInt[2]=eTuesday;
	m_DayMapInt[3]=eWednesday;
	m_DayMapInt[4]=eThursday;
	m_DayMapInt[5]=eFriday;
	m_DayMapInt[6]=eSaturday;
	m_DayMapInt[0]=eSunday;

	m_DayMapChar[1]="星期一";
	m_DayMapChar[2]="星期二";
	m_DayMapChar[3]="星期三";
	m_DayMapChar[4]="星期四";
	m_DayMapChar[5]="星期五";
	m_DayMapChar[6]="星期六";
	m_DayMapChar[0]="星期日";
	m_DayMapChar[7]="满签";
}
Date::~Date(void)
{

}
std::string Date::GetDay(eSINGED day)
{
	if (day==eCompelete)
		iterChar = m_DayMapChar.find(7);
	else
		iterChar = m_DayMapChar.find(day%7);
	if (iterChar!=m_DayMapChar.end())
	{
		return iterChar->second;
	}
	return NULL;
}
std::string Date::GetDayToChar(int year,int month,int day)
{
	int sum = 0;
	int d[13]={0,31,28,31,30,31,30,31,31,30,31,30};
	if(year%400==0||year%4==0&&year%100!=0)
		d[2]++;
	for (int i = 1900;i<year;i++)
	{
		if(i%400==0||i%4==0&&i%100!=0)
			sum += 366;
		else
			sum += 365;
	}
	for(int i=1;i<month;i++)
		sum += d[i];
	sum += day;
	iterChar = m_DayMapChar.find(sum%7);
	if (iterChar!=m_DayMapChar.end())
	{
		return iterChar->second;
	}
	else 
		return NULL;
}
eSINGED Date::GetDayToInt(int year,int month,int day)
{
	int sum = 0;
	int d[13]={0,31,28,31,30,31,30,31,31,30,31,30};
	if(year%400==0||year%4==0&&year%100!=0)
		d[2]++;
	for (int i = 1900;i<year;i++)
	{
		if(i%400==0||i%4==0&&i%100!=0)
			sum += 366;
		else
			sum += 365;
	}
	for(int i=1;i<month;i++)
		sum += d[i];
	sum += day;
	iterInt = m_DayMapInt.find(sum%7);
	if (iterInt!=m_DayMapInt.end())
	{
		return iterInt->second;
	}
	else 
		return eCompelete;
}
