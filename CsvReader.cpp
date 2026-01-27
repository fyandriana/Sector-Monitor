#include "CsvReader.h"
#include <algorithm>
#include <sstream>
#include <utility>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <format>
#include <stdexcept>
#include <cctype>

CsvReader::CsvReader(char sep):m_sep(sep)
{
}

std::string CsvReader::trim(const std::string& field) const
{
   // find first space
    auto start = std::find_if_not(field.begin(), field.end(),
        [](unsigned char c) {
            return std::isspace(c);
        });
   //find last space
    auto end = std::find_if_not(field.rbegin(), field.rend(),
        [](unsigned char c){
            return std::isspace(c);
        }
    ).base();

    return (start < end) ? std::string(start, end) : std::string{};
}


Row CsvReader::readOne(const std::string& line, const int n) const
{
    Row csvRow;
    std::string field;
    std::istringstream iss(line);
    std::vector<std::string> fields;
    fields.reserve(4);
    while (std::getline(iss, field, m_sep)) {
        fields.push_back(trim(field));
    }
    if (fields.size() != 4) {
        throw std::invalid_argument(std::format("Invalid column count at line {} expected 4 got {}", n, fields.size()));
    }
   
    csvRow.line = n;
    csvRow.timestamp = fields[0];
    csvRow.sectorId = fields[1];
    csvRow.capacity = std::stoi(fields[2]);
    csvRow.aircraft_count = std::stoi(fields[3]);
    return csvRow;
}

std::vector<Row> CsvReader::readAll(const std::string& filepath) const
{
    std::ifstream csvFile(std::filesystem::absolute( filepath));
    if (!csvFile) {
        throw std::runtime_error(std::format("File does not exist or cannot be opened: {}", filepath));
    }

    std::vector<Row> csvRows;
    std::string csvLine;
    int i = 0;

    while (std::getline(csvFile, csvLine)) {
        if (!isValidUtf8(csvLine)) {
            throw std::runtime_error("File is not valid UTF-8");
        }
        if (i == 0) { //skip header
            ++i;
            continue;
        }
        auto row = readOne(csvLine, i);
        csvRows.push_back(row);
        ++i;
    }
    return csvRows;
}

bool CsvReader::isValidUtf8(const std::string& s) const
{
    int remaining = 0;

    for (unsigned char c : s) {
        if (remaining == 0) {
            if ((c >> 5) == 0b110) remaining = 1;
            else if ((c >> 4) == 0b1110) remaining = 2;
            else if ((c >> 3) == 0b11110) remaining = 3;
            else if ((c >> 7)) return false; // invalid leading byte
        }
        else {
            if ((c >> 6) != 0b10) return false;
            --remaining;
        }
    }
    return remaining == 0;
}
