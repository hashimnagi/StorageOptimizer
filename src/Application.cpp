#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"

#include <iostream>
#include <exception>

void Application::run() {
    try {
        auto folder = promptForFolder();

        auto files = FileScanner::scanFolder(folder);

        auto result = Analyzer::analyze(files);

        Reporter::printReport(result);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
}

std::filesystem::path Application::promptForFolder() const {
    std::cout << "Enter the folder path to analyze: " << std::flush;

    std::string input;
    std::getline(std::cin, input);

    return std::filesystem::path(input);
}