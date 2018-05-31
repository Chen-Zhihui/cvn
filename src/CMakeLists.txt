# CMakeList.txt : CMake project for cpptmpl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (cpptmpl "cpptmpl.cpp" "cpptmpl.h")

target_link_libraries(cpptmpl PRIVATE Poco::OSP)

# TODO: Add tests and install targets if needed.
