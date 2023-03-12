#pragma once
#include <sstream>
#include <string>
using namespace std;

struct Date
{
    int year;
    int month;
    int day;
    int DOW; // Day Of Week
    int hour;
    int min;
    int sec;
};
string showDate(Date date);