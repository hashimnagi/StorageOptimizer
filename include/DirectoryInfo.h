#pragma once
#include <filesystem>
#include <cstdint>

// Represents one directory and the total size of all files inside it
// (including subdirectories' files, since we accumulate by parent_path()).
// Deliberately minimal for now, exactly like FileInfo started out.
// Later this can grow: fileCount, subdirectoryCount, percentage, etc.
class DirectoryInfo {
private:
    std::filesystem::path path;
    std::uintmax_t totalSize;

public:
    DirectoryInfo(const std::filesystem::path& path, std::uintmax_t totalSize);

    const std::filesystem::path& getPath() const;
    std::uintmax_t getTotalSize() const;
};
