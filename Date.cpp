#include "Date.h"

string showDate(Date date)
{
    stringstream buff;
    string dow;
    string month;
    switch (date.DOW)
    {
        case 0:
            dow = "Sunday";
            break;
        case 1: 
            dow = "Monday";
            break;
        case 2:
            dow = "Tuesday";
            break;
        case 3:
            dow = "Wednesday";
            break;
        case 4: 
            dow = "Thursday";
            break;
        case 5:
            dow = "Friday";
            break;
        case 6:
            dow = "Saturday";
            break;
    }
    switch (date.month)
    {
        case 1:
            month = "January";
            break;
        case 2:
            month = "February";
            break;
        case 3:
            month = "March";
            break;
        case 4:
            month = "April";
            break;
        case 5:
            month = "May";
            break;
        case 6:
            month = "June";
            break;
        case 7:
            month = "July";
            break;
        case 8:
            month = "August";
            break;
        case 9:
            month = "September";
            break;
        case 10:
            month = "October";
            break;
        case 11:
            month = "November";
            break;
        case 12:
            month = "December";
            break;
        default:
            break;
    }
    int hour = (date.hour > 12) ? (date.hour - 12) : date.hour; //hour in session
    string isDay = (date.hour > 12) ? "pm" : "am";  // pm/am
    
    buff << dow << ", " << month << " " << date.day << ", " << date.year << " " 
        << hour << ":" << date.min << ":" << date.sec << isDay;

    return buff.str();
}