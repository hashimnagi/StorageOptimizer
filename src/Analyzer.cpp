#include "Analyzer.h"
#include <algorithm>
#include <numeric>

// Calls each helper function in turn and collects everything into one
// AnalysisResult, and hands it back.
AnalysisResult Analyzer::analyze(const std::vector<FileInfo>& files, std::size_t topN) {
    AnalysisResult result;

    result.totalFileCount = files.size();
    result.totalSize = calculateTotalSize(files);
    result.sizeByExtension = groupByExtension(files);
    result.countByExtension = countExtensions(files);

    result.topLargestFiles = findTopLargestFiles(files, topN);

    return result;
}

// ---------------------------------------------------------
// Percentage helper -- works off an already-built AnalysisResult
// so it doesn't redo calculateTotalSize()/groupByExtension() again.
// ---------------------------------------------------------
double Analyzer::getExtensionPercentage(const AnalysisResult& result, const std::string& extension) {
    if (result.totalSize == 0) return 0.0;

    auto it = result.sizeByExtension.find(extension);
    if (it == result.sizeByExtension.end()) return 0.0;

    return (static_cast<double>(it->second) / static_cast<double>(result.totalSize)) * 100.0;
}

// ===========================================================
// Everything below is a private implementation detail of analyze().
// A caller outside this class can't reach these directly anymore.
// ===========================================================

// total storage used. Add up every file's size.
std::uintmax_t Analyzer::calculateTotalSize(const std::vector<FileInfo>& files) {
    return std::accumulate(
        files.begin(), files.end(),
        std::uintmax_t{ 0 },
        [](std::uintmax_t runningTotal, const FileInfo& file) {
            return runningTotal + file.getSize();
        }
    );
}

// storage used per extension.
std::unordered_map<std::string, std::uintmax_t> Analyzer::groupByExtension(const std::vector<FileInfo>& files) {
    std::unordered_map<std::string, std::uintmax_t> sizeMap;

    for (const auto& file : files) {
        sizeMap[file.getExtension()] += file.getSize();
    }

    return sizeMap;
}

// file count per extension.
std::unordered_map<std::string, std::size_t> Analyzer::countExtensions(const std::vector<FileInfo>& files) {
    std::unordered_map<std::string, std::size_t> countMap;

    for (const auto& file : files) {
        countMap[file.getExtension()] += 1;
    }

    return countMap;
}

// Returns the largest topN files, sorted from largest to smallest.
std::vector<FileInfo> Analyzer::findTopLargestFiles(const std::vector<FileInfo>& files, std::size_t topN) {
    std::vector<FileInfo> largestFiles = files;

    std::sort(
        largestFiles.begin(),
        largestFiles.end(),
        [](const FileInfo& a, const FileInfo& b) {
            return a.getSize() > b.getSize();
        }
    );

    if (topN < largestFiles.size()) {
        largestFiles.erase(largestFiles.begin() + topN, largestFiles.end());
    }

    return largestFiles;
}