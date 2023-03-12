#include "Converter.h"

// char[] -> string
std::string char2String(char c[],int size)
{
    std::string res;
    for(int i = 0; i < size;i++)
            res += c[i];

    return res;
}
//FILETIME -> SYSTEMTIME -> Date 
SYSTEMTIME filetime2Systime(BYTE *byte)
{
    FILETIME time;
    SYSTEMTIME systime;
    time.dwLowDateTime = reverseByte(byte,4);
    time.dwHighDateTime = reverseByte(byte+4,4);
    
    FileTimeToSystemTime(&time,&systime);
    return systime;
}
void systime2Date(SYSTEMTIME systime,Date &date)
{
    date.year = systime.wYear;
    date.month = systime.wMonth;
    date.day = systime.wDay;
    date.DOW = systime.wDayOfWeek;
    date.hour = systime.wHour;
    date.min = systime.wMinute;
    date.sec = systime.wSecond;
}
void filetime2Date(BYTE *byte,Date &date)
{
    SYSTEMTIME systime = filetime2Systime(byte);
    systime2Date(systime,date);
}
using namespace std;
vector<string> split(string source, string delim)
{
	int beg = 0, pos = 0;
	string token;
	vector<string> result;

	pos = source.find(delim, beg);
	while (pos > -1)
	{
		token = source.substr(beg, pos - beg);
		result.push_back(token);
		beg = pos + delim.size();
		pos = source.find(delim, beg); 
	}
	token = source.substr(beg, source.size() - beg);
	result.push_back(token);
	
	return result;
}