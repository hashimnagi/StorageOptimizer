#include"FileScanner.h"
#include<stdexcept>

std::vector<FileInfo> FileScanner::scanFolder(const std::filesystem::path& folderPath){
    std::vector<FileInfo> files;
    
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
            if (!entry.is_regular_file())
                continue;

            const auto& filePath = entry.path();

            files.emplace_back(
                filePath.filename().string(),
                filePath,
                filePath.extension().string(),
                entry.file_size()
            );
        }
        catch (const std::exception&)
        {
            // Skip this file and continue scanning.
            continue;
        }
    }
    return files;
}
