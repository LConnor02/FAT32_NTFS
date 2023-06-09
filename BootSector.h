//#pragma once
//
//#include "Sector.h"
//#include "Disk.h"
//#include <memory>
//#include <string>
//// #include "NTFSBootSector.h"
//
//// class NTFSBootSector;
//
//
//class BootSector: public Sector {
//public:
//    static const int BYTES_PER_SECTOR_OFFSET = 0xB;
//    static const int SECTORS_PER_CLUSTER_OFFSET = 0x0D;
//    static const int FAT_COUNT_OFFSET = 0x10;
//    static const int SIZE = 512;
//
//protected:
//    BootSector();
//    BootSector(std::shared_ptr<Disk>);
//    ~BootSector();
//
//public:
//    static std::shared_ptr<BootSector> read(std::shared_ptr<Disk> device);
//    virtual std::string  getType() = 0;
//    virtual long getBytesPerSector() = 0;
//    virtual long getSectorsPerCluster() = 0;
//    virtual std::string toString() = 0;
//};
