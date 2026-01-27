#pragma once
#include <string>

struct SectorSnapshot
{
	std::string timestamp;
	std::string sectorId;
	int aircraft_count=0;
	int line;
};
