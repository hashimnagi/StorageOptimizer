#pragma once
#include <filesystem>

// Coordinates the application by calling each module in order.
// It doesn't perform scanning, analysis, or reporting itself.
class Application {
public:
    // Runs the complete application workflow.
    void run();

private:
    // Prompts the user for a folder to scan.
    std::filesystem::path promptForFolder() const;
};