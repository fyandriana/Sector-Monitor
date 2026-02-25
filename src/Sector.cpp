#include "Sector.h"

std::string Sector::id() const noexcept
{
    return m_sector_id;
}

int Sector::capacity() const noexcept
{
    return m_capacity;
}
