#include "Reporter.h"
#include "Analyzer.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <stdexcept>

// ---------------------------------------------------------
// THE public entry point for building report text.
// ---------------------------------------------------------
// This function does no formatting itself , it just calls each
// section helper in order. Read this function and you can read
// the whole shape of the report at a glance.
std::string Reporter::buildReportText(const AnalysisResult& result) {
    std::ostringstream out;

    out << "========== Storage Optimizer ==========\n\n";

    appendSummary(out, result);
    appendExtensionStatistics(out, result);
    appendTopFiles(out, result);

    return out.str();
}

// ---------------------------------------------------------
// Section: Total files / total size
// ---------------------------------------------------------
void Reporter::appendSummary(std::ostringstream& out, const AnalysisResult& result) {
    out << "Total Files: " << result.totalFileCount << "\n";
    out << "Total Size: " << formatSize(result.totalSize) << "\n\n";
}

// ---------------------------------------------------------
// Section: Storage used per extension, biggest first
// ---------------------------------------------------------
void Reporter::appendExtensionStatistics(std::ostringstream& out, const AnalysisResult& result) {
    out << "------------------------------------\n";
    out << "Storage by Extension\n\n";

    // The map itself has no guaranteed order, so we copy it into a vector
    // and sort that copy by size (descending) just for display purposes.
    std::vector<std::pair<std::string, std::uintmax_t>> extensionsBySize(
        result.sizeByExtension.begin(), result.sizeByExtension.end()
    );
    std::sort(
        extensionsBySize.begin(), extensionsBySize.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; }
    );

    for (const auto& entry : extensionsBySize) {
        const std::string& extension = entry.first;
        std::uintmax_t sizeForExtension = entry.second;

        // Reuse Analyzer's own percentage logic instead of recalculating
        // it here -- one source of truth for this calculation.
        double percentage = Analyzer::getExtensionPercentage(result, extension);

        // Files like "README" have no extension at all -- print something
        // readable instead of a blank label.
        const std::string displayName = extension.empty() ? "<no extension>" : extension;

        out << displayName << " : " << formatSize(sizeForExtension)
            << " (" << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }

    out << "\n";
}

// ---------------------------------------------------------
// Section: Top N largest files, numbered, with size and path
// ---------------------------------------------------------
void Reporter::appendTopFiles(std::ostringstream& out, const AnalysisResult& result) {
    out << "------------------------------------\n";
    out << "Top " << result.topLargestFiles.size() << " Largest Files\n\n";

    if (result.topLargestFiles.empty()) {
        out << "(no files found)\n";
        return;
    }

    std::size_t rank = 1;
    for (const auto& file : result.topLargestFiles) {
        out << rank << ". " << file.getName() << "\n";
        out << "   Size : " << formatSize(file.getSize()) << "\n";
        out << "   Path : " << file.getPath().string() << "\n\n";
        rank++;
    }
}

// ---------------------------------------------------------
// Formatting helper: raw bytes -> human readable string
// ---------------------------------------------------------
// Logic: keep dividing by 1024 while the number is still big,
// moving up through B -> KB -> MB -> GB -> TB, then print with
// 2 decimal places and the matching unit.
std::string Reporter::formatSize(std::uintmax_t bytes) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    double size = static_cast<double>(bytes);
    int unitIndex = 0;

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
}

// ---------------------------------------------------------
// Send the report to the console.
// ---------------------------------------------------------
void Reporter::printReport(const AnalysisResult& result) {
    std::cout << buildReportText(result);
}

// ---------------------------------------------------------
// Send the report to a file instead.
// Same text as printReport() ,just a different destination.
// ---------------------------------------------------------
void Reporter::saveReport(const AnalysisResult& result, const std::filesystem::path& outputFile) {
    std::ofstream fileStream(outputFile);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + outputFile.string());
    }

    fileStream << buildReportText(result);
}