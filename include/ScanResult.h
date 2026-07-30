#pragma once
#include <vector>
#include <cstddef>
#include "FileInfo.h"

// The contract between FileScanner and Analyzer.
// FileScanner will build one of these; Analyzer will read one of these.
// Pure data, no behavior, so no .cpp file is needed for it (yet),
// exactly like AnalysisResult.
struct ScanResult {
    std::vector<FileInfo> files;

    std::size_t fileCount = 0;
    std::size_t directoryCount = 0;
    std::size_t skippedFiles = 0;
    std::size_t permissionDenied = 0;
};