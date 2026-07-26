#include "Application.h"
#include "FileScanner.h"
#include "Analyzer.h"
#include "Reporter.h"

#include <iostream>
#include <exception>

void Application::run() {
    try {
        auto folder = promptForFolder();

        std::cout << "\nScanning...\n";
        std::cout << "Please wait.\n\n";

        auto files = FileScanner::scanFolder(folder);

        std::cout << "Scan completed.\n\n";

        auto result = Analyzer::analyze(files);

        Reporter::printReport(result);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
}

std::filesystem::path Application::promptForFolder() const {
    std::cout << "=========================================\n";
    std::cout << "        Storage Optimizer v2\n";
    std::cout << "=========================================\n\n";

    std::cout << "Enter a folder OR drive to analyze.\n\n";

    std::cout << "Examples:\n";
    std::cout << "  C:\\\n";
    std::cout << "  D:\\\n";
    std::cout << "  C:\\Users\\WelCome\\Downloads\n\n";

    std::cout << "Path: ";

    std::string input;
    std::getline(std::cin, input);

    return std::filesystem::path(input);
}