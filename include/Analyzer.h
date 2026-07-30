#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include "FileInfo.h"
#include "DirectoryInfo.h"
#include "ScanResult.h"
#include "AnalysisResult.h"

// Analyzer is intentionally STATELESS.
// It owns no data -- every function takes the files it needs to work on
// as a parameter, and returns a fresh answer. Nothing is stored between calls.
class Analyzer {
public:
    // The ONE public entry point most callers will use:
    //     auto result = Analyzer::analyze(scanResult, 10);
    // topN controls how many files/directories go into the top-N lists.
    static AnalysisResult analyze(const ScanResult& scanResult, std::size_t topN = 10);

    // Useful on its own once you already have a result, so it doesn't
    // have to recompute totalSize / sizeByExtension from scratch.
    static double getExtensionPercentage(const AnalysisResult& result, const std::string& extension);

private:
    // These are implementation details of analyze() -- not part of the
    // public API. A caller never needs to touch these directly.
    static std::uintmax_t calculateTotalSize(const std::vector<FileInfo>& files);
    static std::unordered_map<std::string, std::uintmax_t> groupByExtension(const std::vector<FileInfo>& files);
    static std::unordered_map<std::string, std::size_t> countExtensions(const std::vector<FileInfo>& files);

    // Copies 'files', sorts the copy descending by size, and keeps
    // only the first topN of them (or fewer, if there aren't that many).
    static std::vector<FileInfo> findTopLargestFiles(const std::vector<FileInfo>& files, std::size_t topN);

    // Groups files by their parent directory, sums each directory's
    // total size, and returns the topN largest directories.
    static std::vector<DirectoryInfo> findTopLargestDirectories(const std::vector<FileInfo>& files, std::size_t topN);
};