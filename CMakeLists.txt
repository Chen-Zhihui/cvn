# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("cvn")

if(0)
    set(Poco_DI "E:\\ws.local\\buildtree\\poco-poco-1.9.0-release.install\\lib\\cmake\\Poco")
    set(JSON_DIR "E:\\ws.local\\buildtree\\json\\nlohmann")
    set(Cereal_DIR "E:\\ws.local\\buildtree\\cereal-1.2.2\\include")
    set(Catch_DIR "E:\\ws.local\\buildtree\\Catch2-2.2.3\\single_include")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(config_package_location lib/cmake/${PROJECT_NAME})


# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(proj_version_file)
proj_version_file(${PROJECT_SOURCE_DIR}/VERSION)

message(STATUS VERSION=${PROJECT_VERSION})

#Poco
find_package(Poco 1.9.0 REQUIRED COMPONENTS Foundation Util Net Zip)

#OpenCV
set(OpenCV_FIND_QUIETLY OFF)
find_package( OpenCV REQUIRED COMPONENTS core imgproc calib3d highgui objdetect OPTIONAL_COMPONENTS viz)
if(${OpenCV_FOUND})
	MESSAGE("OpenCV information:") 
	MESSAGE("  OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}") 
	MESSAGE("  OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}") 
	MESSAGE("  OpenCV_LIBRARY_DIRS: ${OpenCV_LINK_DIRECTORIES}") 
	MESSAGE("  OpenCV_LIBS: ${OpenCV_LIBS}") 
else()
    MESSAGE(FATAL_ERROR "OpenCV not found in the system.")
endif()
foreach(file ${OpenCV_LIBS})
    #file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})	
    #install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach()

#cereal
include_directories(${Cereal_DIR})

#json
include_directories(${JSON_DIR})

#catch
include_directories(${Catch_DIR})

#expected
include_directories(${expected_DIR})

# Include sub-projects.
add_subdirectory (src)

# example
add_subdirectory(example)
# test
add_subdirectory(test)


# install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/${PROJECT_NAME}Config.cmake.in 
"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" 
@ONLY
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
        ${config_package_location}
    COMPONENT
        Devel
)

message("\n\n")
include(FeatureSummary)
feature_summary(
  DESCRIPTION "------- FEATURE SUMMARY FOR ${PROJECT_NAME} -------"
  WHAT ALL
)
