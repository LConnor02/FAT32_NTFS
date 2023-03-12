#define _CRT_SECURE_NO_WARNINGS
#include "NTFS.h"
#include <iterator>

//------VBS------
VBR *vbr;
//-----MFT-------
MFTEntryHeader *meh;
//-----Attr------
MFTAttributeHeader *mah;

int secPerClus;
wchar_t headPath[10] = L"\\\\.\\";
unsigned int byteRead;  //read bytes from ATTRIBUTE HEADER 
//store entire File Infomation
FileInfo file;
vector<FileInfo> files;

vector<FileInfo> root;
vector< vector<FileInfo> > preShow;
vector<FileInfo> curShow;
void NTFS(BYTE sector[],wchar_t driver[])
{
    showVBR(sector);

    wcscat(headPath,driver);
    readEntry(sector);
    initFileTree();
    
    fflush(stdin);

    string choice;
    string d;
    vector<string> path;
    for(int i = 0; i < 2;i++)
        d += driver[i];
    path.push_back(d);

    while(true)
    {
        cout << endl;
        for(int i = 0; i < path.size();i++)
            cout << "\\" << path[i]; 
        wcout << L"> ";
        getline(cin,choice);
        
        string pref = split(choice," ")[0];

        if(pref == "ls")
        {
            path.resize(1);
            curShow = root;
            preShow.clear();
            preShow.resize(0);
            displayFiles();
        }
        else if(pref == "exit")
        {
            break;
        }
        else if(pref == "gt")
        {
            if(split(choice," ").size() > 1)
            {
                string suf = split(choice," ")[1];
                string sub = getFiles(atoi(suf.c_str()));
                if(sub != "?")
                {
                    path.push_back(sub);
                    displayFiles();
                }
            }
            else 
                cout << "# Error input #\n";
            
        }
        else if(pref == "rt" && path.size() > 1)
        {
            path.pop_back();
            curShow = preShow[preShow.size() - 1];
            preShow.pop_back();

            displayFiles();
        }
        else if(pref == "help")
        {
            cout << "ls: Display root list files.\n";
            cout << "gt <id>: Get folder list files by id.\n";
            cout << "rt: Return directory.\n";
            cout << "cls: Clear screen.\n";
            cout << "exit: Exit program.\n";
        }
        else if(pref == "")  {}
        else if(pref == "cls")  {system("cls");} 
        else 
        {
            cout << "Invalid command.\n";
        }
        
    }
    

    delete[] vbr;
    delete[] meh;
    delete[] mah;
}
//READ-PARTITION
//-------------------------------------------------------------------------------------------------------
void showVBR(BYTE sector[])
{
    vbr = (VBR *)sector;

    printf("\nPartition info:\n\n");

    char* oem = byte2PChar(vbr->oem,6);

    //show VBR
    printf("File System: %s\n",oem);
    printf("Bytes per sector: %d\n",reverseByte(vbr->bytePerSector,2));
    printf("Sectors by cluster (Sc): %d\n",reverseByte(vbr->sectorByCluster,1));
    printf("Sectors of Disk: %d\n",reverseByte(vbr->sectorOfDisk,8));
    printf("MFT begin sector: %d\n",reverseByte(vbr->sectorByCluster,1)*reverseByte(vbr->startClusterMFT,8));
    printf("MFT entry size: %d\n\n",reverseByte(vbr->MFTEntrySize,1));

    delete[] oem;
}
//READ-ENTRY ~ READ-FILE
//-------------------------------------------------------------------------------------------------------
void readEntry(BYTE sector[])
{
    secPerClus = reverseByte(vbr->sectorByCluster,1);
    unsigned int mbs = reverseByte(vbr->startClusterMFT,8)*secPerClus; // MFT Entry Begin Sector

    for(int i = 0; i < 256; i++) // Read 256 entries
    {
        ReadSectorEx(headPath, mbs, sector);

        mbs += 2;  //next entry (1 entry = 2 sector)
        file.begSector = i*2;

        if(reverseByte(sector,4) == 0)
            continue;

        read_MFT_Header(sector);
        readAttribute(sector);

        files.push_back(file);
    }
}
void read_MFT_Header(BYTE sector[])
{
    meh = (MFTEntryHeader*)sector;  //Start Point
    
    // SET UP
    file.data = "";
    file.size = 0;
    file.idP = -1;
    file.id = -1;


    file.id = reverseByte((BYTE*)meh + 0x2C,4);
    file.isUsed = reverseByte(meh->flag,2);

    //update Read Bytes
    byteRead = 56;
}
void readAttribute(BYTE sector[])
{
    bool endFile = false;
    while(!endFile)
    {
        //read Attribute Header
        read_Attr_Header(sector);

        int typeID = reverseByte(mah->typeID,4);
        if(typeID > 256 || typeID < 1 || byteRead > 1000)
            break;

        //read Attribute Infomation
        switch(typeID)
        {
            case 16:
                read_Attr_$Info(sector);
                break;
            case 48:
                read_Attr_$FileName(sector);
                break;
            case 128:
                read_Attr_$Data(sector);
                break;
            case 0xFFFF:
                endFile = true;
                break;
            default:
                break;
        }
        byteRead += reverseByte(mah->attrSize,4) - reverseByte(mah->attrInfoOffset,2);
    }
}
void read_Attr_Header(BYTE sector[])
{
    mah = (MFTAttributeHeader*)(sector + byteRead);

    byteRead += reverseByte(mah->attrInfoOffset,2);
} 
void read_Attr_$Info(BYTE sector[])
{   
    BYTE *mai = sector + byteRead;  //Start Point

    //read
    //--Created Time
    filetime2Date(mai,file.created);
    
    //--File Status
    file.flag = reverseByte(mai + 32,4);

    //--File/Folder 
    if(file.flag == 0)
        file.isFile = 0;
    else
        file.isFile = 1;
}
void read_Attr_$FileName(BYTE sector[])
{
    BYTE *maf = sector + byteRead;  //Start Point
    
    //show File Name
    file.name = "";

    char *name = byte2PChar(maf + 66,reverseByte(mah->attrInfoSize,4)-66);
    file.name = char2String(name,reverseByte(mah->attrInfoSize,4)-66);

    file.idP = reverseByte(maf,6);

    delete[] name;
}
void read_Attr_$Data(BYTE sector[])
{
    BYTE *mad = sector + byteRead;

    // If non-res
    if(reverseByte(mah->nonRes,2) == 1)
    {
        mad -= 6;
        BYTE byte[1024];
        file.size = reverseByte((BYTE*)mah + 48,8);
        ReadSectorEx(L"\\\\.\\E:",reverseByte((BYTE*)mah + 66,2) * secPerClus,byte);
        if(file.size < 1024)
            file.data = byte2PChar(byte,file.size);
        else
        {
            file.data = byte2PChar(byte,1024);
            file.data += "...to be continued";
        }
        return;
    }

    file.size = reverseByte(mah->attrInfoSize,4);

    if(file.size == 0)
    {
        return;
    }

    char* data = byte2PChar(mad,file.size);
    file.data = data;
}
//SHOW-TIME
//-------------------------------------------------------------------------------------------------------
void showFileInfomation(FileInfo fi)
{
    cout << "\n-------------------------------------------\n";
    cout << "ID: " << fi.id << endl;
    cout << "File name: " << fi.name << endl;
    cout << "Created: " << showDate(fi.created) << endl;
    cout << "Begin Sector: " << fi.begSector << endl;
    cout << "Status: ";
    if(fi.flag >= 0x20 && fi.flag <= 0x22)
    {
        cout << "archived";
        if(fi.flag == 0x21)
            cout << ", read-only";
        if(fi.flag == 0x22)
            cout << ", hidden";
    }
    else if(fi.flag >= 0 && fi.flag <= 0x2)
    {
        cout << "folder";
        if(fi.flag == 0x1)
            cout << ", read-only";
        if(fi.flag == 0x2)
            cout << ", hidden";
    }
    else
        cout << "undefined";
    cout << endl;
    if(fi.size > 0)
        cout << "Size: " << fi.size << " bytes\n"; 
    if(!fi.isFile)
    {
        cout << "Contained: " << getNumChild(fi.id) << " subfiles\n";
    }
    else if (fi.size > 0)
        cout << "Data: \n" << fi.data;
    cout << "\n-------------------------------------------\n";
}
int getNumChild(int id)
{
    int count = 0;
    for(int i = 0; i < files.size();i++)
        if(files[i].idP == id)
            count++;

    return count;
}
string getFiles(int idP)
{
    bool haveID = false;
    string Pname;
    for(int i = 0; i < files.size();i++)
        if(files[i].id == idP)
        {
            Pname = files[i].name;
            haveID = true;
            break;
        }
    if(!haveID)
        return "?";

    preShow.push_back(curShow);

    curShow.clear(); curShow.resize(0);
    for(int i = 0; i < files.size();i++)
        if(files[i].idP == idP)
            curShow.push_back(files[i]);

    return Pname;
}
void displayFiles()
{
    for(int i = 0; i < curShow.size();i++)
    {
        showFileInfomation(curShow[i]);
    }
}
void initFileTree()
{
    for(int i = 0; i < files.size();i++)
        if(files[i].idP <= 5 && (files[i].flag > 31 || files[i].flag == 0) && files[i].id > 10)
            root.push_back(files[i]);

    curShow = root;
}
//-------------------------------------------------------------------------------------------------------
