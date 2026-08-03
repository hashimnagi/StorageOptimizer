#pragma once
#include <string>
#include <sstream>
#include <filesystem>
#include "AnalysisResult.h"

// Reporter is stateless -- just like FileScanner and Analyzer.
// It never stores an AnalysisResult or a vector<FileInfo> as a member.
// Its only job: turn an AnalysisResult into readable text, and send
// that text somewhere (console or file).
class Reporter {
public:
    // Builds the full human-readable report as a single string.
    static std::string buildReportText(const AnalysisResult& result);

    // Convenience: builds the report and prints it straight to std::cout.
    static void printReport(const AnalysisResult& result);

    // Convenience: builds the report and writes it to a file instead.
    // Same text as printReport() -- just a different destination.
    static void saveReport(const AnalysisResult& result, const std::filesystem::path& outputFile);

private:
    // Each of these writes ONE section of the report into 'out'.
    // buildReportText() just calls them in order -- it doesn't
    // format anything itself.
    static void appendSummary(std::ostringstream& out, const AnalysisResult& result);
    static void appendScanSummary(std::ostringstream& out, const AnalysisResult& result);
    static void appendTopDirectories(std::ostringstream& out, const AnalysisResult& result);
    static void appendExtensionStatistics(std::ostringstream& out, const AnalysisResult& result);
    static void appendTopFiles(std::ostringstream& out, const AnalysisResult& result);

    // Formatting helper: turns raw bytes into something readable,
    // e.g. 112300000000 -> "112.30 GB"
    static std::string formatSize(std::uintmax_t bytes);
};