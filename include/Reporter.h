#pragma once

#include <string>
#include <sstream>
#include <filesystem>
#include "AnalysisResult.h"


class Reporter {
public:
    // Builds the complete report as a string.
    static std::string buildReportText(const AnalysisResult& result);

    // Prints the report to the console.
    static void printReport(const AnalysisResult& result);

    // Saves the report to a text file.
    static void saveReport(const AnalysisResult& result,
                        const std::filesystem::path& outputFile);

private:
    // Appends the report summary.
    static void appendSummary(std::ostringstream& out,
                            const AnalysisResult& result);

    // Appends information about the largest file.
    static void appendLargestFile(std::ostringstream& out,
                                const AnalysisResult& result);

    // Appends storage usage grouped by file extension.
    static void appendExtensionStatistics(std::ostringstream& out,
                                        const AnalysisResult& result);

    // Appends the list of the largest files.
    static void appendTopFiles(std::ostringstream& out,
                            const AnalysisResult& result);

    // Converts raw bytes into KB, MB, GB, etc.
    static std::string formatSize(std::uintmax_t bytes);
};