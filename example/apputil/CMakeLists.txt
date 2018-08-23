# CMakeList.txt : CMake project for cpptmpl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (exam_apputil 
cpptmpl.cpp 
)

target_link_libraries(exam_apputil PRIVATE Apputil)
target_include_directories( exam_apputil
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    )
# target_link_libraries(cpptmpl PRIVATE Poco::OSP)

# TODO: Add tests and install targets if needed.
