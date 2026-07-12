#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include "FileInfo.h"

// The contract between Analyzer and Reporter.
// Analyzer builds one of these; Reporter reads one of these.
// Pure data -- no behavior, so no .cpp file is needed for it (yet).
struct AnalysisResult {
    // optional because if there are zero files, there IS no largest file.
    std::optional<FileInfo> largestFile;

    std::uintmax_t totalSize = 0;
    std::unordered_map<std::string, std::uintmax_t> sizeByExtension;
    std::unordered_map<std::string, std::size_t> countByExtension;

    // Kept here so the Reporter never has to re-sort or re-slice files itself.
    std::vector<FileInfo> filesSortedBySize;   // descending, biggest first
    std::vector<FileInfo> topLargestFiles;     // e.g. top 10, ready to print
};