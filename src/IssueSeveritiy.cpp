#include "IssueSeveritiy.h"

std::string toString(IssueSeverity s)
{
	switch (s)
	{
	case IssueSeverity::OK:
		return "OK";
		
	case IssueSeverity::WARNING:
		return "WARNING";
		
	case IssueSeverity::ERROR:
		return "ERROR";
		
	case IssueSeverity::CRITICAL:
		return "CRITICAL";
		
	default:
		return "UNKNOWN";
	}
}
