cmake_minimum_required(VERSION 3.20)

# Project name
project(SectorMonitor VERSION 1.0 LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically collect all .cpp files inside src/
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Create executable
add_executable(sector_monitor ${SRC_FILES})

# add header files inside include/
target_include_directories(sector_monitor PRIVATE include)

# add json lib 
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(sector_monitor PRIVATE nlohmann_json::nlohmann_json)

# copying the default data sectors.csv
add_custom_command(TARGET sector_monitor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_SOURCE_DIR}/sectors.csv
          $<TARGET_FILE_DIR:sector_monitor>/sectors.csv
)
# enable warnings
if(MSVC)
    target_compile_options(sector_monitor PRIVATE /W4)
else()
    target_compile_options(sector_monitor PRIVATE -Wall -Wextra -Wpedantic)
endif()