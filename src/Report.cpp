#include "Report.h"
#include <format>
#include "IssueSeveritiy.h"
std::string displayReport(const Report& report)
{
	return std::format(
		"{:<5} {:<20} {:<10} {:<10} {}",
		report.line,
		report.timestamp,
		report.sectorId,
		toString(report.severity),
		report.message
	);
}

std::string displayHeader()
{
	return std::format(
		"{:<5} {:<20} {:<10} {:<10} {}",
		"Line",
		"Time",
		"Sector Id",
		"Severity",
		"Details"
	);
}
