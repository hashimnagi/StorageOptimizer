# pragma once
#include<string>
#include<filesystem>
#include<vector>
#include"FileInfo.h"

class FileScanner{
    public:
    static std::vector<FileInfo> scanFolder(const std::filesystem::path& folderPath);

};