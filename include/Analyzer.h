#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <cstddef>
#include "FileInfo.h"

struct AnalysisResult {
    // kept because if there are zero files, there IS no largest file.
    std::optional<FileInfo> largestFile;

    std::uintmax_t totalSize = 0;
    std::unordered_map<std::string, std::uintmax_t> sizeByExtension;
    std::unordered_map<std::string, int> countByExtension;

    // Kept here so the Reporter never has to Resort or Reslice files itself.
    std::vector<FileInfo> filesSortedBySize;   // descending, biggest first
    std::vector<FileInfo> topLargestFiles;     // e.g. top 10, ready to print
};

// Stateless analyzer for computing storage statistics.
class Analyzer {
    public:
        // The topN controls how many files go into result.topLargestFiles.
        static AnalysisResult analyze(const std::vector<FileInfo>& files, std::size_t topN = 10);

        static double getExtensionPercentage(const AnalysisResult& result, const std::string& extension);

    private:

        static std::optional<FileInfo> findLargestFile(const std::vector<FileInfo>& files);

        static std::uintmax_t calculateTotalSize(const std::vector<FileInfo>& files);

        static std::unordered_map<std::string, std::uintmax_t> groupByExtension(const std::vector<FileInfo>& files);

        static std::unordered_map<std::string, int> countExtensions(const std::vector<FileInfo>& files);

        static std::vector<FileInfo> sortFilesBySize(const std::vector<FileInfo>& files, bool descending = true);

        static std::vector<FileInfo> getTopNLargestFiles(const std::vector<FileInfo>& sortedDescending, std::size_t n);
};