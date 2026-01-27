#include "Monitor.h"
#include <stdexcept>
#include <regex>
#include <sstream>
#include <format>
#include "CsvReader.h"
#include <iostream>

std::optional<SectorSnapshot> Monitor::validateFields(const Row& row)
{
    SectorSnapshot snapshot;
    std::vector<std::string> invalidFields;
    // check empty fields
    if (row.sectorId.empty()) {
        invalidFields.push_back("Unknown Sector Id");
        //throw std::invalid_argument("Unknown sector Id");
    }
    // check capacity
    if (row.capacity <= 0) {
        invalidFields.push_back("Unknown Capacity");
        // throw std::invalid_argument("Unknown Capacity");
    }

    // check time
    if (!isValidTimestamp(row.timestamp)) {
        invalidFields.push_back("Invalid Timestamp Format");
    }
   
    // check aircraft count
    if (row.aircraft_count < 0) {
        invalidFields.push_back("Invalid aircraft count");
    }
    // generate snapshot for the sector
    if(invalidFields.size()) {
        // create report
        std::ostringstream oss;
        for (auto& m : invalidFields) {
            oss << " " << m;
        }
        oss << " => Row Skipped";
        m_reports.push_back(
            Report{
                row.line,
                row.timestamp,
                row.sectorId,
                IssueSeverity::ERROR,
                oss.str()
            }
        );
        return std::nullopt;
    }

    snapshot.timestamp = row.timestamp;
    snapshot.sectorId = row.sectorId;
    snapshot.aircraft_count = row.aircraft_count;
    snapshot.line = row.line;

    // emplace sector unique
    auto [it, inserted] = m_sectors.emplace(
        row.sectorId,
        Sector{ row.sectorId,row.capacity }
    );
    if (!inserted && (it->second.capacity() != row.capacity)) {
        m_reports.push_back(
            Report{
                row.line,
                row.timestamp,
                row.sectorId,
                IssueSeverity::WARNING,
                std::format("Capacity mismatch detected for sector {}: expected {}, but found {}. Sector capacity must remain consistent across all snapshots.",
                    row.sectorId, it->second.capacity(), row.capacity
                    )
            }
        );
    }
    return snapshot;
}

bool Monitor::checkOverload(const SectorSnapshot& snapshot)
{
    // check sector
    auto itSector = m_sectors.find(snapshot.sectorId);
    if (itSector == m_sectors.end()) {// the iterator reach the end of the vector sectors
        // add report unknown sector
        m_reports.emplace_back(
            snapshot.line,
            snapshot.timestamp,
            snapshot.sectorId,
            IssueSeverity::CRITICAL,
            std::format("Unkown sector {}",snapshot.sectorId)
        );
        return false;
    }

    // found sector but capacity is 0 or negative
    if (itSector->second.capacity() <= 0) {
        m_reports.emplace_back(
            snapshot.line,
            snapshot.timestamp,
            snapshot.sectorId,
            IssueSeverity::ERROR,
            "Invalid sector capacity (capacity = 0). Capacity must be greater than zero to evaluate overload."
        );
        return false;
    }
    // check ratio
    const double ratio = (double)snapshot.aircraft_count /(double) itSector->second.capacity();
    //if (ratio < m_ratio) { // good, skipped, silent no need to report
    //    m_reports.emplace_back(
    //        snapshot.timestamp,
    //        snapshot.sectorId,
    //        IssueSeverity::OK,
    //        ""
    //    );
    //}
    if ((m_ratio <= ratio) && (ratio < 1.0)) { // warning of overload
        m_reports.emplace_back(
            snapshot.line,
            snapshot.timestamp,
            snapshot.sectorId,
            IssueSeverity::WARNING,
            std::format("High sector load detected. Only {} aircraft remaining before reaching capacity.",
                (itSector->second.capacity()-snapshot.aircraft_count)
                )
        );
        return true;
    } 
    if (snapshot.aircraft_count == itSector->second.capacity()) { // warning of full
        m_reports.emplace_back(
            snapshot.line,
            snapshot.timestamp,
            snapshot.sectorId,
            IssueSeverity::WARNING,
            std::format("Sector {} is at full capacity. Additional traffic cannot be accommodated.",
                itSector->second.id()
            )
        );
        return true;
    }
    if (ratio > 1.0) { // critical error
        m_reports.emplace_back(
            snapshot.line,
            snapshot.timestamp,
            snapshot.sectorId,
            IssueSeverity::CRITICAL,
            std::format("Aircraft count {} exceeds capacity {}. Immediate attention required.",
                snapshot.aircraft_count, itSector->second.capacity()
                )
        );
        return true;
    }
    return false;

}

std::string Monitor::displayReports() const
{
    std::ostringstream oss;
    for (auto& report : m_reports) {
        oss << displayReport(report);
        oss << "\n";
    }
    return oss.str();
}

void Monitor::run(const std::string& filepath)
{
    reset();
    try {
        // read csv
        CsvReader csv;
        std::vector<Row> rows = csv.readAll(filepath);
        std::cout << displayHeader();
        std::cout << "\n";
        // validate data
        for (auto& row : rows) {
            auto sn = validateFields(row);
            if (!sn) {
                continue;
            }
            checkOverload(*sn);
        }
        std::cout << displayReports();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

    }

}

void Monitor::reset()
{
    m_reports.clear();
    m_sectors.clear();
}



bool Monitor::isValidTimestamp(const std::string& ts) const
{
    static const std::regex pattern(
        R"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)"
    );
    return std::regex_match(ts, pattern);
}

