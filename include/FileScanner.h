# pragma once
#include<string>
#include<filesystem>
#include<vector>
#include"FileInfo.h"

class FileScanner{
    public:
    // Scans the given folder recursively and returns metadata
    // for every regular file that is found.
    static std::vector<FileInfo> scanFolder(const std::filesystem::path& folderPath);

};