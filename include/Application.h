#pragma once
#include <filesystem>
#include "ScanProgress.h"

class Application {
public:
    void run();

private:
    std::filesystem::path promptForFolder() const;

    // Passed to FileScanner as the progress callback. Prints live
    // counters to the console, overwriting the same line each time
    // via '\r' so it feels like one updating display, not scrolling text.
    static void printScanProgress(const ScanProgress& progress);
};