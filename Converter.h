#pragma once
#include "Byte.h"
#include "Date.h"
#include <string>
#include <iostream>
#include <vector>

// char[] -> string
std::string char2String(char c[],int size);
//FILETIME -> SYSTEMTIME -> Date 
SYSTEMTIME filetime2Systime(BYTE *byte);
void systime2Date(SYSTEMTIME systime,Date &date);
void filetime2Date(BYTE *byte,Date &date);
vector<string> split(string source, string delim);
