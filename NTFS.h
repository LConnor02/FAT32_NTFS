#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#include "Converter.h"
#include "Byte.h"
#include "Date.h"
#include <list>
using namespace std;


struct FileInfo
{
    string name;  
    long long size;       // file size
    int begSector;  // from MFT begin sector  
    int refCount;   // level directory (begin from level 1)
    bool isUsed;    // is using or not
    int id;         
    int idP;        //ID Parent
    int flag;       //file status (read-only,hidden,system file,...)
    bool isFile;    //File(1)/Folder(0)
    Date created;
    string data;
};

    // NTFS
struct VBR
{
    //--------------------------
    //11 bytes
    BYTE bootstrapJump[3];
    BYTE oem[8];
    //--------------------------
    //BPB (73 bytes)
    BYTE bytePerSector[2];
    BYTE sectorByCluster[1];
    BYTE UNTR1[7];
    BYTE diskCode[1];
    BYTE UNTR2[18];
    BYTE sectorOfDisk[8];
    BYTE startClusterMFT[8];
    BYTE MFTMirror[8];
    BYTE MFTEntrySize[1];
    BYTE UNTR3[19];
    //--------------------------
    //428 bytes
    BYTE UNTR4[428];
};
    //--------------------------

    //          UNTR  

    //--------------------------
struct MFTEntryHeader
{
    //-------------------------- (56 bytes)
    //MFT Header (42 bytes)
    BYTE sign[4];
    BYTE UNTR1[12];
    BYTE seqNum[2];
    BYTE repCount[2];
    BYTE startAttr[2]; 
    BYTE flag[2];                   //  (0x1)Used Entry  - (0x2)Folder Entry
    BYTE usedByte[4];
    BYTE UNTR2[24];
    BYTE id[4];
};
    //--------------------------

    //          UNTR

    //--------------------------
struct MFTAttributeHeader
{
    //Attribute Header (16 bytes)
    BYTE typeID[4];
    BYTE attrSize[4];
    BYTE nonRes[1];
    BYTE attrNameLeng[1];
    BYTE attrNameOffset[2];
    BYTE attrFlag[2];
    BYTE attrID[2];
    //--------------------------
    BYTE attrInfoSize[4];
    BYTE attrInfoOffset[2];         //offset from attribute header
    //--------------------------
};

void NTFS(BYTE sector[],wchar_t path[]);

void showVBR(BYTE sector[]);

void readEntry(BYTE sector[]);
void read_MFT_Header(BYTE sector[]);

void readAttribute(BYTE sector[]);
void read_Attr_Header(BYTE sector[]);
void read_Attr_$Info(BYTE sector[]);
void read_Attr_$FileName(BYTE sector[]);
void read_Attr_$Data(BYTE sector[]);

void showFileInfomation(FileInfo fi);
void initFileTree();
void displayFiles();
string getFiles(int idP);
int getNumChild(int id);
