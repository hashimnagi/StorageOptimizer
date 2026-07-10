#include "Analyzer.h"
#include <algorithm>
#include <numeric>


// it calls each helper function in a turn and collects everything into one
// AnalysisResult, and hand it back.
AnalysisResult Analyzer::analyze(const std::vector<FileInfo>& files, std::size_t topN) {
    AnalysisResult result;

    result.largestFile = findLargestFile(files);
    result.totalSize = calculateTotalSize(files);
    result.sizeByExtension = groupByExtension(files);
    result.countByExtension = countExtensions(files);
    result.filesSortedBySize = sortFilesBySize(files, true);
    result.topLargestFiles = getTopNLargestFiles(result.filesSortedBySize, topN);

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

// largest file. Walks through every file, keep the biggest seen so far.
std::optional<FileInfo> Analyzer::findLargestFile(const std::vector<FileInfo>& files) {
    if (files.empty()) {
        return std::nullopt; 
    }

    auto largestIt = std::max_element(
        files.begin(), files.end(),
        [](const FileInfo& a, const FileInfo& b) {
            return a.getSize() < b.getSize();
        }
    );

    return *largestIt;
}

// total storage used. Add up every file's size.
std::uintmax_t Analyzer::calculateTotalSize(const std::vector<FileInfo>& files) {
    return std::accumulate(
        files.begin(), files.end(),
        std::uintmax_t{0},
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

// sort files by size. Copies first so the original order is untouched.
std::vector<FileInfo> Analyzer::sortFilesBySize(const std::vector<FileInfo>& files, bool descending) {
    std::vector<FileInfo> sortedFiles = files;

    std::sort(
        sortedFiles.begin(), sortedFiles.end(),
        [descending](const FileInfo& a, const FileInfo& b) {
            if (descending) {
                return a.getSize() > b.getSize();
            }
            return a.getSize() < b.getSize();
        }
    );

    return sortedFiles;
}

// top N largest files. Takes an already sorted descending vector
// (analyze() passes result.filesSortedBySize in) and just slices the front.
std::vector<FileInfo> Analyzer::getTopNLargestFiles(const std::vector<FileInfo>& sortedDescending, std::size_t n) {
    if (n > sortedDescending.size()) {
        n = sortedDescending.size();
    }

    return std::vector<FileInfo>(sortedDescending.begin(), sortedDescending.begin() + n);
}