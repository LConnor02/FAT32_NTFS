#include "Byte.h"
#include <stdio.h>
#include <iostream>

void ReadSectorEx(LPCWSTR drive, unsigned int readPoint, BYTE sector[])
{
    DWORD dwBytesRead(0);

    HANDLE hFloppy = NULL;
    hFloppy = CreateFileW(drive,                // Floppy drive to open
        GENERIC_READ,                           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,     // Share Mode
        NULL,                                   // Security Descriptor
        OPEN_EXISTING,                          // How to create
        0,                                      // File attributes
        NULL);                                  // Handle to template

    if (hFloppy != NULL)
    {
        LARGE_INTEGER li;
        li.QuadPart = readPoint * 512;
        SetFilePointerEx(hFloppy, li, 0, FILE_BEGIN);
        // Read the boot sector
        if (!ReadFile(hFloppy, sector, 1024, &dwBytesRead, NULL))
        {
            printf("Error in reading floppy disk\n");
        }
        CloseHandle(hFloppy);
        // Close the handle
    }
}
int ReadSector(LPCWSTR drive, long readPoint, BYTE sector[])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFileW(drive,                             // Drive to open
        GENERIC_READ,                                       // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,                 // Share Mode
        NULL,                                               // Security Descriptor
        OPEN_EXISTING,                                      // How to create
        0,                                                  // File attributes
        NULL);                                              // Handle to template

    if (device == INVALID_HANDLE_VALUE)                     // Open Error
    {
        printf("ERROR\nCreateFile: %u\n", GetLastError());
        return 0;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);    //Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }

    return 1;
}
void showByteTable(BYTE sector[])
{
    printf("   ");
    for (int i = 0; i < 16; i++)
        printf("%2hx ", i);
    for (int i = 0; i < 512 / 16; i++)
    {
        printf("\n%2hx ", i);
        for (int j = 0; j < 16; j++)
        {
            if (sector[16 * i + j] < 16)
                printf("0");
            printf("%hx ", sector[16 * i + j]);
        }
    }
}

long long reverseByte(BYTE* byte,unsigned int count)
{
    long long result = 0;
    for (int i = count - 1; i >= 0; i--)
		result = (result << 8) | byte[i];

    return result;
}
char* byte2PChar(BYTE byte[],int size)
{
    char *pc = new char[size];
    memcpy(pc,byte,size);

    return pc;
}
wchar_t* byte2WChar(BYTE byte[],int size)
{
    wchar_t *pc = new wchar_t[size];
    wmemcpy(pc,(wchar_t*)byte,size);

    return pc;
}
