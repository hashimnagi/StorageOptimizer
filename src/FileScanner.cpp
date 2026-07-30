#include"FileScanner.h"
#include<stdexcept>
#include<system_error>

ScanResult FileScanner::scanFolder(const std::filesystem::path& folderPath) {
    ScanResult result;

    if (!std::filesystem::exists(folderPath)) {  //check if path exists or not
        throw std::runtime_error("The specified path does not exist.");
    }

    if (!std::filesystem::is_directory(folderPath)) {  //here checking if actually is directory
        throw std::runtime_error("The specified path is not a directory.");
    }

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
                filePath.extension().string(),
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

    return result;
}