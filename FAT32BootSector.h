#pragma once


#include <string>
#include <sstream>
#include "Disk.h"
#include "Sector.h"

class FAT32BootSector: public Sector{
public:
    static const int BYTES_PER_SECTOR_OFFSET = 0xB;
    static const int SECTORS_PER_CLUSTER_OFFSET = 0x0D;
    static const int FAT_COUNT_OFFSET = 0x10;
    static const int SIZE = 512;

public:
    static const int SECTORS_PER_FAT_OFFSET = 0x24;
    static const int ROOT_DIR_FIRST_CLUSTER_OFFSET = 0x2C;
    static const int RESERVED_SECTORS_OFFSET = 14;
    static const int TOTAL_SECTORS_32_OFFSET = 32;

public:
    FAT32BootSector(){}
    FAT32BootSector(std::shared_ptr<Disk> device);
    ~FAT32BootSector(){}
public:
    static std::shared_ptr<FAT32BootSector> read(std::shared_ptr<Disk> device);
    long getBytesPerSector();
    long getSectorsPerCluster();
    std::string getType();
    std::string toString();

public:
    long getSectorsPerFat();
    long getSectorCount();
    long getNrReservedSectors();
    long getNrFats();
    long getRootDirFirstCluster();
    long getFatOffset(int fatNr);
    
    long getFilesOffset();
    long getRootDirOffset();

    int getBytesPerCluster();
    long getDataClusterCount();
    
private:
    long getDataSize();
};

