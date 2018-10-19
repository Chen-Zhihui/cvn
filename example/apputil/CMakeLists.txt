# CMakeList.txt : CMake project for cpptmpl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (demo_apputil 
cpptmpl.cpp 
)

target_link_libraries(demo_apputil PRIVATE Apputil)
target_include_directories( demo_apputil
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>

    )
# target_link_libraries(cpptmpl PRIVATE Poco::OSP)

# TODO: Add tests and install targets if needed.
