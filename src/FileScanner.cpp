#include"FileScanner.h"
#include<stdexcept>
#include<system_error>
#include<chrono>
#include<cctype>

ScanResult FileScanner::scanFolder(const std::filesystem::path& folderPath) {
    ScanResult result;

    if (!std::filesystem::exists(folderPath)) {  //check if path exists or not
        throw std::runtime_error("The specified path does not exist.");
    }

    if (!std::filesystem::is_directory(folderPath)) {  //here checking if actually is directory
        throw std::runtime_error("The specified path is not a directory.");
    }

    // steady_clock is the right tool for measuring elapsed time -- unlike
    // system_clock, it can never jump backward if the system clock changes
    // mid-scan, so the duration we compute is always trustworthy.
    const auto startTime = std::chrono::steady_clock::now();

    for (const auto& entry : std::filesystem::recursive_directory_iterator(
        folderPath,
        std::filesystem::directory_options::skip_permission_denied))
    {
        try
        {
            if (entry.is_directory())
            {
                result.totalDirectories++;
                continue;
            }

            if (!entry.is_regular_file())
                continue;

            const auto& filePath = entry.path();

            result.files.emplace_back(
                filePath.filename().string(),
                filePath,
                normalizeExtension(filePath.extension().string()),
                entry.file_size()
            );
            result.totalFiles++;
        }
        catch (const std::filesystem::filesystem_error& fsError)
        {
            // A filesystem_error carries an error_code -- if that code
            // specifically means "permission denied", count it separately
            // from other skipped files so the summary can tell them apart.
            if (fsError.code() == std::errc::permission_denied)
            {
                result.permissionDenied++;
            }
            else
            {
                result.skippedFiles++;
            }
        }
        catch (const std::exception&)
        {
            // Anything else (e.g. a bad Unicode filename) -- skip and continue.
            result.skippedFiles++;
        }
    }

    const auto endTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = endTime - startTime;
    result.scanDurationSeconds = elapsed.count();

    return result;
}

// Converts an extension to lowercase, e.g. ".EXE" -> ".exe".
// Note: std::tolower expects an unsigned char (or EOF) as input -- passing
// a plain 'char' directly is undefined behavior if it happens to be
// negative (which can occur with certain characters on some platforms).
// Casting to unsigned char first avoids that pitfall.
std::string FileScanner::normalizeExtension(const std::string& extension) {
    std::string result = extension;

    for (char& character : result) {
        character = static_cast<char>(
            std::tolower(static_cast<unsigned char>(character))
            );
    }

    return result;
}