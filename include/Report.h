#pragma once
#include <string>
#include "IssueSeveritiy.h"
struct Report
{
	int line;
	std::string timestamp;
	std::string sectorId;
	IssueSeverity severity;
	std::string message;
};

std::string displayReport(const Report& report);
std::string displayHeader();