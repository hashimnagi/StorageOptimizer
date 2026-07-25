#include "Application.h"
#include <iostream>

int main() {
    std::cerr << "MAIN STARTED\n";

    Application app;

    std::cerr << "APPLICATION CREATED\n";

    app.run();

    std::cerr << "APPLICATION FINISHED\n";

    return 0;
}