# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Cvn")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

option(USE_Qt5 "use qt gui" ON)
option(USE_Caffe2 "caffe2 libs" ON)
option(USE_CvFace "use opencv::face" ON)

set(eproot ${CMAKE_SOURCE_DIR}/3rdparty)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(config_package_location lib/cmake/${PROJECT_NAME})


# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CvnMacros)
include(proj_version_file)

proj_version_file(${PROJECT_SOURCE_DIR}/VERSION)

message(STATUS VERSION=${PROJECT_VERSION})

#Poco
find_package(Poco REQUIRED COMPONENTS Foundation Util Net Zip)

#OpenCV
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
foreach(file ${OpenCV_LIBS})
    #file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})	
    #install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach()

if(0)
set(OpenBLAS_DIR "E:/ws.local/OpenBLAS")
set(OpenBLAS_FOUND TRUE)
set(OpenBLAS_INCLUDE_DIR ${OpenBLAS_DIR}/include)
set(OpenBLAS_LIBRARY_DIR ${OpenBLAS_DIR}/lib/x64)
set(OpenBLAS_DLL_DIR     ${OpenBLAS_DIR}/bin/x64)
set(OpenBLAS_LIB ${OpenBLAS_LIBRARY_DIR}/libopenblas.dll.a)
set(OpenBLAS_dlls 	${OpenBLAS_DLL_DIR}/libgcc_s_seh-1.dll  
					${OpenBLAS_DLL_DIR}/libgfortran-3.dll  
					${OpenBLAS_DLL_DIR}/libopenblas.dll  
					${OpenBLAS_DLL_DIR}/libquadmath-0.dll
                    )                    
foreach(file ${OpenBLAS_dlls})
    file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach()
endif()

#json
find_package(nlohmann_json REQUIRED)

#catch
find_package(Catch2 REQUIRED)

#fmt
find_package(fmt REQUIRED)

if(USE_Caffe2)
find_package(Caffe2 REQUIRED)
endif()

#sqlite_orm
find_package(sqlite3 CONFIG REQUIRED)
find_package(sqlite-orm REQUIRED)

#
find_package(Async++ REQUIRED)

#protobuf
find_package(Protobuf CONFIG QUIET)
if (NOT Protobuf_FOUND)
  find_package(Protobuf MODULE QUIET)
endif()
    message(STATUS "    Protobuf compiler   : ${PROTOBUF_PROTOC_EXECUTABLE}")
    message(STATUS "    Protobuf includes   : ${PROTOBUF_INCLUDE_DIRS}")
    message(STATUS "    Protobuf libraries  : ${PROTOBUF_LIBRARIES}")

find_package(Boost REQUIRED COMPONENTS filesystem)

if(USE_Qt5)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets)
endif()

find_package(libopenshot)

find_package(nmslib COMPONENTS nmslib)

add_subdirectory(3rdparty)

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
