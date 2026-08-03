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

private:
    // Converts an extension like ".EXE" or ".ExE" into ".exe", so files
    // that only differ by extension casing are treated as the same type
    // everywhere downstream (Analyzer, Reporter, etc).
    static std::string normalizeExtension(const std::string& extension);
};