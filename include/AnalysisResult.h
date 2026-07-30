#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include "FileInfo.h"
#include "DirectoryInfo.h"

// The contract between Analyzer and Reporter.
// Analyzer builds one of these; Reporter reads one of these.
// Pure data -- no behavior, so no .cpp file is needed for it (yet).
struct AnalysisResult {
    std::size_t totalFileCount = 0;
    std::uintmax_t totalSize = 0;

    // Scan-summary numbers, carried over from ScanResult so Reporter
    // can print them without needing to see ScanResult itself.
    std::size_t totalDirectories = 0;
    std::size_t skippedFiles = 0;
    std::size_t permissionDenied = 0;

    std::unordered_map<std::string, std::uintmax_t> sizeByExtension;
    std::unordered_map<std::string, std::size_t> countByExtension;

    // The largest topN files, sorted biggest first.
    // topLargestFiles.front() is the single largest file, if it's non-empty.
    std::vector<FileInfo> topLargestFiles;

    // The largest topN directories (by total size of files inside them),
    // sorted biggest first.
    std::vector<DirectoryInfo> topLargestDirectories;
};