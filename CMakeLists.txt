# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Cvn")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

set(config_package_location lib/cmake/${PROJECT_NAME})
# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CvnMacros)
include(proj_version_file)
proj_version_file(${PROJECT_SOURCE_DIR}/VERSION)
message(STATUS VERSION=${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

option(USE_Qt5 "use qt gui" ON)
option(USE_Caffe2 "caffe2 libs" ON)
option(USE_CvFace "use opencv::face" ON)

set(eproot ${CMAKE_SOURCE_DIR}/3rdparty)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net Zip Data)
find_package(nlohmann-json REQUIRED)
find_package(tl-expected REQUIRED)
find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(sqlite3 CONFIG REQUIRED)
find_package(sqlite-orm REQUIRED)
find_package(Async++ REQUIRED)
find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
find_package(pcre REQUIRED)
# find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(libopenshot)
find_package(ffmpeg REQUIRED)
foreach(FFMPEG_SUBLIBRARY avformat avdevice avcodec avutil swscale avfilter postproc swresample swscale)
    #find_library(FFMPEG_lib${FFMPEG_SUBLIBRARY}_LIBRARY NAMES ${FFMPEG_SUBLIBRARY})
    message(STATUS ${FFMPEG_lib${FFMPEG_SUBLIBRARY}_LIBRARY})
endforeach()
find_package(nmslib COMPONENTS nmslib)
# find_package(glog REQUIRED)
# find_package(gflags REQUIRED)
#find_package(openblas)
if(USE_Qt5)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets)
endif()
if(USE_Caffe2)
find_package(Caffe2 REQUIRED)
if(${Caffe2_FOUND})
MESSAGE("Caffe2 information:") 
MESSAGE("  Caffe2_INCLUDE_DIRS: ${Caffe2_INCLUDE_DIRS}") 
MESSAGE("  Caffe2_LIBRARIES: ${Caffe2_LIBRARIES}") 
MESSAGE("  Caffe2_LIBRARY_DIRS: ${Caffe2_LINK_DIRECTORIES}") 
MESSAGE("  Caffe2_LIBS: ${Caffe2_LIBS}") 
    get_target_property(Caffe2dylib_release caffe2 IMPORTED_LOCATION_RELEASE)
    get_target_property(Caffe2dylib_debug caffe2 IMPORTED_LOCATION_DEBUG)
    file(COPY ${Caffe2dylib_debug}  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    file(COPY ${Caffe2dylib_release}  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    install(FILES ${Caffe2dylib_debug} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    install(FILES ${Caffe2dylib_release} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
MESSAGE("  Caffe2: ========================================== ") 
endif()

message(STATUS ${PCRE_LIBRARIES})
message(STATUS ${EXPAT_LIBRARIES})

# find_package(Protobuf CONFIG QUIET)
# if (NOT Protobuf_FOUND)
#   find_package(Protobuf MODULE QUIET)
# endif()
# message(STATUS "    Protobuf compiler   : ${PROTOBUF_PROTOC_EXECUTABLE}")
# message(STATUS "    Protobuf includes   : ${PROTOBUF_INCLUDE_DIRS}")
# message(STATUS "    Protobuf libraries  : ${PROTOBUF_LIBRARIES}")

set(OpenCV_FIND_QUIETLY OFF)
#find_package( OpenCV COMPONENTS core imgproc calib3d highgui objdetect videoio face tracking OPTIONAL_COMPONENTS viz)
find_package(OpenCV)
if(${OpenCV_FOUND})
	MESSAGE("OpenCV information:") 
	MESSAGE("  OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}") 
	MESSAGE("  OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}") 
	MESSAGE("  OpenCV_LIBRARY_DIRS: ${OpenCV_LINK_DIRECTORIES}") 
	MESSAGE("  OpenCV_LIBS: ${OpenCV_LIBS}") 
else()
    MESSAGE(FATAL_ERROR "OpenCV not found in the system.")
endif()
if(TARGET opencv_world)
    get_target_property(worddylib_release opencv_world IMPORTED_LOCATION_RELEASE)
    get_target_property(worddylib_debug opencv_world IMPORTED_LOCATION_DEBUG)
    file(COPY ${worddylib_debug}  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    file(COPY ${worddylib_release}  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    install(FILES ${worddylib_debug} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    install(FILES ${worddylib_release} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
else()
    # foreach(file ${OpenCV_LIBRARIES})
    #     file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)	
    #     install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    # endforeach()
endif()

add_subdirectory(3rdparty)

# Include sub-projects.
add_subdirectory(src)

# test
add_subdirectory(test)

# install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/${PROJECT_NAME}Config.in.cmake
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
