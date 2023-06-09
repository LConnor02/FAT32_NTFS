#pragma once
#include "FatDirectoryEntry.h"
#include "FatLfnDirectory.h"
#include <string>

class FatLfnDirectory;
class FatDirectoryEntry;

class FatLfnDirectoryEntry 
{
private:
    std::shared_ptr<FatLfnDirectory> parent;
    std::string fileName;

public:
    std::shared_ptr<FatDirectoryEntry> realEntry;
    ~FatLfnDirectoryEntry(){
        // delete parent;
    }
    // FatLfnDirectoryEntry(std::string name, ShortName sn, FatLfnDirectory* parent, bool directory);
    FatLfnDirectoryEntry(std::shared_ptr<FatLfnDirectory> parent, std::shared_ptr<FatDirectoryEntry> realEntry, std::string fileName);
    static std::shared_ptr<FatLfnDirectoryEntry> extract(std::shared_ptr<FatLfnDirectory> dir, int offset, int len);
public:
    bool isHiddenFlag();
    bool isSystemFlag();
    bool isReadOnlyFlag();
    bool isArchiveFlag();
    bool isFile();
    bool isDirectory();
public:
    std::string getName();
    std::shared_ptr<FatLfnDirectory> getParent();
    std::shared_ptr<FATFile> getFile();
    std::shared_ptr<FatLfnDirectory> getDirectory();
};