# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("cpptmpl")

# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(proj_version_file)
proj_version_file(${PROJECT_SOURCE_DIR}/VERSION)

message(STATUS VERSION=${PROJECT_VERSION})

find_package(Poco COMPONENTS Foundation Util Net Zip)
find_package(PocoOSP CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory (src)
