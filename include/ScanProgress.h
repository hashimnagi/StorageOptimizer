#pragma once
#include <cstddef>

// A snapshot of how a scan is progressing, reported periodically by
// FileScanner while it works. Pure data -- no behavior, so no .cpp
// file is needed for it, same as ScanResult and AnalysisResult.
//
// Deliberately has NO percentage field: a recursive filesystem scan
// doesn't know the total amount of work in advance, so a percentage
// here would just be a guess dressed up as a fact.
struct ScanProgress {
    std::size_t filesDiscovered = 0;
    std::size_t directoriesVisited = 0;
    std::size_t skippedFiles = 0;
    std::size_t permissionDenied = 0;
};