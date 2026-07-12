#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"
#include <iostream>
#include <exception>

// Runs the complete application workflow.
void Application::run() {
    try {
        auto folder = promptForFolder();

        auto files = FileScanner::scanFolder(folder);

        auto result = Analyzer::analyze(files);

        Reporter::printReport(result);
    }
    catch (const std::exception& ex) {
        // Report any error that occurs while running the application.
        std::cerr << "Error: " << ex.what() << '\n';
    }
}

// Prompts the user for the folder to analyze.
std::filesystem::path Application::promptForFolder() const {
    std::cout << "Enter the folder path to analyze: ";

    std::string input;
    std::getline(std::cin, input);

    return std::filesystem::path(input);
}