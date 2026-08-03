#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"
#include <iostream>
#include <stdexcept>

void Application::run() {
    try {
        auto folder = promptForFolder();

        std::cout << "\nScanning...\n";
        auto scanResult = FileScanner::scanFolder(folder, printScanProgress);

        // The progress line ends without a newline (so it can be
        // overwritten in place) -- add one now so the report below
        // starts on its own clean line.
        std::cout << "\n\n";

        auto result = Analyzer::analyze(scanResult);
        Reporter::printReport(result);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
}

std::filesystem::path Application::promptForFolder() const {
    std::cout << "Enter the folder path to analyze: ";
    std::string input;
    std::getline(std::cin, input);
    return std::filesystem::path(input);
}

void Application::printScanProgress(const ScanProgress& progress) {
    // '\r' moves the cursor back to the start of the current line
    // without advancing to a new one, so each update overwrites the
    // previous numbers instead of scrolling the terminal.
    std::cout << "\rFiles discovered: " << progress.filesDiscovered
        << " | Directories visited: " << progress.directoriesVisited
        << " | Files skipped: " << progress.skippedFiles
        << " | Permission errors: " << progress.permissionDenied
        << std::flush;
}