#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"
#include <iostream>
#include <stdexcept>

void Application::run() {
    try {
        auto folder = promptForFolder();
        auto scanResult = FileScanner::scanFolder(folder);
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