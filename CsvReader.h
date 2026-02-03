#pragma once
#include <string>
#include "SectorSnapshot.h"
#include <vector>
#include "Report.h"
#include <filesystem>
struct Row {
	std::string timestamp;
	std::string sectorId;
	int aircraft_count;
	int capacity;
	int line;
};
class CsvReader {
public:
	CsvReader(char sep=',');
	std::string trim(const std::string& field) const;
	Row readOne(const std::string& line,const int i) const;
	std::vector<Row> readAll(const std::filesystem::path& filepath) const;
	bool isValidUtf8(const std::string& s) const;
	// std::vector<Report> build(std::vector<SectorSnapshot> sectors) const;
private:
	char m_sep;
};

