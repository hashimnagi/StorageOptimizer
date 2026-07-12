#include "Reporter.h"
#include "Analyzer.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <stdexcept>

// Builds the complete report by combining all report sections.
std::string Reporter::buildReportText(const AnalysisResult& result) {
    std::ostringstream out;

    out << "========== Storage Optimizer ==========\n\n";

    appendSummary(out, result);
    appendLargestFile(out, result);
    appendExtensionStatistics(out, result);
    appendTopFiles(out, result);

    return out.str();
}

// Writes the overall scan summary.
void Reporter::appendSummary(std::ostringstream& out, const AnalysisResult& result) {
    out << "Total Files: " << result.filesSortedBySize.size() << "\n";
    out << "Total Size: " << formatSize(result.totalSize) << "\n\n";
}

// Writes information about the largest file found.
void Reporter::appendLargestFile(std::ostringstream& out, const AnalysisResult& result) {
    out << "Largest File:\n";
    out << "------------------------------------\n";

    if (result.largestFile.has_value()) {
        const FileInfo& largest = result.largestFile.value();
        out << largest.getName() << "\n";
        out << formatSize(largest.getSize()) << "\n";
        out << largest.getPath().string() << "\n";
    } else {
        out << "(no files found)\n";
    }

    out << "\n";
}

// Groups extensions by storage usage and displays the largest first.
void Reporter::appendExtensionStatistics(std::ostringstream& out, const AnalysisResult& result) {
    out << "------------------------------------\n";
    out << "Storage by Extension\n\n";

    // unordered_map has no ordering, so copy the data into a vector
    // and sort it before displaying.
    std::vector<std::pair<std::string, std::uintmax_t>> extensionsBySize(
        result.sizeByExtension.begin(), result.sizeByExtension.end()
    );

    std::sort(
        extensionsBySize.begin(),
        extensionsBySize.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        }
    );

    for (const auto& entry : extensionsBySize) {
        const std::string& extension = entry.first;
        std::uintmax_t sizeForExtension = entry.second;

        // Reuse the Analyzer's calculation so the percentage
        // is computed consistently throughout the project.
        double percentage = Analyzer::getExtensionPercentage(result, extension);

        // Some files don't have an extension (e.g. README).
        const std::string displayName = extension.empty() ? "<no extension>" : extension;

        out << displayName << " : "
            << formatSize(sizeForExtension)
            << " ("
            << std::fixed
            << std::setprecision(1)
            << percentage
            << "%)\n";
    }

    out << "\n";
}

// Writes the largest files in descending order.
void Reporter::appendTopFiles(std::ostringstream& out, const AnalysisResult& result) {
    out << "------------------------------------\n";
    out << "Top " << result.topLargestFiles.size() << " Largest Files\n\n";

    std::size_t rank = 1;

    for (const auto& file : result.topLargestFiles) {
        out << rank << ". "
            << file.getName()
            << " - "
            << formatSize(file.getSize())
            << "\n";

        rank++;
    }
}

// Converts a byte count into a readable size (KB, MB, GB).
std::string Reporter::formatSize(std::uintmax_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};

    double size = static_cast<double>(bytes);
    int unitIndex = 0;

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::ostringstream oss;
    oss << std::fixed
        << std::setprecision(2)
        << size
        << " "
        << units[unitIndex];

    return oss.str();
}

// Prints the generated report to the console.
void Reporter::printReport(const AnalysisResult& result) {
    std::cout << buildReportText(result);
}

// Saves the generated report to a text file.
void Reporter::saveReport(const AnalysisResult& result,
                        const std::filesystem::path& outputFile) {
    std::ofstream fileStream(outputFile);

    if (!fileStream.is_open()) {
        throw std::runtime_error(
            "Could not open file for writing: " + outputFile.string()
        );
    }

    fileStream << buildReportText(result);
}