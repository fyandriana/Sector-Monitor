#pragma once
#include <vector>
#include "Report.h"
#include "CsvReader.h"
#include <optional>
#include "Sector.h"
#include <unordered_map>


class Monitor {
public:
	std::optional<SectorSnapshot> validateFields(const Row& row);
	bool checkOverload(const SectorSnapshot& snapshot);
	std::string displayReports() const;
	bool isValidTimestamp(const std::string& ts) const;
	void run(const std::string& filepath);
	void reset();
private:
	const double m_ratio = 0.80;
	std::vector<Report> m_reports;
	std::unordered_map<std::string, Sector> m_sectors;
};
