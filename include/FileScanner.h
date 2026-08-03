#pragma once
#include<string>
#include<filesystem>
#include<functional>
#include"ScanResult.h"
#include"ScanProgress.h"

class FileScanner {
public:
    // Called periodically during a scan so a caller (Application, a GUI,
    // etc.) can show live progress. FileScanner never prints anything
    // itself -- it just reports what it's found; the caller decides how
    // (or whether) to display it.
    using ProgressCallback = std::function<void(const ScanProgress&)>;

    // Scans the given folder recursively and returns everything the
    // scan discovered -- not just the files, but counts of directories
    // seen and anything that had to be skipped.
    //
    // onProgress is optional -- pass nothing (the default) and the scan
    // behaves exactly as before, with no progress reporting at all.
    static ScanResult scanFolder(
        const std::filesystem::path& folderPath,
        const ProgressCallback& onProgress = nullptr
    );

private:
    // Converts an extension like ".EXE" or ".ExE" into ".exe", so files
    // that only differ by extension casing are treated as the same type
    // everywhere downstream (Analyzer, Reporter, etc).
    static std::string normalizeExtension(const std::string& extension);
};