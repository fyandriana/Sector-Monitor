// Sector-Monitor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Monitor.h"

int main()
{
    Monitor m;
    std::string filepath = "sectors.csv";
    m.run(filepath);
}
