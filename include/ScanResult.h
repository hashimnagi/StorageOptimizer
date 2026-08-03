#pragma once
#include <vector>
#include <cstddef>
#include "FileInfo.h"

// The contract between FileScanner and Analyzer.
// FileScanner builds one of these; Analyzer will read one of these.
// Pure data -- no behavior, so no .cpp file is needed for it (yet),
// exactly like AnalysisResult.
struct ScanResult {
    std::vector<FileInfo> files;

    std::size_t totalFiles = 0;
    std::size_t totalDirectories = 0;
    std::size_t skippedFiles = 0;
    std::size_t permissionDenied = 0;

    // How long the scan itself took, measured by FileScanner.
    double scanDurationSeconds = 0.0;
};