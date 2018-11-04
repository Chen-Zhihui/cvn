# CMakeList.txt : CMake project for cpptmpl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (sample_apputil
        sample_apputil.cpp
)

target_link_libraries(sample_apputil PRIVATE Apputil
) 
target_include_directories( sample_apputil
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
    )

# TODO: Add tests and install targets if needed.
