#pragma once
#include <windows.h>

void ReadSectorEx(LPCWSTR drive, unsigned int readPoint, BYTE sector[]);
int ReadSector(LPCWSTR drive, long readPoint, BYTE sector[]);
void showByteTable(BYTE sector[]);
long long reverseByte(BYTE* byte,unsigned int count);
char* byte2PChar(BYTE byte[],int size);
wchar_t* byte2WChar(BYTE byte[],int size);
