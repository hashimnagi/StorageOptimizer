#pragma once
#include<string>
#include<filesystem>
#include<cstdint>

class FileInfo{
private:
    std::string name;
    std::filesystem::path path;
    std::string extension;
    std::uintmax_t size;   
public:
    FileInfo(const std::string& name,
        const std::filesystem::path& path,
        const std::string& extension,
        std::uintmax_t size);
    const std::string& getName() const;
    const std::filesystem::path& getPath() const;
    const std::string& getExtension() const;
    std::uintmax_t getSize() const;


};