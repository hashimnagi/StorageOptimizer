#pragma once
#include<string>
#include<filesystem>
#include"ScanResult.h"

class FileScanner {
public:
    // Scans the given folder recursively and returns everything the
    // scan discovered -- not just the files, but counts of directories
    // seen and anything that had to be skipped.
    static ScanResult scanFolder(const std::filesystem::path& folderPath);
};