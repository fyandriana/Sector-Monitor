# Airspace Sector Capacity Monitoring System (C++)

## Overview

This project is a C++ command-line utility that monitors airspace sector load using CSV input data.  
It demonstrates backend software development skills with a strong focus on **defensive programming**, **data validation**, and **predictable behavior** in a safety-critical aviation context.

The program processes structured sector snapshots over time, applies explicit monitoring rules, and reports **all detected conditions in a single run**, without stopping at the first warning or error.  
This ensures complete visibility into sector load evolution and supports traceable operational diagnostics.

---

## Objectives

This project demonstrates the ability to:

- Design backend logic for processing time-based operational data
- Apply object-oriented and structured programming principles in C++
- Work with capacity constraints in a safety-oriented domain
- Write clear, testable, and maintainable code
- Separate responsibilities between parsing, monitoring, and reporting
- Reason about correctness, edge cases, and operational thresholds

---

## Core Components

### Sector Snapshot Model

Each observation is represented by a `SectorSnapshot` structure containing:

- Timestamp (ISO 8601 UTC)
- Sector identifier
- Observed aircraft count
- Source CSV line number (for traceability)

Only rows that pass field-level validation are converted into `SectorSnapshot` objects.

---

### Sector Configuration

Each sector is represented by a `Sector` object containing:

- Sector identifier
- Declared maximum capacity

Sector configuration is immutable once created. Capacity inconsistencies for the same sector are detected and reported.

---

### CSV Reader

The `SectorCsvReader` component is responsible for:

- Reading the CSV file line by line
- Trimming and extracting exactly four expected fields per row
- Guarding against malformed or incomplete lines
- Converting numeric fields safely
- Preserving CSV line numbers for reporting
- Never crashing on invalid input

The reader performs **no monitoring logic**.  
Its responsibility is limited to safe parsing and data extraction.

---

### Monitoring Engine

The `SectorMonitor` applies all monitoring rules and produces structured `SectorIssue` objects.

Key characteristics:

- Monitoring logic is deterministic and repeatable
- All detected conditions are collected (no early exit)
- Warning and critical states are clearly distinguished
- Capacity rules are applied consistently per sector
- Results are suitable for audit and review

---

### Issue Severity

Operational conditions are classified using an enumeration:

- `OK`
- `WARNING`
- `CRITICAL`

Using an enum ensures type safety and prevents invalid severity states.

---

## Monitoring Rules

For each sector snapshot:

- **Normal Load (OK)**  
  Aircraft count < 80% of declared capacity

- **High Load (WARNING)**  
  Aircraft count ≥ 80% of declared capacity

- **Over Capacity (CRITICAL)**  
  Aircraft count > declared capacity

Thresholds are explicit, deterministic, and easy to audit.

---

## CSV Format

### Header

```
timestamp,sector_id,capacity,aircraft_count
```

### Timestamp Format

- ISO 8601 UTC format
- Example: `2026-01-18T09:15:00Z`
- `Z` indicates Zulu (UTC) time

Capacity must remain consistent for a given sector across all rows.


---

## Input File Argument

The program accepts the CSV input filename as an optional command-line argument. If a filename is provided, the program uses it as the input file. If no argument is supplied, the program defaults to using `sectors.csv`, which must be located at the root of the project directory. 

File paths may be specified as relative or absolute paths. When passing file paths that contain spaces or special characters, the path must be wrapped in double quotes. 

The program validates the input file before processing and reports an error if the file cannot be found or opened.

---

## Example Execution

```
Timestamp               Sector     Level      Details
2026-01-18T09:15:00Z    SECTOR_A   CRITICAL   aircraft count (13) exceeds capacity (12)
2026-01-18T09:20:00Z    SECTOR_B   WARNING    sector operating at 100% capacity
```

All detected conditions are reported in a single run.  
The program never stops after the first warning or error.

---

## Build & Run

### Requirements

- C++17 or later compatible compiler
- Visual Studio 2022 (tested)

### Steps

1. Open the project in Visual Studio
2. Set the language standard to **C++17 or later**
3. Build the solution
4. Place the CSV file in the working directory
5. Run the program

---

## Design Considerations

- Clear separation of concerns between parsing, monitoring, and reporting
- Monitoring logic favors correctness and traceability over premature optimization
- All thresholds and assumptions are explicit and documented
- Code prioritizes readability and maintainability

---

## Assumptions & Scope

- Sector capacity is static during the observation period
- No prediction or forecasting is performed
- No visualization or UI components are included
- No external dependencies are used

---

## Summary

This project demonstrates disciplined C++ backend design, structured data monitoring, and safety-oriented reasoning in an air traffic management context.  
It reflects an engineering approach aligned with the expectations of operational systems such as airspace capacity monitoring.

