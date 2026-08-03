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

    // Called periodically during a scan to ask "should I stop?".
    // Returning true stops the scan early -- this is treated as a normal
    // outcome (ScanResult.cancelled = true), never as an error.
    using CancellationCallback = std::function<bool()>;

    // Scans the given folder recursively and returns everything the
    // scan discovered -- not just the files, but counts of directories
    // seen and anything that had to be skipped.
    //
    // onProgress and onCancellationCheck are both optional -- pass
    // nothing (the defaults) and the scan behaves exactly as before,
    // with no progress reporting and no way to cancel early.
    static ScanResult scanFolder(
        const std::filesystem::path& folderPath,
        const ProgressCallback& onProgress = nullptr,
        const CancellationCallback& onCancellationCheck = nullptr
    );

private:
    // Converts an extension like ".EXE" or ".ExE" into ".exe", so files
    // that only differ by extension casing are treated as the same type
    // everywhere downstream (Analyzer, Reporter, etc).
    static std::string normalizeExtension(const std::string& extension);
};