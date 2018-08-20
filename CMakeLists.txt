# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("cpptmpl")

if(1)
    set(Poco_DI "E:\\ws.local\\buildtree\\poco-poco-1.9.0-release.install\\lib\\cmake\\Poco")
    set(JSON_DIR "E:\\ws.local\\buildtree\\json\\nlohmann")
    set(Cereal_DIR "E:\\ws.local\\buildtree\\cereal-1.2.2\\include")
    set(Catch_DIR "E:\\ws.local\\buildtree\\Catch2-2.2.3\\single_include")
endif()

# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(proj_version_file)
proj_version_file(${PROJECT_SOURCE_DIR}/VERSION)

message(STATUS VERSION=${PROJECT_VERSION})

#Poco
find_package(Poco 1.9.0 REQUIRED COMPONENTS Foundation Util Net Zip)

#cereal
include_directories(${Cereal_DIR})

#json
include_directories(${JSON_DIR})

#catch
include_directories(${Catch_DIR})

# Include sub-projects.
add_subdirectory (src)

# test
add_subdirectory(test)



message("\n\n")
include(FeatureSummary)
feature_summary(
  DESCRIPTION "------- FEATURE SUMMARY FOR ${PROJECT_NAME} -------"
  WHAT ALL
)
