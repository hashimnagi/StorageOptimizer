#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#endif

void Application::run() {
    try {
        auto folder = promptForFolder();

        std::cout << "\nScanning...  (Press ESC to cancel)\n";
        auto scanResult = FileScanner::scanFolder(folder, printScanProgress, checkForCancellation);

        // The progress line ends without a newline (so it can be
        // overwritten in place) -- add one now so whatever comes next
        // starts on its own clean line.
        std::cout << "\n\n";

        if (scanResult.cancelled) {
            // Cancellation is a normal outcome, not an error -- no
            // exception was thrown, so we just report it and stop here.
            // For this first version we deliberately do NOT analyze or
            // report on the partial results.
            std::cout << "Scan cancelled by user.\n";
            return;
        }

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

bool Application::checkForCancellation() {
#ifdef _WIN32
    // GetAsyncKeyState checks the CURRENT physical state of a key,
    // independent of console input focus/buffering -- exactly what we
    // want for "is the user holding/pressing ESC right now".
    return (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
#else
    // No portable equivalent to GetAsyncKeyState outside Windows.
    // On other platforms, cancellation is simply never triggered here --
    // this keeps the rest of the pipeline compiling and testable
    // everywhere, while the real key-based cancellation is Windows-only,
    // matching the project's actual target platform.
    return false;
#endif
}