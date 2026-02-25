#pragma once
#include <string>
class Sector {
public: 
	Sector(std::string sId, int c) :m_sector_id(sId), m_capacity(c) {};
	std::string id() const noexcept;
	int capacity() const noexcept;
private:
	std::string m_sector_id;
	int m_capacity;
};
