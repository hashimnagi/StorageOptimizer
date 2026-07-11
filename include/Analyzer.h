#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <cstddef>
#include "FileInfo.h"
#include"AnalysisResult.h"

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

        static std::unordered_map<std::string, std::size_t> countExtensions(const std::vector<FileInfo>& files);

        static std::vector<FileInfo> sortFilesBySize(const std::vector<FileInfo>& files, bool descending = true);

        static std::vector<FileInfo> getTopNLargestFiles(const std::vector<FileInfo>& sortedDescending, std::size_t n);
};