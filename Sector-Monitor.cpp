// Sector-Monitor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Monitor.h"
#include <filesystem>
#include <string>

int main(int argc, char* argv[])
{
    try {
        Monitor m;
        std::filesystem::path filepath = (argc > 1) ? argv[1] : "sectors.csv";

        std::cout << "Sector Monitoring\n";
        std::cout << std::string(17, '-');
        std::cout << "\n";
        std::cout << "File: " << filepath << "\n\n";
        if (!std::filesystem::exists(filepath)) {
            throw std::runtime_error("File does not exist or restricted\n");
        }

        m.run(std::filesystem::absolute(filepath));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    
}