//#include <FileSystem.h>
#define _CRT_SECURE_NO_WARNINGS
#include "FAT32.h"
#include "Disk.h"
#include "NTFS.h"
#include "Byte.h"
#include <windows.h>
//#include <NTFS.h>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
int main()
{   
    BYTE sector[1024];
    wchar_t driver[3];
    wchar_t path[10] = L"\\\\.\\";
    printf("================== PLEASE ENTER YOUR VOLUMN ==================\n");
    printf("Enter one CAPITAL LETTER. Example: C, D, E, ...\n\n");
    printf(">> Enter your drive path: ");


    wcin >> driver;
   
    wcscat(driver, L":");
    wcscat(path, driver);

    wcout << driver << endl;

    std::shared_ptr<Disk> usb = Disk::create(path);

    ReadSector(path, 0, sector);

    if (strcmp(byte2PChar(sector + 3, 4), "NTFS") == 0)
    {
        NTFS(sector, driver);
    }
    else
    {
        std::shared_ptr<FAT32> file_system;
        try {
            file_system = FAT32::read(usb);
            // std::cout << file_system->getRoot()->getEntry("kh.txt")->getName();




            // file_system->getRoot()->getDirectory(entry)->showDirectory();

            system("pause");
        }
        catch (std::exception e) {
            std::cout << e.what();
        }

        auto Fat32BS = file_system->getFAT32BootSector();
        std::cout << std::endl << Fat32BS->toString();

        std::cout << file_system->getVolumeLabel() << std::endl;
        std::cout << "\tName\tSize\tAttr\tSector\n";
        std::cout << file_system->getRoot()->showDirectory();
        std::string cmd = "start";
        std::shared_ptr <FatLfnDirectory> currentDir = file_system->getRoot();
        std::shared_ptr <FatLfnDirectoryEntry> entry = 0;
        std::shared_ptr <FatLfnDirectoryEntry> tempParentEntry = 0;
        std::vector <long> NameStr;
        while (true) {
            std::cout << ">>";
            std::cin >> cmd;

            if (cmd == "do") {
                std::cin.ignore();
                std::string TheName;
                std::getline(std::cin, TheName);

                /*if(currentDir!=0){
                    tempParentEntry = entry;
                    entry = currentDir->getEntry(TheName);
                }
                else {
                    entry = file_system->getRoot()->getEntry(TheName);
                }*/

                entry = currentDir->getEntry(TheName);

                if (entry->isDirectory()) {

                    currentDir = entry->getDirectory();

                    if (currentDir->isFreeName(".")) {
                        currentDir = file_system->getRoot();
                    }
                    std::cout << "\tName\tSize\tAttr\tSector\n";
                    std::cout << currentDir->showDirectory();
                }
                else if (entry->isFile()) {
                    //auto file = entry->getFile();
                    //BYTE* result = new BYTE[file->getLength()];
                    //file->read(0, result, file->getLength());
                    //char* cstr = new char[file->getLength() + 1];
                    //cstr[file->getLength()] = '\0';
                    //memcpy(cstr, result, file->getLength());
                    //std::cout << std::string(cstr);
                    //delete result;
                    ////delete cstr;
                    auto file = entry->getFile();
                    long size = file->getLength();
                    long bps = file_system->getFAT32BootSector()->getBytesPerSector();
                    size = (int)(size / file_system->getFAT32BootSector()->getBytesPerSector() + 1);
                    size *= bps;

                    BYTE* result = new BYTE[size];
                    file->read(0, result, size);
                    char* cstr = new char[file->getLength() + 1];
                    cstr[file->getLength()] = '\0';
                    memcpy(cstr, result, file->getLength());
                    std::cout << std::string(cstr);
                    delete[] result;
                    delete[] cstr;
                }
            }
            else if (cmd == "exit" || cmd == "Exit") {
                break;
            }
        }


        system("pause");
        return 0;
    }

    
}