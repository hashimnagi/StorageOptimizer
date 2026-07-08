#include"FileInfo.h"

FileInfo::FileInfo(const std::string& name,
        const std::filesystem::path& path,
        const std::string& extension,
        std::uintmax_t size) : name(name), path(path),extension(extension),size(size){}

const std::string& FileInfo::getName() const {
            return name;
}

const std::filesystem::path& FileInfo::getPath() const{
    return path;
}

const std::string& FileInfo::getExtension() const {
    return extension;
}

std::uintmax_t FileInfo::getSize() const {
    return size;
}