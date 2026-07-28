#include "DirectoryInfo.h"

DirectoryInfo::DirectoryInfo(const std::filesystem::path& path, std::uintmax_t totalSize)
    : path(path), totalSize(totalSize) {
}

const std::filesystem::path& DirectoryInfo::getPath() const {
    return path;
}

std::uintmax_t DirectoryInfo::getTotalSize() const {
    return totalSize;
}