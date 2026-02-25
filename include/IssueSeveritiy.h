#pragma once
#include <string>
enum class IssueSeverity{
	OK,
	WARNING,
	ERROR,
	CRITICAL
};
std::string toString(IssueSeverity s);